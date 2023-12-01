#-------------------------------------------------
#
# Project created by QtCreator 2018-08-28T10:55:17
#
#-------------------------------------------------

QT       += core gui opengl
#UpgradeQt6: QT += core gui opengl openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Mesh_Computational_Geometry
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    gldisplaywidget.cpp \
    mesh.cpp

HEADERS  += mainwindow.h \
    Vector3D.h \
    gldisplaywidget.h \
    mesh.h

FORMS    += mainwindow.ui

#---- Comment the following line on MacOS
#---- Uncomment it on Windows and Linux
#LIBS = -lGLU

#---- Uncomment the following line on Windows
#---- Comment it on Linux and MacOS
#LIBS += -lglu32
LIBS += -lOpengl32
LIBS += -LC:/Qt/Tools/mingw810_32/i686-w64-mingw32/lib -lGLU32

DISTFILES += \
    alpes_poisson.txt


