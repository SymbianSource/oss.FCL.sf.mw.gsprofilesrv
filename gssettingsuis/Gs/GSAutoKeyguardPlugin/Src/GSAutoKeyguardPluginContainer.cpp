/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Container for the Device & SIM security sub-folder
*
*/


#include "GSAutoKeyguardPluginContainer.h"
#include "GSAutoKeyguardPluginModel.h"
#include "GSAutoKeyguardPlugin.hrh"

#include <bldvariant.hrh>
#include <aknlists.h>
#include <etelmm.h>
#include <RSSSettings.h>
#include <StringLoader.h>
#include <featmgr.h>
#include <csxhelp/cp.hlp.hrh>
#include <gsfwviewuids.h>
#include <gsautokeyguardpluginrsc.rsg>
#include <gslistbox.h>
#include <AknsConstants.h>

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS


// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ==============================


// ---------------------------------------------------------------------------
// CGSAutoKeyguardPluginContainer::ConstructL()
// 
// Symbian OS two phased constructor
// ---------------------------------------------------------------------------
//
void CGSAutoKeyguardPluginContainer::ConstructL( const TRect& aRect )
    { 
    iListBox = new( ELeave ) CAknSettingStyleListBox;
    if(AknLayoutUtils::PenEnabled())
        {
            BaseConstructL( aRect, R_GS_AUTO_KEYGUARD_VIEW_TITLE, R_TOUCH_AUTO_KEYGUARD_LBX );
        }
    else
        {
            BaseConstructL( aRect, R_GS_AUTO_KEYGUARD_VIEW_TITLE, R_AUTO_KEYGUARD_LBX );
        }
        
    }


// ---------------------------------------------------------------------------
// CGSAutoKeyguardPluginContainer::~CGSSettListSecurityContainer()
// 
// Destructor 
// ---------------------------------------------------------------------------
//

CGSAutoKeyguardPluginContainer::~CGSAutoKeyguardPluginContainer()
    {
    if(iAutoKeyguardItems)
        {
            delete iAutoKeyguardItems;
        }
	
	if(iListboxItemArray)
        {
           delete iListboxItemArray; 
        }
      
    }


// ---------------------------------------------------------------------------
// CGSAutoKeyguardPluginContainer::CGSAutoKeyguardPluginContainer()
// 
// Constructor
// ---------------------------------------------------------------------------
//
CGSAutoKeyguardPluginContainer::CGSAutoKeyguardPluginContainer(
                          CGSAutoKeyguardPluginModel* aModel )
    : iModel ( aModel )
    {
    }


// ---------------------------------------------------------------------------
// CGSAutoKeyguardPluginContainer::ConstructListBoxL()
// 
//  
// ---------------------------------------------------------------------------
//
void CGSAutoKeyguardPluginContainer::ConstructListBoxL( TInt aResLbxId )
    {

    iListBox->ConstructL( this, EAknListBoxSelectionList );
    iListboxItemArray = CGSListBoxItemTextArray::NewL( aResLbxId, 
                        *iListBox, *iCoeEnv );
    iListBox->Model()->SetItemTextArray( iListboxItemArray );
    iListBox->Model()->SetOwnershipType( ELbmDoesNotOwnItemArray );
	iAutoKeyguardItems = iCoeEnv->ReadDesC16ArrayResourceL( R_AUTOKEYGUARD_ARRAY );
    CreateListBoxItemsL();
    }


// ---------------------------------------------------------------------------
// CGSSettListIdleContainer::CreateListBoxItemsL()
// 
//  
// ---------------------------------------------------------------------------
//
void CGSAutoKeyguardPluginContainer::CreateListBoxItemsL()
    {
	MakeAutoKeyguardItemL();
    }

// ---------------------------------------------------------------------------
// CGSSettListIdleContainer::UpdateListBoxL( TInt aFeatureId )
// 
//  
// ---------------------------------------------------------------------------
//
void CGSAutoKeyguardPluginContainer::UpdateListBoxL( TInt aFeatureId )
    {
    switch( aFeatureId )
        {
        case EGSSettIdAutoKeyguard:
        	MakeAutoKeyguardItemL();
        	break;
        default:
            return;
        }
    
    iListBox->HandleItemAdditionL();
    }

