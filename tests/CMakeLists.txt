# テストバイナリは build/tests/ 以下
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests)

# doctest 記述パターンのサンプルテスト
add_executable(test_doctest_usage
    test_doctest_usage.cpp
)
target_include_directories(test_doctest_usage PRIVATE
    ${PROJECT_SOURCE_DIR}/tests
)
target_link_libraries(test_doctest_usage PRIVATE doctest::doctest)
add_test(
    NAME test_doctest_usage
    COMMAND $<TARGET_FILE:test_doctest_usage>
)

# ライブラリのサンプルテスト
add_executable(test_template_library_cpp
    test_template_library_cpp.cpp
)
target_include_directories(test_template_library_cpp PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
target_link_libraries(test_template_library_cpp PRIVATE
    template_library_cpp
    doctest::doctest
)
add_test(
    NAME test_template_library_cpp
    COMMAND $<TARGET_FILE:test_template_library_cpp>
)
