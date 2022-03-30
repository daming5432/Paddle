// Copyright (c) 2022 PaddlePaddle Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#if defined(__NVCC__) || defined(__HIPCC__)
#include "paddle/phi/kernels/primitive/functor_primitives.h"
#ifdef __NVCC__
#include "cub/cub.cuh"
#else
#include <hipcub/hipcub.hpp>
namespace cub = hipcub;
#endif
#endif

#include "paddle/fluid/memory/buffer.h"
#include "paddle/phi/core/dense_tensor.h"
#include "paddle/phi/kernels/funcs/eigen/common.h"
#include "paddle/phi/kernels/funcs/eigen/eigen_function.h"

namespace phi {
namespace funcs {

template <typename DeviceContext, typename T1, typename T2 = T1>
void SquaredL2Norm(const DeviceContext& ctx,
                   const T1* x,
                   T2* y,
                   size_t numel,
                   paddle::memory::Buffer* buffer = nullptr) {
  if (std::is_same<T1, T2>::value) {
    using EigenT = typename EigenTensor<T1, 1>::Type;
    using ConstEigenT = typename EigenTensor<T1, 1>::ConstType;
    using EigenDim = typename EigenDim<1>::Type;
    ConstEigenT input(x, EigenDim(numel));
    EigenT output(reinterpret_cast<T1*>(y), EigenDim(1));
    output.device(*ctx.eigen_device()) = input.square().sum();
  } else {
    T2 ret = static_cast<T2>(0);
    for (size_t i = 0; i < numel; ++i) {
      auto tmp = static_cast<T2>(x[i]);
      ret += tmp * tmp;
    }
    *y = ret;
  }
}

#if defined(__NVCC__) || defined(__HIPCC__)
template <typename T1, typename T2 = T1>
void SquaredL2Norm(const DeviceContext& ctx,
                   const T1* x,
                   T2* y,
                   size_t numel,
                   paddle::memory::Buffer* buffer = nullptr) {
  if (UNLIKELY(buffer == nullptr)) {
    paddle::memory::Buffer tmp_buffer(ctx.GetPlace());
    return SquaredL2Norm(ctx, x, y, numel, &tmp_buffer);
  }

  using FunctorT = kernel_primitives::SquareFunctor<T1, T2>;
  cub::TransformInputIterator<T2, FunctorT, const T1*> iter(x, FunctorT());
  size_t temp_storage_bytes = 0;
  void* d_temp_storage = nullptr;
  auto stream = ctx.stream();
#pragma unroll 2
  for (size_t i = 0; i < 2; ++i) {
    if (temp_storage_bytes > 0) {
      d_temp_storage = buffer->Alloc<void>(temp_storage_bytes);
    }
    PADDLE_ENFORCE_GPU_SUCCESS(cub::DeviceReduce::Reduce(d_temp_storage,
                                                         temp_storage_bytes,
                                                         iter,
                                                         y,
                                                         numel,
                                                         cub::Sum(),
                                                         static_cast<T2>(0)));
  }
}
#endif

}  // namespace funcs
}  // namespace phi
