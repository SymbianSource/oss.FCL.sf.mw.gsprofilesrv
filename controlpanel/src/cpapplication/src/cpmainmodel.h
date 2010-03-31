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
#ifndef CP_MAIN_MODEL_H
#define CP_MAIN_MODEL_H

#include <cpcategorysettingformmodel.h>


class CpItemDataHelper;

class CpMainModel : public CpCategorySettingFormModel
{
	Q_OBJECT
public:
	CpMainModel();
	virtual ~CpMainModel();
	const CpItemDataHelper *itemDataHelper() const;
private:
	CpItemDataHelper *mItemDataHelper;
};

#endif
