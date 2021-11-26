OPTS = -I"../../include" -L"/usr/lib" /usr/local/include/irrKlang/bin/macosx-gcc/libIrrKlang.dylib -pthread

all: main

main: main.cpp shader.cpp 
	g++ main.cpp shader.cpp -std=c++11 -o main $(OPTS) -lglfw -lglew -framework OpenGL 

clean: 
	rm -f *.o