cmake_minimum_required(VERSION 3.20)
project(mywxproject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# -------------------------
# vcpkg dependency (always)
# -------------------------
find_package(fmt CONFIG REQUIRED)

# -------------------------
# wxWidgets
# -------------------------
if(WIN32)
    # Windows: from vcpkg
    find_package(wxWidgets CONFIG REQUIRED)
    set(WX_LIBS wx::core wx::base)
elseif(UNIX)
    # Linux: from system (bypassing vcpkg)
    find_program(WX_CONFIG wx-config REQUIRED)

    # Get flags
    execute_process(
        COMMAND ${WX_CONFIG} --cxxflags
        OUTPUT_VARIABLE WX_CXX_FLAGS
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    execute_process(
        COMMAND ${WX_CONFIG} --libs
        OUTPUT_VARIABLE WX_LIBS
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    # Convert flags string -> list
    separate_arguments(WX_CXX_FLAGS_LIST NATIVE_COMMAND "${WX_CXX_FLAGS}")
    separate_arguments(WX_LIBS_LIST NATIVE_COMMAND "${WX_LIBS}")
endif()

# -------------------------
# executable
# -------------------------
add_executable(${PROJECT_NAME} WIN32 src/main.cpp
)

target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<PLATFORM_ID:Linux>:${WX_CXX_FLAGS_LIST}>
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        fmt::fmt
        $<$<PLATFORM_ID:Windows>:wx::core wx::base>
        $<$<PLATFORM_ID:Linux>:${WX_LIBS_LIST}>
)

# -------------------------
# Create 'build/active' symlink for clangd
# -------------------------
if(EXISTS "${CMAKE_BINARY_DIR}")
    # Remove old symlink if it exists
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E remove -f "${CMAKE_SOURCE_DIR}/build/active"
    )
    # Create symlink pointing to current binary dir
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_BINARY_DIR}" "${CMAKE_SOURCE_DIR}/build/active"
        RESULT_VARIABLE symlink_result
    )
    if(NOT symlink_result EQUAL 0)
        message(WARNING "Failed to create build/active symlink for clangd")
    endif()
endif()