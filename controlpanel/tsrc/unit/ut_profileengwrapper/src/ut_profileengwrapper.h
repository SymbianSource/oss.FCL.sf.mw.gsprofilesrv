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
#ifndef	UT_PROFILEENGWRAPPER_H
#define	UT_PROFILEENGWRAPPER_H

#include <QObject>

class CpProfileEngine;

class TestProfileEngWrapper : public QObject
{
	Q_OBJECT
private slots:
    void initTestCase();	// initialize test data
    
    void testProfileList();
    void testModifyProfile();
    void testCreateAndDeleteProfile();
    
    void cleanupTestCase();	//Finalize test data
private:
    CpProfileEngine *mProfileEng;
};


#endif	//UT_PROFILEENGWRAPPER_H
