# Copyright (c) 2018 Intel Corporation
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
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

include(Lint.cmake)

include(CustomProtoBuf.cmake)

if (WIN32)
set (Protobuf_PROTOC_EXECUTABLE protoc)
else()
set (Protobuf_PROTOC_EXECUTABLE ${CMAKE_BINARY_DIR}/protoc)
endif()

PROTOBUF_GENERATE_CPP(ProtoSources ProtoHeaders
  playback.proto
  )

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories (
  ${CMAKE_SOURCE_DIR}/retrace
  ${CMAKE_SOURCE_DIR}/lib/image
  ${CMAKE_BINARY_DIR}/dispatch
  ${CMAKE_SOURCE_DIR}/dispatch
  ${CMAKE_SOURCE_DIR}/common
  ${CMAKE_SOURCE_DIR}/helpers
  ${CMAKE_BINARY_DIR}/retrace/daemon
  ${CMAKE_SOURCE_DIR}/retrace/daemon/context
  ${CMAKE_SOURCE_DIR}/retrace/daemon/gldispatch
  ${CMAKE_SOURCE_DIR}/thirdparty/md5
  )

set (RETRACE_LINUX_SOURCE
  glframe_batch.cpp
  glframe_gpu_speed_linux.cpp
  glframe_os_linux.cpp
  glframe_perf_enabled_linux.cpp
  glframe_stderr.cpp
  glframe_thread_linux.cpp
  )

set (RETRACE_WIN_SOURCE
  glframe_os_win.cpp
  glframe_thread_win.cpp
  glframe_win_shaders.cpp
  )

if (WIN32)
  set (RETRACE_OS_SOURCE ${RETRACE_WIN_SOURCE})
else()
  set (RETRACE_OS_SOURCE ${RETRACE_LINUX_SOURCE})
endif()

set (RETRACE_SOURCES
  ${RETRACE_OS_SOURCE}
  glframe_batch.hpp
  glframe_gpu_speed.hpp
  glframe_metrics.cpp
  glframe_metrics.hpp
  glframe_metrics_intel.cpp
  glframe_metrics_intel.hpp
  glframe_os.hpp
  glframe_logger.cpp
  glframe_logger.hpp
  glframe_perf_enabled.hpp
  glframe_retrace.cpp
  glframe_retrace.hpp
  glframe_retrace_context.cpp
  glframe_retrace_context.hpp
  glframe_retrace_interface.hpp
  glframe_retrace_render.cpp
  glframe_retrace_render.hpp
  glframe_retrace_skeleton.cpp
  glframe_retrace_skeleton.hpp
  glframe_retrace_stub.cpp
  glframe_retrace_stub.hpp
  glframe_socket.hpp
  glframe_socket.cpp
  glframe_state.cpp
  glframe_state.hpp
  glframe_state_enums.cpp
  glframe_state_enums.hpp
  glframe_state_override.cpp
  glframe_state_override.hpp
  glframe_stderr.hpp
  glframe_texture_override.cpp
  glframe_texture_override.hpp
  glframe_thread.hpp
  glframe_traits.hpp
  glframe_uniforms.cpp
  glframe_uniforms.hpp
  glretrace_globals.cpp
  )  

add_library(retrace_daemon STATIC
  ${ProtoHeaders}
  ${ProtoSources}
  ${RETRACE_SOURCES}
  )

target_link_libraries(retrace_daemon
  retrace_context
  glretrace_common
  retrace_gldispatch
  libprotobuf
  )

add_dependencies(retrace_daemon protoc)

if (NOT WIN32)
  Lint( RETRACE_SOURCES )
endif()

add_subdirectory (framestat)
if (Qt5Widgets_FOUND)
  add_subdirectory (ui)
  add_subdirectory (bargraph)
endif ()
if (ENABLE_WAFFLE AND Qt5Widgets_FOUND)
  add_subdirectory (test)
endif ()
add_subdirectory (context)
add_subdirectory (server)
add_subdirectory (gldispatch)
