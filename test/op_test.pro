TEMPLATE = app
CONFIG += console c++14
CONFIG -= qt
CONFIG -= app_bundle

SOURCES += \
    main.cc \
    $$files(onnx/*.cc) \

HEADERS += \
    $$files(onnx/*.h) \

INCLUDEPATH += \
    $$PWD \
    $$PWD/../install/include \
    $$PWD/googletest/googletest/include

DISTFILES += \
    CMakeLists.txt
