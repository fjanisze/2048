all: source animation_engine regression
	g++ -std=c++1y -I../SFML/SFML/include/ -I../gmock-1.7.0/fused-src/ *.o -o ./bin/2048.exe -L../SFML/SFML/lib/ -L../gmock-1.7.0/ -DSFML_STATIC -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lgmock

source:
	g++ -std=c++1y -I../SFML/SFML/include/ -I../gmock-1.7.0/fused-src/ -DSFML_STATIC -c ./src/*.cpp ./src/*.hpp

regression:
	g++ -std=c++1y -I../SFML/SFML/include/ -I../gmock-1.7.0/fused-src/ -DSFML_STATIC -c ./regression/*.cpp

animation_engine:
	g++ -std=c++1y -I../SFML/SFML/include/ -I../gmock-1.7.0/fused-src/ -DSFML_STATIC -c ./src/simple\ animation\ engine/*.cpp
