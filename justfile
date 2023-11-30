CC:="clang++"
CFLAGS:="-std=c++11 -Wall -Wextra -O2"
SRC:="main.cpp nttinterpolator.cpp"
TARGET:="main"

run: build
    ./{{TARGET}}
build:
    {{CC}} {{CFLAGS}} {{SRC}} -o {{TARGET}}