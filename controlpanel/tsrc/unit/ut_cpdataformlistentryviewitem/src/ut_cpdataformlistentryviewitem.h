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
*       test application for qt control panel public apis.
*/

#ifndef TEST_CP_DATAFORM_LISTENTRY_VIEWITEM_H
#define TEST_CP_DATAFORM_LISTENTRY_VIEWITEM_H

#include <QObject>

class HbMainWindow;

class TestCpDataFormListEntryViewItem : public QObject
{
    Q_OBJECT
    
private slots:
    void initTestCase();
    void cleanupTestCase();
    
//public slots:
    void testCpDataFormListEntryViewItemWithDataForm();
    void testCpDataFormListEntryViewItemWithNullPointer();
    void testCreateItemNoDataForm();
    void testCreateItemWithDataForm();
    void testCanSetModelIndexWithDataForm();
    void testCreateCustomedWidgetNoParam();
    
private:
    HbMainWindow* mainWindow;
};

#endif // TEST_CP_DATAFORM_LISTENTRY_VIEWITEM_H
