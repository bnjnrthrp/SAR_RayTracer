# CMakeList.txt : CMake project for SAR_RayTracer, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("SAR_RayTracer")

# Add source to this project's executable.
add_executable (SAR_RayTracer "src/SAR_RayTracer.cpp"   "include/vec3.h" "include/ray.h" "include/hittable.h" "include/sphere.h" "include/hittable_list.h" "include/camera.h" "include/material.h" "include/common.h" "include/color.h"  "src/newMain.cpp" "include/interval.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SAR_RayTracer PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
