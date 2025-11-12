cmake_minimum_required(VERSION 3.20.0)

project(toy-ex5-pass LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # 生成 compile_commands.json 便于代码高亮
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

find_package(LLVM REQUIRED CONFIG)
find_package(MLIR REQUIRED CONFIG)
list(APPEND CMAKE_MODULE_PATH "${MLIR_CMAKE_DIR}")
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")

include(TableGen)
include(AddLLVM)
include(AddMLIR)
include(HandleLLVMOptions)

set(LLVM_RUNTIME_OUTPUT_INTDIR ${CMAKE_BINARY_DIR})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../ex3-dialect/include)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/../ex3-dialect/include)
include_directories(${MLIR_INCLUDE_DIRS})

add_subdirectory(include/toy)
add_subdirectory(lib)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE ex5-pass.cpp)

target_link_libraries(${PROJECT_NAME}
  PRIVATE Toy Toy5
  MLIRIR MLIRParser MLIRSupport
  MLIRFuncDialect   # 对应 #include "mlir/Dialect/Func/IR/FuncOps.h"
  MLIRTransforms    # 对应 #include "mlir/Transforms/Passes.h"
)

add_test(NAME ${PROJECT_NAME} COMMAND $<TARGET_FILE:${PROJECT_NAME}>)
