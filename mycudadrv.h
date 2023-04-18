#ifndef MYCUDADRV_H_
#define MYCUDADRV_H

#include <cuda.h>

extern CUresult cuInit(unsigned int flags);
extern CUresult cuDeviceGet(CUdevice* device, int ordinal);
extern CUresult cuCtxCreate(CUcontext* pctx, unsigned int flags, CUdevice dev);
extern CUresult cuModuleLoad(CUmodule* module, const char* fname);
extern CUresult cuModuleGetFunction(CUfunction* hfunc, CUmodule hmod, const char* name);

#endif /* MYCUDADRV_H_ */
