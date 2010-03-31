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
#include "cpplaceholderentryitemdata.h"
#include <cpbasesettingview.h>
#include <hblabel.h>
#include <cpitemdatahelper.h>
#include <hbdataform.h>

CpPlaceholderEntryItemData::CpPlaceholderEntryItemData(CpItemDataHelper &itemDataHelper)
: CpSettingFormEntryItemData(itemDataHelper)
{
	
}

CpPlaceholderEntryItemData::~CpPlaceholderEntryItemData()
{
}

CpBaseSettingView *CpPlaceholderEntryItemData::createSettingView() const
{
	HbDataForm *dataform = new HbDataForm();
	dataform->setHeading(text());
    CpBaseSettingView *view = new CpBaseSettingView(dataform);
    return view;
}
