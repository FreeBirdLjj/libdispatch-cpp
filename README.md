# libdispatch-c++

A C++ wrapper of `libdispatch`.

Although `libdispatch` can be used as a pure C library, sometimes we want to take advantage of lambdas in C++11. That's my first motivation.

## Requirements

- libdispatch
    - OS X (After 10.6 Snow Leopard): Just install Xcode and it's already done.
    - Ubuntu: `sudo apt-get install libdispatch-dev`    
- A C++ compiler supporting C++11
- `make`

## How to use

Just `make`. What you need are `dispatch-c++.h` and `libdispatch-c++.a`. The way to use them depends on your habits, anyway, let your compile toolchain know them.

`make test` to test the library can work on your compile toolchain. The expected result of `make test` is just printing "Success!" on terminal. `test_main.cpp` is an example of using `libdispatch-c++` in your program.

Be careful with the asynchronous functions!
