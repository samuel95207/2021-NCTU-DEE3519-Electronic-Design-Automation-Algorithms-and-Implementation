TARGET = ./bin/Lab3

SRC_DIR = ./src
SRC_SUBDIR += . 
INCLUDE_DIR += ./src
OBJ_DIR = ./src/obj

CC = g++
C_FLAGS = -g -Wall -std=c++11 -O3

# CUDD
CUDD_DIR = ./packages/cudd-3.0.0
CUDD_INCLUDE = 	-I $(CUDD_DIR)/cudd \
				-I $(CUDD_DIR)/util \
				-I $(CUDD_DIR)
CUDD_LIB = $(CUDD_DIR)/cudd/.libs/libcudd.a
CUDD_FLAGS = -lm


LD = $(CC)
INCLUDES += -I $(INCLUDE_DIR)
INCLUDES += $(CUDD_INCLUDE)
		
LD_FLAFS += $(CUDD_FLAGS)
LD_LIBS += $(CUDD_LIB)

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
	$(LD) -o $@ $^ $(LD_FLAGS) $(LD_LIBS) $(CUDD_LIB) 
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
	@./bin/Lab3 ./inputs/input1.txt ./outputs/output1.out

case2:
	@make
	@./bin/Lab3 ./inputs/input2.txt ./outputs/output2.out

case3:
	@make
	@./bin/Lab3 ./inputs/input3.txt ./outputs/output3.out

bigcase:
	@make
	@./bin/Lab3 ./inputs/bigcase.txt ./outputs/bigcase.out

zip:
	@make clean
	@make
	@rm -rf ./src/obj/*
	@rm -f 0710764.zip
	@zip -r 0710764.zip ./bin/Lab3 src ./packages makefile README.md 


