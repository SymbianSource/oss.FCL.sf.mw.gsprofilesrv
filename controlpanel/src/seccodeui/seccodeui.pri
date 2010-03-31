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
# Description: seccodeui source files
#

# Input
HEADERS += src/seccodesettings.h \
           src/seccodeuiutility.h \
           src/seccodeedit.h \
           src/seccodeeditdataformviewitem.h \
           src/seccodemodel.h \
           src/seccodemodel_p.h


SOURCES += src/seccodesettings.cpp \
           src/seccodeuiutility.cpp \
           src/seccodeedit.cpp \
           src/seccodeeditdataformviewitem.cpp \
           src/seccodemodel.cpp 
           
symbian : {
    SOURCES += src/seccodemodel_symbian.cpp
}

win32 : {
    SOURCES += src/seccodemodel_win.cpp
}