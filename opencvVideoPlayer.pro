QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x000000

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    videoplayer.cpp

HEADERS += \
    mainwindow.h \
    videoplayer.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

linux{

    INCLUDEPATH += /usr/local/include/opencv4/opencv2
    INCLUDEPATH += /usr/local/include/opencv4

    LIBS += -L/usr/local/lib \
    -lopencv_core \
    -lopencv_highgui \
    -lopencv_video\
    -lopencv_videoio \
    -lopencv_objdetect\
    -lopencv_imgproc \
    -lopencv_shape\
#    -lopencv_shape\
#    -lopencv_stitching\
#    -lopencv_superres\
#    -lopencv_videostab\
#    -lopencv_imgcodecs\
#    -lopencv_photo\
    -lopencv_ml\
#    -lopencv_tracking\
#    -lopencv_features2d
#    -lopencv_bgsegm
}
