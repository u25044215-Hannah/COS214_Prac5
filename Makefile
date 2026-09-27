CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -pedantic -g
TARGET := campusguard

SOURCES := $(filter-out main.cpp,$(wildcard *.cpp)) main.cpp
OBJECTS := $(SOURCES:.cpp=.o)

.PHONY: all run clean valgrind debug

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)

debug: $(TARGET)
	gdb ./$(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)
