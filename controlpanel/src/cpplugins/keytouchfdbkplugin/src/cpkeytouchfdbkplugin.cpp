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
#include "cpkeytouchfdbkplugin.h"
#include "cpktfdbkview.h"
#include <cpsettingformentryitemdataimpl.h>

CpKeytouchfdbkPlugin::CpKeytouchfdbkPlugin()
{
}

CpKeytouchfdbkPlugin::~CpKeytouchfdbkPlugin()
{
}
CpSettingFormItemData *CpKeytouchfdbkPlugin::createSettingFormItemData(CpItemDataHelper &itemDataHelper) const
{
    return new CpSettingFormEntryItemDataImpl<CpKtfdbkView>(
			itemDataHelper,
            tr("Key&touch screen feedback "), 
			"");
}

Q_EXPORT_PLUGIN2(cpkeytouchfdbkplugin, CpKeytouchfdbkPlugin);
