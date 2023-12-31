CC:="clang++"
CFLAGS:="-std=c++14 -Wall -Wextra -O3"
SRC:="main.cpp nttinterpolator.cpp"
TARGET:="main"

run: build
    ./{{TARGET}}
build:
    {{CC}} {{CFLAGS}} {{SRC}} -o {{TARGET}}