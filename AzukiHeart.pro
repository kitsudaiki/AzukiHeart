QT -= qt core gui

TARGET = AzukiHeart
CONFIG += console
CONFIG += c++17

LIBS += -L../libAzukiHeart/src -lAzukiHeart
LIBS += -L../libAzukiHeart/src/debug -lAzukiHeart
LIBS += -L../libAzukiHeart/src/release -lAzukiHeart
INCLUDEPATH += ../libAzukiHeart/include

LIBS += -L../libMisakiGuard/src -lMisakiGuard
LIBS += -L../libMisakiGuard/src/debug -lMisakiGuard
LIBS += -L../libMisakiGuard/src/release -lMisakiGuard
INCLUDEPATH += ../libMisakiGuard/include

LIBS += -L../libKitsunemimiHanamiNetwork/src -lKitsunemimiHanamiNetwork
LIBS += -L../libKitsunemimiHanamiNetwork/src/debug -lKitsunemimiHanamiNetwork
LIBS += -L../libKitsunemimiHanamiNetwork/src/release -lKitsunemimiHanamiNetwork
INCLUDEPATH += ../libKitsunemimiHanamiNetwork/include

LIBS += -L../libKitsunemimiHanamiCommon/src -lKitsunemimiHanamiCommon
LIBS += -L../libKitsunemimiHanamiCommon/src/debug -lKitsunemimiHanamiCommon
LIBS += -L../libKitsunemimiHanamiCommon/src/release -lKitsunemimiHanamiCommon
INCLUDEPATH += ../libKitsunemimiHanamiCommon/include

LIBS += -L../libKitsunemimiSakuraHardware/src -lKitsunemimiSakuraHardware
LIBS += -L../libKitsunemimiSakuraHardware/src/debug -lKitsunemimiSakuraHardware
LIBS += -L../libKitsunemimiSakuraHardware/src/release -lKitsunemimiSakuraHardware
INCLUDEPATH += ../libKitsunemimiSakuraHardware/include

LIBS += -L../libKitsunemimiArgs/src -lKitsunemimiArgs
LIBS += -L../libKitsunemimiArgs/src/debug -lKitsunemimiArgs
LIBS += -L../libKitsunemimiArgs/src/release -lKitsunemimiArgs
INCLUDEPATH += ../libKitsunemimiArgs/include

LIBS += -L../libKitsunemimiConfig/src -lKitsunemimiConfig
LIBS += -L../libKitsunemimiConfig/src/debug -lKitsunemimiConfig
LIBS += -L../libKitsunemimiConfig/src/release -lKitsunemimiConfig
INCLUDEPATH += ../libKitsunemimiConfig/include

LIBS += -L../libKitsunemimiSakuraNetwork/src -lKitsunemimiSakuraNetwork
LIBS += -L../libKitsunemimiSakuraNetwork/src/debug -lKitsunemimiSakuraNetwork
LIBS += -L../libKitsunemimiSakuraNetwork/src/release -lKitsunemimiSakuraNetwork
INCLUDEPATH += ../libKitsunemimiSakuraNetwork/include

LIBS += -L../libKitsunemimiCommon/src -lKitsunemimiCommon
LIBS += -L../libKitsunemimiCommon/src/debug -lKitsunemimiCommon
LIBS += -L../libKitsunemimiCommon/src/release -lKitsunemimiCommon
INCLUDEPATH += ../libKitsunemimiCommon/include

LIBS += -L../libKitsunemimiNetwork/src -lKitsunemimiNetwork
LIBS += -L../libKitsunemimiNetwork/src/debug -lKitsunemimiNetwork
LIBS += -L../libKitsunemimiNetwork/src/release -lKitsunemimiNetwork
INCLUDEPATH += ../libKitsunemimiNetwork/include

LIBS += -L../libKitsunemimiJson/src -lKitsunemimiJson
LIBS += -L../libKitsunemimiJson/src/debug -lKitsunemimiJson
LIBS += -L../libKitsunemimiJson/src/release -lKitsunemimiJson
INCLUDEPATH += ../libKitsunemimiJson/include

