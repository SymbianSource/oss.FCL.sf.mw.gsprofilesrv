/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Plug-In that implements C3DRingingToneInterface.
*
*/



#ifndef RINGINGTONE_3D_PLUGIN_DEBUG_H
#define RINGINGTONE_3D_PLUGIN_DEBUG_H


#ifdef _DEBUG

#include <f32file.h>
#include <flogger.h>

_LIT(KLogFilename,"3d.txt");
_LIT(KLogDir,"3d");

// Declare the FPrint function
inline void FPrint(const TRefByValue<const TDesC> aFmt, ...)
    {
    VA_LIST list;
    VA_START(list,aFmt);
    RFileLogger::WriteFormat(KLogDir, KLogFilename, EFileLoggingModeAppend, aFmt, list);
    }


#ifdef __WINS__
#define FLOG(arg...)   { FPrint(arg); }
//#define FLOG(a) { RDebug::Print(a);  }
#else
//#define FLOG(arg...) { RDebug::Print(arg);  }
#define FLOG(arg...)   { FPrint(arg); }
#endif

#else // _DEBUG

#define FLOG(arg...)  //  No loggings

#endif // _DEBUG




#endif  // RINGINGTONE_3D_PLUGIN_DEBUG_H

// End of File
