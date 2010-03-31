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
TARGET = cpprivacyplugin

CONFIG += hb plugin

LIBS += -lcpframework

include ( ../cpplugincommon.pri )
include ( privacyplugin.pri )

symbian: { 
    TARGET.UID3 = 0x20025FE1
}

symbian {
    deploy.path = C:
    headers.sources = data/cpprivacyplugin.cpcfg
    headers.path = /resource/qt/plugins/controlpanel/config
    DEPLOYMENT += exportheaders

    # This is for new exporting system coming in garden
    for(header, headers.sources):BLD_INF_RULES.prj_exports += "./$$header $$deploy.path$$headers.path/$$basename(header)"
}