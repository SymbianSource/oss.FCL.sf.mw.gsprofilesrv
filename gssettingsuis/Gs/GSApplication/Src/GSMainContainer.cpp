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
* Description:  Container for the main view of General Settings
*
*/


// INCLUDE FILES
#include    <bldvariant.hrh>

#include    "GSUi.h"
#include    "gsmainview.h"
#include    "GSMainContainer.h"
#include    "gsplugininterface.h"
#include    "GsLogger.h"
#include    <GSApp.rsg>         // Resources for mainview

#include    <aknlists.h>
#include    <calslbs.h>
#include    <eikclbd.h>
#include    <AknIconArray.h>    // For loading icons
#include    <AknsUtils.h>       // For loading icons
#include    <aknViewAppUi.h>
#include    <akntitle.h>
#include    <barsread.h>
#include    <gulicon.h>         // For CGulIcon
#include    <csxhelp/cp.hlp.hrh>
#include    <gsfwviewuids.h>    // for KUidGS

//CONSTANTS

// ========================= MEMBER FUNCTIONS ================================


// ---------------------------------------------------------------------------
// CGSMainContainer::ConstructL(const TRect& aRect)
// Symbian OS two phased constructor
//
// ---------------------------------------------------------------------------
//
void CGSMainContainer::ConstructL(
    const TRect& aRect,
    CAknViewAppUi* aAppUi,
    CArrayPtrFlat<CGSPluginInterface>* aPluginArray )
    {
    iAppUi = aAppUi;
    iPluginArray = aPluginArray;

    // Initialize the array containing pointers to plugins that are actually
    // displayed in lbx.
    iVisiblePlugins = new CArrayPtrFlat<CGSPluginInterface>( 10 );

    CEikStatusPane* sp = iAppUi->StatusPane();
    CAknTitlePane* title = static_cast<CAknTitlePane*>
        ( sp->ControlL( TUid::Uid( EEikStatusPaneUidTitle ) ) );

    // Set view title from resource:
    TResourceReader rReader;
    iCoeEnv->CreateResourceReaderLC( rReader, R_GS_MAIN_VIEW_TITLE );
    title->SetFromResourceL( rReader );
    CleanupStack::PopAndDestroy(); //rReader

    CreateWindowL(); // Makes this control a window-owning control

    // Create listbox:
    iListBox = new( ELeave ) CAknSingleLargeStyleListBox;
    iListBox->ConstructL( this );
    iListBox->SetContainerWindowL( *this );
    iListBox->SetListBoxObserver( this );
    iListBox->CreateScrollBarFrameL(ETrue);
    iListBox->ScrollBarFrame()->SetScrollBarVisibilityL(
        CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto );

    // Obtain reference to listbox's item text array:
    iItemTextArray
        = STATIC_CAST( CDesCArray*, iListBox->Model()->ItemTextArray() );

    SetRect( aRect );
    ActivateL();
    __GSLOGSTRING( "[CGSMainContainer] ConstructL()" );
    }


// ---------------------------------------------------------------------------
// CGSMainContainer::~CGSMainContainer()
// Destructor
//
// ---------------------------------------------------------------------------
//
CGSMainContainer::~CGSMainContainer()
    {
    if  ( iVisiblePlugins )
        {
        iVisiblePlugins->Reset();
        }
    delete iVisiblePlugins;

    if  ( iItemTextArray )
        {
        iItemTextArray->Reset();
        }

    if ( iListBox )
        {
        delete iListBox;
        }
    __GSLOGSTRING( "[CGSMainContainer] ~CGSMainContainer" );
  }

