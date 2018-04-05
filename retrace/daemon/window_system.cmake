###########################################################################
 #
 # Copyright 2018 Intel Corporation
 # All Rights Reserved.
 #
 # Permission is hereby granted, free of charge, to any person obtaining a copy
 # of this software and associated documentation files (the "Software"), to deal
 # in the Software without restriction, including without limitation the rights
 # to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 # copies of the Software, and to permit persons to whom the Software is
 # furnished to do so, subject to the following conditions:
 #
 # The above copyright notice and this permission notice shall be included in
 # all copies or substantial portions of the Software.
 #
 # THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 # IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 # FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 # AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 # LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 # OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 # THE SOFTWARE.
 #
 # Authors:
 #   Mark Janes <mark.a.janes@intel.com>
###########################################################################

# define window system sources/includes/libs for all executables
# avoid linking against X11 if EGL/Waffle is enabled

add_definitions (-DRETRACE)

if (WIN32)
  set (glws_os
    ${CMAKE_SOURCE_DIR}/retrace/glws_wgl.cpp)
else ()
  if (ENABLE_WAFFLE)
    set (glws_os
      ${CMAKE_SOURCE_DIR}/retrace/glws_waffle.cpp)
    set (ws_lib ${Waffle_LIBRARIES})
    set (ws_include ${Waffle_INCLUDE_DIRS})
  else()
    set (glws_os
      ${CMAKE_SOURCE_DIR}/retrace/glws_xlib.cpp
      ${CMAKE_SOURCE_DIR}/retrace/glws_egl_xlib.cpp)
    set (ws_lib ${X11_X11_LIB})
    set (ws_include ${CMAKE_SOURCE_DIR}/thirdparty/khronos)
  endif ()
endif ()
