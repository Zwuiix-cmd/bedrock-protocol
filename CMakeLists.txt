cmake_minimum_required(VERSION 3.24)

project(bedrock-protocol VERSION 1.0.0 LANGUAGES CXX)

include(FetchContent)
include(CTest)
include(GNUInstallDirs)

option(BEDROCK_PROTOCOL_BUILD_POOL "Build the generated PacketPool implementation" ON)
option(BEDROCK_PROTOCOL_FETCH_DEPENDENCIES "Fetch GLM, magic_enum, and EnTT" ON)
option(BEDROCK_PROTOCOL_FETCH_ENTT "Fetch EnTT when PacketPool is enabled" ON)
option(BEDROCK_PROTOCOL_FETCH_GLM "Fetch GLM" ON)
option(BEDROCK_PROTOCOL_FETCH_MAGIC_ENUM "Fetch magic_enum" ON)
option(BEDROCK_PROTOCOL_BUILD_TESTS "Build the protocol regression tests" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(BEDROCK_PROTOCOL_FETCH_DEPENDENCIES)
    set(MAGIC_ENUM_OPT_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(MAGIC_ENUM_OPT_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(ENTT_BUILD_TESTING OFF CACHE BOOL "" FORCE)
    set(GLM_BUILD_TESTS OFF CACHE BOOL "" FORCE)

    if(BEDROCK_PROTOCOL_FETCH_MAGIC_ENUM
        AND NOT TARGET magic_enum::magic_enum
        AND NOT TARGET magic_enum)
        FetchContent_Declare(
            magic_enum
            GIT_REPOSITORY https://github.com/Neargye/magic_enum.git
            GIT_TAG 7c58a87f7785720cfe3a9fc38115a175b5433394
        )
        FetchContent_MakeAvailable(magic_enum)
    endif()

    if(BEDROCK_PROTOCOL_FETCH_GLM AND NOT TARGET glm::glm)
        FetchContent_Declare(
            glm
            GIT_REPOSITORY https://github.com/g-truc/glm.git
            GIT_TAG 8d1fd52e5ab5590e2c81768ace50c72bae28f2ed
        )
        FetchContent_MakeAvailable(glm)
    endif()

    if(BEDROCK_PROTOCOL_BUILD_POOL
        AND BEDROCK_PROTOCOL_FETCH_ENTT
        AND NOT TARGET EnTT::EnTT
        AND NOT TARGET EnTT)
        FetchContent_Declare(
            entt
            GIT_REPOSITORY https://github.com/skypjack/entt.git
            GIT_TAG f931687ff04d435871ac9664bb299f71f2a8fafc
        )
        FetchContent_MakeAvailable(entt)
    endif()
endif()

if(NOT TARGET magic_enum::magic_enum AND NOT TARGET magic_enum)
    find_package(magic_enum CONFIG REQUIRED)
endif()
if(NOT TARGET glm::glm)
    find_package(glm CONFIG REQUIRED)
endif()
if(BEDROCK_PROTOCOL_BUILD_POOL AND NOT TARGET EnTT::EnTT AND NOT TARGET EnTT)
    find_package(EnTT CONFIG REQUIRED)
endif()

if(TARGET magic_enum::magic_enum)
    set(BEDROCK_PROTOCOL_MAGIC_ENUM_TARGET magic_enum::magic_enum)
else()
    set(BEDROCK_PROTOCOL_MAGIC_ENUM_TARGET magic_enum)
endif()

if(TARGET EnTT::EnTT)
    set(BEDROCK_PROTOCOL_ENTT_TARGET EnTT::EnTT)
elseif(TARGET EnTT)
    set(BEDROCK_PROTOCOL_ENTT_TARGET EnTT)
endif()

if(BEDROCK_PROTOCOL_BUILD_POOL)
    add_library(bedrock-protocol STATIC)
    set(BEDROCK_PROTOCOL_TARGET_SCOPE PUBLIC)
else()
    add_library(bedrock-protocol INTERFACE)
    set(BEDROCK_PROTOCOL_TARGET_SCOPE INTERFACE)
endif()

add_library(bedrock-protocol::bedrock-protocol ALIAS bedrock-protocol)

if(BEDROCK_PROTOCOL_BUILD_POOL)
    target_sources(
        bedrock-protocol
        PRIVATE
            include/bedrock/protocol/pool/PacketPool.cpp
    )
endif()

target_compile_features(bedrock-protocol ${BEDROCK_PROTOCOL_TARGET_SCOPE} cxx_std_20)
target_include_directories(
    bedrock-protocol
    ${BEDROCK_PROTOCOL_TARGET_SCOPE}
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(
    bedrock-protocol
    ${BEDROCK_PROTOCOL_TARGET_SCOPE}
    ${BEDROCK_PROTOCOL_MAGIC_ENUM_TARGET}
    glm::glm
)

if(BEDROCK_PROTOCOL_BUILD_POOL)
    target_link_libraries(bedrock-protocol PUBLIC ${BEDROCK_PROTOCOL_ENTT_TARGET})
endif()

if(MSVC)
    if(BEDROCK_PROTOCOL_BUILD_POOL)
        target_compile_options(bedrock-protocol PRIVATE /bigobj)
    else()
        target_compile_options(bedrock-protocol INTERFACE /bigobj)
    endif()
endif()

if(BEDROCK_PROTOCOL_BUILD_TESTS AND BUILD_TESTING)
    add_executable(bedrock-protocol-tests
        tests/ProtocolTests.cpp
    )
    target_compile_features(bedrock-protocol-tests PRIVATE cxx_std_20)
    target_link_libraries(bedrock-protocol-tests PRIVATE bedrock-protocol)
    add_test(NAME bedrock-protocol-tests COMMAND bedrock-protocol-tests)
endif()

install(
    TARGETS bedrock-protocol
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
