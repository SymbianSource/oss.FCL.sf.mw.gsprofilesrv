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


#ifndef CPLAUNCHERINTERFACE_H
#define CPLAUNCHERINTERFACE_H

#include <QtPlugin>
#include <QList>

/*!
    \class CpLauncherInterface
    \brief The class CpLauncherInterface defines an interface for plugins which can be launched a specific view from outside controlpanel application.
 */

class QVariant;
class CpBaseSettingView;

class CpLauncherInterface
{
public:
    /*!
     Destructor of CpLauncherInterface.
     */
    virtual ~CpLauncherInterface()
    {
    }
    
    /*!
     Create a CpBaseSettingView by the parameter hint.
     */
    virtual CpBaseSettingView *createSettingView(const QVariant &hint) const = 0;
};

Q_DECLARE_INTERFACE(CpLauncherInterface, "com.nokia.controlpanel.launcher.interface/1.0");

#endif /* CPLAUNCHERINTERFACE_H */
