//===-- umodti3.c - Implement __umodti3 -----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements __umodti3 for the compiler_rt library.
//
//===----------------------------------------------------------------------===//

#include "int_t.h"
#include "udivmodti4.c"

// Returns: a % b

tu_int ___umodti3(tu_int a, tu_int b) {
  tu_int r;
  __udivmodti4(a, b, &r);
  return r;
}
