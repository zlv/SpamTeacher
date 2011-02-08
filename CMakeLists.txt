cmake_minimum_required (VERSION 2.6)
include_directories (../)

set (PROJECT 
	SpamTeacher)
 
set (HEADERS 
	src/send/u82w1251.hpp
    src/send/send.hpp
    src/send/base64.h
    src/Receiver.h
    src/Data.h
    src/interface/func.h
    src/interface/qt/SendWindow.h
    src/interface/qt/SenderWindow.h
    src/interface/qt/ReceiverWindow.h
    src/interface/qt/PassWindow.h
    src/interface/qt/MainWindow.h
    src/interface/qt/GenderWindow.h
    src/interface/qt/Delegate.h
    src/interface/qt/BaseWindow.h
    src/interface/qt/AtachmentWindow.h
    src/interface/qt/AddReceiverWindow.h
    src/interface/qt/AddMultReceiverWindow.h
    src/interface/qt/About.h)
 
set (SOURCES 
    src/send/u82w1251.cpp
    src/send/send.cpp
    src/send/base64.cpp
    src/Data.cpp
    src/interface/func.cpp
    src/interface/qt/SendWindow.cpp
    src/interface/qt/SenderWindow.cpp
    src/interface/qt/ReceiverWindow.cpp
    src/interface/qt/PassWindow.cpp
    src/interface/qt/MainWindow.cpp
    src/interface/qt/GenderWindow.cpp
    src/interface/qt/Delegate.cpp
    src/interface/qt/BaseWindow.cpp
    src/interface/qt/AtachmentWindow.cpp
    src/interface/qt/AddReceiverWindow.cpp
    src/interface/qt/AddMultReceiverWindow.cpp
    src/interface/qt/About.cpp
    src/interface/qt/main.cpp)

set (LANGUAGES
	ru)
 
set (TRANSLATIONS_PATH 
    ../language)
 
project (${PROJECT})
 
find_package (Qt4 REQUIRED)
include (${QT_USE_FILE})
qt4_wrap_cpp (MOC_SOURCES ${HEADERS})
foreach (LANGUAGE ${LANGUAGES})
	set (TS ${TRANSLATIONS_PATH}/${LANGUAGE}.ts)
	set (QM ${TRANSLATIONS_PATH}/${LANGUAGE}.qm)
	set (TRANSLATIONS ${TRANSLATIONS} ${TS})
	set (TRANSLATIONS_BINARY ${TRANSLATIONS_BINARY} ${QM})
	add_custom_command (
		OUTPUT ${QM}
		COMMAND ${QT_LRELEASE_EXECUTABLE} ${TS}
		MAIN_DEPENDENCY ${TS})
endforeach()
add_custom_target (
	translations 
	COMMAND ${QT_LUPDATE_EXECUTABLE} ${HEADERS} ${SOURCES} -ts ${TRANSLATIONS})
add_custom_command (
	TARGET translations
	COMMAND ${QT_LRELEASE_EXECUTABLE} ${TRANSLATIONS})
 
foreach (LIBRARY ${LIBRARIES})
	add_subdirectory (../${LIBRARY}/build bin/${LIBRARY})
endforeach ()
 
if (MSVC)
	add_definitions (/W3)
elseif (CMAKE_COMPILER_IS_GNUCXX)
	add_definitions (-Wall -pedantic)
else ()
	message ("Unknown compiler")
endif ()
 
source_group ("Header Files" FILES ${HEADERS})
source_group ("Source Files" FILES ${SOURCES})
source_group ("Generated Files" FILES ${MOC_SOURCES})
 
add_executable (${PROJECT} ${HEADERS} ${SOURCES} ${MOC_SOURCES} ${TRANSLATIONS})
 
target_link_libraries (${PROJECT} ${QT_LIBRARIES})
