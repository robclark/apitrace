// Copyright (C) Intel Corp.  2015.  All Rights Reserved.

// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:

// The above copyright notice and this permission notice (including the
// next paragraph) shall be included in all copies or substantial
// portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE COPYRIGHT OWNER(S) AND/OR ITS SUPPLIERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

//  **********************************************************************/
//  * Authors:
//  *   Mark Janes <mark.a.janes@intel.com>
//  **********************************************************************/

#ifndef _GLFRAME_BARGRAPH_HPP_
#define _GLFRAME_BARGRAPH_HPP_

#include <GLES2/gl2.h>

#include <vector>

namespace glretrace {


// helper class to encapsulate color, dimension and placement of a
// metric bar.  Coordinates indicate the lower left/top right of the
// bar.  The coordinate system is 0.0 - 1.0 on both axis.
class BarMetrics {
 public:
  float metric1;  // bar height
  float metric2;  // bar width
  bool selected;
};

// - BarGraphRenderer
//   - draws image, including all details:
//     - selected ergs
//     - mouse area
//     - bars
//   - Independent of Qt
class BarGraphRenderer {
 public:
  explicit BarGraphRenderer(bool invert = false);  // Qt draws top-to-bottom
  void setBars(const std::vector<BarMetrics> &bars);
  void setMouseArea(float x1, float y1, float x2, float y2);
  void render();

 private:
  static const char *vshader, *fshader;
  GLuint vbo;
  GLint att_coord, uni_max_x, uni_max_y, uni_invert_y, uni_bar_color, prog;
  struct Vertex {
    float x;
    float y;
  };

  std::vector<Vertex> vertices;
  float max_y, total_x, invert_y;
};

}  // namespace glretrace

#endif  // _GLFRAME_BARGRAPH_HPP_