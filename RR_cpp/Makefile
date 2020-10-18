TARGET=sim

SOURCES=main.cpp serve.cpp common.cpp
CXX=g++
OBJECTS = $(SOURCES:.cpp=.o)

%.o:%.cpp
	$(CXX) -c $(SOURCES)
$(TARGET):$(OBJECTS)
	$(CXX) -o $(TARGET) $(OBJECTS)

.PHONY:cleanw clean
cleanw:
	del $(OBJECTS) $(TARGET)
clean:
	rm -f $(OBJECTS) $(TARGET)