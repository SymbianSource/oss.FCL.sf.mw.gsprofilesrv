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

#ifndef CPPLUGINPLATINTERFACE_H
#define CPPLUGINPLATINTERFACE_H

#include <QtPlugin>

/*!
    \class CpPluginPlatInterface
    \brief The class CpPluginPlatInterface is an interface for plugins which can be displayed in control panel application. 
 */

class CpSettingFormItemData;
class CpItemDataHelper;

class CpPluginPlatInterface
{
public:
    /*!
     Destructor of CpPluginPlatInterface.
     */
    virtual ~CpPluginPlatInterface()
    {
    }
    
    /*!
     Create a data form mode item, by which the control panel framework can build a data form model.
     */
    virtual CpSettingFormItemData *createSettingFormItemData(CpItemDataHelper &itemDataHelper) const = 0;
};

Q_DECLARE_INTERFACE(CpPluginPlatInterface, "com.nokia.plugin.controlpanel.platform.interface/1.0");

#endif /* CPPLUGINPLATINTERFACE_H */
