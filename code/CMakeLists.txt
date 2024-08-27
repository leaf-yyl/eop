



cmake_minimum_required(VERSION 3.15)

project(eol)
set(Target eol)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/../install CACHE PATH ${CMAKE_SOURCE_DIR}/../install FORCE)
endif()

# 指定h文件
file(GLOB headers ${CMAKE_SOURCE_DIR}/include/*.h)

# 指定cpp文件
file(GLOB_RECURSE op_src ${CMAKE_SOURCE_DIR}/op/*.cc)
file(GLOB_RECURSE res_src ${CMAKE_SOURCE_DIR}/res/*.cc)
file(GLOB_RECURSE pass_src ${CMAKE_SOURCE_DIR}/pass/*.cc)
file(GLOB_RECURSE utils_src ${CMAKE_SOURCE_DIR}/utils/*.cc)
set(srcs ${op_src} ${res_src} ${pass_src} ${utils_src})

# 添加头文件目录和链接目录
set(SDK_DIR $ENV{DLICC_PATH}/../)
include_directories(${SDK_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/)
link_directories(${SDK_DIR}/lib)

# 根据需要设置相应的编译选项
set(CXX_FLAGS -std=c++17 -fPIC -fpermissive -Wno-attributes)
# 兼容老版本gcc
add_compile_options(-std=c++17)
add_library(${Target} SHARED ${srcs})
set_target_properties(${Target} PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(${Target} PRIVATE curt)
target_compile_options(${Target} PRIVATE -I${SDK_DIR}/include -I${CMAKE_BINARY_DIR} ${CXX_FLAGS})
install(FILES ${headers} DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
install(TARGETS ${Target} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
