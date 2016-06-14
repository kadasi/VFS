TEMPLATE = app
CONFIG += console c++11
CONFIG += ggdb3
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    fs-dummyEntrypoint.cpp \
    fs-manager.cpp \
    fs-memory.cpp \
    fs.cpp \
    generator.cpp \
    snapshot.cpp \
    _glue.cpp \
    tests.cpp

HEADERS += \
    dirent.h \
    divine.h \
    fcntl.h \
    fs-constants.h \
    fs-descriptor.h \
    fs-directory.h \
    fs-file.h \
    fs-inode.h \
    fs-manager.h \
    fs-memory.h \
    fs-path.h \
    fs-snapshot.h \
    fs-storage.h \
    fs-utils.h \
    unistd.h \
    bits/select.h \
    bits/sockaddr.h \
    bits/stat.h \
    bits/types.h \
    bits/poll.h \
    sys/poll.h \
    sys/select.h \
    sys/socket.h \
    sys/stat.h \
    sys/types.h \
    sys/uio.h \
    sys/un.h \
    catch.h