// ---------------------------------------------------------------------------
// CGSAutoKeyguardPluginContainer::MakeAutoKeyguardItemL()
// 
//  
// ---------------------------------------------------------------------------
//
void CGSAutoKeyguardPluginContainer::MakeAutoKeyguardItemL()
    {
    #ifdef RD_AUTO_KEYGUARD
    
    HBufC* dynamicText = HBufC::NewLC( KGSBufSize128 );
    TPtr ptrBuffer ( dynamicText->Des() );

    TInt period = iModel->AutoKeyguardPeriod();
    
    //calculate minutes and seconds
    TInt minutes = 0;
    TInt sec = 0;
    if(period >= 60)
    {
    	minutes = period / 60;	
    }    
    sec = period - minutes * 60;
    
    if((minutes > 1 ) && (sec > 0))
    {
	    CArrayFixFlat<TInt> *minSec=new(ELeave)CArrayFixFlat<TInt>( 2 ); 
	    CleanupStack::PushL(minSec);
	    minSec->AppendL(minutes);
	    minSec->AppendL(sec);
	    
	    //Load the resource string; this function replaces the %0N and %1N in the
        //localisation string with numbers from the array having the same index as the number preceding the N.
        HBufC* tempDynamicText = StringLoader::LoadL( R_AUTO_KEYGUARD_MINSEC, *minSec, iCoeEnv);
        ptrBuffer = tempDynamicText->Des();
        delete tempDynamicText;
        CleanupStack::PopAndDestroy( minSec );
    }
    else if ((minutes == 1 ) && (sec > 0))
    {
    	CArrayFixFlat<TInt> *minSec=new(ELeave)CArrayFixFlat<TInt>( 2 ); 
	    CleanupStack::PushL(minSec);
	    minSec->AppendL(minutes);
	    minSec->AppendL(sec);
	    
	    //Load the resource string; this function replaces the %0N and %1N in the
        //localisation string with numbers from the array having the same index as the number preceding the N.
        HBufC* tempDynamicText = StringLoader::LoadL( R_AUTO_KEYGUARD_ONE_MIN_SEC, *minSec, iCoeEnv); 
        ptrBuffer = tempDynamicText->Des();
        delete tempDynamicText;
        CleanupStack::PopAndDestroy( minSec );
    }
    else if ((minutes > 1 ) && (sec == 0))
    {
    	TBuf<KGSBufSize128> tempString;
    	StringLoader::Format( tempString, 
                         	( ( *iAutoKeyguardItems )[3] ),
                          		-1, // no index in the key string
                          		minutes );                
    	ptrBuffer = tempString;	
    }
    else if ((minutes == 1 ) && (sec == 0))
    {  
        TBuf<KGSBufSize128> tempString;
    	StringLoader::Format( tempString, 
                         	( ( *iAutoKeyguardItems )[2] ),
                          		-1, // no index in the key string
                          		minutes );
    	ptrBuffer = tempString;	
    }
    else if ((minutes == 0 ) && (sec > 0))
    {
    	TBuf<KGSBufSize128> tempString;
    	StringLoader::Format( tempString, 
                         	( ( *iAutoKeyguardItems )[1] ),
                          		-1, // no index in the key string
                          		sec );                       
    	ptrBuffer = tempString;
    }
    else //both are 0, period is not user defined
    {
    	ptrBuffer = ( *iAutoKeyguardItems )[0];
    }
    
     
    iListboxItemArray->SetDynamicTextL( EGSSettIdAutoKeyguard, ptrBuffer );
    CleanupStack::PopAndDestroy( dynamicText );
    iListboxItemArray->SetItemVisibilityL( EGSSettIdAutoKeyguard, 
                       CGSListBoxItemTextArray::EVisible );
    #endif //RD_AUTO_KEYGUARD
    }

// ---------------------------------------------------------------------------
// CGSAutoKeyguardPluginContainer::CurrentFeatureId()
//  
// ---------------------------------------------------------------------------
//
TInt CGSAutoKeyguardPluginContainer::CurrentFeatureId() const
    {
    return iListboxItemArray->CurrentFeature( );
    }


// ---------------------------------------------------------------------------
// CGSAutoKeyguardPluginContainer::GetHelpContext() const
// Gets Help 
//  
// ---------------------------------------------------------------------------
//
void CGSAutoKeyguardPluginContainer::GetHelpContext( TCoeHelpContext& aContext ) const
    {
    aContext.iMajor = KUidGS;
    // aContext.iContext = KCP_HLP_DEVICE_LOCKING; // This was used before, when keylock and keguard had the same help file
    _LIT(KCP_HLP_AUTOKEYGUARD_LOCAL, "CP_HLP_AUTOKEYGUARD");	// the global KCP_HLP_AUTOKEYGUARD is defined only on some products but not all.
    aContext.iContext = KCP_HLP_AUTOKEYGUARD_LOCAL;
    }

// ---------------------------------------------------------------------------
// CGSAutoKeyguardPluginContainer::HandleResourceChangeL()
// 
// Updates view layout 
// ---------------------------------------------------------------------------
//
void CGSAutoKeyguardPluginContainer::HandleResourceChangeL( TInt aType )
    {
    if( aType == KAknsMessageSkinChange ||
        aType == KEikDynamicLayoutVariantSwitch )
        {
        TRect mainPaneRect;
        AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::EMainPane, 
                                           mainPaneRect );
        SetRect( mainPaneRect );
        DrawNow();
        }
    }    
    
//End of File
