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
#ifndef	CPKTFDBKVIEW_H
#define	CPKTFDBKVIEW_H

#include <cpbasesettingview.h>

class HbDataFormModelItem;
class CpSettingFormItemData;
class CpProfileModel;
class CpKtfdbkView : public CpBaseSettingView
{
    Q_OBJECT
public:
    explicit CpKtfdbkView(QGraphicsItem *parent = 0);
    virtual ~CpKtfdbkView();
private slots:
    //need handling your member's value change
    void KTsliderValueChanged(int value);
    void STsliderValueChanged(int value);
    void SVsliderValueChanged(int value);
private:
	CpProfileModel *mModel;
    CpSettingFormItemData *mKTSliderItem;
    CpSettingFormItemData *mSTSliderItem;
	CpSettingFormItemData *mSVSliderItem;
};
#endif//	CPKTFDBKVIEW_H
