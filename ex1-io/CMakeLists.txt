cmake_minimum_required(VERSION 3.20.0)

project(toy-ex1-io LANGUAGES C CXX)

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

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE ex1-io.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${LLVM_INCLUDE_DIRS}
    ${MLIR_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} MLIRIR MLIRParser MLIRFuncDialect
    MLIRArithDialect)

add_test(NAME ${PROJECT_NAME} COMMAND $<TARGET_FILE:${PROJECT_NAME}>
    ${CMAKE_CURRENT_SOURCE_DIR}/ex1.mlir)
