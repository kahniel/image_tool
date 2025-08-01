add_executable(image_tool image_tool.cpp)
target_link_libraries(image_tool
        PRIVATE
        image_tool_core
        CLI11::CLI11
)
set_target_properties(image_tool PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)