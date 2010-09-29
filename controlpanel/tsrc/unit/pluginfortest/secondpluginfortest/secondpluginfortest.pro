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
TARGET = secondpluginfortest

MOC_DIR = moc
OBJECT_DIR = obj
RCC_DIR = rcc

symbian {
    INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE
    INCLUDEPATH += $$MOC_DIR
    TARGET.CAPABILITY = ALL -TCB
    TARGET.EPOCALLOWDLLDATA = 1
}

CONFIG += hb plugin
LIBS += -lcpframework
CONFIG += symbian_test

symbian: plugin { # copy qtstub and manifest
	
	PLUGIN_STUB_PATH = /resource/qt/plugins/controlpanel
	
	deploy.path = C:
	pluginstub.sources = $${TARGET}.dll
	pluginstub.path = $$PLUGIN_STUB_PATH
	DEPLOYMENT += pluginstub

    qtplugins.path = $$PLUGIN_STUB_PATH
    qtplugins.sources += qmakepluginstubs/$${TARGET}.qtplugin
     
    for(qtplugin, qtplugins.sources):BLD_INF_RULES.prj_exports += "./$$qtplugin  $$deploy.path$$qtplugins.path/$$basename(qtplugin)" 
}

DEPENDPATH += .
INCLUDEPATH += .
			 
# Input
HEADERS += src/secondpluginfortest.h \
           src/secondpluginviewfortest.h
    
SOURCES += src/secondpluginfortest.cpp \
           src/secondpluginviewfortest.cpp 


symbian:MMP_RULES += SMPSAFE
