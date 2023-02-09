CC := g++
CFLAGS := -Wall -g -I include
LFLAGS := -l sfml-graphics -l sfml-window -l sfml-system -l sfml-audio 

TARGET := main
SRCS := $(wildcard src/*.cpp)
OBJS := $(patsubst src/%.cpp,bin/%.o,$(SRCS))

DIR_GUARD=@mkdir -p $(@D)

all: $(TARGET) 

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LFLAGS) 

bin/%.o: src/%.cpp
	$(DIR_GUARD)
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf $(TARGET) bin

.PHONY: all clean
