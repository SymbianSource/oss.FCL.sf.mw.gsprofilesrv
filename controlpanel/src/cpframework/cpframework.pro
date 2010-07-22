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
TARGET = cpframework

include ( ../common.pri )
include ( ../logger/logger.pri)
include ( ./cpframework.pri )
include ( ../../controlpanel_plat/inc/controlpanel_plat.pri )

RESOURCES += cpframework.qrc

symbian: { 
    TARGET.UID3 = 0x20025FDA
}

CONFIG += Hb xml
DEFINES += BUILD_CP_FRAMEWORK BUILD_LOGGER_DLL
                    
symbian: {  
    # export headers to mw
    EXPORT_PLATFORM_HEADERS += \
                             ../../controlpanel_plat/inc/cpglobal.h \
                             ../../controlpanel_plat/inc/cpbasepath.h \
                             ../../controlpanel_plat/inc/cpplugininterface.h \
                             ../../controlpanel_plat/inc/cppluginplatinterface.h \
                             ../../controlpanel_plat/inc/cppluginloader.h \
                             ../../controlpanel_plat/inc/cppluginlauncher.h \
                             ../../controlpanel_plat/inc/cpbasesettingview.h \
                             ../../controlpanel_plat/inc/cpitemdatahelper.h \
                             ../../controlpanel_plat/inc/cpsettingformitemdata.h \
                             ../../controlpanel_plat/inc/cpsettingformentryitemdata.h \
                             ../../controlpanel_plat/inc/cpsettingformentryitemdataimpl.h \
                             ../../controlpanel_plat/inc/cppluginutility.h \
                             ../../controlpanel_plat/inc/cplauncherinterface.h
    
    headers.sources = $$EXPORT_PLATFORM_HEADERS
    for(header, headers.sources):BLD_INF_RULES.prj_exports += "./$$header $$MW_LAYER_PLATFORM_EXPORT_PATH($$basename(header))"
}
include (../logger/logger_export.pri)
    
