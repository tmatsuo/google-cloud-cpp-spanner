# ~~~
# Copyright 2019 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ~~~

find_package(google_cloud_cpp_common CONFIG REQUIRED)
find_package(google_cloud_cpp_grpc_utils CONFIG REQUIRED)

include(EnableClangTidy)
include(EnableWerror)
set(DOXYGEN_PROJECT_NAME "Google Cloud Spanner C++ Client")
set(DOXYGEN_PROJECT_BRIEF "A C++ Client Library for Google Cloud Spanner")
set(DOXYGEN_PROJECT_NUMBER "${SPANNER_CLIENT_VERSION}")
set(DOXYGEN_EXAMPLE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/samples
                         ${CMAKE_CURRENT_SOURCE_DIR}/../../../quickstart)
include(EnableDoxygen)

# Define a function to fetch the current git revision. Using a function creates
# a new scope, so the CMake variables do not leak to the global namespace.
function (google_cloud_cpp_spanner_initialize_git_head var)
    set(result "unknown")
    # If we cannot find a `.git` directory do not even try to guess the git
    # revision.
    if (IS_DIRECTORY ${PROJECT_SOURCE_DIR}/.git)
        # We need `git` to find the revision.
        find_program(GOOGLE_CLOUD_CPP_SPANNER_GIT_PROGRAM NAMES git)
        mark_as_advanced(GOOGLE_CLOUD_CPP_SPANNER_GIT_PROGRAM)
        if (GOOGLE_CLOUD_CPP_SPANNER_GIT_PROGRAM)
            # Run `git rev-parse --short HEAD` and capture the output in a
            # variable.
            execute_process(
                COMMAND "${GOOGLE_CLOUD_CPP_SPANNER_GIT_PROGRAM}" rev-parse
                        --short HEAD
                OUTPUT_VARIABLE GIT_HEAD_LOG
                ERROR_VARIABLE GIT_HEAD_LOG)
            string(REPLACE "\n" "" result "${GIT_HEAD_LOG}")
        endif ()
    endif ()
    set(${var}
        "${result}"
        PARENT_SCOPE)
endfunction ()

# Capture the compiler version and the git revision into variables, then
# generate a config file with the values.
if (NOT "${GOOGLE_CLOUD_CPP_SPANNER_BUILD_METADATA}" STREQUAL "")
    # The build metadata flag is already defined, do not re-compute the
    # initialization value. This works both when the user supplies
    # -DGOOGLE_CLOUD_CPP_SPANNER_METADATA=value in the command line, and when
    # GOOGLE_CLOUD_CPP_SPANNER_METADATA has a cached value
    set(GOOGLE_CLOUD_CPP_SPANNER_GIT_HEAD "unused")
else ()
    google_cloud_cpp_spanner_initialize_git_head(
        GOOGLE_CLOUD_CPP_SPANNER_GIT_HEAD)
endif ()

# Define a CMake configuration option to set the build metadata. By default this
# is initialized from `git rev-parse --short HEAD`, but the developer (or the
# script building via CMake) can override the value.
set(GOOGLE_CLOUD_CPP_SPANNER_BUILD_METADATA
    "${GOOGLE_CLOUD_CPP_SPANNER_GIT_HEAD}"
    CACHE STRING "Append build metadata to the library version number")
# This option is rarely needed. Mark it as "advanced" to remove it from the
# default CMake UIs.
mark_as_advanced(GOOGLE_CLOUD_CPP_SPANNER_BUILD_METADATA)

message(STATUS "google-cloud-cpp-spanner build metadata set to"
               " ${GOOGLE_CLOUD_CPP_SPANNER_BUILD_METADATA}")

# Create the file that captures build information. Having access to the compiler
# and build flags at runtime allows us to print better benchmark results.
string(TOUPPER "${CMAKE_BUILD_TYPE}" GOOGLE_CLOUD_CPP_SPANNER_BUILD_TYPE_UPPER)
configure_file(internal/build_info.cc.in internal/build_info.cc)

