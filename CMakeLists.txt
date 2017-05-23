cmake_minimum_required (VERSION 3.2 FATAL_ERROR)

project (kulak)

file (GLOB kulak_src
	"src/*.hpp"
	"src/*.cpp"
	"src/systems/*.hpp"
	"src/systems/*.cpp"
	"src/components/*.hpp"
	"src/components/*.cpp"
)
include_directories (${CMAKE_SOURCE_DIR}/src)

set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/build; ${CMAKE_MODULE_PATH})

set (CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set (EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set (CMAKE_CXX_FLAGS "-std=c++14")

add_executable (kulak ${kulak_src})

## Libraries

# Windows defines
if (WIN32)
#SFML
set (SFML_STATIC_LIBRARIES TRUE)
	set (SFML_ROOT C:/lib/SFML)
	link_directories (C:/lib/SFML/lib)
	set (CMAKE_CXX_FLAGS "/EHsc")
#Entityx
link_directories (C:/lib/entityx/lib)
#TGUI
link_directories (C:/lib/TGUI-0.7.4/lib)
#	set (TGUI_ROOT C:/lib/TGUI-0.7.3)
endif()

find_package(EntityX REQUIRED)
find_package(TGUI REQUIRED)
# Dynamically linked on Linux
find_package (SFML 2 COMPONENTS system window graphics audio REQUIRED)
if (SFML_FOUND)
	# SFML dependencies for static linking on Windows
	if (WIN32)
		#target_link_libraries (kulak ${ws2_32_PATH})	this is for sfml_network
		target_link_libraries (kulak opengl32.lib)
		target_link_libraries (kulak winmm.lib)
		target_link_libraries (kulak gdi32.lib)
		# graphics
		target_link_libraries (kulak ${FREETYPE_LIBRARY}) # FREETYPE_LIBRARY should be found by find_package (sfml)
		target_link_libraries (kulak ${JPEG_LIBRARY}) # JPEG_LIBRARY should be found by find_package (sfml)
		# audio
		target_link_libraries (kulak ${OPENAL_LIBRARY})
		target_link_libraries (kulak ${OGG_LIBRARY})
		target_link_libraries (kulak ${VORBIS_LIBRARY})
		target_link_libraries (kulak ${VORBISFILE_LIBRARY})
		target_link_libraries (kulak ${VORBISENC_LIBRARY})
		target_link_libraries (kulak ${FLAC_LIBRARY})
	endif()

	include_directories (${SFML_INCLUDE_DIR})
	include_directories (${ENTITYX_INCLUDE_DIR})
	include_directories (${TGUI_INCLUDE_DIR})
	target_link_libraries (kulak ${SFML_LIBRARIES} ${TGUI_LIBRARY} debug ${ENTITYX_LIBRARY_DEBUG}optimized ${ENTITYX_LIBRARY})
endif()
