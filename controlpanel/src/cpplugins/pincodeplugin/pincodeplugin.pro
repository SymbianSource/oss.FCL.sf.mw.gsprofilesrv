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
TARGET = cppincodeplugin

CONFIG += hb plugin

LIBS += -lcpframework -lseccodeui

include ( ../cpplugincommon.pri )
include ( pincodeplugin.pri )

symbian { 
    TARGET.UID3 = 0X20028731
}