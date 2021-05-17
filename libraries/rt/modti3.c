//===-- modti3.c - Implement __modti3 -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements __modti3 for the compiler_rt library.
//
//===----------------------------------------------------------------------===//

#include "int_t.h"
#include "udivmodti4.c"

// Returns: a % b

#define fixint_t ti_int
#define fixuint_t tu_int
#define ASSIGN_UMOD(res, a, b) __udivmodti4((a), (b), &(res))
#include "int_div_impl.inc"

ti_int ___modti3(ti_int a, ti_int b) { return __modXi3(a, b); }
