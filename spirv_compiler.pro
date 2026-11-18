#QT      += core gui
CONFIG -= qt

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# check is use shadow build
!equals(_PRO_FILE_PWD_, $${OUT_PWD}) {
    message(_PRO_FILE_PWD_: $$_PRO_FILE_PWD_)
    message(OUT_PWD: $$OUT_PWD)
    error(you is use shadow build)
}

# platform
contains(QT_ARCH, i386) {
    PLATFORM = x86
}
else {
    PLATFORM = x64
}

#CONFIG += c++11
CONFIG += sse2

# c/c++ 静态编译
#QMAKE_LFLAGS += -static

# GBK       GBK 中文编码
# UTF-8     UTF-8 编码
#QMAKE_CXXFLAGS += -finput-charset=GBK
#QMAKE_CXXFLAGS += -fexec-charset=GBK

DEFINES -= UNICODE _UNICODE
DEFINES += CGL_USE_GLFW
DEFINES += CGL_USE_GLEW
DEFINES += GLEW_STATIC
DEFINES += CGL_USE_IMGUI_DOCKING

# configure
TEMPLATE = app
# TARGET = console
DESTDIR = bin/$${PLATFORM}
DLLDESTDIR = bin/$${PLATFORM}

# include and library directory
INCLUDEPATH += "include"
LIBS += -L./lib
LIBS += -L./lib/$${PLATFORM}

INCLUDEPATH += "x:/include" "x:/lib/include" "x:/include/imgui-docking"
LIBS += -Lx:/lib/windows/gcc/$${PLATFORM}

equals(TEMPLATE, app) {
    CONFIG += console
}
equals(TEMPLATE, lib) {
    # shared | dll | staticlib
    CONFIG += shared
    contains(CONFIG, shared) {
    }
    else {
    }
}

# intermediate directory
CONFIG(debug, debug|release) {
    MOC_DIR = obj/$${PLATFORM}/debug/moc
    OBJECTS_DIR = obj/$${PLATFORM}/debug
    RCC_DIR = obj/$${PLATFORM}/debug/rcc
    UI_DIR = obj/$${PLATFORM}/debug/ui
} else {
    MOC_DIR = obj/$${PLATFORM}/release/moc
    OBJECTS_DIR = obj/$${PLATFORM}/release
    RCC_DIR = obj/$${PLATFORM}/release/rcc
    UI_DIR = obj/$${PLATFORM}/release/ui
}

# include(x:/lib/gl.pri)

# vulkan
# INCLUDEPATH += "x:/vulkan/include"

# glslang-16.2.0-shared
#INCLUDEPATH += "x:/vulkan/glslang-16.2.0-shared/include"
#LIBS += -Lx:/vulkan/glslang-16.2.0-shared/lib/$${PLATFORM}
#LIBS += -lglslang-default-resource-limits

LIBS += -limgui-docking-glfw
LIBS += -limgui-docking-gl3
LIBS += -limgui-docking.dll
contains(DEFINES, GLEW_STATIC) {
    LIBS += -lglew32static
}
else {
    LIBS += -lglew32
}
LIBS += -lglfw3

win32 {
    LIBS += -lOpenGL32
    LIBS += -lgdi32
}

SOURCES += \
    main.cpp \
    pch.cpp \
    spirv_compiler.cpp \
    spirv_disassemble.cpp \
    spirv_tools.cpp \
    src/TextEditor.cpp \
    src/glslang_loader.cpp \
    src/spirv_tools_loader.cpp \
    ui.cpp

HEADERS += \
    include/glslang_loader.h \
    include/spirv_tools_loader.h \
    pch.h \
    spirv.hpp \
    spirv_compiler.h \
    spirv_disassemble.h \
    spirv_exsample.h \
    spirv_tools.h \
    ui.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
