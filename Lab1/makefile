TARGET = ./bin/lab1

SRC_DIR = ./src
SRC_SUBDIR += . 
INCLUDE_DIR += ./src
OBJ_DIR = ./src/obj

CC = g++
C_FLAGS = -g -Wall -std=c++11
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
	mkdir -p outputs
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

simplecase:
	@make
	@./bin/lab1 inputs/input_toy.hgr
	@echo ""
	@echo "Evaluation"
	@./utilities/verifier inputs/input_toy.hgr ./output.txt
	@echo ""
	@echo "Shmetis Evaluation"
	@./utilities/shmetis inputs/input_toy.hgr 2 5
	@./utilities/verifier inputs/input_toy.hgr ./inputs/input_toy.hgr.part.2




case1:
	@make
	@./bin/lab1 inputs/input1.txt.hgr
	@echo ""
	@echo "Evaluation"
	@./utilities/verifier inputs/input1.txt.hgr ./output.txt
	@echo ""
	@echo "Shmetis Evaluation"
	@./utilities/shmetis inputs/input1.txt.hgr 2 5
	@./utilities/verifier inputs/input1.txt.hgr ./inputs/input1.txt.hgr.part.2
	

case2:
	@make
	@./bin/lab1 inputs/input2.txt.hgr
	@echo ""
	@echo "Evaluation"
	@./utilities/verifier inputs/input2.txt.hgr ./output.txt
	@echo ""
	@echo "Shmetis Evaluation"
	@./utilities/shmetis inputs/input2.txt.hgr 2 5
	@./utilities/verifier inputs/input2.txt.hgr ./inputs/input2.txt.hgr.part.2

case3:
	@make
	@./bin/lab1 inputs/input3.txt.hgr
	@echo ""
	@echo "Evaluation"
	@./utilities/verifier inputs/input3.txt.hgr ./output.txt
	@echo ""
	@echo "Shmetis Evaluation"
	@./utilities/shmetis inputs/input3.txt.hgr 2 5
	@./utilities/verifier inputs/input3.txt.hgr ./inputs/input3.txt.hgr.part.2

largecase:
	@make
	@./bin/lab1 inputs/input_very_large.txt.hgr
	@echo ""
	@echo "Evaluation"
	@./utilities/verifier inputs/input_very_large.txt.hgr ./output.txt
	@echo ""
	@echo "Shmetis Evaluation"
	@./utilities/shmetis inputs/input_very_large.txt.hgr 2 5
	@./utilities/verifier inputs/input_very_large.txt.hgr ./inputs/input_very_large.txt.hgr.part.2


