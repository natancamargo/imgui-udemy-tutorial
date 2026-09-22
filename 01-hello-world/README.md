[![](https://img.shields.io/badge/c++-black?logo=c++&style=for-the-badge)](https://learnxinyminutes.com/c++/)

## Hello World with ImGui
Hello world with ImGui.Built with cmake.

### Install VCPKG

Official Link: <https://vcpkg.io/en/index.html>

```cmd
cd external
git clone https://github.com/Microsoft/vcpkg.git
.\vcpkg\bootstrap-vcpkg.bat # windows
./vcpkg/bootstrap-vcpkg.sh # Unix
```

### Build
```shell
export CC=/usr/bin/gcc
export CXX=/usr/bin/g++
cmake -S . -B ./build
cmake --build build
```

### Run
```shell
./build/tutorial/exe
```

### Debug
```shell
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
gdb ./build/tutorial/exe
```

### Watch with nodemon
```shell
npx nodemon --exec "cmake -S . -B build && cmake --build build && ./build/tutorial/exe" --watch src -e cpp,hpp,txt
```
