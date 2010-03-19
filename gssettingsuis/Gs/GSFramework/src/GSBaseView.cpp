/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Base view for all General Settings views.
*
*/


// INCLUDE FILES
#include    <gsbaseview.h>
#include    <gsbasecontainer.h>

#include    <aknnavide.h>
#include    <akntabgrp.h>
#include    <akntitle.h>
#include    <AknUtils.h>
#include    <aknViewAppUi.h>
#include    <barsread.h>
#include    <bautils.h> // BaflUtils
#include    <bldvariant.hrh>
#include    <featmgr.h>


//Disabling warning caused by KGSDoActivateError constant
#pragma diag_suppress 177
// CONSTANTS
_LIT( KGSDoActivateError, "DoActivateL" );

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSBaseView::CGSBaseView
//
// C++ constructor
// ---------------------------------------------------------------------------
EXPORT_C CGSBaseView::CGSBaseView()
    : iResourceLoader( *iCoeEnv )
    {
    iAppUi = AppUi();
    iElaf = ( AknLayoutUtils::Variant() == EEuropeanVariant );
    }


// ---------------------------------------------------------------------------
// CGSBaseView::~CGSBaseView
//
// Destructor
// ---------------------------------------------------------------------------
EXPORT_C CGSBaseView::~CGSBaseView()
    {
    iResourceLoader.Close();
    if (iContainer)
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        delete iContainer;
        }
    if ( iNaviPaneContext )
        {
        delete iNaviPaneContext;
        }
    }


// ---------------------------------------------------------------------------
// CGSBaseView::Container
//
//
// ---------------------------------------------------------------------------
EXPORT_C CGSBaseContainer* CGSBaseView::Container()
    {
    return iContainer;
    }


// ---------------------------------------------------------------------------
// CGSBaseView::SetCurrentItem
//
//
// ---------------------------------------------------------------------------
EXPORT_C void CGSBaseView::SetCurrentItem( TInt aIndex )
    {
    iCurrentItem = aIndex;
    }


// ---------------------------------------------------------------------------
// CGSBaseView::HandleListBoxEventL
//
//
// ---------------------------------------------------------------------------

EXPORT_C void CGSBaseView::HandleListBoxEventL( CEikListBox* /*aListBox*/,
                                                TListBoxEvent aEventType )
    {
    switch ( aEventType )
        {
        case EEventEnterKeyPressed:
        case EEventItemDoubleClicked:
            HandleListBoxSelectionL();
            break;
        default:
           break;
        }
    }


// ---------------------------------------------------------------------------
// CGSBaseView::SetNaviPaneL
//
//
// ---------------------------------------------------------------------------
//
EXPORT_C void CGSBaseView::SetNaviPaneL()
    {
    CAknNavigationControlContainer* naviControlContainer = 
                                            NaviControlContainerL();
    if ( iNaviPaneContext )
        {
        naviControlContainer->PushL( *iNaviPaneContext );
        }
    else
        {
        naviControlContainer->PushDefaultL();
        }
    }


// ---------------------------------------------------------------------------
// CGSBaseView::CreateNaviPaneContextL
//
//
// ---------------------------------------------------------------------------
//
EXPORT_C void CGSBaseView::CreateNaviPaneContextL( TInt aResourceId )
    {
    TResourceReader reader;
    iCoeEnv->CreateResourceReaderLC( reader, aResourceId );
    CAknNavigationControlContainer* naviControlContainer = 
                                             NaviControlContainerL();
    iNaviPaneContext = naviControlContainer->CreateMessageLabelL( reader );
    CleanupStack::PopAndDestroy();  // reader
    }


// ---------------------------------------------------------------------------
// CGSBaseView::NaviControlContainerL
//
//
// ---------------------------------------------------------------------------
//
CAknNavigationControlContainer* CGSBaseView::NaviControlContainerL()
    {
    CAknNavigationControlContainer* control = NULL;
    if( !iNaviControlContainer )
        {
        iNaviControlContainer =
            static_cast<CAknNavigationControlContainer*>(
                AppUi()->StatusPane()->ControlL( TUid::Uid(
                    EEikStatusPaneUidNavi ) ) );
        }
    else
        {
        control = iNaviControlContainer;
        }
    
    return control;
    }


// ---------------------------------------------------------------------------
// CGSBaseView::HandleClientRectChange
//
//
// ---------------------------------------------------------------------------
void CGSBaseView::HandleClientRectChange()
    {
    if ( iContainer && iContainer->iListBox )
        {
        iContainer->SetRect( ClientRect() );
        }
    }


// ---------------------------------------------------------------------------
// CGSBaseView::DoActivateL
// Activates the view.
//
// ---------------------------------------------------------------------------
EXPORT_C void CGSBaseView::DoActivateL( const TVwsViewId& aPrevViewId,
                                        TUid /*aCustomMessageId*/,
                                        const TDesC8& /*aCustomMessage*/ )
    {
    iPrevViewId = aPrevViewId;
//    if( iContainer )
//        {
//        AppUi()->RemoveFromViewStack( *this, iContainer );
//
//        delete iContainer;
//        iContainer = NULL;
//        }
    if ( !iContainer )
    	{
    	CreateContainerL();
		AppUi()->AddToViewStackL( *this, iContainer );
		iContainer->iListBox->SetListBoxObserver( this ) ;
		iContainer->iListBox->SetTopItemIndex( iTopItemIndex );
		iContainer->SetSelectedItem( iCurrentItem );
    	}
    }


// ---------------------------------------------------------------------------
// CGSBaseView::DoDeactivate()
//
//
// ---------------------------------------------------------------------------
EXPORT_C void CGSBaseView::DoDeactivate()
    {
    if ( iContainer )
        {
        iCurrentItem = iContainer->SelectedItem();
        AppUi()->RemoveFromViewStack( *this, iContainer );
        delete iContainer;
        iContainer = NULL;
        }
    }


// ---------------------------------------------------------------------------
// CGSBaseView::CreateContainerL()
//
//
// ---------------------------------------------------------------------------
//
EXPORT_C void CGSBaseView::CreateContainerL()
    {
    NewContainerL();
    __ASSERT_DEBUG(
        iContainer, User::Panic( KGSDoActivateError, EGSViewPanicNullPtr ) );
    iContainer->SetMopParent( this );

    TRAPD( error, iContainer->ConstructL( ClientRect() ) );

    if ( error )
        {
        delete iContainer;
        iContainer = NULL;
        User::Leave( error );
        }
    }


// ---------------------------------------------------------------------------
// CGSBaseView::OpenLocalizedResourceFileL()
//
//
// ---------------------------------------------------------------------------
//
EXPORT_C void CGSBaseView::OpenLocalizedResourceFileL(
    const TDesC& aResourceFileName,
    RConeResourceLoader& aResourceLoader )
    {
    RFs fsSession;
    User::LeaveIfError( fsSession.Connect() );

    // Find the resource file:
    TParse parse;
    parse.Set( aResourceFileName, &KDC_RESOURCE_FILES_DIR, NULL );
    TFileName fileName( parse.FullName() );

    // Get language of resource file:
    BaflUtils::NearestLanguageFile( fsSession, fileName );

    // Open resource file:
    aResourceLoader.OpenL( fileName );

    // If leave occurs before this, close is called automatically when the
    // thread exits.
    fsSession.Close();
    }


// -----------------------------------------------------------------------------
// CGSBaseView::ResetSelectedItemIndex()
//
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CGSBaseView::ResetSelectedItemIndex()
    {
    iCurrentItem = 0;
    if( iContainer )
        {
        iContainer->SetSelectedItem( iCurrentItem );
        }
    }

//End of File
