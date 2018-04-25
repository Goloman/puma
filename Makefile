

puma: src/*.cpp src/*.hpp
	clang++ -lSDL2 -lGL -lGLEW -Iinclude -std=c++11 -g src/*.cpp -o puma

.PHONY: clean

clean:
	rm -f puma
