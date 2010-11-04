/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  This cpp file has the functions to test SettingsPluginApi.
*
*/


// [INCLUDE FILES]
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include "SettingsPluginApiTest.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSettingsPluginApiTest::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CSettingsPluginApiTest::Delete() 
    { 
    if ( iGSPluginInterface )
    	{
    	delete iGSPluginInterface;
    	iGSPluginInterface = NULL;
    	}

    }

// -----------------------------------------------------------------------------
// CSettingsPluginApiTest::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CSettingsPluginApiTest::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "CallGetValueL", CSettingsPluginApiTest::CallGetValueL ),
        ENTRY( "CallMenuActivationItemL", CSettingsPluginApiTest::CallMenuActivationItemL ),
        ENTRY( "CallVisibleL", CSettingsPluginApiTest::CallVisibleL ),
        ENTRY( "CallResetSelectedItemIndexL", CSettingsPluginApiTest::CallResetSelectedItemIndexL ),
        // [test cases entries]
        };

    const TInt count = sizeof( KFunctions ) / sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// CSettingsPluginApiTest::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CSettingsPluginApiTest::SetupCGSParentPluginL()
	{  
	TRAPD( error, iTestGsPlugin = CTestGSPlugin::NewL() );
	
	if ( KErrNone == error )
		{
        iGSPluginInterface = static_cast<CGSPluginInterface*> ( iTestGsPlugin );
		}
    
    return error;
	}

// -----------------------------------------------------------------------------
// CServicesDbApiTest::CallGetValueL
// Is used to test CGSPluginInterface::GetValue
// -----------------------------------------------------------------------------
TInt CSettingsPluginApiTest::CallGetValueL( CStifItemParser& /*aItem*/ )
    {
    //
    TInt result = 0;
    TRAPD( error, result = SetupCGSParentPluginL() );
    if( KErrNone != error )
        {
        iLog->Log( _L( "Initial GSPluginInterface failed,the failed code is %d" ), error );
        return error;
        }
    else
        {
        if( KErrNone != result )
            {            
            return result;
            }
        };
    TBuf<256> value;
    TRAPD( errorCall, iGSPluginInterface->GetValue( EGSPluginKeySettingsItemValueString, value ) );
    if( errorCall != KErrNone )
        {
        iLog->Log( _L( "Function GetValue is called failed. The failed code is %d" ), errorCall );
        return errorCall;
        }
    else
        {
        iLog->Log( _L( "Function GetValue called successfully." ) );
        }
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CServicesDbApiTest::CallMenuActivationItemL
// Is used to test CGSPluginInterface::MenuActivationItem
// -----------------------------------------------------------------------------
TInt CSettingsPluginApiTest::CallMenuActivationItemL( CStifItemParser& /*aItem*/ )
    {
    //
    TInt result = 0;
    TRAPD( error, result = SetupCGSParentPluginL() );
    if(KErrNone != error)
        {
        iLog->Log( _L( "Initial GSPluginInterface failed,the failed code is %d" ), error );
        return error;
        }
    else
        {
        if( KErrNone != result )
            {            
            return result;
            }
        };
    TGSMenuActivationItems GsMenuActivationItems;
    TRAPD( errorCall, GsMenuActivationItems = iGSPluginInterface->MenuActivationItem() );
    if( errorCall != KErrNone )
        {
        iLog->Log( _L( "Function MenuActivationItem is called failed. The failed code is %d" ), errorCall );
        return errorCall;
        }
    else
        {
        iLog->Log( _L( "Function MenuActivationItem is called successfully." ) );
        }
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CServicesDbApiTest::CallVisibleL
// Is used to test CGSPluginInterface::Visible
// -----------------------------------------------------------------------------
TInt CSettingsPluginApiTest::CallVisibleL( CStifItemParser& /*aItem*/ )
    {
    //
    TInt result = 0;
    TRAPD( error, result = SetupCGSParentPluginL() );
    if(KErrNone != error)
        {
        iLog->Log( _L( "Initial GSPluginInterface failed,the failed code is %d" ), error );
        return error;
        }
    else
        {
        if( KErrNone != result )
            {            
            return result;
            }
        };
    TBool IsVisible;
    TRAPD( errorCall, IsVisible = iGSPluginInterface->Visible() );
    if( errorCall != KErrNone )
        {
        iLog->Log( _L( "Function Visible is called failed. The failed code is %d" ), errorCall );
        return errorCall;
        }
    else
        {
        iLog->Log( _L( "Function Visible is called successfully." ) );
        }
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CServicesDbApiTest::CallResetSelectedItemIndexL
// Is used to test CGSPluginInterface::ResetSelectedItemIndex
// -----------------------------------------------------------------------------
TInt CSettingsPluginApiTest::CallResetSelectedItemIndexL( CStifItemParser& /*aItem*/ )
    {
    //
    TInt result = 0 ;
    TRAPD( error, result = SetupCGSParentPluginL() );
    if(KErrNone != error)
        {
        iLog->Log( _L( "Initial GSPluginInterface failed,the failed code is %d" ), error );
        return error;
        }
    else
        {
        if( KErrNone != result )
            {            
            return result;
            }
        };
    TRAPD( errorCall, iGSPluginInterface->ResetSelectedItemIndex() );
    if( errorCall != KErrNone )
        {
        iLog->Log( _L( "Function ResetSelectedItemIndex is called failed. The failed code is %d" ), errorCall );
        return errorCall;
        }
    else
        {
        iLog->Log( _L( "Function ResetSelectedItemIndex is called successfully." ) );
        }
    
    return KErrNone;
    }

//
//  [End of File]
