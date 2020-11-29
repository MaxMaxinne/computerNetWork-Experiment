TARGET=lab2

CXX=g++
SOURCES=main.cpp common.cpp
FLAG=-g
OBJ=$(SOURCES:.cpp=.o)

all:$(TARGET)
.PHONY:clean

%.o:%.cpp
	$(CXX) -c $< -o $@ $(FLAG)
$(TARGET):$(OBJ)
	$(CXX) -o $(TARGET) $(OBJ) $(FLAG)

clean:
	rm -f $(TARGET) $(OBJ)