if (MSVC)
    # MSVC warns about using sscanf(), this is a valuable warning, so we do not
    # want to disable for everything. But for these files the usage is "safe".
    # This is a relatively obscure feature to add compile flags to just few
    # sources:
    set_property(
        SOURCE value.cc internal/date.cc
        APPEND_STRING
        PROPERTY COMPILE_DEFINITIONS "_CRT_SECURE_NO_WARNINGS")
endif ()

configure_file(version_info.h.in ${CMAKE_CURRENT_SOURCE_DIR}/version_info.h)
add_library(
    spanner_client
    ${CMAKE_CURRENT_BINARY_DIR}/internal/build_info.cc
    backoff_policy.h
    backup.cc
    backup.h
    batch_dml_result.h
    bytes.cc
    bytes.h
    client.cc
    client.h
    client_options.h
    commit_result.h
    connection.h
    connection_options.cc
    connection_options.h
    create_instance_request_builder.h
    database.cc
    database.h
    database_admin_client.cc
    database_admin_client.h
    database_admin_connection.cc
    database_admin_connection.h
    date.cc
    date.h
    iam_updater.h
    instance.cc
    instance.h
    instance_admin_client.cc
    instance_admin_client.h
    instance_admin_connection.cc
    instance_admin_connection.h
    internal/api_client_header.cc
    internal/api_client_header.h
    internal/build_info.h
    internal/channel.h
    internal/compiler_info.cc
    internal/compiler_info.h
    internal/connection_impl.cc
    internal/connection_impl.h
    internal/database_admin_logging.cc
    internal/database_admin_logging.h
    internal/database_admin_metadata.cc
    internal/database_admin_metadata.h
    internal/database_admin_stub.cc
    internal/database_admin_stub.h
    internal/date.cc
    internal/date.h
    internal/instance_admin_logging.cc
    internal/instance_admin_logging.h
    internal/instance_admin_metadata.cc
    internal/instance_admin_metadata.h
    internal/instance_admin_stub.cc
    internal/instance_admin_stub.h
    internal/log_wrapper.cc
    internal/log_wrapper.h
    internal/logging_result_set_reader.cc
    internal/logging_result_set_reader.h
    internal/logging_spanner_stub.cc
    internal/logging_spanner_stub.h
    internal/merge_chunk.cc
    internal/merge_chunk.h
    internal/metadata_spanner_stub.cc
    internal/metadata_spanner_stub.h
    internal/partial_result_set_reader.h
    internal/partial_result_set_resume.cc
    internal/partial_result_set_resume.h
    internal/partial_result_set_source.cc
    internal/partial_result_set_source.h
    internal/polling_loop.h
    internal/retry_loop.cc
    internal/retry_loop.h
    internal/session.cc
    internal/session.h
    internal/session_pool.cc
    internal/session_pool.h
    internal/spanner_stub.cc
    internal/spanner_stub.h
    internal/status_utils.cc
    internal/status_utils.h
    internal/time_format.cc
    internal/time_format.h
    internal/time_utils.h
    internal/transaction_impl.cc
    internal/transaction_impl.h
    internal/tuple_utils.h
    keys.cc
    keys.h
    mutations.cc
    mutations.h
    partition_options.cc
    partition_options.h
    partitioned_dml_result.h
    polling_policy.h
    query_options.h
    query_partition.cc
    query_partition.h
    read_options.h
    read_partition.cc
    read_partition.h
    results.cc
    results.h
    retry_policy.h
    row.cc
    row.h
    session_pool_options.h
    sql_statement.cc
    sql_statement.h
    timestamp.h
    timestamp.cc
    tracing_options.h
    transaction.cc
    transaction.h
    update_instance_request_builder.h
    value.cc
    value.h
    version.cc
    version.h
    version_info.h)
target_include_directories(
    spanner_client
    PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
           $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
           $<INSTALL_INTERFACE:include>)
