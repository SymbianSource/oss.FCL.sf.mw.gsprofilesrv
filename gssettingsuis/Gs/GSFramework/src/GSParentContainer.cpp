/*
* Copyright (c) 2005-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Container for GSParentPlugin.
*
*/

// INCLUDE FILES
#include    <gsparentcontainer.h>
#include    <gsparentplugin.h>
#include    <gsplugininterface.h>
#include    "GsLogger.h"
#include    <gsparentpluginrsc.rsg>
#include    <gsfwviewuids.h>
#include    "GSBaseDocument.h"
#include    <mgswatchdog.h>

#include    <AknIconArray.h>  //for loading icons
#include    <aknlists.h>
#include    <AknsUtils.h>     //for loading icons
#include    <akntitle.h>
#include    <akntitle.h>
#include    <aknViewAppUi.h>
#include    <barsread.h> // For TResourceReader
#include    <bldvariant.hrh>
#include    <calslbs.h>
#include    <eikclbd.h>
#include    "GsContainerExt.h" //For CGsContainerExt

//CONSTANTS
const TInt KGSPluginArrayInitSize = 10;
const TInt KGSCaptionBufSize = 256;

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSParentContainer::ConstructL(const TRect& aRect)
// Symbian OS two phased constructor
//
// ---------------------------------------------------------------------------
//
void CGSParentContainer::ConstructL(
    const TRect& aRect,
    CAknViewAppUi* aAppUi,
    CArrayPtrFlat<CGSPluginInterface>* aPluginArray,
    TInt aTitleRscId,
    CGSParentPlugin* aParentPlugin,
    TGSListboxTypes aListBoxType )
    {
    iAppUi = aAppUi;
    iPluginArray = aPluginArray;
    iListBoxType = aListBoxType;
    iParentPlugin = aParentPlugin;

    CGSBaseDocument* doc = static_cast< CGSBaseDocument* >( iAppUi->Document() );
    iExt = CGsContainerExt::NewL();
    iExt->iGSWatchDog = doc->WatchDog();

    // Initialize the array containing pointers to plugins that are actually
    // displayed in lbx.
    iVisiblePlugins = new (ELeave) CArrayPtrFlat<CGSPluginInterface>( KGSPluginArrayInitSize );

    __GSLOGSTRING( "[CGSParentContainer] ConstructL" );

    // Set view title from resource:
    CEikStatusPane* sp = iAppUi->StatusPane();
    CAknTitlePane* title = static_cast<CAknTitlePane*>
        ( sp->ControlL( TUid::Uid( EEikStatusPaneUidTitle ) ) );
    TResourceReader rReader;
    iCoeEnv->CreateResourceReaderLC( rReader, aTitleRscId );
    title->SetFromResourceL( rReader );
    CleanupStack::PopAndDestroy(); // rReader

    CreateWindowL(); //makes this control a window-owning control

    CreateListBoxL( iListBoxType );

    SetRect( aRect );
    ActivateL();
    }


// ---------------------------------------------------------------------------
// CGSParentContainer::~CGSParentContainer()
// Destructor
//
// ---------------------------------------------------------------------------
//
CGSParentContainer::~CGSParentContainer()
    {
    __GSLOGSTRING( "[CGSParentContainer] ~CGSParentContainer()|->" );
    if  ( iVisiblePlugins )
        {
        iVisiblePlugins->Reset();
        }
    delete iVisiblePlugins;
    if ( iListBox )
        {
        delete iListBox;
        }
    delete iExt;
    __GSLOGSTRING( "[CGSParentContainer] ~CGSParentContainer()-|" );
  }


