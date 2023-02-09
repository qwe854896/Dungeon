CC := g++
CFLAGS := -g -I include
LFLAGS := -l sfml-graphics -l sfml-window -l sfml-system -l sfml-audio 

TARGET := main
SRCS := $(wildcard src/*.cpp)
OBJS := $(patsubst src/%.cpp,bin/%.o,$(SRCS))

all: $(TARGET) 

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LFLAGS) 

bin/%.o: src/%.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf $(TARGET) bin/*.o

.PHONY: all clean
