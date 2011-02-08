TARGET = MassDelivery
SOURCES += \
    src/send/u82w1251.cpp \
    src/send/send.cpp \
    src/send/base64.cpp \
    src/Data.cpp \
    src/interface/qt/SendWindow.cpp \
    src/interface/qt/SenderWindow.cpp \
    src/interface/qt/ReceiverWindow.cpp \
    src/interface/qt/PassWindow.cpp \
    src/interface/qt/MainWindow.cpp \
    src/interface/qt/GenderWindow.cpp \
    src/interface/qt/Delegate.cpp \
    src/interface/qt/BaseWindow.cpp \
    src/interface/qt/AtachmentWindow.cpp \
    src/interface/qt/AddReceiverWindow.cpp \
    src/interface/qt/AddMultReceiverWindow.cpp \
    src/interface/qt/About.cpp \
    src/interface/qt/main.cpp \
    src/interface/func.cpp
HEADERS += \
    src/send/u82w1251.hpp \
    src/send/send.hpp \
    src/send/base64.h \
    src/Receiver.h \
    src/func.h \
    src/Data.h \
    src/interface/qt/SendWindow.h \
    src/interface/qt/SenderWindow.h \
    src/interface/qt/ReceiverWindow.h \
    src/interface/qt/PassWindow.h \
    src/interface/qt/MainWindow.h \
    src/interface/qt/GenderWindow.h \
    src/interface/qt/Delegate.h \
    src/interface/qt/BaseWindow.h \
    src/interface/qt/AtachmentWindow.h \
    src/interface/qt/AddReceiverWindow.h \
    src/interface/qt/AddMultReceiverWindow.h \
    src/interface/qt/About.h \
    src/interface/interface.h \
    src/interface/func.h
OTHER_FILES += data.txt \
    README \
    todo.txt \
    info.txt \
    src/send/Makefile \
    src/interface/gtk/SpamTeacher \
    src/interface/gtk/sendEmail.pl \
    src/interface/gtk/sendEmail.exe \
    src/interface/gtk/README2 \
    src/interface/gtk/readme.txt \
    src/interface/gtk/Makefile2 \
    src/interface/gtk/Makefile
#для винды / for windows
#LIBS += -lws2_32
