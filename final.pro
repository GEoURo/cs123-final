# -------------------------------------------------
# Project created by QtCreator 2010-08-22T14:12:19
# -------------------------------------------------
QT += opengl xml
TARGET = final
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++14
CONFIG += c++14

unix:!macx {
    LIBS += -lGLU
}
macx {
    QMAKE_CFLAGS_X86_64 += -mmacosx-version-min=10.7
    QMAKE_CXXFLAGS_X86_64 = $$QMAKE_CFLAGS_X86_64
    CONFIG += c++11
}
win32 {
    DEFINES += GLEW_STATIC
    LIBS += -lopengl32 -lglu32
}

SOURCES += ui/mainwindow.cpp \
    camera/OrbitingCamera.cpp \
    gl/GLDebug.cpp \
    gl/datatype/FBO.cpp \
    gl/datatype/IBO.cpp \
    gl/datatype/VAO.cpp \
    gl/datatype/VBO.cpp \
    gl/datatype/VBOAttribMarker.cpp \
    gl/shaders/CS123Shader.cpp \
    gl/shaders/Shader.cpp \
    gl/textures/DepthBuffer.cpp \
    gl/textures/RenderBuffer.cpp \
    gl/textures/ShadowCube.cpp \
    gl/textures/ShadowMap.cpp \
    gl/textures/Texture.cpp \
    gl/textures/Texture2D.cpp \
    gl/textures/TextureCube.cpp \
    gl/textures/TextureParameters.cpp \
    gl/textures/TextureParametersBuilder.cpp \
    gl/util/FullScreenQuad.cpp \
    gl/util/TextureManager.cpp \
    main.cpp \
    glew-1.10.0/src/glew.c \
    scene/OpenGLScene.cpp \
    scene/SceneviewScene.cpp \
    shapes/primitives/BaseShape.cpp \
    shapes/primitives/ConeShape.cpp \
    shapes/primitives/CubeShape.cpp \
    shapes/primitives/CylinderShape.cpp \
    shapes/primitives/MobiusShape.cpp \
    shapes/primitives/OpenGLShape.cpp \
    shapes/primitives/SphereShape.cpp \
    shapes/primitives/TorusShape.cpp \
    shapes/tesselators/BarrelTesselator.cpp \
    shapes/tesselators/BaseTesselator.cpp \
    shapes/tesselators/CircleTesselator.cpp \
    shapes/tesselators/ConeHatTesselator.cpp \
    shapes/tesselators/MobiusTesselator.cpp \
    shapes/tesselators/SphereTesselator.cpp \
    shapes/tesselators/SquareTesselator.cpp \
    shapes/tesselators/TorusTesselator.cpp \
    ui/view.cpp \
    ui/viewformat.cpp \
    utils/CS123XmlSceneParser.cpp \
    utils/Databinding.cpp \
    utils/ResourceLoader.cpp \
    utils/settings.cpp

HEADERS += ui/mainwindow.h \
    camera/Camera.h \
    camera/OrbitingCamera.h \
    gl/GLDebug.h \
    gl/datatype/FBO.h \
    gl/datatype/IBO.h \
    gl/datatype/VAO.h \
    gl/datatype/VBO.h \
    gl/datatype/VBOAttribMarker.h \
    gl/shaders/CS123Shader.h \
    gl/shaders/Shader.h \
    gl/shaders/ShaderAttribLocations.h \
    gl/textures/DepthBuffer.h \
    gl/textures/RenderBuffer.h \
    gl/textures/ShadowCube.h \
    gl/textures/ShadowMap.h \
    gl/textures/Texture.h \
    gl/textures/Texture2D.h \
    gl/textures/TextureCube.h \
    gl/textures/TextureParameters.h \
    gl/textures/TextureParametersBuilder.h \
    gl/util/FullScreenQuad.h \
    gl/util/TextureManager.h \
    scene/OpenGLScene.h \
    scene/SceneviewScene.h \
    shapes/primitives/BaseShape.h \
    shapes/primitives/ConeShape.h \
    shapes/primitives/CubeShape.h \
    shapes/primitives/CylinderShape.h \
    shapes/primitives/MobiusShape.h \
    shapes/primitives/OpenGLShape.h \
    shapes/primitives/SphereShape.h \
    shapes/primitives/TorusShape.h \
    shapes/tesselators/BarrelTesselator.h \
    shapes/tesselators/BaseTesselator.h \
    shapes/tesselators/CircleTesselator.h \
    shapes/tesselators/ConeHatTesselator.h \
    shapes/tesselators/MobiusTesselator.h \
    shapes/tesselators/SphereTesselator.h \
    shapes/tesselators/SquareTesselator.h \
    shapes/tesselators/TorusTesselator.h \
    ui_mainwindow.h \
    glew-1.10.0/include/GL/glew.h \
    ui/view.h \
    ui/viewformat.h \
    utils/CS123ISceneParser.h \
    utils/CS123SceneData.h \
    utils/CS123XmlSceneParser.h \
    utils/Databinding.h \
    utils/ResourceLoader.h \
    utils/settings.h

FORMS += ui/mainwindow.ui
INCLUDEPATH += glm ui glew-1.10.0/include
DEPENDPATH += glm ui glew-1.10.0/include

DEFINES += _USE_MATH_DEFINES
DEFINES += TIXML_USE_STL
DEFINES += GLM_SWIZZLE GLM_FORCE_RADIANS
OTHER_FILES += shaders/shader.frag \
    shaders/shader.vert

# Don't add the -pg flag unless you know what you are doing. It makes QThreadPool freeze on Mac OS X
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3
QMAKE_CXXFLAGS_WARN_ON -= -Wall
QMAKE_CXXFLAGS_WARN_ON += -Waddress -Warray-bounds -Wc++0x-compat -Wchar-subscripts -Wformat\
                          -Wmain -Wmissing-braces -Wparentheses -Wreorder -Wreturn-type \
                          -Wsequence-point -Wsign-compare -Wstrict-overflow=1 -Wswitch \
                          -Wtrigraphs -Wuninitialized -Wunused-label -Wunused-variable \
                          -Wvolatile-register-var -Wno-extra

QMAKE_CXXFLAGS += -g

# QMAKE_CXX_FLAGS_WARN_ON += -Wunknown-pragmas -Wunused-function -Wmain

macx {
    QMAKE_CXXFLAGS_WARN_ON -= -Warray-bounds -Wc++0x-compat
}

RESOURCES += \
    resources.qrc

DISTFILES += \
    shaders/normals/normals.vert \
    shaders/normals/normals.frag \
    shaders/normals/normals.gsh \
    shaders/normals/normalsArrow.gsh \
    shaders/normals/normalsArrow.frag \
    shaders/normals/normalsArrow.vert \
    shaders/pointShadow.frag \
    shaders/pointShadow.gsh \
    shaders/pointShadow.vert \
    shaders/shadowmap.frag \
    shaders/shadowmap.vert \
    shaders/tonemapping.frag \
    shaders/wireframe/wireframe.frag \
    shaders/wireframe/wireframe.vert
