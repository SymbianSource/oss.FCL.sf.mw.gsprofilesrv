#
# Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
#
# Initial Contributors:
# Nokia Corporation - initial contribution.
#
# Contributors:
#
# Description: 
#

TEMPLATE = lib
TARGET = seccodeui

CONFIG += hb

DEFINES += BUILD_SECCODEUI

MOC_DIR = moc
OBJECT_DIR = obj
RCC_DIR = rcc

include ( seccodeui.pri )

CONFIG(debug, debug|release) {
    SUBDIRPART = debug
} else {
    SUBDIRPART = release
}

win32: { 
    DESTDIR = C:/ControlPanel/$$SUBDIRPART/bin
    OBJECTS_DIR = $$PWD/../$$SUBDIRPART/tmp/$$TARGET
}

# Add the output dirs to the link path too
LIBS += -L$$DESTDIR

symbian: {
    INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE
    INCLUDEPATH += $$MOC_DIR
    TARGET.CAPABILITY = ALL -TCB
    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.UID3 = 0x20028733
}

