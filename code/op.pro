CONFIG -= qt

TEMPLATE = lib
DEFINES += OP_LIBRARY

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    $$files(*.cc)           \
    $$files(op/*.cc)        \
    $$files(op/onnx/*.cc)   \
    $$files(res/*.cc)       \
    $$files(utils/*.cc)     \

HEADERS += \
    $$files(include/*.h)    \
    $$files(op/*.h)         \
    $$files(op/onnx/*.h)    \
    $$files(res/*.h)        \
    $$files(utils/*.h)      \

DISTFILES += \
    CMakeLists.txt

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
