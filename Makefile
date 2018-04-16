

puma: src/main.cpp
	clang -lSDL2 -lGL -lGLEW src/main.cpp -o puma

.PHONY: clean

clean:
	rm -f puma
