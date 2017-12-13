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

#include <map>
#include <vector>

#include "glframe_mesh_optimize.hpp"

static const char *mesh_file = CMAKE_CURRENT_SOURCE_DIR "/test_verts.txt";

float
average_distance(const std::vector<uint32_t> v) {
  std::vector<int> distance;
  std::map<uint32_t, int> seen;
  for (int i = 0; i < v.size(); ++i) {
    uint32_t vert = v[i];
    if (seen.find(vert) == seen.end()) {
      seen[vert] = i;
      continue;
    }
    const int last_seen = seen[vert];
    distance.push_back(i - last_seen);
    seen[vert] = i;
  }
  float total = 0;
  int cache_misses = 0;
  for (auto d : distance) {
    if (d > 300)
      ++cache_misses;
    total += d;
  }
  std::cout << "misses: " << cache_misses << "\n";
  return total / distance.size();
}

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
  std::cout << "initial distance" << average_distance(verts) << "\n";
  retrace::optimize_mesh(&verts);
  std::cout << "final distance" << average_distance(verts) << "\n";
  int v = 0;
  for (auto i : verts) {
    std::cout << i;
    if (++v < 3) {
      std::cout << "\t";
    } else {
      std::cout << "\n";
      v = 0;
    }
  }
}
