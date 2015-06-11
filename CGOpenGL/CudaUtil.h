#pragma once

#include "Debug.h"

#ifdef _DEBUG
#define CHECK_CUDA_ERROR( ) do \
	    {                                      \
  cudaError err = cudaThreadSynchronize();                                 \
  if ( cudaSuccess != err)  \
      {                                               \
    Debug::LogFailure(__FILE__, __LINE__, cudaGetErrorString( err));              \
      } } while (0)
#else // _DEBUG
#define CHECK_CUDA_ERROR( )
#endif // _DEBUG