cmake -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_TOOLCHAIN_FILE=%PS5SDK%/cmake/toolchain-ps5.cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 .
ninja
copy /Y .\bin\spawner.elf "C:\Users\astre\Documents\PS5\bdj-sdk\samples\hopsploit\src\main\resources\org\homebrew"
