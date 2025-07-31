add_executable(image_tool image_tool.cpp)
target_link_libraries(image_tool
        PRIVATE
        image_tool_core
        CLI11::CLI11
)
set_target_properties(image_tool PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)


#add_executable(image_compare image_compare.cpp)
#target_link_libraries(image_compare
#        PRIVATE
#        image_tool_core
#        CLI11::CLI11
#)
#set_target_properties(image_compare PROPERTIES
#        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
#)
#
#
#add_executable(image_quality image_quality.cpp)
#target_link_libraries(image_quality
#        PRIVATE
#        image_tool_core
#        CLI11::CLI11
#)
#set_target_properties(image_quality PROPERTIES
#        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
#)
