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
* Description:  Base class for plugins containing other plugins. Asynchronous
*                 loading of plugins is used.
*
*/


// INCLUDE FILES
#include    <gsparentplugin.h>
#include    <gsparentcontainer.h>
#include    <gsplugininterface.h>
#include    "GsLogger.h"
#include    <gscommon.hrh>
#include    <GSParentPluginRsc.rsg>
#include    <gsfwviewuids.h>
#include    <avkon.rsg>

#include    <aknlists.h>
#include    <ConeResLoader.h>
#include    <featmgr.h>
#include    <akntitle.h>
#include    <aknViewAppUi.h>
#include    <bautils.h>
#include    <eikmenup.h>
#include    <eikbtgpc.h>
#include    <StringLoader.h>

// Middle Softkey control ID.
const TInt KGSMSKControlID = 3;
const TInt KGSMSKLength = 256;
const TInt KGSPluginArrayInitSize = 10;

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSParentPlugin::CGSParentPlugin()
//
//
// ---------------------------------------------------------------------------
//
EXPORT_C CGSParentPlugin::CGSParentPlugin()
    : iResourceLoader( *iCoeEnv )
    {
    }


// ---------------------------------------------------------------------------
// CGSParentPlugin::BaseConstructL()
//
//
// ---------------------------------------------------------------------------
//
EXPORT_C void CGSParentPlugin::BaseConstructL(
    TInt aViewRscId,
    TInt aTitleRscId )
    {
    iTitleRscId = aTitleRscId;
    iSelectedPluginUid = KGSNoneSelected;
    iTopPluginUid = KGSNoneSelected;

    __GSLOGSTRING( "[CGSParentPlugin] ConstructL()" );
    iAppUi = AppUi();

    OpenLocalizedResourceFileL( KGSParentPluginResourceFileName,
                                iResourceLoader );
    CAknView::BaseConstructL( aViewRscId );

    if( !iOptionFlags[ EGSLoadChildrenOnActivation ] )
        {
        StartAsyncPluginLoadL();
        }
    }

// ---------------------------------------------------------------------------
// CGSParentPlugin::StartAsyncPluginLoadL()
//
//
// ---------------------------------------------------------------------------
//
void CGSParentPlugin::StartAsyncPluginLoadL()
    {
    //Initialize array and start loading plugins into it.
    iPluginArray = new (ELeave) CArrayPtrFlat<CGSPluginInterface>( KGSPluginArrayInitSize );
    iPluginLoader = CGSPluginLoader::NewL( iAppUi );
    iPluginLoader->SetObserver( this );
    iPluginLoader->LoadAsyncL( KGSPluginInterfaceUid, Id(), iPluginArray );
    }

// ---------------------------------------------------------------------------
// CGSParentPlugin::UpperLevelViewUid()
//
//
// ---------------------------------------------------------------------------
//
EXPORT_C TUid CGSParentPlugin::UpperLevelViewUid()
    {
    __GSLOGSTRING( "[CGSParentPlugin] UpperLevelViewUid()" );
    return iPrevViewId.iViewUid;
    }


// ---------------------------------------------------------------------------
// CGSParentPlugin::~CGSParentPlugin()
//
//
// ---------------------------------------------------------------------------
//
EXPORT_C CGSParentPlugin::~CGSParentPlugin()
    {
    __GSLOGSTRING( "[CGSParentPlugin] ~CGSParentPlugin" );
    if  ( iPluginLoader )
        {
        iPluginLoader->AbortAsyncLoad();
        }
    delete iPluginLoader;

    if  ( iPluginArray )
        {
        iPluginArray->Reset();//Do not destroy - Plugins are owned by iAppUi
        }
    delete iPluginArray;
    iResourceLoader.Close();

    if( iContainer )
        {
        iAppUi->RemoveFromViewStack( *this, iContainer );
        delete iContainer;
        }
    }


