function(add_benchmark name source)
    add_executable(${name} ${source})
    target_link_libraries(${name}
            PRIVATE
            image_tool_core
            CLI11::CLI11
    )

    set_target_properties(${name} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
endfunction()

add_benchmark(prepare_data prepare_data.cpp)
add_benchmark(resize_performance resize_performance.cpp)
add_benchmark(quality_evaluation quality_evaluation.cpp)