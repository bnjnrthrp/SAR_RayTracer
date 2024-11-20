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
add_executable (SAR_RayTracer "src/SAR_RayTracer.cpp"   "include/vec3.h" "include/ray.h" "include/hittable.h" "include/sphere.h" "include/hittable_list.h" "include/camera.h" "include/material.h" "include/common.h" "include/color.h"  "src/newMain.cpp" "include/interval.h" "include/aabb.h" "include/bvh.h" "include/texture.h" "include/rtw_stb_image.h" "include/perlin.h" "include/quad.h" "include/constant_medium.h" "src/monte.cpp" "src/monteCarlo.cpp" "include/onb.h" "include/pdf.h" "include/triangle.h" "external/assimp/aabb.h" "external/assimp/ai_assert.h" "external/assimp/anim.h" "external/assimp/AssertHandler.h" "external/assimp/BaseImporter.h" "external/assimp/Bitmap.h" "external/assimp/BlobIOSystem.h" "external/assimp/ByteSwapper.h" "external/assimp/camera.h" "external/assimp/cexport.h" "external/assimp/cfileio.h" "external/assimp/cimport.h" "external/assimp/ColladaMetaData.h" "external/assimp/color4.h" "external/assimp/commonMetaData.h" "external/assimp/config.h" "external/assimp/CreateAnimMesh.h" "external/assimp/DefaultIOStream.h" "external/assimp/DefaultIOSystem.h" "external/assimp/defs.h" "external/assimp/Exceptional.h" "external/assimp/fast_atof.h" "external/assimp/GenericProperty.h" "external/assimp/GltfMaterial.h" "external/assimp/Hash.h" "external/assimp/importerdesc.h" "external/assimp/IOStreamBuffer.h" "external/assimp/light.h" "external/assimp/LineSplitter.h" "external/assimp/LogAux.h" "external/assimp/material.h" "external/assimp/MathFunctions.h" "external/assimp/matrix3x3.h" "external/assimp/matrix4x4.h" "external/assimp/MemoryIOWrapper.h" "external/assimp/mesh.h" "external/assimp/metadata.h" "external/assimp/ObjMaterial.h" "external/assimp/ParsingUtils.h" "external/assimp/pbrmaterial.h" "external/assimp/postprocess.h" "external/assimp/Profiler.h" "external/assimp/qnan.h" "external/assimp/quaternion.h" "external/assimp/RemoveComments.h" "external/assimp/scene.h" "external/assimp/SceneCombiner.h" "external/assimp/SGSpatialSort.h" "external/assimp/SkeletonMeshBuilder.h" "external/assimp/SmallVector.h" "external/assimp/SmoothingGroups.h" "external/assimp/SpatialSort.h" "external/assimp/StandardShapes.h" "external/assimp/StreamReader.h" "external/assimp/StreamWriter.h" "external/assimp/StringComparison.h" "external/assimp/StringUtils.h" "external/assimp/Subdivision.h" "external/assimp/texture.h" "external/assimp/TinyFormatter.h" "external/assimp/types.h" "external/assimp/vector2.h" "external/assimp/vector3.h" "external/assimp/version.h" "external/assimp/Vertex.h" "external/assimp/XmlParser.h" "external/assimp/XMLTools.h" "external/assimp/ZipArchiveIOSystem.h" "external/assimp/Compiler/poppack1.h" "external/assimp/Compiler/pstdint.h" "external/assimp/Compiler/pushpack1.h" "external/assimp/port/AndroidJNI/AndroidJNIIOSystem.h" "external/assimp/port/AndroidJNI/BundledAssetIOSystem.h" "include/model.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SAR_RayTracer PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
