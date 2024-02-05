all:
	g++ -I src/include/SDL2 -L src/lib -o main main.cpp -lmingw32 -lSDL2main -lSDL2