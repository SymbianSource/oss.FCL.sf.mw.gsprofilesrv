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
* Description:  Main View.
*
*/


// INCLUDE FILES
#include    <gsmainview.h>
#include    "GSMainContainer.h"
#include    "GsLogger.h"
#include    "gsplugininterface.h"
#include    "GSDocument.h"
#include    "GSUi.h"
#include    <gsapp.rsg>
#include    <gsfwviewuids.h>

#include    <aknlists.h>
#include    <ConeResLoader.h>
#include    <featmgr.h>
#include    <akntitle.h>
#include    <bautils.h> // Localization
#include    <hlplch.h>
#include    <gscommon.hrh>
#include    <gfxtranseffect/gfxtranseffect.h>
#include	  <layoutmetadata.cdl.h>

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSMainView::CGSMainView
// C++ constructor.
//
// ---------------------------------------------------------------------------
//
CGSMainView::CGSMainView()
    : iResourceLoader( *iCoeEnv )
    {
    }


// ---------------------------------------------------------------------------
// CGSMainView::NewL()
// Symbian OS two-phased constructor.
//
// ---------------------------------------------------------------------------
//
CGSMainView* CGSMainView::NewL()
    {
    CGSMainView* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CGSMainView::NewLC()
// Symbian OS two-phased constructor.
//
// ---------------------------------------------------------------------------
//
CGSMainView* CGSMainView::NewLC()
    {
    CGSMainView* self = new( ELeave ) CGSMainView();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CGSMainView::ConstructL(const TRect& aRect)
// Symbian OS default constuctor.
//
// ---------------------------------------------------------------------------
//
void CGSMainView::ConstructL()
    {
    __GSLOGSTRING( "[CGSMainView] ConstructL()" );
    iAppUi = AppUi();

    OpenLocalizedResourceFileL( KGSMainViewResourceFileName );
    BaseConstructL( R_GS_MAIN_VIEW );

    iPluginArray = new CArrayPtrFlat<CGSPluginInterface>( 10 );

    iPluginLoader = CGSPluginLoader::NewL( iAppUi );
    iPluginLoader->SetObserver( this );
    iPluginLoader->LoadAsyncL( KGSPluginInterfaceUid, KGSMainViewUid,
        iPluginArray );
    iPosition.Reset();
    }


// ---------------------------------------------------------------------------
// CGSMainView::~CGSMainView()
// Destructor
//
// ---------------------------------------------------------------------------
//
CGSMainView::~CGSMainView()
    {
    __GSLOGSTRING( "[CGSMainView] ~CGSMainView" );
    if ( iPluginLoader )
        {
        delete iPluginLoader;
        }

    if ( iPluginArray )
        {
        // Since the plugins are actually avkon views, avkon is responsible
        // for owning the plugins. This means we do not reset and destroy
        // the contents of the array in which the plugins reside. We have to
        // leave it up to avkon to tidy up.
        delete iPluginArray;
        }

    iResourceLoader.Close();

    if( iContainer && iAppUi )
        {
        iAppUi->RemoveFromViewStack( *this, iContainer );
        delete iContainer;
        }
    }


// ---------------------------------------------------------------------------
// TUid CGSMainView::OpenLocalizedResourceFileL()
//
//
// ---------------------------------------------------------------------------
//
void CGSMainView::OpenLocalizedResourceFileL( const TDesC& aResourceFileName )
    {
    RFs &fsSession = CCoeEnv::Static()->FsSession();

    // Find the resource file
    TParse parse;
    parse.Set( aResourceFileName, &KDC_APP_RESOURCE_DIR, NULL );
    TFileName fileName( parse.FullName() );

    // Get language of resource file
    BaflUtils::NearestLanguageFile( fsSession, fileName );

    // Open resource file
    iResourceLoader.OpenL( fileName );
    }


// ---------------------------------------------------------------------------
// TUid CGSMainView::Id()
//
//
// ---------------------------------------------------------------------------
//
TUid CGSMainView::Id() const
    {
    return KGSMainViewUid;
    }


// ---------------------------------------------------------------------------
// CGSMainView::DoActivateL()
//
//
// ---------------------------------------------------------------------------
//
void CGSMainView::DoActivateL( const TVwsViewId& /*aPrevViewId*/,
                               TUid /*aCustomMessageId*/,
                               const TDesC8& /*aCustomMessage*/ )
    {
    __GSLOGSTRING( "[CGSMainView] DoActivateL()" );
    
    iPluginLoader->RequestPriority( CActive::EPriorityHigh );
    
    if( iContainer )
        {
        iAppUi->RemoveFromViewStack( *this, iContainer );
        delete iContainer;
        iContainer = NULL;
        }
    NewContainerL();
    
    // Set the empty text of list box
    _LIT( KEmptyText, "" );
    TBuf<1> empty( KEmptyText );
    iContainer->SetListBoxEmptyTextL( empty );
    
    // Do this to update listbox from already existing iPluginArray.
    iContainer->UpdateListBoxL();
    if ( iPosition.iCurrentItemIndex != -1 )
		{
		if ( iIsLandscapeOrientation == Layout_Meta_Data::IsLandscapeOrientation() )
			{
			iContainer->RestoreListBoxPositionL( iPosition, EFalse );
			}
		else
			{
			iContainer->RestoreListBoxPositionL( iPosition, ETrue );
			}
		}
    iAppUi->AddToViewStackL( *this, iContainer );
    // Navigating to main view will reset all child plugin selected indexes:
    for( TInt i = 0; i < iPluginArray->Count(); i++ )
        {
        CGSPluginInterface* plugin = iPluginArray->operator[]( i );
        plugin->ResetSelectedItemIndex();
        }
    GfxTransEffect::EndFullScreen();
    }


// ---------------------------------------------------------------------------
// CGSMainView::DoDeactivate()
//
//
// ---------------------------------------------------------------------------
//
void CGSMainView::DoDeactivate()
    {
    __GSLOGSTRING( "[CGSMainView] DoDeactivate()" );
    
    iPluginLoader->RequestPriority( CActive::EPriorityLow );
    
    if ( iContainer )
        {
		
		iPosition.Reset();
		TRAPD( err, iContainer->StoreListBoxPositionL( iPosition ) );
		iIsLandscapeOrientation = Layout_Meta_Data::IsLandscapeOrientation();
		iAppUi->RemoveFromViewStack(*this, iContainer);
        delete iContainer;
        iContainer = NULL;
        }
    }


// ---------------------------------------------------------------------------
// CGSMainView::NewContainerL()
//
//
// ---------------------------------------------------------------------------
//
void CGSMainView::NewContainerL()
    {
    iContainer = new( ELeave ) CGSMainContainer;
    iContainer->SetMopParent( this );

    TRAPD(
        error,
        iContainer->ConstructL( ClientRect() , AppUi(), iPluginArray ) );

    if ( error )
        {
        delete iContainer;
        iContainer = NULL;
        User::Leave( error );
        }
    }


// ---------------------------------------------------------------------------
// TUid CGSMainView::HandleCommandL()
//
//
// ---------------------------------------------------------------------------
//
void CGSMainView::HandleCommandL( TInt aCommand )
    {
    __GSLOGSTRING1( "[CGSMainView] HandleCommandL(%d)", aCommand );

    switch ( aCommand )
        {
        case EGSCmdAppOpen:
        case EAknSoftkeyOpen:
            if( iContainer )
                {
                CGSPluginInterface* plugin = iContainer->SelectedPlugin();
                if ( plugin )
                	{
                    switch (plugin->ItemType())
                        {
                        // In these cases the plugin is a view:
                        case EGSItemTypeSingleLarge:
                        case EGSItemTypeSetting:
                        case EGSItemTypeSettingIcon:
                            iAppUi->ActivateLocalViewL(plugin->Id());
                            break;
                            // In these cases the plugin is a dialog:
                        case EGSItemTypeSettingDialog:
                        case EGSItemTypeSingleLargeDialog:
                            plugin->HandleSelection(EGSSelectionByMenu);
                            break;
                        }
                	}
                }
            break;
        case EAknCmdHelp:
            {
            if( FeatureManager::FeatureSupported( KFeatureIdHelp ) )
                {
                HlpLauncher::LaunchHelpApplicationL(
                    iEikonEnv->WsSession(), iAppUi->AppHelpContextL() );
                }
            break;
            }
        default:
            iAppUi->HandleCommandL( aCommand );
            break;
        }
    }


// ---------------------------------------------------------------------------
// TUid CGSMainView::HandlePluginLoaded()
//
//
// ---------------------------------------------------------------------------
//
void CGSMainView::HandlePluginLoaded( KGSPluginLoaderStatus aStatus )
    {
    __GSLOGSTRING1( "[CGSMainView::HandlePluginLoaded] aStatus:%d", aStatus );
    
    switch( aStatus )
        {
        case MGSPluginLoadObserver::EGSSuccess:
            // Should not update each time when plugin is loaded, only when 
            // finished loading spesific view plugins?
            break;
        case MGSPluginLoadObserver::EGSFinished:
            if( iContainer )
                {
                TRAPD( err,
                       iContainer->UpdateListBoxL(); )
                if( err != KErrNone )
                    {
                    __GSLOGSTRING1(
                        "[CGSMainView] Error updating listbox: %d", 
                        err );
                    }
                }
            break;
        default:
            break;
        }

    }


// ---------------------------------------------------------------------------
// CGSMainView::TransferDynamicPluginL()
//
//
// ---------------------------------------------------------------------------
//
void CGSMainView::TransferDynamicPluginL( CGSPluginInterface* aPlugin )
    {
    __GSLOGSTRING1( "[CGSMainView] CGSMainView::TransferDynamicPluginL() - plugin id: 0x%x added to appUi.", aPlugin->Id() );

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


// -------------------------------------------------    --------------------------
// TUid CGSMainView::TabbedViews()
//
//
// ---------------------------------------------------------------------------
//
CArrayPtrFlat<CGSPluginInterface>* CGSMainView::TabbedViews()
    {
    return iPluginArray;
    }


// -----------------------------------------------------------------------------
// CGSMainView::TabChangedL()
//
//
// -----------------------------------------------------------------------------
//
void CGSMainView::TabChangedL( TUid selectedTabUid )
    {
    // Update selected item because changing tab in main view's children
    // affects also main view's selected item:
    iSelectedPluginUid = selectedTabUid;
    }


// -----------------------------------------------------------------------------
// CGSMainView::UpdateView()
//
//
// -----------------------------------------------------------------------------
//
void CGSMainView::UpdateView()
    {
    TRAP_IGNORE( iContainer->UpdateListBoxL() );
    }
    

// -----------------------------------------------------------------------------
// CGSMainView::DynInitMenuPaneL()
//
// dynamically handle help item if not supported
// -----------------------------------------------------------------------------
//
void CGSMainView::DynInitMenuPaneL( TInt aResourceId, 
                                    CEikMenuPane* aMenuPane )
    {   
    if( aResourceId == R_GS_MENU_ITEM_HELP )
        {
        User::LeaveIfNull( aMenuPane );
        
        if ( FeatureManager::FeatureSupported( KFeatureIdHelp ))
            {
            aMenuPane->SetItemDimmed( EAknCmdHelp, EFalse );
            }
        else
            {
            aMenuPane->SetItemDimmed( EAknCmdHelp, ETrue );
            }
        }
    }

//End of File
