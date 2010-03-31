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

#ifndef CPPRIVACYPLUGIN_H
#define CPPRIVACYPLUGIN_H

#include <cppluginplatinterface.h>
#include <qobject.h>

class CpPrivacyPlugin: public QObject,public CpPluginPlatInterface
{
    Q_OBJECT
    Q_INTERFACES(CpPluginPlatInterface)
public:
    CpPrivacyPlugin();
    virtual ~CpPrivacyPlugin();
    virtual CpSettingFormItemData *createSettingFormItemData(CpItemDataHelper &itemDataHelper) const;
};

#endif /* CPPRIVACYPLUGIN_H */
