/**************************************************************************
 *
 * Copyright 2017 Intel Corporation
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * Authors:
 *   Mark Janes <mark.a.janes@intel.com>
 **************************************************************************/

#include <gtest/gtest.h>

#include <vector>

#include "glframe_mesh_optimize.hpp"

static const char *mesh_file = CMAKE_CURRENT_SOURCE_DIR "/test_verts.txt";

TEST(MeshTest, Optimize) {
  FILE *fh = fopen(mesh_file, "r");
  assert(fh != NULL);
  static const int kMaxLine = 1024;
  std::vector<char> line(kMaxLine);
  std::vector<uint32_t> verts;
  while (fgets(line.data(), kMaxLine, fh) != NULL)  {
    char *saveptr;
    char *pch = strtok_r(line.data(), " \n\t", &saveptr);
    while (pch) {
      verts.push_back(atoi(pch));
      pch = strtok_r(NULL, " \n\t", &saveptr);
    }
  }
}