LIBS += -L../libKitsunemimiIni/src -lKitsunemimiIni
LIBS += -L../libKitsunemimiIni/src/debug -lKitsunemimiIni
LIBS += -L../libKitsunemimiIni/src/release -lKitsunemimiIni
INCLUDEPATH += ../libKitsunemimiIni/include

LIBS += -L../libKitsunemimiJwt/src -lKitsunemimiJwt
LIBS += -L../libKitsunemimiJwt/src/debug -lKitsunemimiJwt
LIBS += -L../libKitsunemimiJwti/src/release -lKitsunemimiJwt
INCLUDEPATH += ../libKitsunemimiJwt/include

LIBS += -L../libKitsunemimiCrypto/src -lKitsunemimiCrypto
LIBS += -L../libKitsunemimiCrypto/src/debug -lKitsunemimiCrypto
LIBS += -L../libKitsunemimiCrypto/src/release -lKitsunemimiCrypto
INCLUDEPATH += ../libKitsunemimiCrypto/include

LIBS += -L../libKitsunemimiCpu/src -lKitsunemimiCpu
LIBS += -L../libKitsunemimiCpu/src/debug -lKitsunemimiCpu
LIBS += -L../libKitsunemimiCpu/src/release -lKitsunemimiCpu
INCLUDEPATH += ../libKitsunemimiCpu/include

LIBS += -lcryptopp -lssl -luuid -lcrypto -lprotobuf -lpthread

INCLUDEPATH += $$PWD \
               src

SOURCES += src/main.cpp \
    src/api/v1/measurements/power_consumption.cpp \
    src/api/v1/measurements/speed.cpp \
    src/api/v1/measurements/temperature_production.cpp \
    src/api/v1/system_info/get_system_info.cpp \
    src/api/v1/threading/get_thread_mapping.cpp \
    src/azuki_root.cpp \
    src/core/power_measuring.cpp \
    src/core/speed_measuring.cpp \
    src/core/temperature_measuring.cpp \
    src/core/thread_binder.cpp \
    src/core/value_container.cpp

HEADERS += \
    src/api/blossom_initializing.h \
    src/api/v1/measurements/power_consumption.h \
    src/api/v1/measurements/speed.h \
    src/api/v1/measurements/temperature_production.h \
    src/api/v1/system_info/get_system_info.h \
    src/api/v1/threading/get_thread_mapping.h \
    src/args.h \
    src/azuki_root.h \
    src/callbacks.h \
    src/config.h \
    src/core/power_measuring.h \
    src/core/speed_measuring.h \
    src/core/temperature_measuring.h \
    src/core/thread_binder.h \
    src/core/value_container.h

AZUKI_PROTO_BUFFER = ../libKitsunemimiHanamiMessages/protobuffers/azuki_messages.proto3

OTHER_FILES += $$AZUKI_PROTO_BUFFER

protobuf_decl.name = protobuf headers
protobuf_decl.name = protobuf headers
protobuf_decl.input = KYOUKO_PROTO_BUFFER
protobuf_decl.output = ${QMAKE_FILE_IN_PATH}/${QMAKE_FILE_BASE}.proto3.pb.h
protobuf_decl.commands = protoc --cpp_out=${QMAKE_FILE_IN_PATH} --proto_path=${QMAKE_FILE_IN_PATH} ${QMAKE_FILE_NAME}
protobuf_decl.variable_out = HEADERS
QMAKE_EXTRA_COMPILERS += protobuf_decl

protobuf_impl.name = protobuf sources
protobuf_impl.input = KYOUKO_PROTO_BUFFER
protobuf_impl.output = ${QMAKE_FILE_IN_PATH}/${QMAKE_FILE_BASE}.proto3.pb.cc
protobuf_impl.depends = ${QMAKE_FILE_IN_PATH}/${QMAKE_FILE_BASE}.proto3.pb.h
protobuf_impl.commands = $$escape_expand(\n)
protobuf_impl.variable_out = SOURCES
QMAKE_EXTRA_COMPILERS += protobuf_impl
