HEADERS = droparea.h \
    dropsitewindow.h \
    imageviewer.h \
    dragwidget.h
SOURCES = droparea.cpp \
    dropsitewindow.cpp \
    main.cpp \
    imageviewer.cpp \
    dragwidget.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/draganddrop/dropsite
sources.files = $$SOURCES \
    $$HEADERS \
    $$RESOURCES \
    *.pro \
    images
sources.path = $$[QT_INSTALL_EXAMPLES]/draganddrop/dropsite
INSTALLS += target \
    sources
