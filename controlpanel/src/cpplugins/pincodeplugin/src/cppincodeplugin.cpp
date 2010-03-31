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
#include "cppincodeplugin.h"
#include "cppincodepluginview.h"
#include <cpsettingformentryitemdataimpl.h>

CpPinCodePlugin::CpPinCodePlugin()
{
}

CpPinCodePlugin::~CpPinCodePlugin()
{
}

CpSettingFormItemData *CpPinCodePlugin::createSettingFormItemData(CpItemDataHelper &itemDataHelper) const
{
    return new CpSettingFormEntryItemDataImpl<CpPinCodePluginView>(
			itemDataHelper,tr("PIN code"),QString());
}

Q_EXPORT_PLUGIN2(cppincodeplugin, CpPinCodePlugin);
