/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0""
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  
*
*/

#ifndef CPBASEPATH_H
#define CPBASEPATH_H

#ifdef Q_OS_SYMBIAN
        #define CP_PLUGIN_PATH QString("/resource/qt/plugins/controlpanel")
        #define CP_PLUGIN_CONFIG_PATH QString("/resource/qt/plugins/controlpanel/config")
#else 
	#ifdef _DEBUG
		#define CP_PLUGIN_PATH QString("/ControlPanel/debug/bin")
		#define CP_PLUGIN_CONFIG_PATH QString("/ControlPanel/debug/bin/config")
	#else
		#define CP_PLUGIN_PATH QString("/ControlPanel/release/bin")
		#define CP_PLUGIN_CONFIG_PATH QString("/ControlPanel/release/bin/config")
	#endif
#endif 

#endif /* CPBASEPATH_H */