// ---------------------------------------------------------------------------
// CGSParentContainer::UpdateListboxL
//
// Updates listbox from iPluginArray. Uses switch statement to handle different
// listbox types from TGSListboxTypes because the list item format
// and icon numbers depend on the listbox type.
// ---------------------------------------------------------------------------
//
void CGSParentContainer::UpdateListBoxL()
    {
    __GSLOGSTRING( "[CGSParentContainer::UpdateListBoxL]" );
    iItemTextArray->Reset();

    // Empty the array because it will be updated.
    iVisiblePlugins->Reset();

    // Array for icons in the listbox. Array owns the icons.
    CAknIconArray* icons = NULL;

    // Create icon array if required by the listbox type:
    switch( iListBoxType )
        {
        case EGSListBoxTypeSingleLarge:
        case EGSListBoxTypeDouble2Large:
        case EGSListBoxTypeDoubleLarge:
            icons = new (ELeave) CAknIconArray( KGSPluginArrayInitSize );
            CleanupStack::PushL( icons );
            break;

        default:
            break;
        }

    // Counter for created icons. This is used to maintain corrext indexing for
    // each plugin's icons in case a plugin has multiple icons and icon indexes
    // can not be mapped directly to plugin index.
    TInt iconCounter = 0;

    // 1) Add each plugin's data to listbox if the plugin is visible.
    for( TInt i = 0; i < iPluginArray->Count(); i++ )
        {
        CGSPluginInterface* plugin = iPluginArray->operator[]( i );

        // Adding plugin data to lbx has a lot of CGSPluginInterface API calls.
        // -> A good place to use quarantine to catch panicking plugins in
        // any of these calls.
        #ifdef GS_ENABLE_WATCH_DOG
            iExt->iGSWatchDog->QuarantineL( plugin->Id() );
        #endif
            
        if( plugin->Visible() )
            {
            AddPluginDataToLbxL( plugin,
                                 iItemTextArray,
                                 icons,
                                 iconCounter );
            }
        #ifdef GS_ENABLE_WATCH_DOG
            iExt->iGSWatchDog->RemoveFromQuarantineL( plugin->Id() );
        #endif
        }

    // 2) Replace old icon array if needed.
    switch( iListBoxType )
        {
        case EGSListBoxTypeSingleLarge:
            {
            // Temporary variable needed to use casted lbx.
            CAknSingleLargeStyleListBox* lbx =
                STATIC_CAST( CAknSingleLargeStyleListBox*, iListBox );
            // Destroy the old icons because SetIconArray() does not destroy them.
            CArrayPtr<CGulIcon>* oldIcons
                = lbx->ItemDrawer()->ColumnData()->IconArray();
            if( oldIcons )
                {
                __GSLOGSTRING( "[CGSParentContainer] Destroying old icons." );
                oldIcons->ResetAndDestroy();
                delete oldIcons;
                }
            // Transfer ownership of icon array to the lbx.
            // SetIconArray() does not delete the old icon array
            lbx->ItemDrawer()->ColumnData()->SetIconArray( icons );
            CleanupStack::Pop( icons );
            }
            break;
        case EGSListBoxTypeDouble2Large:
        case EGSListBoxTypeDoubleLarge:
            {
            // Temporary variable needed to use casted lbx.
            CEikFormattedCellListBox* lbx =
                STATIC_CAST( CEikFormattedCellListBox*, iListBox );
            // Destroy the old icons because SetIconArray() does not destroy them.
            CArrayPtr<CGulIcon>* oldIcons
                = lbx->ItemDrawer()->ColumnData()->IconArray();
            if( oldIcons )
                {
                __GSLOGSTRING( "[CGSParentContainer] Destroying old icons." );
                oldIcons->ResetAndDestroy();
                delete oldIcons;
                }
            // Transfer ownership of icon array to the lbx.
            // SetIconArray() does not delete the old icon array
            lbx->ItemDrawer()->ColumnData()->SetIconArray( icons );
            CleanupStack::Pop( icons );
            }
            break;

        case EGSListBoxTypeSettings:
            break;

        default:
            break;
        }

#ifdef _GS_PARENTPLUGIN_LBX_FORMAT_TRACES
#pragma message("_GS_PARENTPLUGIN_LBX_FORMAT_TRACES activated")
        __GSLOGSTRING( "[CGSParentContainer::UpdateListBoxL] iItemTextArray:" );
        for( TInt i = 0; i < iItemTextArray->Count(); i++ )
            {
            __GSLOGSTRING1( "%S", &iItemTextArray->operator[]( i ) );
            }
#endif // _GS_PARENTPLUGIN_LBX_FORMAT_TRACES

    // 3) Draw listbox again
    iListBox->HandleItemAdditionL();
    __GSLOGSTRING( "[CGSParentContainer] UpdateListBoxL()-|" );
    }


