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
	rm -f ./outputs/*/*
cleanobj :
	@echo "Remove object files"
	rm -rf $(OBJ_DIR)/*.o


case1:
	@make
	@./bin/0710764_lab2.o ./inputs/case1/case1.in ./outputs/case1.out

case2:
	@make
	@./bin/0710764_lab2.o ./inputs/case2/case2.in ./outputs/case2.out

case3:
	@make
	@./bin/0710764_lab2.o ./inputs/case3/case3.in ./outputs/case3.out

case4:
	@make
	@./bin/0710764_lab2.o ./inputs/case4/case4.in ./outputs/case4.out

case5:
	@make
	@./bin/0710764_lab2.o ./inputs/case5/case5.in ./outputs/case5.out

case6:
	@make
	@./bin/0710764_lab2.o ./inputs/case6/case6.in ./outputs/case6.out

zip:
	@make clean
	@make
	@rm -rf ./src/obj/*
	@rm -f 0710764.zip
	@mkdir 0710764
	@cp -r src 0710764/src
	@cp -r makefile 0710764/makefile
	@mkdir 0710764/bin
	@cp -r bin/0710764_lab2.o 0710764/bin/0710764_lab2.o 
	@cp -r README.md 0710764/README.md
	@zip -r 0710764.zip 0710764
	@rm -rf 0710764


