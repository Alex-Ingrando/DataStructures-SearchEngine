TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    entryinterface.cpp \
    invertedentry.cpp \
    porter2_stemmer.cpp

HEADERS += \
    entryinterface.h \
    invertedentry.h \
    avlindex.hpp \
    indexinterface.hpp \
    hashindex.hpp \
    porter2_stemmer.h
