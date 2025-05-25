cd "$(dirname "$0")"
mkdir -p build
g++ Application.cpp $(sdl2-config --cflags --libs) -lGL -lGLU -o build/CppBlocksDemo