// -----------------------------------------------------------------------------
// CGSParentContainer::AddPluginDataToLbxL()
//
//
// -----------------------------------------------------------------------------
//
void CGSParentContainer::AddPluginDataToLbxL( CGSPluginInterface* aPlugin,
                                              CDesCArray* aItemTextArray,
                                              CAknIconArray* aIconArray,
                                              TInt& aIconCounter )
    {
    // Used to disable icon
    const TInt KIconNotSupported = -1;

    // Index of icon used in AB-column
    TInt iconIndexAB = KIconNotSupported;

    // Index of icon used in D-column
    TInt iconIndexD = KIconNotSupported;

#pragma message("TODO: The HBufC buffer sizes could be optimized if possible:")

    // First line of lbx item text (plugin caption)
    HBufC* firstLabelBuf = HBufC::NewLC( KGSCaptionBufSize );
    TPtr firstLabel = firstLabelBuf->Des();

    // Second line of lbx item text (optional)
    HBufC* secondLabelBuf = HBufC::NewLC( KGSCaptionBufSize );
    TPtr secondLabel = secondLabelBuf->Des();

    // The complete formatted lbx item string which is added to lbx
    HBufC* lbxItemStringBuf = HBufC::NewLC( KGSCaptionBufSize * 2 );
    TPtr lbxItemString = lbxItemStringBuf->Des();

    // Separates lbx item elements
    _LIT( KTab, "\t" );

    // Normal icon in AB-Column
    CGulIcon* iconAB = NULL;

    // Small icon used in D-column (optional)
    CGulIcon* iconD = NULL;

    // Add the plugin to visible plugins array:
    iVisiblePlugins->AppendL( aPlugin );

    // 1) Read item descriptor from plugin:
    TRAPD( captionErr,
           aPlugin->GetCaptionL( firstLabel );
         )

    // If plugin caption fails, set caption to error string.
    if( captionErr != KErrNone )
        {
        _LIT( KGSParentPluginCaptionErrorTxt, "Error" );
        firstLabel.Append( KGSParentPluginCaptionErrorTxt );
        }
       else
        {
        __GSLOGSTRING2( "[CGSParentContainer] 0x%X::GetCaptionL Error: %d",
        aPlugin->Id().iUid, captionErr );
        }

    // 2) Add icons if listbox uses them:
    //    - AB-column icon
    //    - D-column icon
    switch( iListBoxType )
        {
        case EGSListBoxTypeSingleLarge:
        case EGSListBoxTypeDouble2Large:
        case EGSListBoxTypeDoubleLarge:
            {
            // This might fail because plugin implementors might do funny stuff.
            // GS Must still behave well...
            TRAPD( iconErr,
                   iconAB = aPlugin->CreateIconL( KGSIconTypeLbxItem );
                 )
            if( iconErr != KErrNone )
                {
                // Use default icon because plugin implementor's CreateIconL
                // failed. If this fails, everything is lost anyway so might as
                // well leave (abort GS).
                iconAB = aPlugin->CGSPluginInterface::CreateIconL( KGSIconTypeLbxItem );
                __GSLOGSTRING2( "[CGSParentContainer] CreateIconL for 0x%X failed:%d",
                                aPlugin->Id().iUid,
                                iconErr );
                }
            if( iconAB ) // Append only if not NULL (not supported)
                {
                CleanupStack::PushL( iconAB );
                aIconArray->AppendL( iconAB );
                CleanupStack::Pop( iconAB );
                iconIndexAB = aIconCounter;
                aIconCounter++;
                }

            TRAPD( iconDColErr,
                   iconD = aPlugin->CreateIconL( KGSIconTypeDColumn );
                 )
            if( iconDColErr != KErrNone )
                {
                iconD = aPlugin->CGSPluginInterface::CreateIconL( KGSIconTypeDColumn );
                __GSLOGSTRING2( "[CGSParentContainer] CreateIconL for 0x%X failed:%d",
                                aPlugin->Id().iUid,
                                iconErr );
                }
            if( iconD ) // Append only if not NULL (not supported)
                {
                CleanupStack::PushL( iconD );
                aIconArray->AppendL( iconD );
                CleanupStack::Pop( iconD );
                iconIndexD = aIconCounter;
                aIconCounter++;
                }
            break;
            }
        case EGSListBoxTypeSettings:
            break;

        default:
            break;
        }

    /**
    * 3) Create formatted item string. Lbx item's format string depends
    *    on lbx type.
    */
    switch( iListBoxType )
        {
        /**
        * list_single_large_pane:
        * list item string format: "1\tTextLabel\t0\t0"
        * where 0 is an index to icon array
        */
        case EGSListBoxTypeSingleLarge:
            {
            if( iconIndexAB != KIconNotSupported ) lbxItemString.AppendNum( iconIndexAB ); // AB-Column icon
            lbxItemString.Append( KTab );
            lbxItemString.Append( firstLabel );
            break;
            }
        /**
        * list_setting_pane and list_big_single_setting_pane:
        * list item string format: "\tFirstLabel\t\tValueText"
        * list item string format: "\tFirstLabel\t0\t"
        * list item string format: "\tFirstLabel\t\tValueText\t*"
        * list item string format: "\tFirstLabel\t\t\t\tSecondLabel"
        * where 0 is an index to icon array
        */
        case EGSListBoxTypeSettings:
            {
            aPlugin->GetValue( EGSPluginKeySettingsItemValueString, secondLabel );

            lbxItemString.Append( KTab );
            lbxItemString.Append( firstLabel );
            lbxItemString.Append( KTab );
            lbxItemString.Append( KTab );
            lbxItemString.Append( secondLabel );
            break;
            }
        /**
        * list_double2_large_graphic_pane:
        * list item string format: "1\tFirstLabel\tSecondLabel\t0"
        * where 0 is an index of small (13x13) icon in icon array
        *   and 1 is an index of a thumbnail image
        */
        case EGSListBoxTypeDouble2Large:
        case EGSListBoxTypeDoubleLarge:
            {
            aPlugin->GetValue( EGSPluginKeySettingsItemValueString, secondLabel );

            if( iconIndexAB != KIconNotSupported ) lbxItemString.AppendNum( iconIndexAB ); // AB-Column icon
            lbxItemString.Append( KTab );
            lbxItemString.Append( firstLabel ); // FirstLabel
            lbxItemString.Append( KTab );
            lbxItemString.Append( secondLabel ); // SecondLabel
            lbxItemString.Append( KTab );
            if( iconIndexD != KIconNotSupported ) lbxItemString.AppendNum( iconIndexD ); // D-Column Icon
            break;
            }
        }
    aItemTextArray->AppendL( lbxItemString );

    CleanupStack::PopAndDestroy( lbxItemStringBuf );
    CleanupStack::PopAndDestroy( secondLabelBuf );
    CleanupStack::PopAndDestroy( firstLabelBuf );
    }


