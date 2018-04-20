

puma: src/*.cpp src/*.hpp
	clang++ -lSDL2 -lGL -lGLEW -std=c++11 src/*.cpp -o puma

.PHONY: clean

clean:
	rm -f puma
