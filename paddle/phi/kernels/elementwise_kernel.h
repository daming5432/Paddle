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

#include "paddle/phi/core/dense_tensor.h"
#include "paddle/phi/infermeta/binary.h"

namespace phi {

template <typename T, typename Context>
void FMaxKernel(const Context& dev_ctx,
                const DenseTensor& x,
                const DenseTensor& y,
                int axis,
                DenseTensor* out);

template <typename T, typename Context>
void FMinKernel(const Context& dev_ctx,
                const DenseTensor& x,
                const DenseTensor& y,
                int axis,
                DenseTensor* out);

template <typename T, typename Context>
void AddRawKernel(const Context& dev_ctx,
                  const DenseTensor& x,
                  const DenseTensor& y,
                  int axis,
                  DenseTensor* out);

template <typename T, typename Context>
void AddKernel(const Context& dev_ctx,
               const DenseTensor& x,
               const DenseTensor& y,
               DenseTensor* out);

template <typename T, typename Context>
void SubtractRawKernel(const Context& dev_ctx,
                       const DenseTensor& x,
                       const DenseTensor& y,
                       int axis,
                       DenseTensor* out);

template <typename T, typename Context>
void SubtractKernel(const Context& dev_ctx,
                    const DenseTensor& x,
                    const DenseTensor& y,
                    DenseTensor* out);

template <typename T, typename Context>
void DivideRawKernel(const Context& dev_ctx,
                     const DenseTensor& x,
                     const DenseTensor& y,
                     int axis,
                     DenseTensor* out);

template <typename T, typename Context>
void DivideKernel(const Context& dev_ctx,
                  const DenseTensor& x,
                  const DenseTensor& y,
                  DenseTensor* out);

template <typename T, typename Context>
void MultiplyRawKernel(const Context& dev_ctx,
                       const DenseTensor& x,
                       const DenseTensor& y,
                       int axis,
                       DenseTensor* out);

template <typename T, typename Context>
void MultiplyKernel(const Context& dev_ctx,
                    const DenseTensor& x,
                    const DenseTensor& y,
                    DenseTensor* out);

template <typename T, typename Context>
DenseTensor Add(const Context& dev_ctx,
                const DenseTensor& x,
                const DenseTensor& y) {
  DenseTensor dense_out;
  MetaTensor meta_out(&dense_out);
  ElementwiseInferMeta(x, y, &meta_out);
  AddKernel<T, Context>(dev_ctx, x, y, &dense_out);
  return dense_out;
}

template <typename T, typename Context>
DenseTensor Subtract(const Context& dev_ctx,
                     const DenseTensor& x,
                     const DenseTensor& y) {
  DenseTensor dense_out;
  MetaTensor meta_out(&dense_out);
  ElementwiseInferMeta(x, y, &meta_out);
  SubtractKernel<T, Context>(dev_ctx, x, y, &dense_out);
  return dense_out;
}

template <typename T, typename Context>
DenseTensor Divide(const Context& dev_ctx,
                   const DenseTensor& x,
                   const DenseTensor& y) {
  DenseTensor dense_out;
  MetaTensor meta_out(&dense_out);
  ElementwiseInferMeta(x, y, &meta_out);
  DivideKernel<T, Context>(dev_ctx, x, y, &dense_out);
  return dense_out;
}

template <typename T, typename Context>
DenseTensor Multiply(const Context& dev_ctx,
                     const DenseTensor& x,
                     const DenseTensor& y) {
  DenseTensor dense_out;
  MetaTensor meta_out(&dense_out);
  ElementwiseInferMeta(x, y, &meta_out);
  MultiplyKernel<T, Context>(dev_ctx, x, y, &dense_out);
  return dense_out;
}

}  // namespace phi
