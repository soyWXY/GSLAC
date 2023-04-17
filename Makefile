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

TARGET_NAME := vectorAdd.out

TARGET := $(TARGET_NAME)
KERNEL_SRC := vectorAdd_kernel.cu
PTX := vectorAdd_kernel.ptx
SRC := vectorAdd.cpp

$(TARGET): $(SRC) $(PTX)
	$(CXX) $(CXXFLAGS) -o vectorAdd.out $(INC) $(LIB) $(SRC) $(LDFLAGS)
$(PTX): $(KERNEL_SRC)
	$(NVCC) $(NVCCFLAGS) $(KERNEL_SRC)

# default rule
default: all

# phony rules
.PHONY: all
all: $(TARGET)

.PHONY: clean
clean:
	@echo CLEAN $(CLEAN_LIST)
	@rm -f $(CLEAN_LIST)