// ---------------------------------------------------------------------------
// CGSParentContainer::ListBox()
//
//
// ---------------------------------------------------------------------------
//
CEikListBox* CGSParentContainer::ListBox()
    {
    return iListBox;
    }


// ---------------------------------------------------------------------------
// CGSParentContainer::SizeChanged
// called by framework when the view size is changed
//
// ---------------------------------------------------------------------------
//
void CGSParentContainer::SizeChanged()
    {
    if ( iListBox )
        {
        iListBox->SetRect( Rect() );
        }
    }


// ---------------------------------------------------------------------------
// CGSParentContainer::CountComponentControls
//
//
// ---------------------------------------------------------------------------
//
TInt CGSParentContainer::CountComponentControls() const
    {
    return 1;
    }


// ---------------------------------------------------------------------------
// CGSParentContainer::ComponentControl
//
//
// ---------------------------------------------------------------------------
//
CCoeControl* CGSParentContainer::ComponentControl(TInt /*aIndex*/) const
    {
    return iListBox;
    }


// ---------------------------------------------------------------------------
// CGSParentContainer::TKeyResponse OfferKeyEventL
// Called when a key is pressed.
//
// ---------------------------------------------------------------------------
//
TKeyResponse  CGSParentContainer::OfferKeyEventL( const TKeyEvent& aKeyEvent,
                                                  TEventCode aType )
    {
    switch ( aKeyEvent.iCode )
        {
        case EKeyUpArrow:
        case EKeyDownArrow:
            {
            TKeyResponse listboxResp =
                iListBox->OfferKeyEventL( aKeyEvent, aType );
            if ( iParentPlugin )
                {
                iParentPlugin->CheckMiddleSoftkeyLabelL();
                }
            return listboxResp;
            }
        case EKeyLeftArrow:
        case EKeyRightArrow:
            // Listbox takes all events even if it doesn't use them
            return EKeyWasNotConsumed;
        default:
            break;
        }
    // now it's iListBox's job to process the key event
    return iListBox->OfferKeyEventL( aKeyEvent, aType );
    }


