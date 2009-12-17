/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Implementation of class CSettingsPluginApiTest
*
*/


#ifndef C_SETTINGSPLUGINAPITEST_H
#define C_SETTINGSPLUGINAPITEST_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <TestclassAssert.h>
#include <aknserverapp.h> 
#include <GSPluginInterface.h>

#include "TestGSPlugin.h"

// MACROS
#define TEST_CLASS_VERSION_MAJOR 0
#define TEST_CLASS_VERSION_MINOR 0
#define TEST_CLASS_VERSION_BUILD 0
// User MACROS
#define KErrGeneral     -2
#define KFirstOption    1
#define KSecondtOption  2

//define client flags
#define KFlagNoObserver     1
#define KFlagReserved1      2
#define KFlagReserved2      4
// Logging path
_LIT( KSettingsPluginApiTestLogPath, "\\logs\\testframework\\SettingsPluginApiTest\\" ); 
// Log file
_LIT( KSettingsPluginApiTestLogFile, "SettingsPluginApiTest.txt" ); 
_LIT( KSettingsPluginApiTestLogFileWithTitle, "SettingsPluginApiTest_[%S].txt" );

//
/**
*  CSettingsPluginApiTest test class for STIF Test Framework TestScripter.
*  @since S60 5.0
*/

NONSHARABLE_CLASS(CSettingsPluginApiTest) : public CScriptBase/*,public MAknServerAppExitObserver*/
    {
public:  // Constructors and destructor

    /**
    * Two-phased constructor.
    */
    static CSettingsPluginApiTest* NewL( CTestModuleIf& aTestModuleIf );

    /**
    * Destructor.
    */
    virtual ~CSettingsPluginApiTest();
    
/*private:
    //function from MAknServerAppExitObserver. is used to handle the exit of the server.
    
    void HandleServerAppExit(TInt aReason);*/

public: // Functions from base classes

    /**
    * From CScriptBase Runs a script line.
    * @since S60 5.0
    * @param aItem Script line containing method name and parameters
    * @return Symbian OS error code
    */
    virtual TInt RunMethodL( CStifItemParser& aItem );
    
private:

    /**
    * C++ default constructor.
    */
    CSettingsPluginApiTest( CTestModuleIf& aTestModuleIf );

    /**
    * By default Symbian 2nd phase constructor is private.
    */
    void ConstructL();

    /**
    * Frees all resources allocated from test methods.
    * @since S60 5.0
    */
    void Delete();

    /**
    * Test method.
    * @since S60 5.0
    * @param aItem Script line containing parameters.
    * @return Symbian OS error code.
    */ 

    TInt SetupCGSParentPluginL();
    
    virtual TInt CallGetValueL( CStifItemParser& aItem );
    
    virtual TInt CallMenuActivationItemL( CStifItemParser& aItem );
    
    virtual TInt CallVisibleL( CStifItemParser& aItem );
    
    virtual TInt CallResetSelectedItemIndexL( CStifItemParser& aItem );
    
    /**
     * Method used to log version of test class
     */
    void SendTestClassVersion();
    
    /**
     * Turn off ScreenSaver
     * @since S60 5.0
     * @return Symbian OS error code.
     */
    void TurnOffScreenSaver();

    /**
     * Restore ScreenSaver
     * @since S60 5.0
     * @return Symbian OS error code.
     */
    void RestoreScreenSaver();

private:    // Data

    /**
     * ScreenSaver Property
     */
    TInt iOldScreenSaverProperty;
    
    CTestGSPlugin* iTestGsPlugin;
    
    /**
     * description_of_pointer_member
     * Not own.
     */
    CGSPluginInterface* iGSPluginInterface;
    
    };

#endif      // C_SETTINGSPLUGINAPITEST_H

// End of File
