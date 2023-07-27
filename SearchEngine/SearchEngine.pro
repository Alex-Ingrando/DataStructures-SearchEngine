TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    tests.cpp \
    entryinterface.cpp \
    queryprocessor.cpp \
    indexhandler.cpp \
    porter2_stemmer.cpp \
    documentparser.cpp

HEADERS += \
    catch.hpp \
    avlindex.hpp \
    entryinterface.h \
    queryprocessor.hpp \
    indexhandler.hpp \
    hashindex.hpp \
    porter2_stemmer.h \
    documentparser.hpp
