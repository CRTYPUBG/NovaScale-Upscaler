cmake_minimum_required(VERSION 3.16)
project(NovaScale C)

set(CMAKE_C_STANDARD 11)

# DirectX libraries
link_libraries(d3d11 dxgi d3dcompiler winmm dxguid)

add_library(novascale SHARED
    core/main.c
    core/capture_dxgi.c
    core/present_dx11.c
    core/upscale_basic.c
    # core/upscale_spatial.c
    # core/upscale_cnn.c
    # core/temporal.c
)

target_include_directories(novascale PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/core)
set_target_properties(novascale PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/ui")
