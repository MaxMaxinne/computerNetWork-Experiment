TARGET=lab2

CXX=g++
SOURCES=main.cpp
OBJ=$(SOURCES:.cpp=.o)

all:$(TARGET)
.PHONY:clean

%.o:%.cpp
	$(CXX) -c $< -o $@
$(TARGET):$(OBJ)
	$(CXX) -o $(TARGET) $(OBJ)

clean:
	rm -f $(TARGET) $(OBJ)