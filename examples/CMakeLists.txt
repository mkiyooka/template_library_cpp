set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples)

add_executable(calculator_demo
    calculator_demo.cpp
)
target_link_libraries(calculator_demo PRIVATE template_library_cpp)
