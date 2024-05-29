cmake_minimum_required(VERSION 3.5)

project(TRT_demo VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets PrintSupport REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets PrintSupport REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS OpenGL PrintSupport REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS 3DCore 3DRender 3DInput 3DExtras PrintSupport SerialPort REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Xml Multimedia MultimediaWidgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Charts REQUIRED)

set(PROJECT_PATH D:/TensorRT_Demo/TRT_Demo)
set(TWINCAT_PATH D:/TwinCAT/TwinCAT)
set(CUDA_PATH C:/Program\ Files/NVIDIA\ GPU\ Computing\ Toolkit/CUDA/v11.1)
set(NIDAQ_PATH $ENV{ProgramFiles\(x86\)}/National\ Instruments/)
set(OPENCV_PATH C:/opencv/build)
set(OPENGL_PATH $ENV{ProgramFiles\(x86\)}/Windows\ Kits/10/Lib/10.0.19041.0/um/x64)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.h
    mainwindow.cpp
    mainwindow.ui
    tensorrt_module.h
    tensorrt_module.cpp
    ni_ai_ao_do.h
    ni_ai_ao_do.cpp
    qcustomplot.h
    qcustomplot.cpp
    scenemodifier.h
    scenemodifier.cpp
    histogramwidget.h
    histogramwidget.cpp
    playercontrols.h
    playercontrols.cpp
    playlistmodel.h
    playlistmodel.cpp
    videowidget.h
    videowidget.cpp
    painttestone.h
    painttestone.cpp
    opengltestone.h
    opengltestone.cpp

    trainingform.h
    trainingform.cpp
    trainingform.ui
    traininguserform.h
    traininguserform.cpp
    traininguserform.ui
    unlabeleddaqform.h
    unlabeleddaqform.cpp
    unlabeleddaqform.ui
    unlabeleddaquserform.h
    unlabeleddaquserform.cpp
    unlabeleddaquserform.ui
    testoneform.h
    testoneform.cpp
    testoneform.ui
    testoneuserform.h
    testoneuserform.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(TRT_demo
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET WDGRL_TRT APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(TRT_demo SHARED
            ${PROJECT_SOURCES}
            main.cpp
            mainwindow.h
            mainwindow.cpp
            mainwindow.ui
            tensorrt_module.h
            tensorrt_module.cpp
            ni_ai_ao_do.h
            ni_ai_ao_do.cpp
            qcustomplot.h
            qcustomplot.cpp
            scenemodifier.h
            scenemodifier.cpp
            histogramwidget.h
            histogramwidget.cpp
            playercontrols.h
            playercontrols.cpp
            playlistmodel.h
            playlistmodel.cpp
            videowidget.h
            videowidget.cpp
            painttestone.h
            painttestone.cpp
            opengltestone.h
            opengltestone.cpp

            trainingform.h
            trainingform.cpp
            trainingform.ui
            traininguserform.h
            traininguserform.cpp
            traininguserform.ui
            unlabeleddaqform.h
            unlabeleddaqform.cpp
            unlabeleddaqform.ui
            unlabeleddaquserform.h
            unlabeleddaquserform.cpp
            unlabeleddaquserform.ui
            testoneform.h
            testoneform.cpp
            testoneform.ui
            testoneuserform.h
            testoneuserform.cpp
            C:/Program\ Files/NVIDIA\ GPU\ Computing\ Toolkit/CUDA/v11.1/samples/common/logger.cpp
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(TRT_demo
            ${PROJECT_SOURCES}
            main.cpp
            mainwindow.h
            mainwindow.cpp
            mainwindow.ui
            tensorrt_module.h
            tensorrt_module.cpp
            ni_ai_ao_do.h
            ni_ai_ao_do.cpp
            qcustomplot.h
            qcustomplot.cpp
            scenemodifier.h
            scenemodifier.cpp
            histogramwidget.h
            histogramwidget.cpp
            playercontrols.h
            playercontrols.cpp
            playlistmodel.h
            playlistmodel.cpp
            videowidget.h
            videowidget.cpp
            painttestone.h
            painttestone.cpp
            opengltestone.h
            opengltestone.cpp

            trainingform.h
            trainingform.cpp
            trainingform.ui
            traininguserform.h
            traininguserform.cpp
            traininguserform.ui
            unlabeleddaqform.h
            unlabeleddaqform.cpp
            unlabeleddaqform.ui
            unlabeleddaquserform.h
            unlabeleddaquserform.cpp
            unlabeleddaquserform.ui
            testoneform.h
            testoneform.cpp
            testoneform.ui
            testoneuserform.h
            testoneuserform.cpp
            C:/Program\ Files/NVIDIA\ GPU\ Computing\ Toolkit/CUDA/v11.1/samples/common/logger.cpp
        )
    endif()
endif()

target_link_libraries(TRT_demo PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
                                        Qt${QT_VERSION_MAJOR}::PrintSupport
                                        Qt${QT_VERSION_MAJOR}::OpenGL
                                        Qt${QT_VERSION_MAJOR}::Core
                                        Qt${QT_VERSION_MAJOR}::Gui
                                        Qt${QT_VERSION_MAJOR}::3DCore
                                        Qt${QT_VERSION_MAJOR}::3DExtras
                                        Qt${QT_VERSION_MAJOR}::3DRender
                                        Qt${QT_VERSION_MAJOR}::3DInput
                                        Qt${QT_VERSION_MAJOR}::SerialPort
                                        Qt${QT_VERSION_MAJOR}::Xml
                                        Qt${QT_VERSION_MAJOR}::Multimedia
                                        Qt${QT_VERSION_MAJOR}::MultimediaWidgets
                                        Qt${QT_VERSION_MAJOR}::Charts)

target_include_directories(TRT_demo PUBLIC
    ${CUDA_PATH}/include
    ${CUDA_PATH}/samples/common
    ${CUDA_PATH}/samples/common/windows
    ${OPENCV_PATH}/include
    ${NIDAQ_PATH}/NI-DAQ/DAQmx\ ANSI\ C\ Dev/include
    ${TWINCAT_PATH}
    ${PROJECT_PATH}/eigen-3.4.0
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(TRT_demo PUBLIC
        ${CUDA_PATH}/lib/x64/myelin64_1.lib
        ${CUDA_PATH}/lib/x64/nvinfer.lib
        ${CUDA_PATH}/lib/x64/nvinfer_plugin.lib
        ${CUDA_PATH}/lib/x64/nvonnxparser.lib
        ${CUDA_PATH}/lib/x64/nvparsers.lib
        ${CUDA_PATH}/lib/x64/cudart.lib
        ${CUDA_PATH}/lib/x64/cuda.lib
        ${OPENCV_PATH}/x64/vc15/lib/opencv_world450d.lib
        ${NIDAQ_PATH}/Shared/ExternalCompilerSupport/C/lib64/msvc/NIDAQmx.lib
        ${OPENGL_PATH}/OpenGL32.Lib
    )
else()
    target_link_libraries(TRT_demo PUBLIC
        ${CUDA_PATH}/lib/x64/myelin64_1.lib
        ${CUDA_PATH}/lib/x64/nvinfer.lib
        ${CUDA_PATH}/lib/x64/nvinfer_plugin.lib
        ${CUDA_PATH}/lib/x64/nvonnxparser.lib
        ${CUDA_PATH}/lib/x64/nvparsers.lib
        ${CUDA_PATH}/lib/x64/cudart.lib
        ${CUDA_PATH}/lib/x64/cuda.lib
        ${OPENCV_PATH}/x64/vc15/lib/opencv_world450.lib
        ${NIDAQ_PATH}/Shared/ExternalCompilerSupport/C/lib64/msvc/NIDAQmx.lib
        ${OPENGL_PATH}/OpenGL32.Lib
    )
endif()

set_target_properties(TRT_demo PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

install(TARGETS TRT_demo
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(TRT_demo)
endif()
