# Look for a version of TGUI on the local machine
#
# By default, this will look in all common places. If TGUI is built or
# installed in a custom location, you're able to either modify the
# CMakeCache.txt file yourself or simply pass the path to CMake using either the
# environment variable `TGUI_ROOT` or the CMake define with the same name.

set(TGUI_PATHS	${TGUI_ROOT}
					$ENV{TGUI_ROOT}
					~/Library/Frameworks
					/Library/Frameworks
					/usr/local
					/usr
					/sw
					/opt/local
					/opt/csw
					/opt
					C:/lib/TGUI-0.7.3
					/usr/lib)

find_path(TGUI_INCLUDE_DIR include/TGUI/TGUI.h PATH_SUFFIXES include PATHS ${TGUI_PATHS})
find_library(TGUI_LIBRARY NAMES tgui-s PATH_SUFFIXES lib PATHS ${TGUI_PATHS})
find_library(TGUI_LIBRARY_DEBUG NAMES tgui-s-d PATH_SUFFIXES lib PATHS ${TGUI_PATHS})
mark_as_advanced(TGUI_INCLUDE_DIR TGUI_LIBRARY)

if(TGUI_INCLUDE_DIR AND TGUI_LIBRARY)
	set(TGUI_FOUND TRUE)
else()
	set(TGUI_FOUND FALSE)
endif()