// ---------------------------------------------------------------------------
// CGSParentPlugin::OpenLocalizedResourceFileL()
//
//
// ---------------------------------------------------------------------------
//
EXPORT_C void CGSParentPlugin::OpenLocalizedResourceFileL(
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


// ---------------------------------------------------------------------------
// CGSParentPlugin::DoActivateL()
//
//
// ---------------------------------------------------------------------------
//
EXPORT_C void CGSParentPlugin::DoActivateL( const TVwsViewId& aPrevViewId,
                                            TUid /*aCustomMessageId*/,
                                            const TDesC8& /*aCustomMessage*/ )
    {
    __GSLOGSTRING( "[CGSParentPlugin::DoActivateL]" );
    iPrevViewId = aPrevViewId;
    
    // If iPluginArray does not exist. Lazy loading is enabled and should start 
    // loading now.
    if( !iPluginArray )
        {
        StartAsyncPluginLoadL();
        }
    
    // Set priority only after plugin loader is created in 
    // StartAsyncPluginLoadL. 
    iPluginLoader->RequestPriority( CActive::EPriorityHigh );
    
    //if( iContainer )
    //    {
    //    iAppUi->RemoveFromViewStack( *this, iContainer );
    //    delete iContainer;
    //    iContainer=NULL;
    //    }
    
    if ( !iContainer )
        {
        ConstructContainerL();
    
        // Set the empty text of list box.
        _LIT( KEmptyText, "" );
        TBuf<1> empty( KEmptyText );
        iContainer->SetListBoxEmptyTextL( empty );
    
        // Update listbox from already existing iPluginArray:
        iContainer->UpdateListBoxL();
        iAppUi->AddToViewStackL( *this, iContainer );
        if( iTopPluginUid != KGSNoneSelected )
            {
            iContainer->SetTopItem( iTopPluginUid );
            }
        if( iSelectedPluginUid != KGSNoneSelected )
            {
            iContainer->SetSelectedItem( iSelectedPluginUid );
            }
        }

    // Navigating to parent view will reset all child plugin selected indexes:
    for( TInt i = 0; i < iPluginArray->Count(); i++ )
        {
        CGSPluginInterface* plugin = iPluginArray->operator[]( i );
        plugin->ResetSelectedItemIndex();
        }

    iMskCommandId = EAknSoftkeyOpen;

    SetMiddleSoftKeyLabelL( R_QTN_MSK_OPEN, EAknSoftkeyOpen );
    CheckMiddleSoftkeyLabelL();

	// If this view was launched from external source, use "exit" as RSK
	if ( iPrevViewId.iAppUid != KUidGS )
		{
		CEikButtonGroupContainer* cbaGroup = Cba();
		if(cbaGroup)
			{
			HBufC* rightSKText = StringLoader::LoadLC (R_GS_PARENTPLUGIN_CBA_EXIT);
			TPtr rskPtr = rightSKText->Des();
			cbaGroup->SetCommandL(2,EAknSoftkeyExit,*rightSKText);
			CleanupStack::PopAndDestroy(rightSKText);
			}
		}

	}

// ---------------------------------------------------------------------------
// CGSParentPlugin::DoDeactivate()
//
//
// ---------------------------------------------------------------------------
EXPORT_C void CGSParentPlugin::DoDeactivate()
    {
    __GSLOGSTRING( "[CGSParentPlugin::DoDeactivate]" );
    
    iPluginLoader->RequestPriority( CActive::EPriorityLow );
    
    if ( iContainer )
        {
        CGSPluginInterface* selectedPlugin = iContainer->SelectedPlugin();
        if( selectedPlugin )
            {
            iSelectedPluginUid = selectedPlugin->Id();
            }
        else
            {
            iSelectedPluginUid = KGSNoneSelected;
            }

        CGSPluginInterface* topPlugin = iContainer->TopPlugin();
        if ( topPlugin )
            {
            iTopPluginUid = topPlugin->Id();
            }
        else
            {
            iTopPluginUid = KGSNoneSelected;
            }
        iAppUi->RemoveFromViewStack( *this, iContainer );
        delete iContainer;
        iContainer = NULL;
        }
    __GSLOGSTRING( "[CGSParentPlugin::DoDeactivate] Done" );
    }


// ---------------------------------------------------------------------------
// CGSParentPlugin::ConstructContainerL()
//
//
// ---------------------------------------------------------------------------
//
void CGSParentPlugin::ConstructContainerL()
    {
    iContainer = new( ELeave ) CGSParentContainer;
    iContainer->SetMopParent( this );

    TRAPD
        (
        error,
        iContainer->ConstructL(
            ClientRect() ,
            AppUi(),
            iPluginArray,
            iTitleRscId,
            this,
            ListBoxType() )
        );

    if ( error )
        {
        delete iContainer;
        iContainer = NULL;
        User::Leave( error );
        }
    }


// ---------------------------------------------------------------------------
// CGSParentPlugin::HandleCommandL()
//
//
// ---------------------------------------------------------------------------
//
EXPORT_C void CGSParentPlugin::HandleCommandL( TInt aCommand )
    {
    __GSLOGSTRING1( "[CGSParentPlugin] HandleCommandL(%d)", aCommand );

    switch ( aCommand )
        {
        case EAknSoftkeyBack:
            __GSLOGSTRING1( "[CGSParentPlugin] Returning to view 0x%X",
                            UpperLevelViewUid().iUid );
            iAppUi->ActivateLocalViewL( UpperLevelViewUid() );
            break;
        // These all should be handled similarily:
        case EAknSoftkeyOpen:
        case EGSCmdAppChange:
        case EGSCmdAppOpen:
            if( iContainer && iPluginArray->Count() > 0 )
                {
                CGSPluginInterface* selectedPlugin =
                    iContainer->SelectedPlugin();

                if ( NULL != selectedPlugin )
                    {
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
                                EGSSelectionByMenu );
                             break;
                        }                    
                    }

                }
            break;
        case EGSMSKCmdAppChange:
            iContainer->HandleSelectionKeyL();
            break;
        case EAknCmdHelp:
            {
            break;
            }
        default:
            iAppUi->HandleCommandL( aCommand );
            break;
        }
    }


