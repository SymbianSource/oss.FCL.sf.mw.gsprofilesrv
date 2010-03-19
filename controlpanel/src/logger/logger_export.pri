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
# Description: controlpanel project - common qmake settings
#

symbian: {  
    # export headers to mw
    EXPORT_PLATFORM_CPLOGGER_HEADERS += \
                             ../../controlpanel_plat/inc/logger.h \
                             ../../controlpanel_plat/inc/logoutput.h \
                             ../../controlpanel_plat/inc/loggerglobal.h \
                             ../../controlpanel_plat/inc/logoutputfactory.h \
                             ../../controlpanel_plat/inc/cplogger.h
    
    loggers.sources = $$EXPORT_PLATFORM_CPLOGGER_HEADERS
    for(logger, loggers.sources):BLD_INF_RULES.prj_exports += "./$$logger $$MW_LAYER_PLATFORM_EXPORT_PATH(cplogger/$$basename(logger))"
}