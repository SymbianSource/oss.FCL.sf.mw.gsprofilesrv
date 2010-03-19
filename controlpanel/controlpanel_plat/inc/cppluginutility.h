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

#ifndef CP_PLUGINUTILITY_H
#define CP_PLUGINUTILITY_H

#include <cpglobal.h>
class HbDataForm;

class CP_EXPORT CpPluginUtility
{
/*!
     This function can add the proto type of setting items (like entry item) 
     which is supplied by control panel into your dataform's instance
 */
public:
    static void addCpItemPrototype(HbDataForm *settingForm);
};

#endif
