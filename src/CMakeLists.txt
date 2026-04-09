# BUILD_SHARED_LIBS=ON で共有ライブラリ、OFF（デフォルト）で静的ライブラリとしてビルドされる
add_library(template_library_cpp
    template_library_cpp.cpp
)
target_include_directories(template_library_cpp
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
set_target_properties(template_library_cpp PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)
add_library(template_library_cpp::template_library_cpp ALIAS template_library_cpp)
