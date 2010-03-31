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
#ifndef	CPKEYTOUCHFDBKPLUGIN_H
#define	CPKEYTOUCHFDBKPLUGIN_H

#include <qobject.h>
#include <cppluginplatinterface.h>

class CpKeytouchfdbkPlugin : public QObject, public CpPluginPlatInterface
{
Q_OBJECT
    Q_INTERFACES(CpPluginPlatInterface)
public:
    CpKeytouchfdbkPlugin();
    ~CpKeytouchfdbkPlugin();
    virtual CpSettingFormItemData *createSettingFormItemData(CpItemDataHelper &itemDataHelper) const;
};

#endif	//CPKEYTOUCHFDBKPLUGIN
