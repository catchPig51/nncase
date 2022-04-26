/* Copyright 2019-2021 Canaan Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once
#include <nncase/kernels/kernel_context.h>
#include <nncase/runtime/datatypes.h>
#include <nncase/runtime/error.h>
#include <nncase/runtime/result.h>
#include <nncase/tensor.h>
#include <nncase/value.h>

BEGIN_NS_NNCASE_KERNELS

NNCASE_API result<tensor>
batch_to_space(tensor input, tensor block_shape, tensor crops,
               tensor output = nullptr,
               kernel_context &context = default_kernel_context()) noexcept;

NNCASE_API result<tensor>
broadcast(tensor input, tensor out_shape, tensor output = nullptr,
          kernel_context &context = default_kernel_context()) noexcept;

NNCASE_API result<tensor>
concat(tuple inputs, size_t axis, tensor output = nullptr,
       kernel_context &context = default_kernel_context()) noexcept;

NNCASE_API result<tensor>
convert(tensor input, datatype_t out_type, tensor output = nullptr,
        kernel_context &context = default_kernel_context()) noexcept;

NNCASE_API result<tensor>
copy(tensor input, tensor output = nullptr,
     kernel_context &context = default_kernel_context()) noexcept;

NNCASE_API result<tensor>
transpose(tensor input, tensor perm, tensor output = nullptr,
          kernel_context &context = default_kernel_context()) noexcept;

NNCASE_API result<void>
binary(tensor input, tensor perm, tensor output = nullptr,
       kernel_context &context = default_kernel_context()) noexcept;

NNCASE_API result<void>
dequantize(tensor input, tensor zero_point, tensor scale,
           tensor output = nullptr,
           kernel_context &context = default_kernel_context()) noexcept;

NNCASE_API result<void>
equal(tensor input_a, tensor input_b, tensor output = nullptr,
      kernel_context &context = default_kernel_context()) noexcept;

NNCASE_API result<void>
matmul(tensor input_a, tensor input_b, tensor output = nullptr,
       kernel_context &context = default_kernel_context()) noexcept;

NNCASE_API result<void>
onehot(datatype_t type, const int32_t *indices, gsl::byte *output,
       const runtime_shape_t &indices_shape, const runtime_shape_t &out_shape,
       const runtime_shape_t &out_strides, gsl::byte *depth,
       gsl::byte *off_value, gsl::byte *on_value, size_t axis,
       onehot_mode_t mode,
       kernel_context &context = default_kernel_context()) noexcept;

NNCASE_API result<void>
pad(datatype_t type, const gsl::byte *input, gsl::byte *output,
    const runtime_shape_t &in_shape, const runtime_shape_t &in_strides,
    const runtime_shape_t &out_strides, const runtime_paddings_t &paddings,
    pad_mode_t mode, const scalar &pad_value,
    kernel_context &context = default_kernel_context()) noexcept;

NNCASE_API result<void>
quantize(datatype_t in_type, datatype_t out_type, const gsl::byte *input,
         gsl::byte *output, const runtime_shape_t &in_shape,
         const runtime_shape_t &in_strides, const runtime_shape_t &out_strides,
         float scale, float bias,
         kernel_context &context = default_kernel_context()) noexcept;

NNCASE_API result<void>
unary(unary_op_t op, const float *input, float *output,
      const runtime_shape_t &shape, const runtime_shape_t &in_strides,
      const runtime_shape_t &out_strides,
      kernel_context &context = default_kernel_context()) noexcept;

NNCASE_API result<void>
reduce(reduce_op_t op, float init_value, const float *input, float *output,
       const runtime_shape_t &in_shape, const runtime_shape_t &axis,
       const runtime_shape_t &in_strides, const runtime_shape_t &out_strides,
       bool keep_dims,
       kernel_context &context = default_kernel_context()) noexcept;

template <typename T>
NNCASE_API result<void>
reduce_arg(reduce_arg_op_t op, const float *input, T *output,
           const runtime_shape_t &in_shape, const runtime_shape_t &in_strides,
           const runtime_shape_t &out_strides, const runtime_shape_t &axis,
           bool keep_dims, bool select_last_index,
           kernel_context &context = default_kernel_context()) noexcept;

template <typename T>
result<void> reduce_prod(const T *input, T *output,
                         const runtime_shape_t &in_shape,
                         const runtime_shape_t &in_strides,
                         const runtime_shape_t &out_strides,
                         const runtime_shape_t &axes, bool keep_dims) noexcept;

NNCASE_API result<void>
resize_bilinear(datatype_t type, const gsl::byte *input, gsl::byte *output,
                const runtime_shape_t &in_shape,
                const runtime_shape_t &in_strides,
                const runtime_shape_t &out_strides, int32_t out_h,
                int32_t out_w, bool align_corners, bool half_pixel_centers,
                kernel_context &context = default_kernel_context()) noexcept;

NNCASE_API result<void> resize_nearest_neighbor(
    datatype_t type, const gsl::byte *input, gsl::byte *output,
    const runtime_shape_t &in_shape, const runtime_shape_t &in_strides,
    const runtime_shape_t &out_strides, int32_t out_h, int32_t out_w,
    bool align_corners, bool half_pixel_centers,
    kernel_context &context = default_kernel_context()) noexcept;

NNCASE_API result<void>
slice(datatype_t type, const gsl::byte *input, gsl::byte *output,
      const runtime_shape_t &in_shape, const runtime_shape_t &in_strides,
      const runtime_shape_t &out_strides, const runtime_shape_t &begins,
      const runtime_axis_t &ends, const runtime_axis_t &strides,
      kernel_context &context = default_kernel_context()) noexcept;

NNCASE_API result<void>
gather(datatype_t in_type, const gsl::byte *input, gsl::byte *output,
       const runtime_shape_t &in_shape, const runtime_shape_t &out_shape,
       const runtime_shape_t &in_strides, const runtime_shape_t &out_strides,
       const int32_t *indices, const runtime_shape_t &indices_shape,
       int32_t axis,
       kernel_context &context = default_kernel_context()) noexcept;

NNCASE_API result<void>
gather_nd(datatype_t type, const gsl::byte *input, gsl::byte *output,
          const runtime_shape_t &in_shape, const runtime_shape_t &out_shape,
          const runtime_shape_t &in_strides, const runtime_shape_t &out_strides,
          const int32_t *indices, const runtime_shape_t &indices_shape,
          int32_t batch_dims,
          kernel_context &context = default_kernel_context()) noexcept;

template <typename T>
NNCASE_API result<void> cumsum(const T *input, T *output,
                               const runtime_shape_t &in_shape, int32_t axis,
                               bool exclusive, bool reverse) noexcept;

template <typename T>
NNCASE_API result<void> hardmax(const T *input, const runtime_shape_t &in_shape,
                                const runtime_shape_t &in_strides, T *output,
                                int32_t axis) noexcept;

template <typename T>
NNCASE_API result<void>
random_normal(T *output, const runtime_shape_t &out_shape, float mean,
              float std, float seed) noexcept;

template <typename T>
NNCASE_API result<void>
random_uniform(T *output, const runtime_shape_t &out_shape, float low,
               float high, float seed) noexcept;

template <typename T>
NNCASE_API result<void>
roi_align(const T *input, const T *rois, int64_t *batch_indices, T *output,
          const runtime_shape_t &in_shape, const runtime_shape_t &out_shape,
          roi_align_mode_t mode, float spatial_scale,
          int64_t sampling_ratio) noexcept;

template <typename T>
NNCASE_API result<void> sigmoid(const T *input, T *output,
                                const runtime_shape_t &in_shape,
                                const runtime_shape_t &in_strides) noexcept;

template <typename T>
NNCASE_API result<void>
ternary(const float *input_a, const T *input_b, const T *input_c, T *output,
        const runtime_shape_t &in_a_shape, const runtime_shape_t &in_a_strides,
        const runtime_shape_t &in_b_shape, const runtime_shape_t &in_b_strides,
        const runtime_shape_t &in_c_shape, const runtime_shape_t &in_c_strides,
        const runtime_shape_t &out_strides) noexcept;

template <typename T>
NNCASE_API result<void>
topk(const T *input, T *output_values, int64_t *output_indices,
     const runtime_shape_t &in_shape, const runtime_shape_t &in_strides,
     const runtime_shape_t &output_values_shape,
     const runtime_shape_t &output_values_strides,
     const runtime_shape_t &output_indices_shape,
     const runtime_shape_t &output_indices_strides, const int64_t k,
     const int32_t axis, const bool largest, const bool sorted) noexcept;

template <typename T>
NNCASE_API result<void> trilu(const T *input, T *output,
                              const runtime_shape_t &in_shape, const bool upper,
                              const int64_t k) noexcept;

END_NS_NNCASE_KERNELS
