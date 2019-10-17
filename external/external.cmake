#############################
# Overriding default values #
#############################

set(BUILD_DEMOS OFF CACHE BOOL "Disable demos" FORCE)
set(BUILD_ICD OFF CACHE BOOL "Disable ICD" FORCE)

include(IncludeTarget)
include(TargetIncludesSystem)

#########################
# External dependencies #
#########################

find_package(Vulkan)
if (DEFINED ENV{VULKAN_SDK})
    message(STATUS "Vulkan environment variable: $ENV{VULKAN_SDK}")
    set(VULKAN_INCLUDE "$ENV{VULKAN_SDK}/include")
else()
    message(STATUS "Vulkan environment variable: undefined")
    set(VULKAN_INCLUDE "")
endif()

# Settings for all dependencies
set(BUILD_STATIC_LIBS ON)

# Dependencies and specific options

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/fmt)

set(GLM_TEST_ENABLE_CXX_17 ON)
set(GLM_TEST_ENABLE OFF)
set(GLM_TEST_ENABLE_SIMD_AVX2 ON)	# TODO: determine minimum CPU for Nova and use the right instruction set
include_target(glm::glm "$CMAKE_CURRENT_LIST_DIR}/glm")

include_target(nlohmann::json "${CMAKE_CURRENT_LIST_DIR}/json/single_include")
include_target(spirv::headers "${CMAKE_CURRENT_LIST_DIR}/SPIRV-Headers")
include_target(vma::vma "${3RD_PARTY_DIR}/VulkanMemoryAllocator/src")
include_target(vulkan::sdk "${VULKAN_INCLUDE}")

# Submodule libraries

set(SPIRV-Headers_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/SPIRV-Headers)
set(SPIRV_SKIP_TESTS ON CACHE BOOL "Disable SPIRV-Tools tests" FORCE)
set(SPIRV_WERROR OFF CACHE BOOL "Enable error on warning SPIRV-Tools" FORCE)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/SPIRV-Tools)

set(ENABLE_EXPORTS ON CACHE BOOL "Enable linking SPIRV_Cross" FORCE)
set(SPIRV_CROSS_CLI OFF)
set(SPIRV_CROSS_ENABLE_TESTS OFF)
set(SPIRV_CROSS_ENABLE_MSL OFF)	# Need to remove this is we add a Metal RHI backend
set(SPIRV_CROSS_ENABLE_CPP OFF)
set(SPIRV_CROSS_SKIP_INSTALL ON)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/SPIRV-Cross)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/SPIRV-Cross/external/glslang)

target_includes_system(glslang)

# Manually built libraries
        
include(minitrace)
include(miniz)

# Hide unnecessary targets from all

set_property(TARGET glslang PROPERTY EXCLUDE_FROM_ALL True)
set_property(TARGET glslang-default-resource-limits PROPERTY EXCLUDE_FROM_ALL True)
set_property(TARGET OGLCompiler PROPERTY EXCLUDE_FROM_ALL True)
set_property(TARGET OSDependent PROPERTY EXCLUDE_FROM_ALL True)
set_property(TARGET SPIRV PROPERTY EXCLUDE_FROM_ALL True)
set_property(TARGET SPVRemapper PROPERTY EXCLUDE_FROM_ALL True)
set_property(TARGET HLSL PROPERTY EXCLUDE_FROM_ALL True)
set_property(TARGET glslangValidator PROPERTY EXCLUDE_FROM_ALL True)
set_property(TARGET spirv-remap PROPERTY EXCLUDE_FROM_ALL True)

set_property(TARGET spirv-tools-build-version PROPERTY EXCLUDE_FROM_ALL True)
set_property(TARGET spirv-tools-header-DebugInfo PROPERTY EXCLUDE_FROM_ALL True)

set_property(TARGET spirv-tools-cpp-example PROPERTY EXCLUDE_FROM_ALL True)

set_property(TARGET spirv-as PROPERTY EXCLUDE_FROM_ALL True)
set_property(TARGET spirv-cfg PROPERTY EXCLUDE_FROM_ALL True)
set_property(TARGET spirv-dis PROPERTY EXCLUDE_FROM_ALL True)
set_property(TARGET spirv-link PROPERTY EXCLUDE_FROM_ALL True)
set_property(TARGET spirv-opt PROPERTY EXCLUDE_FROM_ALL True)
set_property(TARGET spirv-reduce PROPERTY EXCLUDE_FROM_ALL True)
set_property(TARGET spirv-val PROPERTY EXCLUDE_FROM_ALL True)

set_property(TARGET SPIRV-Tools PROPERTY EXCLUDE_FROM_ALL True)
set_property(TARGET SPIRV-Tools-link PROPERTY EXCLUDE_FROM_ALL True)
set_property(TARGET SPIRV-Tools-opt PROPERTY EXCLUDE_FROM_ALL True)
set_property(TARGET SPIRV-Tools-reduce PROPERTY EXCLUDE_FROM_ALL True)
set_property(TARGET SPIRV-Tools-shared PROPERTY EXCLUDE_FROM_ALL True)

set_property(TARGET spirv-tools-vimsyntax PROPERTY EXCLUDE_FROM_ALL True)
set_property(TARGET spirv-tools-pkg-config PROPERTY EXCLUDE_FROM_ALL True)
set_property(TARGET spirv-tools-shared-pkg-config PROPERTY EXCLUDE_FROM_ALL True)

set_property(TARGET spirv-cross-core PROPERTY EXCLUDE_FROM_ALL True)
set_property(TARGET spirv-cross-glsl PROPERTY EXCLUDE_FROM_ALL True)
set_property(TARGET spirv-cross-hlsl PROPERTY EXCLUDE_FROM_ALL True)
set_property(TARGET spirv-cross-reflect PROPERTY EXCLUDE_FROM_ALL True)
set_property(TARGET spirv-cross-util PROPERTY EXCLUDE_FROM_ALL True)

#####################
# Test dependencies #
#####################
if(NOVA_TEST)
	set(BUILD_GMOCK OFF)
	add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/googletest)
endif()