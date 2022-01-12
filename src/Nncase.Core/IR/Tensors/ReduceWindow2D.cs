// Copyright (c) Canaan Inc. All rights reserved.
// Licensed under the Apache license. See LICENSE file in the project root for full license information.

using static Nncase.IR.Utility;

namespace Nncase.IR.Tensors
{
    /// <summary>
    /// ReduceWindow2D.
    /// </summary>
    public sealed record ReduceWindow2D(ReduceOp ReduceOp) : Op
    {
        /// <summary>
        /// Gets input.
        /// </summary>
        public static readonly ParameterInfo Input = new(typeof(ReduceWindow2D), 0, "input", HasRank(4));

        /// <summary>
        /// Get initValue.
        /// </summary>
        public static readonly ParameterInfo InitValue = new(typeof(ReduceWindow2D), 1, "initValue", IsScalar());

        /// <summary>
        /// Get filter.
        /// </summary>
        public static readonly ParameterInfo Filter = new(typeof(ReduceWindow2D), 2, "filter", HasRank(1) & IsIntegral());

        /// <summary>
        /// Gets Stride.
        /// </summary>
        public static readonly ParameterInfo Stride = new(typeof(ReduceWindow2D), 3, "stride", HasRank(1) & IsIntegral());

        /// <summary>
        /// Gets Padding.
        /// </summary>
        public static readonly ParameterInfo Padding = new(typeof(ReduceWindow2D), 4, "padding", HasRank(2) & IsIntegral());

        /// <summary>
        /// Gets CeilMode.
        /// </summary>
        public static readonly ParameterInfo CeilMode = new(typeof(ReduceWindow2D), 6, "ceilMode", IsBool());

        /// <summary>
        /// Gets CountIncludePad.
        /// </summary>
        public static readonly ParameterInfo CountIncludePad = new(typeof(ReduceWindow2D), 6, "countIncludePad", IsBool());

        /// <inheritdoc/>
        public IRType InferInvokeResultType(ITypeInferenceContext context, TensorType input, TensorType initValue, TensorType filter, TensorType stride, TensorType padding, TensorType ceilMode, TensorType countIncludePad)
        {
            var args = context.GetArguments(this, Filter, Stride, Padding, CeilMode);
            return TypeInference.ReduceWindow2DType(input, args[0], args[1], args[2], args[3]);
        }
    }
}