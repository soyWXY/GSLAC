# tool macros
CXX := g++
CXXFLAGS := -std=c++11
INC := -I/usr/local/cuda/include
LIB := -L/usr/local/cuda/lib64
LDFLAGS := -lcuda -lcudart

NVCC := nvcc
NVCCFLAGS := -ptx

CLEAN_LIST := *.out *.so *.ptx

CLIENT := client.out
SERVER := server.out
KERNEL := vectorAdd_kernel.cu
PTX := vectorAdd_kernel.ptx
SRC := vectorAdd.cpp
SO := libmycudadrv.so

$(CLIENT): $(SRC) $(SO) $(PTX)
	$(CXX) $(INC) -L. -o $(CLIENT) $(SRC) -lmycudadrv 
$(SO): mycudadrv.cpp
	$(CXX) -shared -fPIC $(INC) -o $(SO) mycudadrv.cpp -ldl

$(PTX): $(KERNEL)
	$(NVCC) $(NVCCFLAGS) $(KERNEL)

$(SERVER): gpu-vm.cpp
	$(CXX) $(INC) -o $(SERVER) gpu-vm.cpp -ldl

# default rule
default: all

# phony rules
.PHONY: all
all: client server

.PHONY: client
client: $(CLIENT)

.PHONY: server
server: $(SERVER)

.PHONY: clean
clean:
	@echo CLEAN $(CLEAN_LIST)
	@rm -f $(CLEAN_LIST)