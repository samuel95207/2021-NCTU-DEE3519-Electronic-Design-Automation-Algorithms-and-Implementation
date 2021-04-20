TARGET = ./bin/0710764_lab2.o

SRC_DIR = ./src
SRC_SUBDIR += . 
INCLUDE_DIR += ./src
OBJ_DIR = ./src/obj

CC = g++
C_FLAGS = -g -Wall -std=c++11 -O3
LD = $(CC)
INCLUDES += -I$(INCLUDE_DIR)
LD_FLAFS += 
LD_LIBS =

ifeq ($(CC), g++)
	TYPE = cpp
else
	TYPE = c
endif

SRCS += ${foreach subdir, $(SRC_SUBDIR), ${wildcard $(SRC_DIR)/$(subdir)/*.$(TYPE)}}
OBJS += ${foreach src, $(notdir $(SRCS)), ${patsubst %.$(TYPE), $(OBJ_DIR)/%.o, $(src)}}

vpath %.$(TYPE) $(sort $(dir $(SRCS)))

all : $(TARGET)
	@echo "Builded target:" $^
	@echo "Done"

$(TARGET) : $(OBJS)
	@mkdir -p $(@D)
	@echo "Linking" $@ "from" $^ "..."
	$(LD) -o $@ $^ $(LD_FLAGS) $(LD_LIBS)
	@echo "Link finished"

$(OBJS) : $(OBJ_DIR)/%.o:%.$(TYPE)
	@mkdir -p $(@D)
	@echo "Compiling" $@ "from" $< "..."
	$(CC) -c -o $@ $< $(C_FLAGS) $(INCLUDES)
	@echo "Compile finished"

.PHONY : clean cleanobj
clean : cleanobj
	@echo "Remove all executable files and output files"
	rm -f $(TARGET)
	rm -f ./outputs/*
cleanobj :
	@echo "Remove object files"
	rm -rf $(OBJ_DIR)/*.o


case1:
	@make
	@./bin/0710764_lab2.o ./inputs/case1/case1.in outputs/case1/case1.out
	

case2:
	@make
	@./bin/0710764_lab2.o ./inputs/case2/case2.in outputs/case2/case2.out


zip:
	@make clean
	@rm -f 0710764.zip
	@zip -r 0710764.zip src docs/0710764.pdf makefile README.md


