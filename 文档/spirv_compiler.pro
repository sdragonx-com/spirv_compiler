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

CONFIG += c++17
CONFIG += sse2

# GBK       GBK 中文编码
# UTF-8     UTF-8 编码
#QMAKE_CXXFLAGS += -finput-charset=GBK
#QMAKE_CXXFLAGS += -fexec-charset=GBK

DEFINES -= UNICODE _UNICODE
# DEFINES += GLEW_STATIC
# DEFINES += FREEGLUT_STATIC

# c/c++ 静态编译
QMAKE_LFLAGS += -static
DEFINES += GLSLANG_STATIC

QMAKE_CFLAGS += -Wl,--exclude-all-symbols
QMAKE_CXXFLAGS += -Wl,--exclude-all-symbols

# configure
TEMPLATE = app
#TEMPLATE = lib
# TARGET = console
DESTDIR = bin/$${PLATFORM}
DLLDESTDIR = bin/$${PLATFORM}

# include and library directory
INCLUDEPATH += "include"
LIBS += -L./lib
LIBS += -L./lib/$${PLATFORM}

INCLUDEPATH += "x:/include"
LIBS += -Lx:/lib/windows/gcc/$${PLATFORM}

equals(TEMPLATE, app) {
    CONFIG += console
}
equals(TEMPLATE, lib) {
    # shared | dll | staticlib
    CONFIG += shared
    DEFINES += PROJECT_SPIRV_LIBRARY
    contains(DEFINES, GLSLANG_STATIC) {
        TARGET = spirv_compiler
    }
    else {
        TARGET = spirv_compiler_shared
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

# vulkan
INCLUDEPATH += "x:/vulkan/include"

#-------------------------------------------------------------------------------

contains(DEFINES, GLSLANG_STATIC) {
    # glslang-16.2.0-static
    INCLUDEPATH += "x:/vulkan/glslang-16.2.0-static/include"
    LIBS += -Lx:/vulkan/glslang-16.2.0-static/lib/$${PLATFORM}
    LIBS += -lglslang-default-resource-limits

    # 核心库和基础依赖
    LIBS += -lglslang -lglslang-default-resource-limits
    LIBS += -lMachineIndependent -lOSDependent
    LIBS += -lGenericCodeGen

    # SPIRV-Tools 核心（按照依赖顺序）
    LIBS += -lSPIRV-Tools-opt
    LIBS += -lSPIRV-Tools-link
    LIBS += -lSPIRV-Tools-lint
    LIBS += -lSPIRV-Tools-diff
    LIBS += -lSPIRV-Tools-reduce
    LIBS += -lSPIRV-Tools

    # 高级SPIRV库
    LIBS += -lSPIRV

    message("static")
}
else {
    # glslang-16.2.0-shared
    INCLUDEPATH += "x:/vulkan/glslang-16.2.0-shared/include"
    LIBS += -Lx:/vulkan/glslang-16.2.0-shared/lib/$${PLATFORM}
    LIBS += -lglslang-default-resource-limits

    # 核心库和基础依赖
    LIBS += -lglslang.dll -lglslang-default-resource-limits
    LIBS += -lMachineIndependent -lOSDependent
    LIBS += -lGenericCodeGen

    # SPIRV-Tools 核心（按照依赖顺序）
    LIBS += -lSPIRV-Tools-opt
    LIBS += -lSPIRV-Tools-link
    LIBS += -lSPIRV-Tools-lint
    LIBS += -lSPIRV-Tools-diff
    LIBS += -lSPIRV-Tools-reduce
    #LIBS += -lSPIRV-Tools-shared.dll
    LIBS += -lSPIRV-Tools

    # 高级SPIRV库
    LIBS += -lSPIRV.dll

    message("shared")
}

#-------------------------------------------------------------------------------

# windows configure
win32 {
    CONFIG += embed_manifest_dll
    LIBS += -lopengl32 -lglu32
    LIBS += -lgdi32 -lgdiplus
    LIBS += -luuid -lole32 -lcomdlg32 -ldwmapi -lwinmm
    LIBS += -lminiz

    LIBS += -lstdc++
}

SOURCES += \
    dllmain.cpp \
    main.cpp \
    pch.cpp \
    spirv_compiler.cpp \
    spirv_info.cpp

HEADERS += \
    pch.h \
    spirv_compiler.h \
    spirv_exsample.h \
    spirv_info.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