// ---------------------------------------------------------------------------
// CGSParentContainer::HandleListBoxEventL
//
//
// ---------------------------------------------------------------------------
void CGSParentContainer::HandleListBoxEventL( CEikListBox* aListBox,
                                              TListBoxEvent aEventType )
    {
    __GSLOGSTRING1(
        "[CGSParentContainer] HandleListBoxEventL aEventType:%d",
        aEventType);
    switch (aEventType)
        {
        case EEventEnterKeyPressed:
        case EEventItemSingleClicked:
            {
            __GSLOGSTRING1(
                "[CGSParentContainer] Activating view plugin in index[%d]",
                aListBox->CurrentItemIndex() );
            
            if( aListBox->CurrentItemIndex() < 0 )
                {
                // Negative listbox value -> no item selected? 
                break;
                }
            
           if(iExt->iDblClickPreventer->IsActive())
               {
               break;
               }
           iExt->iDblClickPreventer->Start();
            CGSPluginInterface* selectedPlugin = iVisiblePlugins->operator[](
                aListBox->CurrentItemIndex() );

            // Different command is used depending on the plugin type.
            switch( selectedPlugin->ItemType() )
                {
                // In these cases the plugin is a view:
                case EGSItemTypeSingleLarge:
                case EGSItemTypeSetting:
                case EGSItemTypeSettingIcon:
                    iAppUi->ActivateLocalViewL( selectedPlugin->Id() );
                    break;
                // In these cases the plugin is a dialog:
                case EGSItemTypeSettingDialog:
                case EGSItemTypeSingleLargeDialog:
                    selectedPlugin->HandleSelection(
                        EGSSelectionBySelectionKey );
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
// CGSParentContainer::HandleResourceChange()
//
//
// ---------------------------------------------------------------------------
//
void CGSParentContainer::HandleResourceChange( TInt aType )
    {
    CCoeControl::HandleResourceChange( aType );
    //Handle layout orientation or skin change
    if ( aType == KAknsMessageSkinChange ||
         aType == KEikDynamicLayoutVariantSwitch )
        {
        TRect mainPaneRect;
        AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::EMainPane,
                                           mainPaneRect);
        SetRect( mainPaneRect );
        if ( aType == KAknsMessageSkinChange )
            {
            TRAP_IGNORE( UpdateListBoxL() );
            }
        }
    }


// ---------------------------------------------------------------------------
// CGSParentContainer::CreateListBoxL()
//
//
// ---------------------------------------------------------------------------
//
void CGSParentContainer::CreateListBoxL(
    TGSListboxTypes aListBoxType )
    {
    switch( aListBoxType )
        {
        case EGSListBoxTypeSingleLarge:
            {
            CAknSingleLargeStyleListBox* lbx =
                new( ELeave ) CAknSingleLargeStyleListBox;
            iListBox = lbx;
            lbx->ConstructL( this );
            lbx->SetContainerWindowL( *this );
            lbx->SetListBoxObserver( this );
            lbx->CreateScrollBarFrameL( ETrue );
            lbx->ScrollBarFrame()->SetScrollBarVisibilityL(
                CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto );
            // Obtain reference to listbox model's item text array:
            iItemTextArray = STATIC_CAST( CDesCArray*,
                lbx->Model()->ItemTextArray() );
            }
            break;

        case EGSListBoxTypeSettings:
            {
            CAknSettingStyleListBox* lbx = 
                new( ELeave ) CAknSettingStyleListBox;
            iListBox = lbx;
            lbx->ConstructL( this, EAknListBoxSelectionList );
            lbx->SetContainerWindowL( *this );
            lbx->SetListBoxObserver( this );
            lbx->CreateScrollBarFrameL( ETrue );
            lbx->ScrollBarFrame()->SetScrollBarVisibilityL(
                CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto );
            // Obtain reference to listbox model's item text array:
            iItemTextArray = STATIC_CAST( CDesCArray*,
                lbx->Model()->ItemTextArray() );
            }
            break;
            
        case EGSListBoxTypeDouble2Large:
            {
            TInt flags = 0;
            CAknDouble2LargeStyleListBox* lbx = 
                new( ELeave ) CAknDouble2LargeStyleListBox;
            iListBox = lbx;
            lbx->ConstructL( this, flags );
            lbx->SetContainerWindowL( *this );
            lbx->SetListBoxObserver( this );
            lbx->CreateScrollBarFrameL( ETrue );
            lbx->ScrollBarFrame()->SetScrollBarVisibilityL(
                CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto );
            // Obtain reference to listbox model's item text array:
            iItemTextArray = STATIC_CAST( CDesCArray*,
                lbx->Model()->ItemTextArray() );
            }
            break;

        case EGSListBoxTypeDoubleLarge:
            {
            TInt flags = 0;
            CAknDoubleLargeStyleListBox* lbx = 
                new( ELeave ) CAknDoubleLargeStyleListBox;
            iListBox = lbx;
            lbx->ConstructL( this, flags );
            lbx->SetContainerWindowL( *this );
            lbx->SetListBoxObserver( this );
            lbx->CreateScrollBarFrameL( ETrue );
            lbx->ScrollBarFrame()->SetScrollBarVisibilityL(
                CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto );
            // Obtain reference to listbox model's item text array:
            iItemTextArray = STATIC_CAST( CDesCArray*,
                lbx->Model()->ItemTextArray() );
            }
            break;

        default:
            break;
        }
    }


// -----------------------------------------------------------------------------
// CGSParentContainer::SelectedPlugin()
//
//
// -----------------------------------------------------------------------------
//
CGSPluginInterface* CGSParentContainer::SelectedPlugin()
    {
    CGSPluginInterface* plugin = NULL;
    const TInt index = iListBox->CurrentItemIndex();
    if ( index >= 0 && index < iVisiblePlugins->Count() )
        {
        plugin = iVisiblePlugins->operator[]( index );
        __GSLOGSTRING2( "[CGSParentContainer] SelectedPlugin 0x%X in index %d",
            plugin->Id().iUid, index );
        }
    return plugin;
    }


// -----------------------------------------------------------------------------
// CGSParentContainer::SetSelectedItem()
//
//
// -----------------------------------------------------------------------------
//
void CGSParentContainer::SetSelectedItem( TUid aSelectedItemUid )
    {
    __GSLOGSTRING1( "[CGSParentContainer] SetSelectedItem(0x%X)",
        aSelectedItemUid.iUid );

    if( aSelectedItemUid != KGSNoneSelected )
        {
        TInt selectedItemIndex  = 0;
        for( TInt i = 0; i < iVisiblePlugins->Count(); i++ )
            {
            CGSPluginInterface* plugin = iVisiblePlugins->operator[]( i );
            __GSLOGSTRING2( "[CGSParentContainer] Checking [%d] 0x%X)",
                i, plugin->Id().iUid );
            // If plugin is found, select it:
            if( plugin->Id() == aSelectedItemUid )
                {
                __GSLOGSTRING2( "[CGSParentContainer] Match 0x%X in index %d",
                    plugin->Id().iUid, i );
                selectedItemIndex = i;
                break;
                }
            }
        iListBox->SetCurrentItemIndexAndDraw( selectedItemIndex );
        }
    }


// -----------------------------------------------------------------------------
// CGSParentContainer::GetHelpContext()
//
//
// -----------------------------------------------------------------------------
//
void CGSParentContainer::GetHelpContext( TCoeHelpContext& aContext ) const
    {
    // This is forwarded to the CGSParentPlugin type class that actually knows
    // the help context but does not have its own CCoeControl class.
    iParentPlugin->GetHelpContext( aContext );
    }


// ---------------------------------------------------------------------------
// CGSParentContainer::FocusChanged
//
// Set focus on the selected listbox. For animated skins feature.
// ---------------------------------------------------------------------------
EXPORT_C void CGSParentContainer::FocusChanged( TDrawNow /*aDrawNow*/ )
    {
    if( iListBox )
        {
        iListBox->SetFocus( IsFocused() );
        }
    }


// ---------------------------------------------------------------------------
// CGSParentContainer::HandleSelectionKeyL
//
// Behaves like a selection key.
// ---------------------------------------------------------------------------
void CGSParentContainer::HandleSelectionKeyL()
    {
    HandleListBoxEventL( iListBox,EEventEnterKeyPressed );
    }


// -----------------------------------------------------------------------------
// CGSParentContainer::TopPlugin()
//
//
// -----------------------------------------------------------------------------
//
CGSPluginInterface* CGSParentContainer::TopPlugin()
    {
    CGSPluginInterface* plugin = NULL;
    const TInt index = iListBox->TopItemIndex();
    if ( index >= 0 && index < iVisiblePlugins->Count() )
        {
        plugin = iVisiblePlugins->operator[]( index );
        __GSLOGSTRING2( "[CGSParentContainer] TopPlugin 0x%X in index %d",
            plugin->Id().iUid, index );
        }
    return plugin;
    }


// -----------------------------------------------------------------------------
// CGSParentContainer::SetSelectedItem()
//
//
// -----------------------------------------------------------------------------
//
void CGSParentContainer::SetTopItem( TUid aTopItemUid )
    {
    __GSLOGSTRING1( "[CGSParentContainer] SetTopItem(0x%X)",
        aTopItemUid.iUid );

    if( aTopItemUid != KGSNoneSelected )
        {
        TInt topItemIndex  = 0;
        for( TInt i = 0; i < iVisiblePlugins->Count(); i++ )
            {
            CGSPluginInterface* plugin = iVisiblePlugins->operator[]( i );
            __GSLOGSTRING2( "[CGSParentContainer] Checking [%d] 0x%X)",
                i, plugin->Id().iUid );
            // If plugin is found, select it:
            if( plugin->Id() == aTopItemUid )
                {
                __GSLOGSTRING2( "[CGSParentContainer] Match 0x%X in index %d",
                    plugin->Id().iUid, i );
                topItemIndex = i;
                break;
                }
            }
        iListBox->SetTopItemIndex( topItemIndex );
        }
    }

// -----------------------------------------------------------------------------
// CGSParentContainer::SetListBoxEmptyTextL()
//
//Set the empty text of list box.
// -----------------------------------------------------------------------------
//
void CGSParentContainer::SetListBoxEmptyTextL(const TDes& aEmpty )
    {
    if ( iListBox )
        {
        iListBox->View()->SetListEmptyTextL( aEmpty );
        }
    }

// -----------------------------------------------------------------------------
// CGSParentContainer::StoreListBoxPositionL
//
//Store the exact position of listbox.
// -----------------------------------------------------------------------------
//
void CGSParentContainer::StoreListBoxPositionL( CGSParentPlugin::TListBoxPosition& aPosition )
	{
    aPosition.iCurrentItemIndex = iListBox->CurrentItemIndex();
    aPosition.iItemOffsetInPixels = iListBox->View()->ItemOffsetInPixels();
    aPosition.iTopItemIndex = iListBox->View()->TopItemIndex();
	}

// -----------------------------------------------------------------------------
// CGSParentContainer::RestoreListBoxPositionL
//
//Restore the exact position of listbox.
// -----------------------------------------------------------------------------
//
void CGSParentContainer::RestoreListBoxPositionL( const CGSParentPlugin::TListBoxPosition& aPosition, TBool aScreenModeChanged )
	{
	if( iListBox->View()->BottomItemIndex()>= aPosition.iCurrentItemIndex )
		
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
    }
//End of File
