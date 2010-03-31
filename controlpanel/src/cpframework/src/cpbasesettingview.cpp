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

#include "cpbasesettingview.h"
#include "cpbasesettingview_p.h"
#include <hbdataform.h>
#include "cpitemdatahelper.h"


/*!
    \class CpBaseSettingView
    \brief The CpBaseSettingView is base class for all setting views in control panel application.
    This class is responsible for processing some common properties, such as setting view's title, content widget and back key action.
 */

/*!
    Constructor of CpBaseSettingView.
 */
CpBaseSettingView::CpBaseSettingView(QGraphicsWidget *widget /*= 0*/,QGraphicsItem *parent /*= 0*/) :
    HbView(parent),d_ptr(new CpBaseSettingViewPrivate())
{
    d_ptr->init(widget,this);
}

/*!
    Destructor of CpBaseSettingView.
 */
CpBaseSettingView::~CpBaseSettingView()
{
    delete d_ptr;
}

/*!
	\deprecated  void CpBaseSettingView::setSettingForm(HbDataForm *settingForm) will be deprecated, please
	use setWidget(QGraphicsWidget* widget) to instead.
    Set the data form for the setting view.
 */
void CpBaseSettingView::setSettingForm(HbDataForm *settingForm)
{
    d_ptr->setSettingForm(settingForm);
}

/*!
	\deprecated  HbDataForm *CpBaseSettingView::settingForm() const will be deprecated, please
	use widget() const to instead.
    Get the data form for the setting view.
 */
HbDataForm *CpBaseSettingView::settingForm() const
{
    return qobject_cast<HbDataForm*>(widget());
}

//
