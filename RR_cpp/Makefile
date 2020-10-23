TARGET=sim

SOURCES=main.cpp serve.cpp common.cpp
CXX=g++
OBJECTS = $(SOURCES:.cpp=.o)
OBJ_PATH=./obj/
ALL_OBJ_POS = $(addprefix $(OBJ_PATH),$(OBJECTS))

%.o:%.cpp
	$(CXX) -c $< -o $(OBJ_PATH)$@
$(TARGET):$(OBJECTS)
	$(CXX) -o $(TARGET) $(ALL_OBJ_POS)

.PHONY:cleanw clean
cleanw:
	del $(OBJECTS) $(TARGET)
clean:
	rm -f $(ALL_OBJ_POS) $(TARGET)