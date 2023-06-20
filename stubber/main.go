package main

import (
	"debug/elf"
	_ "embed"
	"encoding/csv"
	"fmt"
	"io"
	"io/fs"
	"os"
	"path/filepath"
	"strings"
	"time"
)

//go:embed toolchain.cmake
var toolchainCmake string

//go:embed root.cmake
var rootCmake string

const AEROLIB_ENTRIES = 94275

var CMAKE_FILENAME = "CMakeLists.txt"

type CmakeEntry struct {
	name string
}

func createFile(path string) *os.File {
	err := os.MkdirAll(filepath.Dir(path), 0)
	if err != nil {
		panic(err)
	}
	fp, err := os.Create(path)
	if err != nil {
		panic(err)
	}
	return fp
}

func getOutputLibraryName(path string) string {
	out := filepath.Base(path)
	out = strings.TrimSuffix(out, ".sprx")
	out = strings.TrimSuffix(out, ".c")
	out = strings.TrimPrefix(out, "lib")
	return out
}

func getOutputFile(path string) string {
	path, err := filepath.Rel(getInputPath(), path)
	if err != nil {
		panic(err)
	}
	out := strings.TrimSuffix(path, ".sprx") + ".c"
	return filepath.Join(getOutputPath(), out)
}

func getOutputFolder(path string) string {
	path, err := filepath.Rel(getInputPath(), path)
	if err != nil {
		panic(err)
	}
	return filepath.Join(getOutputPath(), path)
}

func (p *ElfProcessor) process() {
	p.filewg.Add(1)
	defer p.filewg.Done()
	for f := range p.files {
		fp, err := elf.Open(f)
		if err != nil {
			if err.Error() == "EOF" {
				continue
			}
			panic(err)
		}

		p.processElf(&Elf{fp, f, getOutputFile(f)})
	}
}

func (p *ElfProcessor) getProjectKey(path string) string {
	if strings.HasSuffix(path, ".c") {
		path = filepath.Dir(path)
	}
	path, err := filepath.Rel(getOutputPath(), path)
	if err != nil {
		panic(err)
	}
	return path
}

func (p *ElfProcessor) getEntry(key string) chan CmakeEntry {
	p.entryLock.RLock()
	defer p.entryLock.RUnlock()
	entry, ok := p.entries[key]
	if !ok {
		return nil
	}
	return entry
}

func (p *ElfProcessor) setEntry(key string, entry chan CmakeEntry) {
	p.entryLock.Lock()
	defer p.entryLock.Unlock()
	p.entries[key] = entry
}

func (p *ElfProcessor) makeCmake(path string) {
	p.projectwg.Add(1)
	defer p.projectwg.Done()

	key := p.getProjectKey(path)
	cmake := make(chan CmakeEntry)
	p.setEntry(key, cmake)

	dir := filepath.Join(getOutputPath(), key)
	var out *os.File = nil
	defer func() {
		if out != nil {
			out.Close()
		}
	}()

	for entry := range cmake {
		if out == nil {
			out = createFile(filepath.Join(dir, CMAKE_FILENAME))
		}
		name := getOutputLibraryName(entry.name)
		out.WriteString(fmt.Sprintf("add_library(%s SHARED lib%s.c)\n", name, name))
	}
	out.WriteString("set(CMAKE_C_FLAGS \"${CMAKE_C_FLAGS} -g0 -nodefaultlibs -nostdlib -fno-omit-frame-pointer ")
	out.WriteString("-fno-builtin-function -fno-builtin -Wno-builtin-requires-header -Wno-incompatible-library-redeclaration\")\n")
}

func (p *ElfProcessor) getFiles(path string, d fs.DirEntry, err error) error {
	if err != nil {
		panic(err)
	}

	if d.IsDir() {
		go p.makeCmake(getOutputFolder(path))
		return nil
	}

	name := d.Name()

	if strings.HasSuffix(name, ".dll.sprx") {
		return nil
	}

	if strings.HasSuffix(name, "exe.sprx") {
		return nil
	}

	if !strings.HasSuffix(name, ".sprx") {
		return nil
	}

	if !strings.HasPrefix(name, "lib") {
		return nil
	}
	p.files <- path
	return nil
}

func readAerolib() map[string]string {
	aerolib := make(map[string]string, AEROLIB_ENTRIES)
	fp, e := os.OpenFile(getAerolibPath(), os.O_RDONLY, 0)
	if e != nil {
		panic(e)
	}
	defer fp.Close()

	r := csv.NewReader(fp)
	r.Comma = ' '

	for {
		record, err := r.Read()
		if record == nil && err == io.EOF {
			break
		}
		aerolib[record[0]] = record[1]
	}
	return aerolib
}

func getInputPath() string {
	return os.Args[2]
}

func getOutputPath() string {
	return os.Args[3]
}

func getAerolibPath() string {
	return os.Args[1]
}

func globCmake(root string) []string {
	var files []string
	filepath.WalkDir(root, func(s string, d fs.DirEntry, e error) error {
		if filepath.Base(s) == CMAKE_FILENAME {
			files = append(files, s)
		}
		return nil
	})
	return files
}

func makeRootCmake() {
	// system and system_ex

	func() {
		out, err := os.Create(filepath.Join(getOutputPath(), "toolchain.cmake"))
		if err != nil {
			panic(err)
		}
		defer out.Close()

		out.WriteString(toolchainCmake)
	}()

	func() {
		out, err := os.Create(filepath.Join(getOutputPath(), CMAKE_FILENAME))
		if err != nil {
			panic(err)
		}
		defer out.Close()

		out.WriteString(rootCmake)

		dir := getOutputPath()
		matches := globCmake(dir)
		for i := range matches {
			match := matches[i]
			project, err := filepath.Rel(dir, filepath.Dir(match))
			if err != nil {
				panic(err)
			}
			if project == "." {
				continue
			}
			project = strings.ReplaceAll(project, "\\", "/")
			out.WriteString(fmt.Sprintf("add_subdirectory(%s)\n", project))
		}
	}()
}

func main() {
	if len(os.Args) != 4 {
		println("usage: " + os.Args[0] + " <aerolib.csv> <elfdir> <stubdir>")
		os.Exit(0)
	}

	p := newElfProcessor()

	start := time.Now()

	for i := 0; i < 16; i++ {
		go p.process()
	}

	filepath.WalkDir(getInputPath(), p.getFiles)

	close(p.files)
	p.filewg.Wait()

	for key := range p.entries {
		close(p.entries[key])
	}
	p.projectwg.Wait()
	makeRootCmake()
	end := time.Now()
	elapsed := end.Sub(start)
	fmt.Printf("processed %d ELF files and %d symbols in %s\n\n", p.nfiles.Load(), p.nsymbols.Load(), elapsed)
	println("run the following to build the libraries:")
	fmt.Printf("cd %s && mkdir build && cd build\n", getOutputPath())
	println("cmake -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ ..")
	println("ninja")
}