// ---------------------------------------------------------------------------
// CGSParentPlugin::HandlePluginLoaded()
//
//
// ---------------------------------------------------------------------------
//
EXPORT_C void CGSParentPlugin::HandlePluginLoaded( KGSPluginLoaderStatus aStatus )
    {
    __GSLOGSTRING1( "[CGSParentPlugin::HandlePluginLoaded] aStatus:%d", aStatus );
    
    switch( aStatus )
        {
        case MGSPluginLoadObserver::EGSSuccess:
            // Should not update each time when plugin is loaded, only when 
            // finished loading spesific view plugins?
            break;
        case MGSPluginLoadObserver::EGSFinished:
            if( iContainer )
                {
                TRAPD( ignore, iContainer->UpdateListBoxL(); );
                if( ignore != KErrNone )
                  {
                  __GSLOGSTRING1( 
                      "[CGSParentPlugin] HandlePluginLoaded error:%d",
                      ignore );
                  }
                }
            break;
        default:
            break;
        }
    }


// ---------------------------------------------------------------------------
// CGSParentPlugin::TransferDynamicPluginL()
//
//
// ---------------------------------------------------------------------------
//
EXPORT_C void CGSParentPlugin::TransferDynamicPluginL( CGSPluginInterface* aPlugin )
    {
    __GSLOGSTRING1( "[CGSParentPlugin] CGSParentPlugin::TransferDynamicPluginL() - plugin id: 0x%x added to appUi.", aPlugin->Id() );

    CleanupStack::PushL( aPlugin );
    iAppUi->AddViewL( aPlugin );
    CleanupStack::Pop( aPlugin );

    // Add to the overall plugin array for this parent plugin
    iPluginArray->AppendL( aPlugin );

    // Resort the plugins so that they are in order
    iPluginLoader->SortPluginsL( iPluginArray );

    // Update the listbox with the new information
    HandlePluginLoaded( MGSPluginLoadObserver::EGSSuccess );
    }


// ---------------------------------------------------------------------------
// CGSParentPlugin::ListBoxType()
// Default implementation. Overwrite if different type of listbox is needed.
//
// ---------------------------------------------------------------------------
//
EXPORT_C TGSListboxTypes CGSParentPlugin::ListBoxType()
    {
    return EGSListBoxTypeSingleLarge;
    }


// ---------------------------------------------------------------------------
// CGSParentPlugin::UpdateView()
//
//
// ---------------------------------------------------------------------------
//
EXPORT_C void CGSParentPlugin::UpdateView()
    {
    __GSLOGSTRING( "[CGSParentPlugin::UpdateView]" );
    TRAP_IGNORE
        (
        if( iContainer )
            {
            iContainer->UpdateListBoxL();
            }
        );
    }


