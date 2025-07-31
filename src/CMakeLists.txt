set(CORE_SOURCES
        resizers/file_resizer.cpp
        resizers/edsr_neural_resizer.cpp
        resizers/my_resizer.cpp
        resizers/opencv_resizer.cpp
        resizers/real_esrgan_neural_resizer.cpp

        rgb/rgb_image.cpp
        rgb/rgb_pixel.cpp

        metrics/file_comparer.cpp
        metrics/file_quality_evaluator.cpp

        utils/path_utils.cpp
)

add_library(image_tool_core ${CORE_SOURCES})

target_include_directories(image_tool_core
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(image_tool_core
        PUBLIC
        ${OpenCV_LIBS}
)

target_compile_options(image_tool_core PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
)