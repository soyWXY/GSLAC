# tool macros
CXX := g++
CXXFLAGS := -std=c++11
INC := -I/usr/local/cuda/include
LIB := -L/usr/local/cuda/lib64
LDFLAGS := -lcuda -lcudart

NVCC := nvcc
NVCCFLAGS := -ptx

# path macros
# BIN_PATH := bin
# OBJ_PATH := obj
# SRC_PATH := src

CLEAN_LIST := vectorAdd.out vectorAdd_kernel.ptx

TARGET := vectorAdd.out
ANS := test.out
KERNEL_SRC := vectorAdd_kernel.cu
PTX := vectorAdd_kernel.ptx
SRC := vectorAdd.cpp

$(TARGET): $(SRC) $(PTX)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(INC) $(LIB) $(SRC) $(LDFLAGS)
$(PTX): $(KERNEL_SRC)
	$(NVCC) $(NVCCFLAGS) $(KERNEL_SRC)

$(ANS): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(ANS) $(INC) $(LIB) $(SRC) $(LDFLAGS)

# default rule
default: all

# phony rules
.PHONY: all
all: $(TARGET)

.PHONY: ans
ans: $(ANS)

.PHONY: clean
clean:
	@echo CLEAN $(CLEAN_LIST)
	@rm -f $(CLEAN_LIST)