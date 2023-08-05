# libhijacker

Project name is temporary

This repo will likely be short lived. I got tired of waiting and wanted to upload it somewhere.
Proper credits will be attributed later.

# Quick Setup

1. Install LLVM 16.
2. Build and install [PS5SDK](https://github.com/PS5Dev/PS5SDK) (firmware does not matter).
3. Clone this repo.
4. Generate libary stubs using stubber which can be found in  [`/stubber`](/stubber) folder.
   - Build `main.go`: `go build`
   - Dump `/system/lib/common/` folder from your console in decrypted prx files using the [ps5-self-dumper](https://github.com/sleirsgoevy/ps4jb-payloads/tree/bd-jb/ps5-self-dumper) payload from sleirsgoevy.
   - Run the stubber. `./main aerolib.csv (PS5 System Folder) (Output Folder)`
   - Copy `.so` files to this repository folder into `lib`. (Create one if it does not exist)
5. Build the hijacker

```bash
# this assume you have llvm 16
cmake -G Ninja -DCMAKE_C_COMPILER=clang-16 -DCMAKE_CXX_COMPILER=clang++-16 -DCMAKE_TOOLCHAIN_FILE=$PS5SDK/cmake/toolchain-ps5.cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 .
ninja
```

6. Load it on your console using `send_elf.py`. you will see kernel log output on the first instance where you originally send the spawner payload.
   - `python3 send_elf.py (PS5 IP)`
   - To kill the daemon process for easier development, you can use `kill_daemon.py` to kill the process and re run `send_elf.py` to send the new build of the daemon executable. (this can be done outside the web browser or bdj window)
   - You can also find useful commands within `launch.py` and it's arguments.
