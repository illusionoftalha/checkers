Run this from the repository root with MinGW installed:

gcc main.c -o main.exe -ISDL/include -LSDL/lib -lmingw32 -lSDL2main -lSDL2 -mwindows

The executable is generated locally. Keep SDL2.dll beside it when running.
