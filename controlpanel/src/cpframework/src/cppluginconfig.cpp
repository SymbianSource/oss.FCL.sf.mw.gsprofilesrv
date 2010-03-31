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

#include "cppluginconfig.h"
#include "cplogger.h"

CpPluginConfig::CpPluginConfig() : 
    mUid(0),
    mDisplayName(QString()),
    mPluginFile(QString()),
    mDescription(QString())
{

}

void CpPluginConfig::dump()
{
    CpLogger logger;
    
    logger << "id = " << QString("0x%1").arg(mUid,0,16) << "\r\n";
    logger << "diplayname = " << mDisplayName << "\r\n";
    logger << "dll = " << mPluginFile << "\r\n";
    logger << "desc = " << mDescription << "\r\n";
    logger << "\r\n";
}
