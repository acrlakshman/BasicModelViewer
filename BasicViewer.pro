QT       += core gui widgets

TARGET = basicviewer

CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

qtHaveModule(opengl) {
    QT += opengl

    SOURCES += \
        AuxiliaryMethods/tiny_obj_loader.cc \
        ViewerWindow.cpp \
        ViewerCore/ViewerCamera.cpp \
        ViewerCore/ShadersList.cpp \
        ViewerCore/VertexAttributes.cpp \
        ViewerCore/PlanarMesh.cpp \
        ViewerCore/Object.cpp \
        ViewerCore/GenericObject.cpp \
        ViewerCore/ParseObj.cpp \
        Objects/Sphere.cpp \
        Objects/Plane.cpp \
        Objects/DrawingRoom.cpp \
        Objects/RenderObj.cpp \
        RenderScene.cpp

    HEADERS += \
        AuxiliaryMethods/CheckErrors.h \
        AuxiliaryMethods/HelperFunctions.h \
        AuxiliaryMethods/tiny_obj_loader.h \
        ViewerWindow.h \
        ViewerCore/ViewerCamera.h \
        ViewerCore/ShadersList.h \
        ViewerCore/VertexAttributes.h \
        ViewerCore/PlanarMesh.h \
        ViewerCore/Object.h \
        ViewerCore/GenericObject.h \
        ViewerCore/ParseObj.h \
        Objects/Sphere.h \
        Objects/Plane.h \
        Objects/DrawingRoom.h \
        Objects/RenderObj.h \
        RenderScene.h

    RESOURCES += \
        Shaders/Shaders.qrc \
        Textures/Textures.qrc
}