target_link_libraries(
    spanner_client PUBLIC google_cloud_cpp_grpc_utils google_cloud_cpp_common
                          googleapis-c++::spanner_protos)
set_target_properties(
    spanner_client PROPERTIES VERSION "${SPANNER_CLIENT_VERSION}"
                              SOVERSION "${SPANNER_CLIENT_VERSION_MAJOR}")
target_compile_options(spanner_client
                       PUBLIC ${GOOGLE_CLOUD_CPP_SPANNER_EXCEPTIONS_FLAG})

google_cloud_cpp_add_clang_tidy(spanner_client)
google_cloud_cpp_add_common_options(spanner_client)

add_library(googleapis-c++::spanner_client ALIAS spanner_client)

# To avoid maintaining the list of files for the library, export them to a .bzl
# file.
include(CreateBazelConfig)
create_bazel_config(spanner_client)

# Create a header-only library for the mocks. We use a CMake `INTERFACE` library
# for these, a regular library would not work on macOS (where the library needs
# at least one .o file). Unfortunately INTERFACE libraries are a bit weird in
# that they need absolute paths for their sources.
add_library(spanner_client_mocks INTERFACE)
target_sources(
    spanner_client_mocks
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/mocks/mock_database_admin_connection.h
              ${CMAKE_CURRENT_SOURCE_DIR}/mocks/mock_instance_admin_connection.h
              ${CMAKE_CURRENT_SOURCE_DIR}/mocks/mock_spanner_connection.h)
target_link_libraries(
    spanner_client_mocks
    INTERFACE googleapis-c++::spanner_client google_cloud_cpp_testing
              GTest::gmock_main GTest::gmock GTest::gtest)
create_bazel_config(spanner_client_mocks)
target_include_directories(
    spanner_client_mocks
    INTERFACE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
              $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
              $<INSTALL_INTERFACE:include>)
target_compile_options(spanner_client_mocks
                       INTERFACE ${GOOGLE_CLOUD_CPP_SPANNER_EXCEPTIONS_FLAG})