// ---------------------------------------------------------------------------
// CGSMainContainer::UpdateListbox
//
// Updates listbox from iPluginArray.
// ---------------------------------------------------------------------------
//
void CGSMainContainer::UpdateListBoxL()
    {
    
    if( iPluginArray->Count() == 0 )
		{
        return;
		}
    
    __GSLOGSTRING( "[CGSMainContainer::UpdateListBoxL]" );
    
    // Empty item array because it will be reoccupied.
    iItemTextArray->Reset();

    // Empty the array because it will be updated.
    iVisiblePlugins->Reset();

    // Array for icons in the listbox. Owns the icons.
    CAknIconArray* icons = new (ELeave) CAknIconArray( 10 );
    CleanupStack::PushL( icons );

    // Index of the plugin inserted into lbx item array.
    TInt pluginIndex = 0;

    // 1) Add each plugin's data to listbox if the plugin is visible.
    for( TInt i = 0; i < iPluginArray->Count(); i++ )
        {
        CGSPluginInterface* plugin = iPluginArray->operator[]( i );
        if( plugin->Visible() )
            {
            AddPluginDataToLbxL( plugin, pluginIndex, iItemTextArray, icons );
            pluginIndex++;
            }
        }

    // Destroy the old icons because SetIconArray() does not destroy them.
    CArrayPtr<CGulIcon>* oldIcons
        = iListBox->ItemDrawer()->ColumnData()->IconArray();
    if( oldIcons )
        {
        oldIcons->ResetAndDestroy();
        delete oldIcons;
        }

    // Transfer ownership of icon array to the lbx.
    // SetIconArray() does not delete the old icon array
    iListBox->ItemDrawer()->ColumnData()->SetIconArray( icons );
    CleanupStack::Pop( icons );

    // Draws listbox again
    iListBox->HandleItemAdditionL();
    }


//Disabling warning caused by err variable used inside macros
#pragma diag_suppress 550
// -----------------------------------------------------------------------------
// CGSMainContainer::AddPluginDataToLbxL()
//
//
// -----------------------------------------------------------------------------
//
void CGSMainContainer::AddPluginDataToLbxL( CGSPluginInterface* aPlugin,
                                              TInt aIndex,
                                              CDesCArray* aItemTextArray,
                                              CAknIconArray* aIconArray )
    {
    // Add the plugin to visible plugins array:
    iVisiblePlugins->AppendL( aPlugin );

    // 1.1) Read item descriptor from plugin:
    TBuf<256> buf;
    _LIT( KTab, "\t" );

    TRAPD
        (
        captionErr,
        aPlugin->GetCaptionL( buf );
        )

    // If plugin caption fails, set caption to error string.
    if( captionErr != KErrNone )
        {
        _LIT( KGSParentPluginCaptionErrorTxt, "Error" );
        buf.Append( KGSParentPluginCaptionErrorTxt );
        }
       else
        {
        __GSLOGSTRING2( "[CGSMainContainer] 0x%X::GetCaptionL Error: %d",
        aPlugin->Id().iUid, captionErr );
        }

    /**
    * 1.2) Create formatted item string.
    * list_single_large_pane:
    * list item string format: "1\tTextLabel\t0\t0"
    * where 0 is an index to icon array
    */
    TBuf<256> item;
    item.AppendNum( aIndex );
    item.Append( KTab );
    item.Append( buf );

    aItemTextArray->AppendL( item );

    // 1.3) Add icons if listbox uses them:
    CGulIcon* icon;
    TRAPD
        (
        iconErr,
        icon = aPlugin->CreateIconL( KGSIconTypeLbxItem );
        )

    __GSLOGSTRING2(
        "[CGSMainContainer] CreateIconL result for 0x%X: %d",
        aPlugin->Id().iUid, iconErr );
    TSize size = icon->Bitmap()->SizeInPixels();
    CleanupStack::PushL( icon );
    aIconArray->AppendL( icon );
    CleanupStack::Pop( icon );

    }
//Enabling warnings
#pragma diag_default 550
    
    
// ---------------------------------------------------------------------------
// CGSMainContainer::GetHelpContext(TCoeHelpContext& aContext) const
// Gets Help
//
// ---------------------------------------------------------------------------
//
void CGSMainContainer::GetHelpContext( TCoeHelpContext& aContext ) const
    {
    aContext.iMajor = KUidGS;
    aContext.iContext = KCP_HLP_MAIN_VIEW;
    }


