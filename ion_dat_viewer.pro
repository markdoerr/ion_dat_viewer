RESOURCES += images.qrc

HEADERS += mainwindow.h well_plot.h \
    well_view.h \
    dat_loader.h
SOURCES += main.cpp \
    well_view.cpp \
    dat_loader.cpp
SOURCES += mainwindow.cpp well_plot.cpp

QMAKE_CXXFLAGS += -std=c++11

contains(QT_CONFIG, opengl):QT += opengl

build_all:!build_pass {
    CONFIG -= build_all
    CONFIG += release
}

# install
#target.path = $$[QT_INSTALL_DEMOS]/chip
sources.files = $$SOURCES $$HEADERS $$RESOURCES *.png *.pro *.html *.doc images
#sources.path = $$[QT_INSTALL_DEMOS]/chip
INSTALLS += target sources

#symbian: include($$QT_SOURCE_TREE/demos/symbianpkgrules.pri)
