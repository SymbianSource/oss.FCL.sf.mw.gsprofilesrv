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
#include <hbapplication.h>
#include <QTest>
#include "ut_cpapi.h"
int main(int argc, char *argv[])
{
    HbApplication app(argc, argv);
    int res = 0;
    //char *pass[3];
    //pass[0] = argv[0];
    //pass[1] = "-o";
    
    // log folder: \epoc32\winscw\c\data
    
    app.setApplicationName("TestCpAPI");
    //pass[2] = "c:\\data\\u.txt";
    TestCpAPI *tcUtilities = new TestCpAPI();
    res |= QTest::qExec(tcUtilities);
    delete tcUtilities;
    tcUtilities = NULL;
    return res;
}
