﻿// Copyright (c) Canaan Inc. All rights reserved.
// Licensed under the Apache license. See LICENSE file in the project root for full license information.

using System;
using System.Collections.Generic;
using System.Collections.Immutable;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Nncase.IR
{
    /// <summary>
    /// Parameter information.
    /// </summary>
    public sealed record ParameterInfo(string Name);

    /// <summary>
    /// Operator expression.
    /// </summary>
    public abstract record Op(ImmutableArray<ParameterInfo> Parameters) : Expr
    {
        /// <summary>
        /// Inference type.
        /// </summary>
        /// <param name="context">Context.</param>
        /// <returns>Inferred type.</returns>
        public abstract Type InferInvokeResultType(ITypeInferenceContext context);
    }
}
