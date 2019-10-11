# CMake entry point
cmake_minimum_required (VERSION 3.0)
project (Compute_Shader_Ray_Tracing)

find_package(OpenGL REQUIRED)


if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/)" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
	message( "Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
	message( "Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()



# Compile external dependencies 
add_subdirectory (external)

# On Visual 2005 and above, this module can set the debug working directory
cmake_policy(SET CMP0026 OLD)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/rpavlik-cmake-modules")
include(CreateLaunchers)
include(MSVCMultipleProcessCompile) # /MP

if(INCLUDE_DISTRIB)
	add_subdirectory(distrib)
endif(INCLUDE_DISTRIB)



include_directories(
	external/glfw/include/
	external/glm/
	external/glew/include/
	.
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW
	SOIL
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

# User playground
add_executable(playground 
	playground/playground.cpp
	common/block/block.hpp
	common/block/block.cpp
	common/block/blocks.hpp
	common/block/blocks.cpp
	common/effects/fume.hpp
	common/effects/fume.cpp
	common/effects/gravel.hpp
	common/effects/gravel.cpp
	common/pod/pod.hpp
	common/pod/pod.cpp
	common/pod/pod_body.hpp
	common/pod/pod_body.cpp
	common/pod/pod_drill.hpp
	common/pod/pod_drill.cpp
	common/pod/pod_drill_blade.cpp
	common/pod/pod_interface.hpp
	common/pod/pod_propeller.hpp
	common/pod/pod_propeller.cpp
	common/pod/pod_propeller_blade.cpp
	common/pod/pod_propeller_bladeset.cpp
	common/pod/pod_tracks.hpp
	common/pod/pod_tracks.cpp
	common/pod/pod_tracks_gear.cpp
	common/pod/pod_tracks_tooth.cpp
	common/control.hpp
	common/aabb.hpp
	common/aabb.cpp
	common/buffer_system.hpp
	common/buffer_system.cpp
	common/bvh_node.hpp
	common/bvh_node.cpp
	common/camera.hpp
	common/camera.cpp
	common/environment.hpp
	common/environment.cpp
	common/game.hpp
	common/game.cpp
	common/gl_check_error.hpp
	common/gl_check_error.cpp
	common/light.hpp
	common/light.cpp
	common/material.hpp
	common/material.cpp
	common/matrix_d.hpp
	common/matrix_d.cpp
	common/mylib.hpp
	common/mylib.cpp
	common/objloader.hpp
	common/objloader.cpp
	common/rigid_body.hpp
	common/rigid_body.cpp
	common/shader.hpp
	common/shader.cpp
	common/triangle.hpp
	common/triangle.cpp
	common/visible_object.hpp
	common/visible_object.cpp
	common/world.hpp
	common/world.cpp
)
target_link_libraries(playground
	${ALL_LIBS}
)
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "playground")
# Xcode and Visual working directories
set_target_properties(playground PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/playground/")
create_target_launcher(playground WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/playground/")


SOURCE_GROUP(common REGULAR_EXPRESSION ".*/common/.*" )
SOURCE_GROUP(common\\block REGULAR_EXPRESSION ".*/common/block/.*")
SOURCE_GROUP(common\\effects REGULAR_EXPRESSION ".*/common/effects/.*")
SOURCE_GROUP(common\\pod REGULAR_EXPRESSION ".*/common/pod/.*")
SOURCE_GROUP(shaders REGULAR_EXPRESSION ".*/.*shader$" )


if (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )
add_custom_command(
   TARGET playground POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/playground${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/playground/"
)

elseif (${CMAKE_GENERATOR} MATCHES "Xcode" )

endif (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )

