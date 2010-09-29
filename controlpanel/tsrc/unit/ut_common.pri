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

QT		  += testlib

CONFIG += hb qtestlib
CONFIG += symbian_test

QMAKE_EXTRA_TARGETS += test autotest

DEPENDPATH += .

symbian {
    INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE
    INCLUDEPATH += $$MW_LAYER_PLATFORM_EXPORT_PATH(cplogger)
}
							 
LIBS += -lcpframework -lcplogger
