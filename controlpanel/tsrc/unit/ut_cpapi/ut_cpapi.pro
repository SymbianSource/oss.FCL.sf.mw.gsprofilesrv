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

TEMPLATE = app
TARGET = ut_cpapi
QT += testlib
CONFIG += hb qtestlib
unix {
    test.commands = /epoc32/RELEASE/WINSCW/udeb/ut_cpapi.exe
    autotest.commands = /epoc32/RELEASE/WINSCW/udeb/ut_cpapi.exe -xml -o c:/ut_cpapi.xml
} else:win32 {
    test.CONFIG += recursive
    autotest.CONFIG += recursive
    build_pass {
        test.commands =/epoc32/RELEASE/WINSCW/udeb/ut_cpapi.exe
        autotest.commands =/epoc32/RELEASE/WINSCW/udeb/ut_cpapi.exe -xml -o c:/ut_cpapi.xml
        }
}
QMAKE_EXTRA_TARGETS += test autotest

DEPENDPATH += .

INCLUDEPATH += . \
							 src/\
							 ../../../src/inc \
							 ../../../controlpanel_plat/inc
							 
LIBS += -lcpframework

# Input
HEADERS += src/ut_cpapi.h
SOURCES += src/ut_cpapi.cpp

symbian {
	BLD_INF_RULES.prj_exports += "data/controlpanellog.conf C:/data/.config/ut_cpapi/controlpanellog.conf"
}