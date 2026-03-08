# CMake version requirements:
# - cmake --preset: >= 3.19
# - project requirement: >= 3.19
cmake_minimum_required(VERSION 3.19)

# Set default build type（project()より前に設定する必要がある）
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting default build type to Release")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

project(template_library_cpp
    VERSION 0.1.0
    LANGUAGES CXX C
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_POSITION_INDEPENDENT_CODE ON) # Enable position-independent code. add "-fPIC"

# --- コンパイラの確認 ---
# コンパイラは project() 呼び出し時に確定する。
# pixi run 経由で実行する場合は pixi がPATHをセットしてからCMakeを起動するため、
# pixi環境のコンパイラが自動的に検出される。
message(STATUS "C   compiler : ${CMAKE_C_COMPILER} (${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION})")
message(STATUS "C++ compiler : ${CMAKE_CXX_COMPILER} (${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION})")

# --- ccache ---
# pixi run 経由ではpixi環境のccacheがPATHに含まれるため NO_DEFAULT_PATH は使わない
find_program(CCACHE_EXE NAMES ccache)
if(CCACHE_EXE)
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_EXE}")
    set(CMAKE_C_COMPILER_LAUNCHER   "${CCACHE_EXE}")
    message(STATUS "ccache        : ${CCACHE_EXE}")
else()
    message(STATUS "ccache        : not found")
endif()
option(USE_PCH "Use precompiled headers" OFF)

# --- リンカー選択 ---
set(LINKER "auto" CACHE STRING "Linker to use (auto/mold/lld/bfd/default)")
set_property(CACHE LINKER PROPERTY STRINGS auto mold lld bfd default)

# AppleClang は -fuse-ld= オプション非対応のためリンカ切り替え不可
if (CMAKE_CXX_COMPILER_ID MATCHES "^(GNU|Clang)$")
    if (LINKER STREQUAL "auto")
        set(_candidates mold lld)
    elseif(LINKER STREQUAL "default")
        set(_candidates "")
    else()
        set(_candidates ${LINKER})
    endif()

    set(_linker_found FALSE)
    foreach(linker IN LISTS _candidates)
        if (linker STREQUAL "bfd")
            message(STATUS "linker        : bfd (system default)")
            set(_linker_found TRUE)
            break()
        endif()

        # pixi run 経由ではPATHにpixi環境のbinが含まれているため通常のfind_programで発見できる
        # mold: "mold", lld: "lld"（macOSではld.moldやld.lldは存在しない場合がある）
        find_program(_linker_path NAMES ${linker} ld.${linker})
        if (_linker_path)
            message(STATUS "linker        : ${linker} (${_linker_path})")
            add_link_options(-fuse-ld=${linker})
            set(_linker_found TRUE)
            unset(_linker_path CACHE)
            break()
        else()
            message(STATUS "linker        : ${linker} not found, trying next")
            unset(_linker_path CACHE)
        endif()
    endforeach()

    if(NOT _linker_found)
        message(STATUS "linker        : system default (no alternative found)")
    endif()
endif()

# compile_commands.json 生成とプロジェクトルートディレクトリへの自動配置
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_custom_target(copy_compile_commands ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different 
        ${CMAKE_BINARY_DIR}/compile_commands.json 
        ${CMAKE_SOURCE_DIR}/compile_commands.json
    DEPENDS ${CMAKE_BINARY_DIR}/compile_commands.json
    COMMENT "Copying compile_commands.json to project root"
)

include_directories(include)

# カバレッジフラグ（add_subdirectory より前に設定する必要がある）
include(cmake/coverage-flags.cmake)

# サニタイザフラグ（add_subdirectory より前に設定する必要がある）
include(cmake/sanitizer-flags.cmake)

# サードパーティライブラリの導入
include(FetchContent)
include(cmake/local-or-fetch.cmake)
include(cmake/dependencies-lib.cmake)
include(cmake/dependencies-test.cmake)

# ==============================================================================
# Library - メインライブラリ
# ==============================================================================

# メインのソースディレクトリ
add_subdirectory(src)

# Add debug flags for Debug and RelWithDebInfo builds
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0")
    message(STATUS "Debug symbols enabled for CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
elseif (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -g -O2")
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} -g -O2")
    message(STATUS "Release with debug symbols enabled for CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
endif()

# ==============================================================================
# Development utilities
# ==============================================================================

# Enable testing for CTest
enable_testing()

add_subdirectory(tests)
add_subdirectory(benches)

# カスタムターゲット
include(cmake/custom-targets.cmake)

# カバレッジ
include(cmake/coverage.cmake)

# コード品質ツール
include(cmake/quality-setup.cmake)

# ==============================================================================
# Third-Party License Aggregation
# ==============================================================================

# サードパーティライブラリのライセンス集約
add_custom_target(collect-licenses
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/third_party_licenses"

    # FetchContentで取得したライブラリのライセンス
    COMMAND ${CMAKE_COMMAND} -E echo "Collecting FetchContent package licenses..."
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/cmake/collect-fetchcontent-licenses.cmake

    COMMENT "Collecting third-party library licenses"
)

