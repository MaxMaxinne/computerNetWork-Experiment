TARGET=sim

SOURCES=main.cpp serve.cpp common.cpp
CXX=g++
OBJECTS = $(SOURCES:.cpp=.o)
OBJ_PATH=./obj/
ALL_OBJ_POS = $(addprefix $(OBJ_PATH),$(OBJECTS))

obj_dir := ./obj
res_dir := ./res_output

$(shell if [ ! -e $(obj_dir) ];then mkdir -p $(obj_dir); fi)
$(shell if [ ! -e $(res_dir) ];then mkdir -p $(res_dir); fi)

./obj/%.o:%.cpp
	$(CXX) -c $< -o $@ -g
$(TARGET):$(ALL_OBJ_POS)
	$(CXX) -o $(TARGET) $(ALL_OBJ_POS) -g

.PHONY:clean
clean:
	rm -f $(ALL_OBJ_POS) $(TARGET)