// -----------------------------------------------------------------------------
// CGSParentPlugin::ResetSelectedItemIndex()
//
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CGSParentPlugin::ResetSelectedItemIndex()
    {
    iSelectedPluginUid = KGSNoneSelected;
    if( iContainer )
        {
        iContainer->ListBox()->SetCurrentItemIndex( 0 );
        }
    }


// -----------------------------------------------------------------------------
// CGSParentPlugin::SetOptionFlags()
//
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CGSParentPlugin::SetOptionFlags( TBitFlags& aOptionFlags )
    {
    iOptionFlags = aOptionFlags;
    }


// -----------------------------------------------------------------------------
// CGSParentPlugin::OptionFlags()
//
//
// -----------------------------------------------------------------------------
//
EXPORT_C const TBitFlags& CGSParentPlugin::OptionFlags() const
    {
    return iOptionFlags;
    }


// -----------------------------------------------------------------------------
// CGSParentPlugin::DynInitMenuPaneL()
//
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CGSParentPlugin::DynInitMenuPaneL( TInt /*aResourceId*/,
                                                 CEikMenuPane* aMenuPane )
    {
    // Check what is the plugin's desired menu item type for activating the
    // plugin. If it doesn't match to the item defined by parent plugin menu
    // resource, change the item dynamically.
    if ( iContainer->SelectedPlugin() )
        {
        CGSPluginInterface* selectedPlugin = iContainer->SelectedPlugin();
        TGSMenuActivationItems desiredItemType = selectedPlugin->MenuActivationItem();

        // Append new item after this position, delete item in this position.
        TInt removableItemPos;

        // MSK command should be identical to the desired activation command in
        // options menu. Posibilities are such as 'open' (also as default),
        // 'change' or custom text.
        // Note: Activation item in options menu is depending on the highlighted
        // child plugin's MenuActivationItem() function.
        switch( desiredItemType )
            {
            case EGSMenuActivationItemDefault:
            case EGSMenuActivationItemOpen:
                if( aMenuPane->MenuItemExists( EGSCmdAppChange, removableItemPos ) )
                    {
                    aMenuPane->AddMenuItemsL( R_GS_MENU_ITEM_OPEN, removableItemPos );
                    aMenuPane->DeleteBetweenMenuItems( removableItemPos, removableItemPos );
                    }
                break;

            case EGSMenuActivationItemChange:
                if( aMenuPane->MenuItemExists( EAknSoftkeyOpen, removableItemPos ) )
                    {
                    aMenuPane->AddMenuItemsL( R_GS_MENU_ITEM_CHANGE, removableItemPos );
                    aMenuPane->DeleteBetweenMenuItems( removableItemPos, removableItemPos );
                    }
                break;

            case EGSMenuActivationItemCustom:
                {
                // Using EGSCmdAppChange as EGSMenuActivationItemCustom
                // functionality is same as for EGSMenuActivationItemChange.
                // Here we're replacing EGSCmdAppChange always because we
                // cannot be sure whether it's the actual custom command
                // already or just EAknSoftkeyOpen. Custom commands will be set
                // to use the same command ID (EGSCmdAppChange).
                //
                CEikMenuPaneItem::SData menuItem;
                selectedPlugin->GetValue( EGSCustomMenuActivationText, menuItem.iText );
                menuItem.iCommandId = EGSCmdAppChange;
                menuItem.iCascadeId = 0;
                menuItem.iFlags = 0;
                
                if( aMenuPane->MenuItemExists( EGSCmdAppChange, removableItemPos ) )
                    {
                    // Add custom item before 'change' and then remove 'change'
                    if( menuItem.iText.Length() > 0)
                        {
                        aMenuPane->AddMenuItemL( menuItem, EGSCmdAppChange );
                        }
                    aMenuPane->DeleteBetweenMenuItems( removableItemPos, removableItemPos );
                    }
                else if( aMenuPane->MenuItemExists( EAknSoftkeyOpen, removableItemPos ) )
                    {
                    // Add custom item before 'open' and then remove 'open'
                    if( menuItem.iText.Length() > 0 )
                        {
                        aMenuPane->AddMenuItemL( menuItem, EAknSoftkeyOpen );
                        }
                    aMenuPane->DeleteBetweenMenuItems( removableItemPos, removableItemPos );
                    }
                break;
                }
            default:
                break;
            }
        }
    }


