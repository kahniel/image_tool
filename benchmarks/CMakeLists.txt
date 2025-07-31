function(add_benchmark name source)
    add_executable(${name} ${source})
    target_link_libraries(${name}
            PRIVATE
            image_tool_core
    )

    set_target_properties(${name} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
endfunction()

add_benchmark(downsize_benchmark downsize_benchmark.cpp)
add_benchmark(upsize_benchmark upsize_benchmark.cpp)
add_benchmark(evaluate_benchmark evaluate_benchmark.cpp)