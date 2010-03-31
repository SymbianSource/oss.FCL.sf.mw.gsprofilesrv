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
#include "cpaccountsgroup.h"
#include <cpsettingformitemdata.h>
#include <cpitemdatahelper.h>

CpAccountsGroup::CpAccountsGroup(CpItemDataHelper &itemDataHelper) :
    CpSettingFormItemData(HbDataFormModelItem::GroupItem,QString("Accounts"))
{
	Q_UNUSED(itemDataHelper);
	// Initializing the data form item of accounts group here    
	// You can use CpItemDataHelper for adding connection or adding new proto type
	// For more detail information, please refer to controlpanel's wiki
}
CpAccountsGroup::~CpAccountsGroup()
{
    // release resource when necessary
}