// -----------------------------------------------------------------------------
// CGSParentPlugin::GetHelpContext()
//
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CGSParentPlugin::GetHelpContext( TCoeHelpContext& /*aContext*/ )
    {
    // Default implementation does nothing.
    }

// -----------------------------------------------------------------------------
// When this method is called, view checks based on highlight focus, if the MSK
// label is correct.
// -----------------------------------------------------------------------------
//
void CGSParentPlugin::CheckMiddleSoftkeyLabelL()
    {
    if ( iContainer->SelectedPlugin() )
        {
        // Remove current MSK
        RemoveCommandFromMSK( iMskCommandId );

        CGSPluginInterface* selectedPlugin = iContainer->SelectedPlugin();
        TGSMenuActivationItems desiredItemType =
            selectedPlugin->MenuActivationItem();

        switch( desiredItemType )
            {
            case EGSMenuActivationItemChange:
                SetMiddleSoftKeyLabelL( R_QTN_MSK_CHANGE, EGSMSKCmdAppChange );
                iMskCommandId = EGSMSKCmdAppChange;
                break;
            case EGSMenuActivationItemOpen:
            case EGSMenuActivationItemDefault:
                SetMiddleSoftKeyLabelL( R_QTN_MSK_OPEN, EAknSoftkeyOpen );
                iMskCommandId = EAknSoftkeyOpen;
                break;
            case EGSMenuActivationItemCustom:
                {
                HBufC* mskText = HBufC::NewLC( KGSMSKLength );
                TPtr mskPtr = mskText->Des();
                selectedPlugin->GetValue( EGSCustomMenuActivationText, mskPtr );
                SetMiddleSoftKeyLabelTextL( mskPtr, EGSMSKCmdAppChange );
                CleanupStack::PopAndDestroy( mskText );
                break;
                }
            default:
                // Use 'open' as default
                SetMiddleSoftKeyLabelL( R_QTN_MSK_OPEN, EAknSoftkeyOpen );
                iMskCommandId = EAknSoftkeyOpen;
                break;
            }
        }
    }

// -----------------------------------------------------------------------------
// Remove unnecessary commands from Middle softkey.
// -----------------------------------------------------------------------------
//
void CGSParentPlugin::RemoveCommandFromMSK( TInt aMskCommandId )
    {
    CEikButtonGroupContainer* cbaGroup = Cba();
    if ( cbaGroup )
        {
        cbaGroup->RemoveCommandFromStack( KGSMSKControlID, aMskCommandId );
        }
    }

// ---------------------------------------------------------------------------
// Sets middle softkey label.
// ---------------------------------------------------------------------------
//
void CGSParentPlugin::SetMiddleSoftKeyLabelL(
    const TInt aResourceId, const TInt aCommandId )
    {
    CEikButtonGroupContainer* cbaGroup = Cba();
    if ( cbaGroup )
        {
        HBufC* middleSKText = StringLoader::LoadLC( aResourceId );
        TPtr mskPtr = middleSKText->Des();
        cbaGroup->AddCommandToStackL(
            KGSMSKControlID,
            aCommandId,
            mskPtr );
        CleanupStack::PopAndDestroy( middleSKText );
        }
    }

// ---------------------------------------------------------------------------
// Sets middle softkey label.
// ---------------------------------------------------------------------------
//
void CGSParentPlugin::SetMiddleSoftKeyLabelTextL(
    const TPtr aMskLabel, const TInt aCommandId )
    {
    CEikButtonGroupContainer* cbaGroup = Cba();
    if ( cbaGroup )
        {
        cbaGroup->AddCommandToStackL(
            KGSMSKControlID,
            aCommandId,
            aMskLabel );
        }
    }

// ---------------------------------------------------------------------------
// Returns selected plugin.
// ---------------------------------------------------------------------------
//
EXPORT_C CGSPluginInterface* CGSParentPlugin::SelectedPlugin()
    {
    CGSPluginInterface* selectedPlugin = NULL;
    if( iContainer )
        {
        selectedPlugin = iContainer->SelectedPlugin();
        }
    return selectedPlugin;
    }


void CGSParentPlugin::RequestPriority( CActive::TPriority aPriority )
    {
    iPluginLoader->RequestPriority( aPriority );
    __GSLOGSTRING2( "[CGSParentPlugin::RequestPriority] 0x%X aPriority:%d",  Id().iUid, aPriority ); 
    }

// End of File
