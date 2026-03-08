# ベンチマークバイナリは build/benches/ 以下
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/benches)

# ライブラリのサンプルベンチマーク
add_executable(bench_template_library_cpp
    bench_template_library_cpp.cpp
)
target_include_directories(bench_template_library_cpp PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
target_link_libraries(bench_template_library_cpp PRIVATE
    template_library_cpp
    nanobench::nanobench
)
