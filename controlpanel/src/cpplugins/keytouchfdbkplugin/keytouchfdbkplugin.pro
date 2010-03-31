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
TARGET = cpkeytouchfdbkplugin

CONFIG += hb plugin

LIBS += -lcpframework \
        -lcpprofilewrapper

include ( ../cpplugincommon.pri )
include ( keytouchfdbkplugin.pri )

symbian { 
    TARGET.UID3 = 0X20025FDE
}
