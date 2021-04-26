all: compile link

compile:
	g++ -std=c++11 -I src/include -c ../*.cpp

link:
	g++ -std=c++11 *.o -o main -L src/lib -l sfml-graphics -l sfml-window -l sfml-system