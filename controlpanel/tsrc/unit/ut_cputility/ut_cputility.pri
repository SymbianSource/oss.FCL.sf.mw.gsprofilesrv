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

HEADERS   += src/ut_cputility.h \
	../../../src/cpframework/src/cputility.h \
	
SOURCES   += src/ut_cputility.cpp \
    ../../../src/cpframework/src/cputility.cpp

INCLUDEPATH += . \
	 src/\
	 ../../../src/inc \
	 ../../../controlpanel_plat/inc \
	 ../../../src/cpframework/src 