// ---------------------------------------------------------------------------
// CGSMainContainer::SizeChanged
// called by framwork when the view size is changed
//
// ---------------------------------------------------------------------------
//
void CGSMainContainer::SizeChanged()
    {
    if ( iListBox )
        {
        iListBox->SetRect( Rect() );
        }
    }


// ---------------------------------------------------------------------------
// CGSMainContainer::CountComponentControls
//
//
// ---------------------------------------------------------------------------
//
TInt CGSMainContainer::CountComponentControls() const
    {
    return 1;
    }


// ---------------------------------------------------------------------------
// CGSMainContainer::ComponentControl
//
//
// ---------------------------------------------------------------------------
//
CCoeControl* CGSMainContainer::ComponentControl(TInt /*aIndex*/) const
    {
    return iListBox;
    }


// ---------------------------------------------------------------------------
// CGSMainContainer::TKeyResponse OfferKeyEventL
// Called when a key is pressed.
//
// ---------------------------------------------------------------------------
//
TKeyResponse  CGSMainContainer::OfferKeyEventL(const TKeyEvent& aKeyEvent,
                                               TEventCode aType)
    {
    switch ( aKeyEvent.iCode )
        {
        case EKeyLeftArrow:
        case EKeyRightArrow:
            // Listbox takes all event even if it doesn't use them
            return EKeyWasNotConsumed;
        default:
            break;
        }
    // now it's iListBox's job to process the key event
    return iListBox->OfferKeyEventL(aKeyEvent, aType);
    }


// ---------------------------------------------------------------------------
// CGSMainContainer::HandleListBoxEventL
//
//
// ---------------------------------------------------------------------------
void CGSMainContainer::HandleListBoxEventL(
    CEikListBox* aListBox,
    TListBoxEvent aEventType )
    {
    __GSLOGSTRING1(
        "[CGSMainContainer] HandleListBoxEventL aEventType:%d",
        aEventType);
    switch ( aEventType )
        {
        case EEventEnterKeyPressed:
        case EEventItemSingleClicked:
            {
            if (aListBox->CurrentItemIndex() < 0)
                {
                // Negative listbox value -> no item selected? 
                break;
                }

            CGSPluginInterface* selectedPlugin = iVisiblePlugins->operator[](
                    aListBox->CurrentItemIndex());

            // Different command is used depending on the plugin type.
            switch (selectedPlugin->ItemType())
                {
                // In these cases the plugin is a view:
                case EGSItemTypeSingleLarge:
                case EGSItemTypeSetting:
                case EGSItemTypeSettingIcon:
                    iAppUi->ActivateLocalViewL(selectedPlugin->Id());
                    break;
                    // In these cases the plugin is a dialog:
                case EGSItemTypeSettingDialog:
                case EGSItemTypeSingleLargeDialog:
                    selectedPlugin->HandleSelection(
                            EGSSelectionBySelectionKey);
                    UpdateListBoxL(); // Refrest the listbox if value changed.
                    break;
                }
            }
            break;
        default:
           break;
        }
    }


// ---------------------------------------------------------------------------
// CGSMainContainer::ListBox()
//
//
// ---------------------------------------------------------------------------
//
CAknSingleLargeStyleListBox* CGSMainContainer::ListBox()
    {
    return iListBox;
    }


// ---------------------------------------------------------------------------
// CGSMainContainer::HandleResourceChange()
//
//
// ---------------------------------------------------------------------------
//
void CGSMainContainer::HandleResourceChange( TInt aType )
    {
    //Handle layout orientation or skin change
    CCoeControl::HandleResourceChange( aType );
    if ( aType == KAknsMessageSkinChange )
        {
        TRAP_IGNORE( UpdateListBoxL() );
        }
    else if ( aType == KEikDynamicLayoutVariantSwitch  )
	    {
        TRect mainPaneRect;
        AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::EMainPane,
                                           mainPaneRect);
        SetRect( mainPaneRect );
	    }
    }
    

