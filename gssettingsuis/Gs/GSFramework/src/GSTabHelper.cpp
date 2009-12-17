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
* Description:  Utility class for creating tab groups.
*
*/


// INCLUDE FILES
#include "gstabhelper.h"
#include "gstabbedview.h"
#include "GsLogger.h"

#include <aknappui.h>
#include <aknnavi.h>
#include <aknnavide.h>
#include <akntabgrp.h>
#include <AknTabObserver.h>
#include <eikspane.h>
#include <gulicon.h>

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------------------------
// CGSTabHelper::CGSTabHelper
// C++ constructor.
//
// ---------------------------------------------------------------------------
//
CGSTabHelper::CGSTabHelper()
    {
    }


// ---------------------------------------------------------------------------
// CGSTabHelper::NewL()
// Symbian OS two-phased constructor.
//
// ---------------------------------------------------------------------------
//
EXPORT_C CGSTabHelper* CGSTabHelper::NewL()
    {
    CGSTabHelper* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CGSTabHelper::NewLC()
// Symbian OS two-phased constructor.
//
// ---------------------------------------------------------------------------
//
EXPORT_C CGSTabHelper* CGSTabHelper::NewLC()
    {
    CGSTabHelper* self = new(ELeave) CGSTabHelper();
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CGSTabHelper::ConstructL(const TRect& aRect)
// Symbian OS default constuctor.
//
// ---------------------------------------------------------------------------
//
void CGSTabHelper::ConstructL()
    {
    iNaviDecorator = NULL;

    iAppUi =  static_cast<CAknViewAppUi*>( CCoeEnv::Static()->AppUi() );

    CEikStatusPane* statusPane = iAppUi->StatusPane();

    // Fetch pointer to the default navi pane container:
    iNaviContainer = static_cast<CAknNavigationControlContainer*>
        ( statusPane->ControlL( TUid::Uid(EEikStatusPaneUidNavi ) ) );
    }


// ---------------------------------------------------------------------------
// CGSTabHelper::~CGSTabHelper()
// Destructor
//
// ---------------------------------------------------------------------------
//
EXPORT_C CGSTabHelper::~CGSTabHelper()
    {
    RemoveTabGroup();
    }


// -----------------------------------------------------------------------------
// CGSTabHelper::CreateTabGroupL()
//
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CGSTabHelper::CreateTabGroupL( TUid aActiveViewUid,
                                             MGSTabbedViewOwner* aTabOwner )
    {
    iTabOwner = aTabOwner;
    CreateTabGroupL( aActiveViewUid, iTabOwner->TabbedViews() );
    }

// ---------------------------------------------------------------------------
// CGSTabHelper::CreateTabGroupL()
//
//
// ---------------------------------------------------------------------------
//
EXPORT_C void CGSTabHelper::CreateTabGroupL(
    TUid aActiveViewUid,
    CArrayPtrFlat<CGSPluginInterface>* aTabbedViews )
    {
    RemoveTabGroup();//Remove possible old tab group.
    iNaviDecorator = iNaviContainer->CreateTabGroupL( this );
    iTabGroup =
        static_cast<CAknTabGroup*> ( iNaviDecorator->DecoratedControl() );

    switch ( aTabbedViews->Count() )
        {
        case 1:
            iTabGroup->SetTabFixedWidthL( KTabWidthWithOneTab );
            break;
        case 2:
            iTabGroup->SetTabFixedWidthL( KTabWidthWithTwoTabs );
            break;
        case 3:
            iTabGroup->SetTabFixedWidthL( KTabWidthWithThreeTabs );
            break;
        default:
            iTabGroup->SetTabFixedWidthL( KTabWidthWithFourTabs );
            break;
        }

    for( TInt i = 0; i < aTabbedViews->Count(); i++ )
        {
        CGSPluginInterface* tabbedView = aTabbedViews->operator[]( i );
        CGulIcon* icon = NULL;
        TRAPD
            (
            err,
            icon = tabbedView->CreateIconL( KGSIconTypeTab );
            )

        // CreateIconL should not leave outside. If creating icon leaves, use
        // default icon.
        if( err != KErrNone )
            {
            __GSLOGSTRING2(
                "[CGSTabHelper] ERROR: 0x%X::CreateIconL failed (%d), using default icon.",
                tabbedView->Id().iUid, err );

            icon =
                tabbedView->CGSPluginInterface::CreateIconL( KGSIconTypeTab );
            }
        CleanupStack::PushL( icon );
        iTabGroup->AddTabL( tabbedView->Id().iUid,
                           icon->Bitmap(),
                           icon->Mask() );

        // Bitmaps are now owned by the tab:
        icon->SetBitmapsOwnedExternally( ETrue );

        // Delete icon object. Bitmaps are not owned by icon anymore and thus
        // not deleted:
        CleanupStack::PopAndDestroy( icon );
        }

    iNaviContainer->PushL( *iNaviDecorator );

    // The active tab should be same as active plugin
    iTabGroup->SetActiveTabById( aActiveViewUid.iUid );
    }


// ---------------------------------------------------------------------------
// CGSTabHelper::CreateTabGroupL()
//
//
// ---------------------------------------------------------------------------
//
EXPORT_C void CGSTabHelper::CreateTabGroupL(
    TUid aActiveViewUid,
    CArrayPtrFlat<MGSTabbedView>* aTabbedViews )
    {
    RemoveTabGroup();//Remove possible old tab group.
    iNaviDecorator = iNaviContainer->CreateTabGroupL( this );
    iTabGroup =
        static_cast<CAknTabGroup*> ( iNaviDecorator->DecoratedControl() );

    switch ( aTabbedViews->Count() )
        {
        case 1:
            iTabGroup->SetTabFixedWidthL( KTabWidthWithOneTab );
            break;
        case 2:
            iTabGroup->SetTabFixedWidthL( KTabWidthWithTwoTabs );
            break;
        case 3:
            iTabGroup->SetTabFixedWidthL( KTabWidthWithThreeTabs );
            break;
        default:
            iTabGroup->SetTabFixedWidthL( KTabWidthWithFourTabs );
            break;
        }

    for( TInt i = 0; i < aTabbedViews->Count(); i++ )
        {
        MGSTabbedView* tabbedView = aTabbedViews->operator[]( i );

         // Takes icon ownership
        CGulIcon* icon = tabbedView->CreateTabIconL();
        CleanupStack::PushL( icon );


        iTabGroup->AddTabL( tabbedView->Id().iUid,
                           icon->Bitmap(),
                           icon->Mask() );

        //Bitmaps are now owned by the tab:
        icon->SetBitmapsOwnedExternally( ETrue );

        // Delete icon object. Bitmaps are not owned by icon anymore and thus
        // not deleted:
        CleanupStack::PopAndDestroy( icon );
        }

    iNaviContainer->PushL( *iNaviDecorator );

    // The active tab should be same as active plugin
    iTabGroup->SetActiveTabById( aActiveViewUid.iUid );
    }


// ---------------------------------------------------------------------------
// CGSTabHelper::RemoveTabGroup()
//
// Removes the tab group owned by this class from navi pane.
// ---------------------------------------------------------------------------
//
EXPORT_C void CGSTabHelper::RemoveTabGroup()
    {
    if( iNaviDecorator )
        {
        iNaviContainer->Pop( iNaviDecorator );
        delete iNaviDecorator;
        iNaviDecorator = NULL;
        }
    }


// ---------------------------------------------------------------------------
// CGSTabHelper::TabChangedL()
//
//
// ---------------------------------------------------------------------------
//
EXPORT_C void CGSTabHelper::TabChangedL( TInt aIndex )
    {
    TUid uid;
    uid.iUid = iTabGroup->TabIdFromIndex( aIndex );

    // If has a tab owner, notify it:
    if( iTabOwner )
        {
        iTabOwner->TabChangedL( uid );
        }

    iAppUi->ActivateLocalViewL( uid );
    }


//  End of File
