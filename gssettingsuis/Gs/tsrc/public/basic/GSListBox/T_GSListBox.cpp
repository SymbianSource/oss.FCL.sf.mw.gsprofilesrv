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
* Description:
*
*/


//  CLASS HEADER
#include "T_GSListBox.h"

//  EXTERNAL INCLUDES
#include <EUnitMacros.h>
#include <EUnitDecorators.h>
#include <e32base.h>
#include <barsread.h> //TResourceReader
#include <coemain.h>
#include <AknRadioButtonSettingPage.h>
#include <gsnetworkpluginrsc.rsg>           //for resource IDs


//  INTERNAL INCLUDES
//#include "ListBox.cpp"

// CONSTANTS
_LIT( KGSTestCaption, "Test caption" );
const TInt KGSTestLocalFeatureId = 0;

// CONSTRUCTION
T_GSListBox* T_GSListBox::NewL()
    {
    T_GSListBox* self = T_GSListBox::NewLC();
    CleanupStack::Pop();

    return self;
    }

T_GSListBox* T_GSListBox::NewLC()
    {
    T_GSListBox* self = new( ELeave ) T_GSListBox();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
T_GSListBox::~T_GSListBox()
    {
    }

// Default constructor
T_GSListBox::T_GSListBox()
    {
    }

// Second phase construct
void T_GSListBox::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void T_GSListBox::SetupL(  )
    {
    
    }

void T_GSListBox::SetupResourceL()
    {
	iOffset = CCoeEnv::Static()->AddResourceFileL( _L("z:\\resource\\gsnetworkpluginrsc.rsc") ); 
    }


void T_GSListBox::Teardown(  )
    {
    if( iListboxEntry )
        {
        delete iListboxEntry;
        iListboxEntry = NULL;
        }
    if( iItemTextArray )
        {
        delete iItemTextArray;
        iItemTextArray = NULL;
        }
    if( iRBSPArray )
        {
        delete iRBSPArray;
        iRBSPArray = NULL;
        }
        
    if ( iLBITextArray )
    	{
    	delete iLBITextArray;
    	iLBITextArray = NULL;
    	}
        
	if ( iOffset )
		{
		CCoeEnv::Static()->DeleteResourceFile( iOffset );
		iOffset = 0;		
		}
    }

// CGSListboxEntry test cases

void T_GSListBox::SetupListBoxEntryL()
    {
    SetupResourceL();

    TResourceReader resReader;
    CCoeEnv::Static()->CreateResourceReaderLC( resReader, R_NETSL_NETSELECTMODE_LBX );
    // Just read, not actually needed anywhere
    resReader.ReadInt16();
    
    iListboxEntry = CGSListboxEntry::NewLC( resReader );
    EUNIT_ASSERT( iListboxEntry != NULL );
    CleanupStack::Pop( iListboxEntry );
    
    CleanupStack::PopAndDestroy();
    }

void T_GSListBox::T_Global_GSListBox_NewLC()
    {
    TResourceReader resReader;
    CCoeEnv::Static()->CreateResourceReaderLC( resReader, R_NETSL_NETSELECTMODE_LBX );
    // Just read, not actually needed anywhere
    resReader.ReadInt16();
    
    iListboxEntry = CGSListboxEntry::NewLC( resReader );
    EUNIT_ASSERT( iListboxEntry != NULL );
    CleanupStack::Pop( iListboxEntry );
    
    CleanupStack::PopAndDestroy();
    }

void T_GSListBox::T_Global_GSListBox_New2LC()
    {
    TInt localFeatureId = 0;
    iListboxEntry = CGSListboxEntry::NewLC( localFeatureId );
    EUNIT_ASSERT( iListboxEntry != NULL );
    CleanupStack::Pop();
    }

void T_GSListBox::T_Global_GSListBox_New3LC()
    {
    TInt localFeatureId = 0;
    TUint32 resourceId = R_NETSL_NETSELECTMODE_LBX;
    iListboxEntry = CGSListboxEntry::NewLC( resourceId, *CCoeEnv::Static(), localFeatureId );
    EUNIT_ASSERT( iListboxEntry != NULL );
    CleanupStack::Pop();
    }

void T_GSListBox::T_Global_GSListBox_FeatureIdL()
    {
    TInt localFeatureId;
    EUNIT_ASSERT_NO_LEAVE( localFeatureId = iListboxEntry->FeatureId() );
    }

void T_GSListBox::T_Global_GSListBox_TypeL()
    {
    TInt type;
    EUNIT_ASSERT_NO_LEAVE( type = iListboxEntry->Type() );
    }

void T_GSListBox::T_Global_GSListBox_CaptionL()
    {
    //Here we can add comparision of the returned string to the
    //previously saved one
    EUNIT_ASSERT_NO_LEAVE( iListboxEntry->Caption() );
    }

void T_GSListBox::T_Global_GSListBox_SetDynamicTextL()
    {
    EUNIT_ASSERT_NO_LEAVE( iListboxEntry->SetDynamicTextL( KGSTestCaption ) );
    }

void T_GSListBox::T_Global_GSListBox_SetDynamicTextDoublePaneL()
    {
    EUNIT_ASSERT_NO_LEAVE( iListboxEntry->SetDynamicTextDoublePaneL( KGSTestCaption ) );
    }


// CGSItemTextArray test cases

void T_GSListBox::SetupGSItemTextArrayL()
    {
    SetupResourceL();
    iItemTextArray = 
            CGSItemTextArray::NewL( R_NETSL_NETSELECTMODE_LBX, *CCoeEnv::Static() );
    }

void T_GSListBox::T_Global_GSItemTextArray_SetupL()
    {
    EUNIT_ASSERT_NO_LEAVE( iItemTextArray = 
        CGSItemTextArray::NewL( R_NETSL_NETSELECTMODE_LBX, *CCoeEnv::Static() ) ); 
    }

void T_GSListBox::T_Global_GSItemTextArray_MdcaCountL()
    {
    TInt itemCount = 0;
    EUNIT_ASSERT_NO_LEAVE( itemCount = iItemTextArray->MdcaCount() ); 
    }

void T_GSListBox::T_Global_GSItemTextArray_MdcaPointL()
    {
    TInt itemIndex = 0;
    EUNIT_ASSERT_NO_LEAVE( iItemTextArray->MdcaPoint( itemIndex ) ); 
    }

void T_GSListBox::T_Global_GSItemTextArray_SetDynamicTextL()
    {
    TInt featureId = 1;
    EUNIT_ASSERT_NO_LEAVE( iItemTextArray->SetDynamicTextL( 
            featureId, KGSTestCaption ) ); 
    }

void T_GSListBox::T_Global_GSItemTextArray_SetDynamicTextDoublePaneL()
    {
    TInt featureId = 1;
    EUNIT_ASSERT_NO_LEAVE( iItemTextArray->SetDynamicTextDoublePaneL( 
            featureId, KGSTestCaption ) ); 
    }

void T_GSListBox::T_Global_GSItemTextArray_SetItemVisibilityL()
    {
    TInt featureId = 1;
    CGSItemTextArray::TVisibility itemVisibility = CGSItemTextArray::EVisible;
    EUNIT_ASSERT_NO_LEAVE( iItemTextArray->SetItemVisibilityL( 
            featureId, itemVisibility ) ); 
    }
    
void T_GSListBox::T_Global_GSItemTextArray_CurrentFeatureL()
    {
    EUNIT_ASSERT_NO_LEAVE( iItemTextArray->CurrentFeature() ); 
    }

void T_GSListBox::T_Global_GSItemTextArray_IndexForFeatureIdL()
    {
    TInt featureId = 4;
    EUNIT_ASSERT_NO_LEAVE( iItemTextArray->IndexForFeatureIdL( 
            featureId ) ); 
    }

void T_GSListBox::T_Global_GSItemTextArray_FeatureIdByIndexL()
    {
    TInt itemIndex = 5;
    EUNIT_ASSERT_NO_LEAVE( iItemTextArray->FeatureIdByIndex( 
            itemIndex ) ); 
    }

void T_GSListBox::T_Global_GSItemTextArray_FeatureByIdL()
    {
    TInt featureId = 6;
    EUNIT_ASSERT_NO_LEAVE( iItemTextArray->FeatureByIdL( 
            featureId ) ); 
    }

void T_GSListBox::T_Global_GSItemTextArray_GetItemCaptionFromFeatureIdLC()
    {
    TInt featureId = 6;
    HBufC* itemCaption;
    itemCaption = CGSItemTextArray::GetItemCaptionFromFeatureIdLC( 
            R_NETSL_NETSELECTMODE_LBX, featureId, *CCoeEnv::Static() );
    CleanupStack::PopAndDestroy();
    }


// CGSRadioButtonSettingPageItemTextArray test cases

void T_GSListBox::T_Global_GSRBSPItemTextArray_NewL()
    {
    CGSRadioButtonSettingPageItemTextArray* itemArray = 
        CGSRadioButtonSettingPageItemTextArray::NewL( R_NETSL_NETSELECTMODE_LBX, *CCoeEnv::Static(), NULL );
    CleanupStack::PushL( itemArray );
    EUNIT_ASSERT( itemArray );
    CleanupStack::PopAndDestroy();
    }

void T_GSListBox::Setup_GSRBSPItemTextArrayL()
    {
    SetupResourceL();
    
    iRBSPArray = CGSRadioButtonSettingPageItemTextArray::NewL( R_NETSL_NETSELECTMODE_LBX, 
                *CCoeEnv::Static(), NULL );
    }

void T_GSListBox::T_Global_GSRBSPItemTextArray_CurrentFeatureL()
    {
    EUNIT_ASSERT_PANIC( iRBSPArray->CurrentFeature(), "GSEngItemArray", 0 );
    }

void T_GSListBox::T_Global_GSRBSPItemTextArray_SetRadioButtonSettingPageL()
    {
    TUint32 resId = 1;
    TInt currentItem = 0;
    CAknRadioButtonSettingPage* dlg = new (ELeave ) CAknRadioButtonSettingPage(
                                          resId, 
                                          currentItem, iItemTextArray );
    CleanupStack::PushL( dlg );
    EUNIT_ASSERT_NO_LEAVE( iRBSPArray->SetRadioButtonSettingPage( *dlg ) );
    CleanupStack::PopAndDestroy();
    }


// CGSListBoxItemTextArray test cases

void T_GSListBox::Setup_CGSLBITextArrayL()
    {
    SetupResourceL();
    TUint32 resId = R_NETSL_NETSELECTMODE_LBX;
    iLBITextArray = CGSListBoxItemTextArray::NewL( 
            resId, *iListBox, *CCoeEnv::Static() );
    }

void T_GSListBox::T_Global_CGSLBITextArray_NewL()
    {
    TUint32 resId = R_NETSL_NETSELECTMODE_LBX;
    EUNIT_ASSERT_NO_LEAVE( iLBITextArray = CGSListBoxItemTextArray::NewL( 
                resId, *iListBox, *CCoeEnv::Static() ) );
    }

void T_GSListBox::T_Global_CGSLBITextArray_New2L()
    {
    TUint32 resId = R_NETSL_NETSELECTMODE_LBX;
    TInt itemCount = 10;
    EUNIT_ASSERT_NO_LEAVE( iLBITextArray = CGSListBoxItemTextArray::NewL( 
                resId, *iListBox, *CCoeEnv::Static(), itemCount ) );
    }

void T_GSListBox::T_Global_CGSLBITextArray_CurrentFeatureL()
    {
    TInt currentFeature = 0;
    EUNIT_ASSERT_NO_LEAVE( currentFeature = iLBITextArray->CurrentFeature() );
    }


//  TEST TABLE
EUNIT_BEGIN_TEST_TABLE(
    T_GSListBox,
    "Testing GSListBox.dll functionality",
    "UNIT" )


// CGSListboxEntry test cases
EUNIT_TEST(
    "ListBox - NewL construction", //case description
    "CGSListboxEntry",                   //class being tested
    "NewLC",                                 //method being tested
    "FUNCTIONALITY",                        //FUNCTIONALITY, BOUNDARY, ERRORHANDLING
    SetupResourceL, T_Global_GSListBox_NewLC, Teardown )

EUNIT_TEST(
    "ListBox - NewL overloaded construction", //case description
    "CGSListboxEntry",                   //class being tested
    "NewLC",                                 //method being tested
    "FUNCTIONALITY",                        //FUNCTIONALITY, BOUNDARY, ERRORHANDLING
    SetupL, T_Global_GSListBox_New2LC, Teardown )

EUNIT_TEST(
    "ListBox - NewL another overloaded construction", //case description
    "CGSListboxEntry",                   //class being tested
    "NewLC",                                 //method being tested
    "FUNCTIONALITY",                        //FUNCTIONALITY, BOUNDARY, ERRORHANDLING
    SetupResourceL, T_Global_GSListBox_New3LC, Teardown )
    
EUNIT_TEST(
    "ListBox - FeatureId()", //case description
    "CGSListboxEntry",                   //class being tested
    "FeatureId",                                 //method being tested
    "FUNCTIONALITY",                        //FUNCTIONALITY, BOUNDARY, ERRORHANDLING
    SetupListBoxEntryL, T_Global_GSListBox_FeatureIdL, Teardown )

EUNIT_TEST(
    "ListBox - Caption()", //case description
    "CGSListboxEntry",                   //class being tested
    "Caption",                                 //method being tested
    "FUNCTIONALITY",                        //FUNCTIONALITY, BOUNDARY, ERRORHANDLING
    SetupListBoxEntryL, T_Global_GSListBox_CaptionL, Teardown ) 
    
EUNIT_TEST(
    "ListBox - Type()", //case description
    "CGSListboxEntry",                   //class being tested
    "Type",                                 //method being tested
    "FUNCTIONALITY",                        //FUNCTIONALITY, BOUNDARY, ERRORHANDLING
    SetupListBoxEntryL, T_Global_GSListBox_TypeL, Teardown )    

EUNIT_TEST(
    "ListBox - SetDynamicTextL()", //case description
    "CGSListboxEntry",                   //class being tested
    "SetDynamicTextL",                                 //method being tested
    "FUNCTIONALITY",                        //FUNCTIONALITY, BOUNDARY, ERRORHANDLING
    SetupListBoxEntryL, T_Global_GSListBox_SetDynamicTextL, Teardown )

EUNIT_TEST(
    "ListBox - SetDynamicTextDoublePaneL()", //case description
    "CGSListboxEntry",                   //class being tested
    "SetDynamicTextDoublePaneL",            //method being tested
    "FUNCTIONALITY",                        //FUNCTIONALITY, BOUNDARY, ERRORHANDLING
    SetupListBoxEntryL, T_Global_GSListBox_SetDynamicTextDoublePaneL, Teardown )

    
// CGSItemTextArray test cases    

EUNIT_TEST(
    "CGSItemTextArray - construction()", //case description
    "CGSItemTextArray",                   //class being tested
    "NewL",            //method being tested
    "FUNCTIONALITY",                        //FUNCTIONALITY, BOUNDARY, ERRORHANDLING
    SetupResourceL, T_Global_GSItemTextArray_SetupL, Teardown )
    
EUNIT_TEST(
    "CGSItemTextArray - MdcaCount()", //case description
    "CGSItemTextArray",                   //class being tested
    "MdcaCount",            //method being tested
    "FUNCTIONALITY",                        //FUNCTIONALITY, BOUNDARY, ERRORHANDLING
    SetupGSItemTextArrayL, T_Global_GSItemTextArray_MdcaCountL, Teardown )

EUNIT_TEST(
    "CGSItemTextArray - MdcaPoint()", //case description
    "CGSItemTextArray",                   //class being tested
    "MdcaPoint",            //method being tested
    "FUNCTIONALITY",                        //FUNCTIONALITY, BOUNDARY, ERRORHANDLING
    SetupGSItemTextArrayL, T_Global_GSItemTextArray_MdcaPointL, Teardown )

EUNIT_TEST(
    "CGSItemTextArray - SetDynamicTextL()", //case description
    "CGSItemTextArray",                   //class being tested
    "SetDynamicTextL",            //method being tested
    "FUNCTIONALITY",                        //FUNCTIONALITY, BOUNDARY, ERRORHANDLING
    SetupGSItemTextArrayL, T_Global_GSItemTextArray_SetDynamicTextL, Teardown )
    
EUNIT_TEST(
    "CGSItemTextArray - SetDynamicTextDoublePaneL()", //case description
    "CGSItemTextArray",                   //class being tested
    "SetDynamicTextDoublePaneL",            //method being tested
    "FUNCTIONALITY",                        //FUNCTIONALITY, BOUNDARY, ERRORHANDLING
    SetupGSItemTextArrayL, T_Global_GSItemTextArray_SetDynamicTextDoublePaneL, Teardown )

EUNIT_TEST(
    "CGSItemTextArray - SetItemVisibilityL()", //case description
    "CGSItemTextArray",                   //class being tested
    "SetItemVisibilityL",            //method being tested
    "FUNCTIONALITY",                        //FUNCTIONALITY, BOUNDARY, ERRORHANDLING
    SetupGSItemTextArrayL, T_Global_GSItemTextArray_SetItemVisibilityL, Teardown )
    
EUNIT_TEST(
    "CGSItemTextArray - CurrentFeature()", //case description
    "CGSItemTextArray",                   //class being tested
    "CurrentFeature",            //method being tested
    "FUNCTIONALITY",                        //FUNCTIONALITY, BOUNDARY, ERRORHANDLING
    SetupGSItemTextArrayL, T_Global_GSItemTextArray_CurrentFeatureL, Teardown )
        
EUNIT_TEST(
    "CGSItemTextArray - IndexForFeatureIdL()", //case description
    "CGSItemTextArray",                   //class being tested
    "IndexForFeatureIdL",            //method being tested
    "FUNCTIONALITY",                        //FUNCTIONALITY, BOUNDARY, ERRORHANDLING
    SetupGSItemTextArrayL, T_Global_GSItemTextArray_IndexForFeatureIdL, Teardown )

EUNIT_TEST(
    "CGSItemTextArray - FeatureIdByIndex()", //case description
    "CGSItemTextArray",                   //class being tested
    "FeatureIdByIndex",            //method being tested
    "FUNCTIONALITY",                        //FUNCTIONALITY, BOUNDARY, ERRORHANDLING
    SetupGSItemTextArrayL, T_Global_GSItemTextArray_FeatureIdByIndexL, Teardown )

EUNIT_TEST(
    "CGSItemTextArray - FeatureByIdL()", //case description
    "CGSItemTextArray",                   //class being tested
    "FeatureByIdL",            //method being tested
    "FUNCTIONALITY",                        //FUNCTIONALITY, BOUNDARY, ERRORHANDLING
    SetupGSItemTextArrayL, T_Global_GSItemTextArray_FeatureByIdL, Teardown )

EUNIT_TEST(
    "CGSItemTextArray - GetItemCaptionFromFeatureIdLC()", //case description
    "CGSItemTextArray",                   //class being tested
    "GetItemCaptionFromFeatureIdLC",            //method being tested
    "FUNCTIONALITY",                        //FUNCTIONALITY, BOUNDARY, ERRORHANDLING
    SetupGSItemTextArrayL, T_Global_GSItemTextArray_GetItemCaptionFromFeatureIdLC, Teardown )

    
// CGSRadioButtonSettingPageItemTextArray test cases
    
EUNIT_TEST(
    "CGSRadioButtonSettingPageItemTextArray - NewL()", //case description
    "CGSRadioButtonSettingPageItemTextArray",                   //class being tested
    "NewL",            //method being tested
    "FUNCTIONALITY",                        //FUNCTIONALITY, BOUNDARY, ERRORHANDLING
    SetupResourceL, T_Global_GSRBSPItemTextArray_NewL, Teardown )

EUNIT_TEST(
    "CGSRadioButtonSettingPageItemTextArray - CurrentFeatureL()", //case description
    "CGSRadioButtonSettingPageItemTextArray",                   //class being tested
    "CurrentFeatureL",            //method being tested
    "FUNCTIONALITY",                        //FUNCTIONALITY, BOUNDARY, ERRORHANDLING
    Setup_GSRBSPItemTextArrayL, T_Global_GSRBSPItemTextArray_CurrentFeatureL, Teardown )

EUNIT_TEST(
    "CGSRadioButtonSettingPageItemTextArray - SetRadioButtonSettingPageL()", //case description
    "CGSRadioButtonSettingPageItemTextArray",                   //class being tested
    "SetRadioButtonSettingPageL",            //method being tested
    "FUNCTIONALITY",                        //FUNCTIONALITY, BOUNDARY, ERRORHANDLING
    Setup_GSRBSPItemTextArrayL, T_Global_GSRBSPItemTextArray_SetRadioButtonSettingPageL, Teardown )
    
    
// CGSListBoxItemTextArray test cases
    
EUNIT_TEST(
    "CGSListBoxItemTextArray - NewL()", //case description
    "CGSListBoxItemTextArray",                   //class being tested
    "NewL",            //method being tested
    "FUNCTIONALITY",                        //FUNCTIONALITY, BOUNDARY, ERRORHANDLING
    SetupResourceL, T_Global_CGSLBITextArray_NewL, Teardown )
    
EUNIT_TEST(
    "CGSListBoxItemTextArray - another NewL()", //case description
    "CGSListBoxItemTextArray",                   //class being tested
    "overloaded NewL",            //method being tested
    "FUNCTIONALITY",                        //FUNCTIONALITY, BOUNDARY, ERRORHANDLING
    SetupResourceL, T_Global_CGSLBITextArray_New2L, Teardown )
    
EUNIT_TEST(
    "CGSListBoxItemTextArray - CurrentFeature()", //case description
    "CGSListBoxItemTextArray",                   //class being tested
    "CurrentFeature",            //method being tested
    "FUNCTIONALITY",                        //FUNCTIONALITY, BOUNDARY, ERRORHANDLING
    Setup_CGSLBITextArrayL, T_Global_CGSLBITextArray_CurrentFeatureL, Teardown )
    
EUNIT_END_TEST_TABLE

//  END OF FILE
