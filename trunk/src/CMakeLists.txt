CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(convivial)

########################################################################
##
## we need yocto
##
########################################################################
IF(WIN32)
	SET(YROOT E:/program/yocto4/sdk)
ELSE(WIN32)
	SET(YROOT $ENV{HOME}/program/yocto4/sdk)
	#SET(CONV_ROOT $ENV{HOME}/QCP/Perso-ware/convivial)
	#MESSAGE( adresse ${CONV_ROOT} )
ENDIF(WIN32)

INCLUDE( ${YROOT}/share/yocto/rules.cmake)

SET(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/../sdk)


INCLUDE_DIRECTORIES( . ${YROOT}/include )
LINK_DIRECTORIES( ${YROOT}/lib )

########################################################################
##
## we need yocto-rcpack
##
########################################################################
FIND_PROGRAM(RCPACK yocto-rcpack PATH ${YROOT}/bin )
MESSAGE( STATUS "RCPACK=${RCPACK}" )

########################################################################
##
## create the library
##
########################################################################
ADD_LIBRARY(conviv STATIC
	conviv.hpp 
	conviv.cpp 
	conviv-import.cpp
	conviv-json.cpp
	conviv-data.hpp conviv-data.cpp
	conviv-validate.cpp
	schoenflies.hpp schoenflies.cpp 
	mode.hpp	mode.cpp
	basis-info.hpp	basis-info.cpp
	basis.hpp	basis.cpp
	modes.hpp	modes.cpp
#gcc -I/usr/include/python2.7 test.c -lpython2.7
)

########################################################################
##
## list of the resources
##
########################################################################
SET(RESOURCES embed/schoenflies.js embed/doc.html embed/basis.js)


INCLUDE( ${YROOT}/share/yocto/fltk.cmake)

IF(FLTK_FOUND)

########################################################################
##
## create the main executable
##
########################################################################
FLUID_UIC(convivial)
SET(UI_SRCS 	ui/messages.hpp 
		ui/messages.cpp
		ui/cb-gen.cpp
		ui/cb-sym.cpp
		ui/cb-pot.cpp
		ui/cb-obs.cpp
		ui/cb-modes.cpp
		ui/basis.hpp
		ui/basis.cpp
		ui/MFCI.cpp
)
SOURCE_GROUP(ui FILES ${UI_SRCS})

SET(MAIN_SRCS main.hpp main.cpp)
SOURCE_GROUP(main FILES ${MAIN_SRCS})

SOURCE_GROUP(fltk FILES ${convivial_SOURCES})

FIND_PACKAGE(PythonLibs REQUIRED)
INCLUDE_DIRECTORIES( ${PYTHON_INCLUDE_PATH} )

ADD_EXECUTABLE(convivial ${MAIN_SRCS} ${convivial_SOURCES} ${UI_SRCS} ${RESOURCES})
TARGET_LINK_LIBRARIES(convivial conviv)
TARGET_LINK_YOCTO(convivial fltk json lingua)
TARGET_LINK_FLTK(convivial)
TARGET_LINK_LIBRARIES(convivial ${PYTHON_LIBRARIES})


########################################################################
##
## embed data into executable
##
########################################################################
GET_TARGET_PROPERTY(convivial_exe convivial LOCATION)

IF(YOCTO_LINUX)
ADD_CUSTOM_COMMAND( TARGET convivial
	POST_BUILD
	COMMAND strip ${convivial_exe}
	COMMENT "Stripping Executable" )
ENDIF(YOCTO_LINUX)
	
ADD_CUSTOM_COMMAND( TARGET convivial
	POST_BUILD
	COMMAND ${RCPACK} ${convivial_exe} ${CMAKE_CURRENT_SOURCE_DIR}/embed
	COMMENT "Attaching Resources" )

INSTALL( TARGETS convivial RUNTIME DESTINATION bin )

ENDIF(FLTK_FOUND)

ADD_SUBDIRECTORY(tests)

