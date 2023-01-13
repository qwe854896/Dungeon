SFML_PATH=SFML

all: compile link

compile:
	g++ -std=c++11 -I include -I ${SFML_PATH}/include -c src/*.cpp

link:
	g++ -std=c++11 *.o -o main -L ${SFML_PATH}/lib -l sfml-graphics -l sfml-window -l sfml-system -l sfml-audio
	rm *.o

clean:
	rm main