# Define the tests in a function so we have a new scope for variable names.
function (spanner_client_define_tests)
    # The tests require googletest to be installed. Force CMake to use the
    # config file for googletest (that is, the CMake file installed by
    # googletest itself), because the generic `FindGTest` module does not define
    # the GTest::gmock target, and the target names are also weird.
    find_package(GTest CONFIG REQUIRED)

    find_package(google_cloud_cpp_testing CONFIG REQUIRED)

    add_library(
        spanner_client_testing
        testing/cleanup_stale_instances.cc
        testing/cleanup_stale_instances.h
        testing/compiler_supports_regexp.h
        testing/database_environment.cc
        testing/database_environment.h
        testing/matchers.h
        testing/mock_database_admin_stub.h
        testing/mock_instance_admin_stub.h
        testing/mock_partial_result_set_reader.h
        testing/mock_spanner_stub.h
        testing/pick_instance_config.cc
        testing/pick_instance_config.h
        testing/pick_random_instance.cc
        testing/pick_random_instance.h
        testing/policies.h
        testing/random_backup_name.cc
        testing/random_backup_name.h
        testing/random_database_name.cc
        testing/random_database_name.h
        testing/random_instance_name.cc
        testing/random_instance_name.h
        testing/validate_metadata.cc
        testing/validate_metadata.h)
    target_link_libraries(
        spanner_client_testing
        PUBLIC spanner_client_mocks googleapis-c++::spanner_client
               google_cloud_cpp_testing GTest::gmock_main GTest::gmock
               GTest::gtest)
    create_bazel_config(spanner_client_testing)

    target_include_directories(
        spanner_client_testing
        PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
               $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
               $<INSTALL_INTERFACE:include>)
    target_compile_options(spanner_client_testing
                           PUBLIC ${GOOGLE_CLOUD_CPP_SPANNER_EXCEPTIONS_FLAG})

    set(spanner_client_unit_tests
        # cmake-format: sortable
        backup_test.cc
        bytes_test.cc
        client_options_test.cc
        client_test.cc
        connection_options_test.cc
        create_instance_request_builder_test.cc
        database_admin_client_test.cc
        database_admin_connection_test.cc
        database_test.cc
        date_test.cc
        instance_admin_client_test.cc
        instance_admin_connection_test.cc
        instance_test.cc
        internal/api_client_header_test.cc
        internal/build_info_test.cc
        internal/compiler_info_test.cc
        internal/connection_impl_test.cc
        internal/database_admin_logging_test.cc
        internal/database_admin_metadata_test.cc
        internal/date_test.cc
        internal/instance_admin_logging_test.cc
        internal/instance_admin_metadata_test.cc
        internal/log_wrapper_test.cc
        internal/logging_result_set_reader_test.cc
        internal/logging_spanner_stub_test.cc
        internal/merge_chunk_test.cc
        internal/metadata_spanner_stub_test.cc
        internal/partial_result_set_resume_test.cc
        internal/partial_result_set_source_test.cc
        internal/polling_loop_test.cc
        internal/retry_loop_test.cc
        internal/session_pool_test.cc
        internal/spanner_stub_test.cc
        internal/status_utils_test.cc
        internal/time_format_test.cc
        internal/time_utils_test.cc
        internal/transaction_impl_test.cc
        internal/tuple_utils_test.cc
        keys_test.cc
        mutations_test.cc
        partition_options_test.cc
        query_options_test.cc
        query_partition_test.cc
        read_options_test.cc
        read_partition_test.cc
        results_test.cc
        retry_policy_test.cc
        row_test.cc
        session_pool_options_test.cc
        spanner_version_test.cc
        sql_statement_test.cc
        timestamp_test.cc
        transaction_test.cc
        update_instance_request_builder_test.cc
        value_test.cc)

    # Export the list of unit tests to a .bzl file so we do not need to maintain
    # the list in two places.
    export_list_to_bazel("spanner_client_unit_tests.bzl"
                         "spanner_client_unit_tests")

    # Create a custom target so we can say "build all the tests"
    add_custom_target(spanner-client-tests)

    # Generate a target for each unit test.
    foreach (fname ${spanner_client_unit_tests})
        string(REPLACE "/" "_" target ${fname})
        string(REPLACE ".cc" "" target ${target})
        add_executable(${target} ${fname})
        target_link_libraries(
            ${target}
            PRIVATE spanner_client_testing googleapis-c++::spanner_client
                    google_cloud_cpp_testing GTest::gmock_main GTest::gmock
                    GTest::gtest)
        google_cloud_cpp_add_clang_tidy(${target})
        google_cloud_cpp_add_common_options(${target})

        # With googletest it is relatively easy to exceed the default number of
        # sections (~65,000) in a single .obj file. Add the /bigobj option to
        # all the tests, even if it is not needed.
        if (MSVC)
            target_compile_options(${target} PRIVATE "/bigobj")
        endif ()
        add_test(NAME ${target} COMMAND ${target})
        add_dependencies(spanner-client-tests ${target})
    endforeach ()
endfunction ()

# Define the benchmarks in a function so we have a new scope for variable names.
function (spanner_client_define_benchmarks)
    find_package(benchmark CONFIG REQUIRED)

    set(spanner_client_benchmarks
        # cmake-format: sortable
        bytes_benchmark.cc
        internal/date_benchmark.cc
        internal/merge_chunk_benchmark.cc
        internal/time_format_benchmark.cc
        row_benchmark.cc)

    # Export the list of benchmarks to a .bzl file so we do not need to maintain
    # the list in two places.
    export_list_to_bazel("spanner_client_benchmarks.bzl"
                         "spanner_client_benchmarks")

    # Create a custom target so we can say "build all the benchmarks"
    add_custom_target(spanner-client-benchmarks)

    # Generate a target for each benchmark.
    foreach (fname ${spanner_client_benchmarks})
        string(REPLACE "/" "_" target ${fname})
        string(REPLACE ".cc" "" target ${target})
        add_executable(${target} ${fname})
        add_test(NAME ${target} COMMAND ${target})
        target_link_libraries(${target} PRIVATE googleapis-c++::spanner_client
                                                benchmark::benchmark_main)
        google_cloud_cpp_add_clang_tidy(${target})
        google_cloud_cpp_add_common_options(${target})

        add_dependencies(spanner-client-benchmarks ${target})
    endforeach ()
