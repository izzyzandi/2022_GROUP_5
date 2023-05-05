#group
cmake_minimum_required(VERSION 3.5)

project(testProject VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(VTK REQUIRED)

set(PROJECT_SOURCES
      main.cpp
      mainwindow.cpp
      mainwindow.h
      mainwindow.ui
   	  ModelPart.cpp
      ModelPart.h
      ModelPartList.h
      ModelPartList.cpp
      Icons.qrc
      optiondialog.h
      optiondialog.cpp 
      optiondialog.ui
      VRRenderThread.h
      VRRenderThread.cpp
	 
)



if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(testProject
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${APP_ICON_RESOURCE_WINDOWS}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET testProject APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(testProject SHARED
            ${PROJECT_SOURCES}
            ${APP_ICON_RESOURCE_WINDOWS}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(testProject
            ${PROJECT_SOURCES}
            ${APP_ICON_RESOURCE_WINDOWS}
        )
    endif()
endif()

target_link_libraries(testProject PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${VTK_LIBRARIES})


set_target_properties(testProject PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS testProject
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(testProject)
endif()

add_custom_target(VRBindings)
add_custom_command(TARGET VRBindings PRE_BUILD
COMMAND ${CMAKE_COMMAND} -E
		copy_directory ${CMAKE_SOURCE_DIR}/vrbindings ${CMAKE_BINARY_DIR}/ )


# INSTALLER


include(InstallRequiredSystemLibraries) 
set(CPACK_PACKAGE_NAME "stl_file_renderer") 
set(CPACK_PACKAGE_VENDOR "Group 5")
set(CPACK_GENERATOR "NSIS")

# Use NSIS for a Windows installer 

# "Add to the path"" option and add the shortcut to the desktop
set(CPACK_NSIS_MODIFY_PATH ON)
set(CPACK_CREATE_DESKTOP_LINKS testProject)

# Start menu addition 
set(CPACK_PACKAGE_EXECUTABLES "testProject" "stl_file_renderer")

include(CPack)
install(TARGETS testProject RUNTIME DESTINATION bin)



install(DIRECTORY "D:/software/6.4.2/msvc2019_64/bin" DESTINATION QT COMPONENT QT PATTERN "*.dll") 
install(DIRECTORY "D:/software/6.4.2/msvc2019_64/plugins" DESTINATION QT COMPONENT QT FILES_MATCHING PATTERN "*.dll")
install(DIRECTORY "C:/Program Files (x86)/VTK/bin" DESTINATION VTK COMPONENT VTK PATTERN "*.dll")
install(DIRECTORY "D:/docs/Y2 ECE/Practical/COMPUTER/VTK/openvr/bin" DESTINATION OpenVR COMPONENT OPENVR FILES_MATCHING PATTERN "*.dll")
install(DIRECTORY "C:/Users/izzy/Downloads/Level0/Level0" DESTINATION example_stl_files COMPONENT example_stl_files)

cpack_add_component_group(Libraries DESCRIPTION "Libraries required")
cpack_add_component(VTK DISPLAY_NAME "VTK Libraries" DESCRIPTION "Qt dynamic libraries" GROUP Libraries)
cpack_add_component(QT DISPLAY_NAME "QT Libraries" DESCRIPTION "VTK dynamic libraries" GROUP Libraries)
cpack_add_component(OpenVR DISPLAY_NAME "OpenVR" DESCRIPTION "OpenVR dynamic libraries"GROUP Libraries)
cpack_add_component(example_stl_files DISPLAY_NAME "sample files" DESCRIPTION "sample files for software")

