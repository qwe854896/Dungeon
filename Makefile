all: compile link

compile:
	g++ -std=c++11 -I include -I SFML/src/include -c src/*.cpp

link:
	g++ -std=c++11 *.o -o main -L SFML/src/lib -l sfml-graphics -l sfml-window -l sfml-system -l sfml-audio

clean:
	rm *.o
	rm main