endfunction ()

# Only define the tests/benchmarks if testing is enabled. Package maintainers
# may not want to build all the tests everytime they create a new package or
# when the package is installed from source.
if (BUILD_TESTING)
    spanner_client_define_tests()
    spanner_client_define_benchmarks()
endif (BUILD_TESTING)

add_subdirectory(integration_tests)
add_subdirectory(benchmarks) # macro benchmarks

# Only compile the samples if we're building with exceptions enabled. They
# require exceptions to keep them simple and idiomatic.
if (GOOGLE_CLOUD_CPP_SPANNER_ENABLE_CXX_EXCEPTIONS)
    add_subdirectory(samples)
endif (GOOGLE_CLOUD_CPP_SPANNER_ENABLE_CXX_EXCEPTIONS)

# Get the destination directories based on the GNU recommendations.
include(GNUInstallDirs)

# Export the CMake targets to make it easy to create configuration files.
install(EXPORT spanner-targets
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/spanner_client")

# Install the libraries and headers in the locations determined by
# GNUInstallDirs
install(
    TARGETS spanner_client
    EXPORT spanner-targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            COMPONENT google_cloud_cpp_spanner_runtime
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT google_cloud_cpp_spanner_runtime
            NAMELINK_SKIP
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT google_cloud_cpp_spanner_development)
# With CMake-3.12 and higher we could avoid this separate command (and the
# duplication).
install(
    TARGETS spanner_client
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT google_cloud_cpp_spanner_development
            NAMELINK_ONLY
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT google_cloud_cpp_spanner_development)

google_cloud_cpp_install_headers("spanner_client"
                                 "include/google/cloud/spanner")
google_cloud_cpp_install_headers("spanner_client_mocks"
                                 "include/google/cloud/spanner")

# Setup global variables used in the following *.in files.
set(GOOGLE_CLOUD_SPANNER_CONFIG_VERSION_MAJOR ${SPANNER_CLIENT_VERSION_MAJOR})
set(GOOGLE_CLOUD_SPANNER_CONFIG_VERSION_MINOR ${SPANNER_CLIENT_VERSION_MINOR})
set(GOOGLE_CLOUD_SPANNER_CONFIG_VERSION_PATCH ${SPANNER_CLIENT_VERSION_PATCH})
set(GOOGLE_CLOUD_SPANNER_PC_NAME "The Google Cloud Spanner C++ Client Library")
set(GOOGLE_CLOUD_SPANNER_PC_DESCRIPTION
    "Provides C++ APIs to access Google Cloud Spanner.")
set(GOOGLE_CLOUD_SPANNER_PC_REQUIRES
    "google_cloud_cpp_grpc_utils google_cloud_cpp_common")
set(GOOGLE_CLOUD_SPANNER_PC_LIBS "-lspanner_client")

# Create and install the pkg-config files.
configure_file("${PROJECT_SOURCE_DIR}/google/cloud/spanner/config.pc.in"
               "spanner_client.pc" @ONLY)
install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/spanner_client.pc"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
    COMPONENT google_cloud_cpp_spanner_development)

# Create and install the CMake configuration files.
configure_file("config.cmake.in" "spanner_client-config.cmake" @ONLY)
configure_file(
    "${PROJECT_SOURCE_DIR}/google/cloud/spanner/config-version.cmake.in"
    "spanner_client-config-version.cmake" @ONLY)
install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/spanner_client-config.cmake"
          "${CMAKE_CURRENT_BINARY_DIR}/spanner_client-config-version.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/spanner_client"
    COMPONENT google_cloud_cpp_spanner_development)
