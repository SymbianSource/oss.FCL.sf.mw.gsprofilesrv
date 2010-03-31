
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
# Description: cpframework source files
#

HEADERS += src/cpbasepath.h \
           src/cputility.h \
           src/cpviewlauncher.h \
           src/cppluginloader.h \
           src/cppluginconfig.h \
           src/cppluginconfigreader.h \
           src/cpbasesettingview_p.h \
           src/cpwatchdog.h \
           src/cpdataformviewitem.h
           #src/cpitemdatahelper.h

SOURCES += src/cputility.cpp \
           src/cppluginlauncher.cpp \
		       src/cpviewlauncher.cpp \
		       src/cppluginloader.cpp \
		       src/cppluginconfig.cpp \
		       src/cppluginconfigreader.cpp \
		       src/cpsettingformentryitemdata.cpp \
		       src/cpsettingformitemdata.cpp \
		       src/cpcategorysettingformmodel.cpp \
		       src/cpcategorysettingformitemdata.cpp \
		       src/cpbasesettingview.cpp \
		       src/cpbasesettingview_p.cpp \
		       src/cpwatchdog.cpp \
		       src/cpdataformviewitem.cpp \
		       src/cpitemdatahelper.cpp
