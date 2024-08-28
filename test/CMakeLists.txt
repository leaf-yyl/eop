



cmake_minimum_required(VERSION 3.15)

project(eoltest)
set(Target eoltest)

# 指定cpp文件
set(op_src ${CMAKE_SOURCE_DIR}/main.cc)
file(GLOB_RECURSE onnx_src ${CMAKE_SOURCE_DIR}/onnx/*.cc)
set(srcs ${op_src} ${onnx_src})

# 添加头文件目录和链接目录
set(SDK_DIR $ENV{DLICC_PATH}/../)
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/../install/include)
include_directories(${CMAKE_SOURCE_DIR}/googletest/googletest/include)

link_directories(${CMAKE_SOURCE_DIR}/../build/eol)
link_directories(${CMAKE_SOURCE_DIR}/../build/googletest/lib)
link_directories(${SDK_DIR}/lib)

# 根据需要设置相应的编译选项
set(CXX_FLAGS -std=c++17 -fPIC -fpermissive -Wno-attributes)
# 兼容老版本gcc
add_compile_options(-std=c++17)
add_executable(${Target} ${srcs})
set_target_properties(${Target} PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(${Target} PRIVATE gtest eol pthread)
target_compile_options(${Target} PRIVATE ${CXX_FLAGS})
