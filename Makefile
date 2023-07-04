# tool macros
CXX := g++
CXXFLAGS := -std=c++11
INC := -I/usr/local/cuda/include
LIB := -L/usr/local/cuda/lib64
LDFLAGS := -lcuda -lcudart

NVCC := nvcc
NVCCFLAGS := -ptx

CLEAN_LIST := *.out *.so *.ptx

VEC    := vectorAdd.out
MAT    := matrixMul.out
SERVER := server.out
VEC_KER:= vectorAdd_kernel.cu
MAT_KER:= matrixMul_kernel.cu
VEC_PTX:= vectorAdd_kernel.ptx
MAT_PTX:= matrixMul_kernel.ptx
VEC_SRC:= vectorAdd.cpp
MAT_SRC:= matrixMul.cpp
SO     := libmycudadrv.so

$(VEC): $(VEC_SRC) $(SO) $(VEC_PTX)
	$(CXX) $(INC) -L. -o $(VEC) $(VEC_SRC) -lmycudadrv

$(MAT): $(MAT_SRC) $(SO) $(MAT_PTX)
	$(CXX) $(INC) -L. -o $(MAT) $(MAT_SRC) -lmycudadrv 

$(SO): mycudadrv.cpp
	$(CXX) -shared -fPIC $(INC) -o $(SO) mycudadrv.cpp -ldl

$(VEC_PTX): $(VEC_KER)
	$(NVCC) $(NVCCFLAGS) $(VEC_KER)

$(MAT_PTX): $(MAT_KER)
	$(NVCC) $(NVCCFLAGS) $(MAT_KER)

$(SERVER): gpu-vm.cpp
	$(CXX) $(INC) -o $(SERVER) gpu-vm.cpp -ldl

# default rule
default: all

# phony rules
.PHONY: all
all: client server

.PHONY: client
client: $(VEC) $(MAT)

.PHONY: server
server: $(SERVER)

.PHONY: clean
clean:
	@echo CLEAN $(CLEAN_LIST)
	@rm -f $(CLEAN_LIST)
