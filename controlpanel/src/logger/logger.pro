#
# Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
TARGET = cplogger

include ( ../common.pri )
include ( logger.pri)

symbian: { 
    TARGET.UID3 = 0x20028734
}

DEFINES += BUILD_LOGGER_DLL
    


symbian:MMP_RULES += SMPSAFE
