cmake_minimum_required(VERSION 2.8)

find_package(FpSimGrpc CONFIG REQUIRED)

set (CMAKE_CXX_STANDARD 11)


add_library(FpSimulation STATIC
   "FpDisplayRendererGrpc.cpp"
)

target_include_directories(FpSimulation PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(FpSimulation
    FpSimGrpc::FpSimGrpc
    FpBase
    FpGfx
    FpDisplay
)