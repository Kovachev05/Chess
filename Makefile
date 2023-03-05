all:
	g++ -o bin/chess src/main.cpp -I deps/include -L deps/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -static-libgcc -static-libstdc++