// -----------------------------------------------------------------------------
// CGSMainContainer::SelectedPlugin()
//
//
// -----------------------------------------------------------------------------
//
CGSPluginInterface* CGSMainContainer::SelectedPlugin()
    {
    CGSPluginInterface* plugin = NULL;
    const TInt index = iListBox->CurrentItemIndex();
    if ( index >= 0 && index <= iVisiblePlugins->Count() )
        {
        plugin = iVisiblePlugins->operator[]( index );
    __GSLOGSTRING2( "[CGSMainContainer] SelectedPlugin 0x%X in index %d",
        plugin->Id().iUid, index );
        }
    return plugin;
    }


// -----------------------------------------------------------------------------
// CGSMainContainer::SetSelectedItem()
//
//
// -----------------------------------------------------------------------------
//
void CGSMainContainer::SetSelectedItem( TUid aSelectedItemUid )
    {
    __GSLOGSTRING1( "[CGSMainContainer] SetSelectedItem(0x%X)",
        aSelectedItemUid.iUid );

    TInt selectedItemIndex  = 0;
    for( TInt i = 0; i < iVisiblePlugins->Count(); i++ )
        {
        CGSPluginInterface* plugin = iVisiblePlugins->operator[]( i );

        __GSLOGSTRING2( "[CGSMainContainer] Checking [%d] 0x%X)",
            i, plugin->Id().iUid );

        // If plugin is found, select it:
        if( plugin->Id() == aSelectedItemUid )
            {
            __GSLOGSTRING2( "[CGSMainContainer] Match 0x%X in index %d",
                plugin->Id().iUid, i );
            selectedItemIndex = i;
            break;
            }
        }
    iListBox->SetCurrentItemIndexAndDraw( selectedItemIndex );
    }


// ---------------------------------------------------------------------------
// CGSMainContainer::FocusChanged
//
// Set focus on the selected listbox. For animated skins feature.
// ---------------------------------------------------------------------------
void CGSMainContainer::FocusChanged( TDrawNow /*aDrawNow*/ )
    {
    if( iListBox )
        {
        iListBox->SetFocus( IsFocused() );
        }
    }


// -----------------------------------------------------------------------------
// CGSMainContainer::SetListBoxEmptyTextL()
//
//Set the empty text of list box.
// -----------------------------------------------------------------------------
//
void CGSMainContainer::SetListBoxEmptyTextL(const TDes& aEmpty )
    {
    if( iListBox )
        {
        iListBox->View()->SetListEmptyTextL( aEmpty );
        }
    }

// -----------------------------------------------------------------------------
// CGSMainContainer::StoreListBoxPositionL
//
//Store the exact position of listbox.
// -----------------------------------------------------------------------------
//
void CGSMainContainer::StoreListBoxPositionL( CGSMainView::TListBoxPosition& aPosition )
	{
    aPosition.iCurrentItemIndex = iListBox->CurrentItemIndex();
    aPosition.iItemOffsetInPixels = iListBox->View()->ItemOffsetInPixels();
    aPosition.iTopItemIndex = iListBox->View()->TopItemIndex();
	}

// -----------------------------------------------------------------------------
// CGSMainContainer::RestoreListBoxPositionL
//
//Restore the exact position of listbox.
// -----------------------------------------------------------------------------
//
void CGSMainContainer::RestoreListBoxPositionL( const CGSMainView::TListBoxPosition& aPosition, TBool aScreenModeChanged )
	{
    if ( aPosition.iCurrentItemIndex >= 0 )
        {
        iListBox->SetCurrentItemIndex( aPosition.iCurrentItemIndex );
        }
    
    if ( aScreenModeChanged )
        {
        iListBox->View()->VerticalMoveToItemL( aPosition.iCurrentItemIndex,
                CListBoxView::ESingleSelection );
        }
    else
        {
        iListBox->View()->SetItemOffsetInPixels( aPosition.iItemOffsetInPixels );
        iListBox->View()->SetTopItemIndex( aPosition.iTopItemIndex );
        }
    }
// End of File
