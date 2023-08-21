# libhijacker

Project name is temporary

This repo will likely be short lived. I got tired of waiting and wanted to upload it somewhere.
Proper credits will be attributed later.

**Fork with support for game patching. check out the [original project](https://github.com/astrelsky/libhijacker) by astrelsky. (more for developers).**

# Supported Firmwares

- 3.00
- 3.10
- 3.20
- 4.00 (needs checking)
- 4.02 (needs checking)
- 4.03
- 4.50
- 4.51

# Quick Setup

- Download the latest zip from the [release](https://github.com/illusion0001/libhijacker/releases/latest) page.
- Extract the zip with all files to a folder on your computer.
- Load your preferred method of exploiting your PS5. (BDJB or webkit).
- Install [Python 3](https://python.org/).
- Install `aiofiles` using `pip`
```sh
pip install aiofiles
```
- On ELF loader stage, run send_elf.py with your PS5 IP as first argument. Example: `python ./send_elf.py 10.0.6.9`
   - To kill the daemon process for easier development, you can use `kill_daemon.py` to kill the process and re run `send_elf.py` to send the new build of the daemon executable. (this can be done outside the web browser or bdj window)
   - You can also find useful commands within `launch.py` and it's arguments.

# Supported Titles

- Batman: Arkham Knight v1.15: 60 FPS
- Bloodborne v1.09: 60 FPS by Lance McDonald
  - Also includes:
    - No Motion Blur
    - No Chromatic Aberration
    - Debug Camera by Lance McDonald
- Crash Team Racing v1.21: 60 FPS
- Demon Souls v1.00: 60 FPS Unlock in Cinematic Mode
- Driveclub v1.28: 60 FPS
- Gravity Rush 2 v1.11: 60 FPS
- The Order 1886: v1.02: 60 FPS
- The Last Guardian v1.03: 4K at 60FPS 
- The Last of Us Remastered 60 FPS + Debug Menu (v1.11)
- The Last of Us Part 2 60 FPS + Debug Menu (v1.00 + v1.09)
- Just Cause 4 v1.32: 60 FPS
- Shadow of the Colossus: 1440p at 60FPS + Debug Menu
- Silent Hills: P.T at 60FPS
- Red Dead Redemption 2 v1.29: 60FPS

# Known Issues (specific to this fork)

- Daemon sometimes crashes when scrolling through the game list.
- Daemon may crash with nullptr dereference in game patch thread.
- Daemon may reapply patch while game is running.

# Building

1. Install LLVM 16.
2. Build and install [PS5SDK](https://github.com/PS5Dev/PS5SDK) (firmware does not matter).
3. Clone this repo with recursive submodules.
4. Generate libary stubs using stubber which can be found in  [`/stubber`](/stubber) folder.
   - Build `main.go`: `go build`
   - Dump `/system/lib/common/` folder from your console (4.03) in decrypted prx files using the [ps5-self-dumper](https://github.com/sleirsgoevy/ps4jb-payloads/tree/bd-jb/ps5-self-dumper) payload from sleirsgoevy.
   - Or use my [pre generated](https://github.com/illusion0001/libhijacker/releases/tag/stubber-lib).
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

# Special Thanks

To all testers that signed up for game patch beta test!

- ac2pic
- ashenContinuum
- Cerebral-Context
- Cameron
- CrystalCircus
- DrChrisRespect
- embee
- erdosadam24
- hobbo0408
- Lustcyna
- maplemiyazaki
- jrson
- Lord Maximus Farquaad
- iknowcpp
- peekurchoo
- pinoy24
- sirbri83
- razzysxpb
- rudiano
- Voredy
- VIPO777
- WardFail
- Whitehawkx
