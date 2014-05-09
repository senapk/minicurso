TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    novo.cpp

CONFIG += c++11

LIBS += -lbug -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
