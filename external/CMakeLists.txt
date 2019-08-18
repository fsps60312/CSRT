if(MSVC AND NOT "${MSVC_VERSION}" LESS 1400)
	add_definitions( "/MP" )
endif()


add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

### GLEW ###

if ((NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/glew) OR
	(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/glew/src/glew.c))
	execute_process(COMMAND git clone
		#		https://github.com/nigels-com/glew.git
		https://github.com/Perlmint/glew-cmake.git
		glew
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()

set(GLEW_SOURCE
	glew/src/glew.c
)

set(GLEW_HEADERS
)


add_library( GLEW STATIC
	${GLEW_SOURCE}
	${GLEW_INCLUDE}
)

target_link_libraries(GLEW
	${OPENGL_LIBRARY}
	${EXTRA_LIBS}
)

### GLFW ###
if ((NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/glfw) OR
	(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/glfw/include/GLFW))
	execute_process(COMMAND git clone
		https://github.com/glfw/glfw.git
		glfw
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()

add_subdirectory (glfw)

include_directories(
	glfw/include/GLFW/
	glew/include/
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
set(OPENGL_LIBRARY
	${OPENGL_LIBRARY}
	-lGL -lGLU -lXrandr -lXext -lX11 -lrt
	${CMAKE_DL_LIBS}
	${GLFW_LIBRARIES}
)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
set(OPENGL_LIBRARY
	${OPENGL_LIBRARY}
	${CMAKE_DL_LIBS}
	${GLFW_LIBRARIES}
)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")


### SOIL ###

set(SOIL_SOURCE
	soil/src/image_DXT.c
	soil/src/image_helper.c
	soil/src/SOIL.c
	soil/src/stb_image_aug.c
)

set(SOIL_HEADERS
	soil/src/image_DXT.h
	soil/src/image_helper.h
	soil/src/SOIL.h
	soil/src/stb_image_aug.h
)


add_library( SOIL STATIC
	${SOIL_SOURCE}
	${SOIL_INCLUDE}
)

target_link_libraries(SOIL
	${OPENGL_LIBRARY}
	${EXTRA_LIBS}
)
