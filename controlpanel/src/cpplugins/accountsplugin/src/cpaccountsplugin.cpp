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
#include "cpaccountsplugin.h"
#include "cpaccountsgroup.h"

CpAccountsPlugin::CpAccountsPlugin()
{
}

CpAccountsPlugin::~CpAccountsPlugin()
{
}

CpSettingFormItemData *CpAccountsPlugin::createSettingFormItemData(CpItemDataHelper &itemDataHelper) const
{
    return new CpAccountsGroup(itemDataHelper);
}

Q_EXPORT_PLUGIN2(cpaccountsplugin, CpAccountsPlugin);
