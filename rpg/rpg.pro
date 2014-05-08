TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

SOURCES += main.cpp

LIBS += -lbug -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
