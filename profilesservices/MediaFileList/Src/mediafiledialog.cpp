/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Class of Mediafiledialog. UI control of mediafilelist
*
*/




// INCLUDE FILES

#include "mediafiledialog.h"
#include "mediafiledialogutils.h"
#include "mediafilelist.h"
#include "mediafilehandler.h"
#include "mediafilelistdebug.h"
#include "mediafileprotection.h"
#include "mediafilevideopreviewdialog.h"
#include "mediafiletest.h"  // test code

#include <eikclbd.h>        // for CColumnListBoxData
#include <coecobs.h>        // MCoeControlObserver
#include <AknUtils.h>       // AknTextUtils AknFind
#include <AknIconArray.h>   // for GulArray
#include <AknWaitDialog.h>
#include <textresolver.h>
#include <StringLoader.h>
#include <akninputblock.h>      // CAknInputBlock
#include <mmferrors.h> 
// search field
#include <aknselectionlist.h>  // AknSelectionList.cpp
#include <aknsfld.h>           // CAknSearchField
#include <eikcapc.h>         // CEikCaptionedControl

#include <aknconsts.h>         // KAvkonBitmapFile, KAvkonVariatedBitmapsFile
#include <AknsConstants.h>     // icon constants
#include <aknmemorycardui.mbg> // memory card icon
#include <avkon.mbg>

#include <avkon.loc>
#include <mediafilelist.rsg>
#include <mediafilelist.mbg>
#include <data_caging_path_literals.hrh> // KDC_APP_RESOURCE_DIR





/******************************************************************************
 * class CMediaFileDialog
 ******************************************************************************/



// -----------------------------------------------------------------------------
// CMediaFileDialog::NewL
//
// -----------------------------------------------------------------------------
CMediaFileDialog* CMediaFileDialog::NewL()
    {
    FLOG( _L("CMediaFileDialog::NewL START") );

    CMediaFileDialog* self = new (ELeave) CMediaFileDialog();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    FLOG( _L("CMediaFileDialog::NewL END") );
    return self;
    }


// -----------------------------------------------------------------------------
// CMediaFileDialog::CMediaFileDialog
// 
// -----------------------------------------------------------------------------
//
CMediaFileDialog::CMediaFileDialog()
	{
	FLOG( _L("CMediaFileDialog::CMediaFileDialog START") );
    
	iResourceFileId = KErrNotFound;
    iForeGround = ETrue;
    iDatabaseOpen = EFalse;
    iPeninputActive = EFalse;
    
    iNullItemFileTypeOne = KErrNotFound;
    iNullItemFileTypeTwo = KErrNotFound;
    iDialogTitle = KNullDesC;
    iIsDoubleClicked = EFalse;
   
    FLOG( _L("CMediaFileDialog::CMediaFileDialog END") );
    }


// ----------------------------------------------------------------------------
// Destructor
//
// ----------------------------------------------------------------------------
//
CMediaFileDialog::~CMediaFileDialog()
    {
    FLOG( _L("CMediaFileDialog::~CMediaFileDialog START") );

    if ( iResourceFileId != KErrNotFound )
        {
        CEikonEnv::Static()->DeleteResourceFile( iResourceFileId );
        }
 	
    delete iStatusPaneHandler;
 	delete iMFListHandler;
 	delete iMediaFileHandler;
 	delete iState;
	
    delete iFindCtrl;
    delete iCaller;
    delete iSearchCaller;
    
    delete iAudioPreviewHandler;
    delete iVideoPreviewHandler;
    
    delete iNullItemFileNameOne;
    delete iNullItemFileNameTwo;
    
    if ( iAvkonAppUi )
    	{
    	iAvkonAppUi->RemoveFromStack(this);
    	}
    	
    CCoeEnv::Static()->RemoveForegroundObserver( *this );
    
    if ( iPopupNote )
        {
        iPopupNote->HideInfoPopupNote();
        delete iPopupNote;
        }
    
    delete iWaitNote;
    if ( AknLayoutUtils::PenEnabled() )
        {
        iPeninputServer.RemovePenUiActivationHandler();
        iPeninputServer.Close();
        }
    
    delete iIdle;
    
    delete iProtectionHandler;
    
    delete iInputBlock;
   
#ifdef _DEBUG    
    delete iTest;  // test code
#endif
	
    FLOG( _L("CMediaFileDialog::~CMediaFileDialog END") );
    }


// -----------------------------------------------------------------------------
// CMediaFileDialog::ConstructL
//
// -----------------------------------------------------------------------------
//
void CMediaFileDialog::ConstructL()
    {
    FLOG( _L("CMediaFileDialog::ConstructL START") );
	
    iResourceFileId = TMFDialogUtil::AddResFileL();
    
    CCoeEnv* coeEnv = CCoeEnv::Static();
    coeEnv->AddForegroundObserverL( *this );

	CAknDialog::ConstructL( R_MEDIA_FILE_DIALOG_MENU );
	
    if ( iMenuBar )
        {
    	iMenuBar->SetContextMenuTitleResourceId(
		R_MEDIA_FILE_DIALOG_CONTEXT_MENU );
	    }
	
	// get previous title so it can be restored
	iStatusPaneHandler = CStatusPaneHandler::NewL( iAvkonAppUi );
	iStatusPaneHandler->StoreOriginalTitleL();
	
	iMediaFileHandler = CMediaFileHandler::NewL();
	iMediaFileHandler->SetObserver( this );
	
    iState = CMediaFileDialogState::NewL();
    
#ifndef RD_VIDEO_AS_RINGING_TONE    
    iState->HideRootFolder( EEFolderVideos, ETrue,
                            KNullDesC, KErrNotFound );
    
    iMediaFileHandler->SetAttrL( CMediaFileList::EAttrExcludeFolder, CMediaFileList::EFolderVideos );
#endif
 
    iMFListHandler = CMFListHandler::NewL();
    iMFListHandler->SetObserver( this );
    
    iCaller = CMFActiveCaller::NewL( this );
    iSearchCaller = CMFActiveCaller::NewL( this );
    
    iAudioPreviewHandler = CMFAudioPreviewHandler::NewL();
    iVideoPreviewHandler = CMFVideoPreviewHandler::NewL();
    
    iAudioPreviewHandler->SetObserver( this );
    
    iPopupNote = CAknInfoPopupNoteController::NewL();
    
    iWaitNote = CMediaFileWaitNote::NewL( this );
    
    if ( AknLayoutUtils::PenEnabled() )
        { 
        User::LeaveIfError( iPeninputServer.Connect() ); 
        iPeninputServer.AddPenUiActivationHandler( this, EPluginInputModeAll );
        }
    
    iProtectionHandler = CMFProtectionHandler::NewL();
    
#ifdef _DEBUG    
    iTest = CMFDialogTest::NewL( this );  // test code
#endif
   
	FLOG( _L("CMediaFileDialog::ConstructL END") );
    }


// -----------------------------------------------------------------------------
// CMediaFileDialog::ShowDialogL
// 
// -----------------------------------------------------------------------------
TBool CMediaFileDialog::ShowDialogL( TDes* aFileName, TInt* aNullItem,
                                     TInt* aInfo, TDes* aDesInfo )
	{
	__ASSERT_ALWAYS( aFileName, Panic( KErrGeneral ) );
	__ASSERT_ALWAYS( aNullItem, Panic( KErrGeneral ) );
	
	iFileName = aFileName;
	
	iNullItem = aNullItem;
	*iNullItem = KErrNotFound;
	
	iInfo = aInfo;
	iDesInfo = aDesInfo;
	
	TBool ret = ExecuteLD( R_MEDIA_FILE_DIALOG );
	
	return ret;
	}


// -----------------------------------------------------------------------------
// CMediaFileDialog::SetAttrL
// 
// -----------------------------------------------------------------------------
void CMediaFileDialog::SetAttrL( TInt aAttr, TInt aValue )
	{
	iAudioPreviewHandler->SetAttrL( aAttr, aValue );
	iVideoPreviewHandler->SetAttrL( aAttr, aValue );
	iMediaFileHandler->SetAttrL( aAttr, aValue );
	iProtectionHandler->SetAttrL( aAttr, aValue );

    if ( aAttr == CMediaFileList::EAttrExcludeFolder )
        {
        if ( aValue  == CMediaFileList::EFolderDownload )
            {
            iState->HideRootFolder( EEFolderDownload, ETrue,
                                    KNullDesC, KErrNotFound );
            }
        if ( aValue  == CMediaFileList::EFolderVideos )
            {
            iState->HideRootFolder( EEFolderVideos, ETrue,
                                    KNullDesC, KErrNotFound );
            }
        }
	}


// -----------------------------------------------------------------------------
// CMediaFileDialog::SetAttrL
// 
// -----------------------------------------------------------------------------
void CMediaFileDialog::SetAttrL( TInt aAttr, const TDesC& aValue )
	{
	iAudioPreviewHandler->SetAttrL( aAttr, aValue );
	iVideoPreviewHandler->SetAttrL( aAttr, aValue );
	iMediaFileHandler->SetAttrL( aAttr, aValue );
    iProtectionHandler->SetAttrL( aAttr, aValue );
	
	if ( aAttr == CMediaFileList::EAttrTitle )
	    {
	    TMFDialogUtil::StrCopy( iDialogTitle, aValue );
	    }
	}


// -----------------------------------------------------------------------------
// CMediaFileDialog::SetAttrL
// 
// -----------------------------------------------------------------------------
void CMediaFileDialog::SetAttrL( TInt aAttr, TAny* aValue )
    {
    iAudioPreviewHandler->SetAttrL( aAttr, aValue );
    iVideoPreviewHandler->SetAttrL( aAttr, aValue );
    iMediaFileHandler->SetAttrL( aAttr, aValue );
    }


// -----------------------------------------------------------------------------
// CMediaFileDialog::SetNullItemL
//
// -----------------------------------------------------------------------------
//
void CMediaFileDialog::SetNullItemL( const TDesC& aText, 
                                     const TDesC& aMediaFile,
                                     TInt aMediaFileType, TInt aIconId )
    {
    TInt iconId = EEIconEmpty;
    if ( aIconId == CMediaFileList::ENullItemIconDefaultTone )
        {
        iconId = EEIconTone;
        }
    else if ( aIconId == CMediaFileList::ENullItemIconOff )
        {
        iconId = EEIconNullItemOff;
        }
        
    TInt mediaType = CMediaFileList::EMediaFileTypeAudio;
    if ( aMediaFileType == CMediaFileList::EMediaFileTypeVideo )
        {
        mediaType = CMediaFileList::EMediaFileTypeVideo;
        }
        
        
    if ( !iNullItemFileNameOne )
        {
        iNullItemFileNameOne = aMediaFile.AllocL();
        iNullItemFileTypeOne = mediaType;
        
        iState->EnableNullItemL( EEFolderNullItemOne, aText, iconId );
        }
    else if ( !iNullItemFileNameTwo )
        {
        iNullItemFileNameTwo = aMediaFile.AllocL();
        iNullItemFileTypeTwo = mediaType;
        
        iState->EnableNullItemL( EEFolderNullItemTwo, aText, iconId );
        }
    else
        {
        User::Leave( KErrNotSupported ); // two null items supported
        }
    }



//------------------------------------------------------------------------------
// CMediaFileDialog::ActivateL
//
// Called by system when dialog is activated.
//------------------------------------------------------------------------------
//
void CMediaFileDialog::ActivateL()
	{
    CAknDialog::ActivateL();

	// this cannot be in ConstructL which is executed before dialog is launched
	iAvkonAppUi->AddToStackL( this );
	AknFind::HandleFixedFindSizeChanged( this, iListBox, FindBox() );
	DrawNow();
    }


// -----------------------------------------------------------------------------
// CMediaFileDialog::PreLayoutDynInitL
// 
// -----------------------------------------------------------------------------
//
void CMediaFileDialog::PreLayoutDynInitL()
    {
    const TInt KFindBoxLength = 20;
    
    iListBox = (CAknSingleGraphicStyleListBox*) (ControlOrNull(1));
    
   	__ASSERT_ALWAYS( iListBox, TMFDialogUtil::Panic( KErrGeneral ) );
    
    iListBox->Model()->SetItemTextArray( iMFListHandler );
    iListBox->Model()->SetOwnershipType( ELbmDoesNotOwnItemArray );
    
	iListBox->SetListBoxObserver( this );
	iListBox->CreateScrollBarFrameL( ETrue );
	iListBox->ScrollBarFrame()->SetScrollBarVisibilityL(
	                 CEikScrollBarFrame::EOn, CEikScrollBarFrame::EAuto );
	
	iListBox->SetContainerWindowL( *this );
	
	SetIconsL();
	
	iListBox->ItemDrawer()->ColumnData()->EnableMarqueeL( ETrue );


    iState->Init();
    
    if ( iDialogTitle.Length() > 0 )
        {
        iStatusPaneHandler->SetTitleL( iDialogTitle );
        }
    else
        {
	    iStatusPaneHandler->SetTitleL( R_QTN_MEDIA_FILE_DIALOG_TITLE );
        }
	iStatusPaneHandler->SetNaviPaneTitleL( KNullDesC );
	
	
    //
    // create find box. NOTE: findbox functionality is quite complex and
	// is implemented by copy pasting code from other apps.
    //
	
    CFindControl* control = ( CFindControl* )ControlOrNull( EFindControl );
    if ( control )
        {
        CAknSearchField::TSearchFieldStyle flags = CAknSearchField::EFixed;
        control->iFind = CAknSearchField::NewL( 
                                   *control, flags, 0, KFindBoxLength );
        //control->iFind = searchField;
        control->SetContainerWindowL( *this );
        CEikEdwin& edwin = control->iFind->Editor(); 
        control->iFind->AddAdaptiveSearchTextObserverL( this );
        }	

     if ( FindBox() )
        {
        CAknFilteredTextListBoxModel* model = 
        STATIC_CAST( CAknFilteredTextListBoxModel*, iListBox->Model() );
       
        FindBox()->MakeVisible( EFalse );
        }
        
      
    UpdateListBoxL();
    
    DrawDeferred();
    }


//------------------------------------------------------------------------------
// CMediaFileDialog::ProcessCommandL
//
// Handle commands from menu. NOTE: Current implemntation does not use menu.
// Menu handling code is here for (possible) future use.
//------------------------------------------------------------------------------
//
void CMediaFileDialog::ProcessCommandL( TInt aCommandId )
    {
    HideMenu();
    
    switch (aCommandId)
        {
        case EAknCmdHelp:
            {
            break;
            }

        case EAknSoftkeySelect:
            {
            TBool ret = HandleOKL( EAttrSelect );
            if ( ret )
                {
                TryExitL( EEikCmdExit );
                }
            break;
            }
            
        case EAknSoftkeyView:
            {
            PreviewL( CMediaFileList::EMediaFileTypeVideo );
            break;
            }
        case EAknCmdOpen:
            {
            HandleOKL( EAttrOpen );
            break;
            }

        case EAknCmdExit:
        case EEikCmdExit:
            {
            // close dialog and exit calling application
            iAvkonAppUi->ProcessCommandL( EAknCmdExit );
            break;
            }

        default:            
            break;
        }
    }


//------------------------------------------------------------------------------
// CMediaFileDialog::OkToExitL
//
// Hanlde commands from cba.
//------------------------------------------------------------------------------
//
TBool CMediaFileDialog::OkToExitL(TInt aButtonId)
    {
    if ( aButtonId == EEikBidCancel )
        {
        return ETrue;  // close dialog
        }

    if ( aButtonId == EEikBidOk )
        {
        TBool ret = HandleOKL( EAttrSelect );
        return ret;
        }

    if ( aButtonId == EAknSoftkeyBack )
        {
        if ( !HandleBackL() )
            {
#ifdef _DEBUG            
            // start test code
            TInt currentIndex = CurrentItemListIndex();
            if ( iState->FolderId( currentIndex ) == EEFolderRecordings )
                {
                if ( !iTest->TestL( 0 ) )
                    {
                    return ETrue; // test canceled
                    }
                return EFalse;  // test accepted
                }
            // end test code
#endif            
            
            return ETrue; // Back pressed in top level folder
            }
            
        return EFalse; // leave dialog open
        }

    if ( aButtonId == EAknSoftkeyContextOptions )
        {
        ShowContextMenuL( R_MEDIA_FILE_DIALOG_CONTEXT_MENU );
        return EFalse;
        }

    if ( aButtonId == EAknSoftkeyOpen )
        {
        TBool ret = HandleOKL( EAttrOpen );
        return ret;
        }
    
    if ( aButtonId == EAknSoftkeySelect )
        {
        TBool ret = HandleOKL( EAttrSelect );
        return ret;
        }
    
    if ( aButtonId == EAknSoftkeyView )
        {
        PreviewL( CMediaFileList::EMediaFileTypeVideo );
        return EFalse;
        }

    if ( aButtonId == EAknSoftkeyEmpty )
        {
        return EFalse;
        }

    return CAknDialog::OkToExitL( aButtonId );
    }


// ----------------------------------------------------------------------------
// CMediaFileDialog::OfferKeyEventL
// 
// ----------------------------------------------------------------------------
//
TKeyResponse CMediaFileDialog::OfferKeyEventL(const TKeyEvent& aKeyEvent,
                                                      TEventCode aType)
    {
    // must pass escape event to dialog - findbox and listbox "eat" escape events
    TKeyResponse res = EKeyWasNotConsumed;
    if ( aType == EEventKey && aKeyEvent.iCode == EKeyEscape )
        {
        res = CAknDialog::OfferKeyEventL( aKeyEvent, aType );
        }
    if ( res == EKeyWasConsumed ) 
        {
        return res;
        }
        

    TBool backSpace = EFalse;
    
    if (aType == EEventKey)
        {
        switch (aKeyEvent.iCode)
            {
            case EKeyEnter:
                {
                break;
                }
            case EKeyUpArrow:
            case EKeyDownArrow:
                {
                TInt itemCount = iListBox->Model()->NumberOfItems();
                if ( itemCount == 1 )
                    {
                    PreviewL( CMediaFileList::EMediaFileTypeAudio );    
                    }
                break;
                }

            case EKeyLeftArrow:
                {
                if (FindBox() && FindBox()->IsVisible()
                        && FindBox()->TextLength() != 0)
                    {
                    FindBoxCursorMoveL( 0 );
                    res = EKeyWasConsumed;
                    }
                else
                    {
                    HandleBackL();
                    return EKeyWasConsumed;
                    }
                break;
                }

            case EKeyRightArrow:
                {
                if (FindBox() && FindBox()->IsVisible()
                        && FindBox()->TextLength() != 0)
                    {
                    FindBoxCursorMoveL( 1 );
                    res = EKeyWasConsumed;
                    }
                else
                    {
                    HandleOKL( EAttrOpen );
                    return EKeyWasConsumed;
                    }
                break;
                }

            case EKeyBackspace:
                {  
                backSpace = ETrue;
                break;
                }

            default:
                {
                break;
                }
            }
        
    if ( FindBox() && FindBox()->IsVisible() )
        {
        res = FindBox()->OfferKeyEventL( aKeyEvent, aType );
        
        if ( backSpace && FindBox()->TextLength() == 0 &&
             ( iState->CurrentFolder() == EEFolderSearch ||
             iState->CurrentFolder() == EEFolderSearchMusic ) && !iPeninputActive )
            {
            // user cleared search field - go back to main view
            HandleBackL();
            return EKeyWasConsumed;
            }
        }
        }
        
  
    if ( res == EKeyWasNotConsumed )
         {
         res = CAknDialog::OfferKeyEventL( aKeyEvent, aType);
         }
    return res;
    }


// -----------------------------------------------------------------------------
// CMediaFileDialog::FindBoxCursorMoveL( TInt aType )
//
// NOTE: This function is used to move the cursor of the find box.
// -----------------------------------------------------------------------------
//
void CMediaFileDialog::FindBoxCursorMoveL( TInt aType )
    {
    if ( FindBox() && FindBox()->IsVisible() )
        {
        TInt pos = FindBox()->Editor().CursorPos();
        TInt textLength = FindBox()->Editor().TextLength();
        if ( aType == 0 )//left
            {
            pos = pos - 1;
            if ( pos < 0 )
                {
                pos = textLength;
                }
            }
        else if ( aType == 1 )
            {
            pos = pos + 1;
            if ( pos > textLength )
                {
                pos = 0;
                }
            }
        else
            {
            pos = textLength;
            }
        FindBox()->Editor().SetCursorPosL( pos ,EFalse );
        }
    }
// -----------------------------------------------------------------------------
// CMediaFileDialog::SetSizeAndPosition (from CAknDialog)
//
// NOTE: This code is copy paste from elsewhere.
// -----------------------------------------------------------------------------
//
void CMediaFileDialog::SetSizeAndPosition( const TSize& /*aSize*/ )
    {
    TRect mainPane;
    AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::EMainPane, mainPane );
    
    SetRect( mainPane );

    if ( FindBox() && FindBox()->IsVisible() )
        {
        AknFind::HandleFixedFindSizeChanged( this, iListBox, FindBox() );
        }

    Line( ESelectionListControl)->SetRect( iListBox->Rect() );
    if ( FindBox() ) 
        {
        Line( EFindControl )->SetRect( iListBox->Rect() );
        }    
    }


// ----------------------------------------------------------------------------
// CMediaFileDialog::HandleResourceChange
// 
// ----------------------------------------------------------------------------
//
void CMediaFileDialog::HandleResourceChange(TInt aType)
    {   
    if ( aType == KEikDynamicLayoutVariantSwitch )
        {
        // handle change in layout orientation
        CAknDialog::HandleResourceChange( aType );
         if ( FindBox() && FindBox()->IsVisible() )
	        {
	        AknFind::HandleFixedFindSizeChanged( this, iListBox, FindBox() );
	        }
        return;
        }
        
    if (aType == KAknsMessageSkinChange)
        {
        TRAP_IGNORE( SetIconsL() );
        }
        
    CCoeControl::HandleResourceChange( aType );
    }


//------------------------------------------------------------------------------
// CMediaFileDialog::DynInitMenuPaneL
//
// Called by system before menu is shown. NOTE: Menu currently not used. 
//------------------------------------------------------------------------------
//
void CMediaFileDialog::DynInitMenuPaneL(TInt aResourceID, CEikMenuPane* aMenuPane )
    {
    TInt menu1 = R_MEDIA_FILE_DIALOG_MENU_PANE;
    TInt menu2 = R_MEDIA_FILE_DIALOG_CONTEXT_MENU_PANE;
    
    if ( aResourceID != menu1 && aResourceID != menu2 )
        {
        return; // not one of our menus
        }

    TInt itemCount = iListBox->Model()->NumberOfItems();
    TInt currentIndex = iListBox->CurrentItemIndex();
    TInt folderId = iState->FolderId( currentIndex );
    TInt currentFolderId = iState->CurrentFolder();


    // context menu only used in video folder
    if ( aResourceID == R_MEDIA_FILE_DIALOG_CONTEXT_MENU_PANE )
        {
        if ( itemCount == 0 )
            {
            // folder is empty
            DeleteMenuItem( aMenuPane, EAknSoftkeySelect );
            DeleteMenuItem( aMenuPane, EAknSoftkeyView );
            }
        return;
        }

    
    if ( itemCount == 0 )
        {
        // list has no items
        DeleteMenuItem( aMenuPane, EAknCmdOpen );
        DeleteMenuItem( aMenuPane, EAknSoftkeySelect );
        DeleteMenuItem( aMenuPane, EAknSoftkeyView );
        return;
        }
    
    if ( folderId == EEFolderDownload || folderId == EEFolderNullItemOne || 
         folderId == EEFolderNullItemTwo )
        {
        // selected item is null item
        DeleteMenuItem( aMenuPane, EAknCmdOpen );
        DeleteMenuItem( aMenuPane, EAknSoftkeyView );
        }
    else if ( folderId != KErrNotFound )
        {
        // selected item is folder
        DeleteMenuItem( aMenuPane, EAknSoftkeySelect );
        DeleteMenuItem( aMenuPane, EAknSoftkeyView );
        }
    else
        {
        // selected item is file
        if ( iState->CurrentFolder() == EEFolderVideos )
            {
            DeleteMenuItem( aMenuPane, EAknCmdOpen );
            }
        else if ( iState->CurrentFolder() == EEFolderSearch )
            {
            DeleteMenuItem( aMenuPane, EAknCmdOpen );
        
            if ( !IsVideoFile( currentIndex ) )
                {
                DeleteMenuItem( aMenuPane, EAknSoftkeyView );
                }
            }
        else
            {
            DeleteMenuItem( aMenuPane, EAknCmdOpen );
            DeleteMenuItem( aMenuPane, EAknSoftkeyView );    
            }
        }
    }


// ---------------------------------------------------------------------------
// CMediaFileDialog::CountComponentControls
//
// Gets the number of controls contained in a compound control.
// ---------------------------------------------------------------------------
//
TInt CMediaFileDialog::CountComponentControls() const
    {
    TInt newitems = 0;
    if ( FindBox() )
        {
        newitems++;
        }
    return CAknDialog::CountComponentControls() + newitems;
    }


// ---------------------------------------------------------------------------
// CMediaFileDialog::CountComponentControls
//
// ---------------------------------------------------------------------------
//
CCoeControl* CMediaFileDialog::ComponentControl( TInt aIndex ) const
    {
    if ( aIndex == CAknDialog::CountComponentControls() )
        {
        return FindBox();
        }
    return CAknDialog::ComponentControl( aIndex );
    }


// -----------------------------------------------------------------------------
// CMediaFileDialog::FindBox
//
// -----------------------------------------------------------------------------
//
CAknSearchField* CMediaFileDialog::FindBox() const
     {
     CFindControl* control = ( CFindControl* )ControlOrNull( EFindControl );
     if ( control )
         {
         return control->iFind;
         }
     else
         {
         return NULL;
         }
     }

// ---------------------------------------------------------------------------
// CMediaFileDialog::ResetFindBoxL
//
// ---------------------------------------------------------------------------
//
void CMediaFileDialog::ResetFindBoxL()
    {
    if ( !FindBox() )
        {
        return;
        }
   
    CAknFilteredTextListBoxModel* model = 
    STATIC_CAST( CAknFilteredTextListBoxModel*, iListBox->Model() );
    CAknListBoxFilterItems* filter = model->Filter();
    TInt currentFolder = iState->CurrentFolder();
    
    // reset find box
    if ( filter )
        {
        filter->ResetFilteringL();
        filter->SetObserver( NULL );
        model->RemoveFilter();
        
        // CreateFilterL sets CAknSearchField to observe filter
        // (CAknListBoxFilterItems) but RemoveFilter does not remove observer
        FindBox()->SetObserver( NULL );
        }
    
    if (FindBox()->TextLength() > 0 && (currentFolder != EEFolderSearch
            && currentFolder != EEFolderSearchMusic))
        {
        FindBox()->SetSearchTextL( KNullDesC );    
        }
    
 
    // show/hide find box
    if ( iState->FindBoxNeeded() )
        {
        if ( currentFolder != EEFolderSearch && 
             currentFolder != EEFolderSearchMusic &&
             currentFolder != EEFolderRoot &&
             currentFolder != EEFolderMusic )
            {
            // normal find only finds matches from listbox item texts
            model->CreateFilterL( iListBox, FindBox() );
            filter = model->Filter();
            filter->ResetFilteringL();
            filter->SetObserver( this );     // MCoeControlObserver
            }
        else
            {
            // search and root folders have custom find that makes
            // database search
            FindBox()->SetObserver( this );  // MCoeControlObserver
            }
//        if ( currentFolder == EEFolderSearch || 
//             currentFolder == EEFolderSearchMusic )
//            {
//            FindBox()->SetSearchTextL( iSearchText );
//            }
 
        FindBox()->MakeVisible( ETrue );
        }
    else
        {
        FindBox()->MakeVisible( EFalse );
        }
            
    TSize size;
    SetSizeAndPosition( size );
    }


// -----------------------------------------------------------------------------
// CMediaFileDialog::CreateCustomControlL (from CAknDialog)
//
// Create find control.
// -----------------------------------------------------------------------------
//
SEikControlInfo CMediaFileDialog::CreateCustomControlL( TInt aControlType )
    {
    if ( aControlType != EAknCtSelectionListPopupFind &&
         aControlType != EAknCtSelectionListFixedFind )
        {
        SEikControlInfo info;
        info.iControl = NULL;
        info.iFlags = 0;
        info.iTrailerTextId = 0;
        return info;
        }

    CCoeControl* findControl = NULL;
    findControl = new (ELeave) CFindControl;

    SEikControlInfo info;
    info.iControl = findControl;
    info.iFlags = 0;
    info.iTrailerTextId = 0;
    return info;
    }

    
// ----------------------------------------------------------------------------
// CMediaFileDialog::SetIconsL
//
// ----------------------------------------------------------------------------
//
void CMediaFileDialog::SetIconsL()
    {
    _LIT( KBitmapFile, "mediafilelist.mbm" );
    
        
    if ( !iListBox )
        {
        return;
        }
    
    TMFDialogUtil::SetIconFileNameL( KBitmapFile, iIconFileName );
    
    CArrayPtr<CGulIcon>* icons = new (ELeave) CAknIconArray(1);
    CleanupStack::PushL( icons );

    
    //
    // NOTE: append order must match TMediaFileListIcons
    //
    
    // music item icon
    icons->AppendL( IconL( KAknsIIDQgnIndiAiNtMusic, iIconFileName,
                           EMbmMediafilelistQgn_indi_ai_nt_music,
                           EMbmMediafilelistQgn_indi_ai_nt_music_mask ) );

    // tone item icon, default tone icon
    icons->AppendL( IconL( KAknsIIDQgnPropMupAudio, iIconFileName,
                           EMbmMediafilelistQgn_prop_mup_audio,
                           EMbmMediafilelistQgn_prop_mup_audio_mask ) );

    // video item icon
    icons->AppendL( IconL( KAknsIIDQgnPropFmgrFileVideo, iIconFileName,
                           EMbmMediafilelistQgn_prop_fmgr_file_video,
                           EMbmMediafilelistQgn_prop_fmgr_file_video_mask ) );

    // recording item icon
    icons->AppendL( IconL( KAknsIIDQgnPropFmgrFileVoicerec, iIconFileName,
                           EMbmMediafilelistQgn_prop_fmgr_file_voicerec,
                           EMbmMediafilelistQgn_prop_fmgr_file_voicerec_mask ) );

    // artist icon
    icons->AppendL( IconL( KAknsIIDQgnPropMupArtist, iIconFileName,
                           EMbmMediafilelistQgn_prop_mup_artist,
                           EMbmMediafilelistQgn_prop_mup_artist_mask ) );

    // album icon
    icons->AppendL( IconL( KAknsIIDQgnPropMupAlbum, iIconFileName,
                           EMbmMediafilelistQgn_prop_mup_album,
                           EMbmMediafilelistQgn_prop_mup_album_mask ) );

    // genre icon
    icons->AppendL( IconL( KAknsIIDQgnPropMupGenre, iIconFileName,
                           EMbmMediafilelistQgn_prop_mup_genre,
                           EMbmMediafilelistQgn_prop_mup_genre_mask ) );

    // composer icon
    icons->AppendL( IconL( KAknsIIDQgnPropMupComposer, iIconFileName,
                           EMbmMediafilelistQgn_prop_mup_composer,
                           EMbmMediafilelistQgn_prop_mup_composer_mask ) );

    // folder icon (tones, music, videos, recordings, all music, artist,
    // album, genre, composer, artist all)
    icons->AppendL( IconL( KAknsIIDQgnPropFolderSmall, iIconFileName,
                           EMbmMediafilelistQgn_prop_folder_small,
                           EMbmMediafilelistQgn_prop_folder_small_mask ) );

    // unknown folder icon
    icons->AppendL( IconL( KAknsIIDQgnPropMceUnknownRead, iIconFileName,
                           EMbmMediafilelistQgn_prop_mce_unknown_read,
                           EMbmMediafilelistQgn_prop_mce_unknown_read ) );

    // download item icon (null item)
    icons->AppendL( IconL( KAknsIIDQgnPropLinkEmbdSmall, iIconFileName,
                           EMbmMediafilelistQgn_prop_link_embd_small,
                           EMbmMediafilelistQgn_prop_link_embd_small_mask ) );

    // off item icon (null item)
    icons->AppendL( IconL( KAknsIIDQgnPropSmlSyncOff, iIconFileName,
                           EMbmMediafilelistQgn_prop_sml_sync_off,
                           EMbmMediafilelistQgn_prop_sml_sync_off_mask ) );

    // memory card icon
    icons->AppendL( IconL( KAknsIIDQgnIndiMmcAdd, iIconFileName,
                           EMbmMediafilelistQgn_indi_mmc_add,
                           EMbmMediafilelistQgn_indi_mmc_add_mask, EAknsCIQsnIconColorsCG13 ) );

     // mass storage icon
    icons->AppendL( IconL( KAknsIIDQgnPropLinkEmbdSmall, iIconFileName,
                           EMbmMediafilelistQgn_indi_fmgr_ms_add,
                           EMbmMediafilelistQgn_indi_fmgr_ms_add_mask,EAknsCIQsnIconColorsCG13 ) );

    // empty icon
    icons->AppendL( IconL( KAknsIIDQgnPropEmpty, KAvkonBitmapFile,
                           EMbmAvkonQgn_prop_empty,
                           EMbmAvkonQgn_prop_empty_mask ) );
    
    // delete old icons
    CArrayPtr<CGulIcon>* arr = iListBox->ItemDrawer()->ColumnData()->IconArray();
    if ( arr )
        {
        arr->ResetAndDestroy();
        delete arr;
        arr = NULL;
        }

    iListBox->ItemDrawer()->ColumnData()->SetIconArray( icons );
    CleanupStack::Pop( icons );
    }


// -----------------------------------------------------------------------------
// CMediaFileDialog::IconL
// 
// -----------------------------------------------------------------------------
//
CGulIcon* CMediaFileDialog::IconL(TAknsItemID aId, const TDesC& aFileName,
                                   TInt aFileIndex, TInt aFileMaskIndex)
	{
    CGulIcon* icon = AknsUtils::CreateGulIconL(AknsUtils::SkinInstance(), aId,
                                aFileName, aFileIndex, aFileMaskIndex);
	return icon;	
	}


// -----------------------------------------------------------------------------
// CMediaFileDialog::IconL
// For MMC icon and Mass Storage icon
// -----------------------------------------------------------------------------
//
CGulIcon* CMediaFileDialog::IconL(TAknsItemID aId, const TDesC& aFileName,
                                   TInt aFileIndex, TInt aFileMaskIndex, TAknsQsnIconColorsIndex colorIndex)
	{
	TRgb defaultColour(KRgbBlack);
	CFbsBitmap* bmap = NULL;
	CFbsBitmap* mask = NULL;

	AknsUtils::GetCachedColor(AknsUtils::SkinInstance(), defaultColour, KAknsIIDQsnIconColors,
			colorIndex);
	AknsUtils::CreateColorIconLC(AknsUtils::SkinInstance(), KAknsIIDQgnIndiMmcAdd,
			KAknsIIDQsnIconColors, colorIndex, bmap, mask,
			aFileName, aFileIndex, aFileMaskIndex, defaultColour);

	CGulIcon* icon = CGulIcon::NewL(bmap, mask);
	icon->SetBitmapsOwnedExternally(EFalse);
	CleanupStack::Pop(2);
	return icon;
	}


// ----------------------------------------------------------------------------
// CMediaFileDialog::ShowContextMenuL
// 
// ----------------------------------------------------------------------------
//
void CMediaFileDialog::ShowContextMenuL( TInt /*aResource*/ )
	{
	// Switch to Context specific options menu,
	if ( iMenuBar )
	    {
	    TRAP_IGNORE( iMenuBar->TryDisplayContextMenuBarL() );    
	    }
	}


//------------------------------------------------------------------------------
// CMediaFileDialog::HandleBackL
//
//------------------------------------------------------------------------------
//
TBool CMediaFileDialog::HandleBackL()
	{
	TInt err = KErrNone;
	TBool ret = ETrue;

	TRAP( err, ret = DoHandleBackL() );
	
    if ( err != KErrNone )
        {
        FLOG( _L("### CMediaFileDialog::HandleBackL (%d) ###"), err );
        
        ResetDialogL( err );
        return ETrue;
        }
        
    return ret;
	}


//------------------------------------------------------------------------------
// CMediaFileDialog::DoHandleBackL
//
//------------------------------------------------------------------------------
//
TBool CMediaFileDialog::DoHandleBackL()
	{
    if ( iState->CurrentFolder() == EEFolderRoot )
        {
        // return EFalse only when called from root folder
        return EFalse;    
        }

	if ( !iMediaFileHandler->QueryReady() )
	    {
	    FLOG( _L("### CMediaFileDialog::DoHandleBackL: query in progress ###") );
	    return ETrue;  // query in progress
	    }
  
    TInt folderId = iState->NextFolderUp();
    TInt currentIndex = CurrentItemListIndex();
    
    iState->SetQueryInfo( folderId, KNullDesC, currentIndex, EEQueryUp );
    QueryL( folderId );
    
    return ETrue;
    }


//------------------------------------------------------------------------------
// CMediaFileDialog::HandleOKL
//
//------------------------------------------------------------------------------
//
TBool CMediaFileDialog::HandleOKL( TInt aAttr )
	{
    TInt err = KErrNone;
    TBool ret = EFalse;
    
    TRAP( err, ret = DoHandleOKL( aAttr ) );
    
    if ( err != KErrNone )
        {
        FLOG( _L("### CMediaFileDialog::HandleOKL (%d) ###"), err );
        
        ResetDialogL( err );
        return EFalse;
        }
        
    return ret;
    }


//------------------------------------------------------------------------------
// CMediaFileDialog::DoHandleOKL
//
// Selects file or opens folder. If file is selected (ie dialog can
// be closed) return value is ETrue.
//------------------------------------------------------------------------------
//
TBool CMediaFileDialog::DoHandleOKL( TInt aAttr )
	{
	if ( !iMediaFileHandler->QueryReady() )
	    {
	    FLOG( _L("### CMediaFileDialog::DoHandleOKL: query in progress ###") );
	    return EFalse;  // query in progress
	    }
	    
    TInt currentIndex = CurrentItemListIndex();
    if ( currentIndex == KErrNotFound )
        {
        return EFalse; // empty folder
        }
        
    TBool isPersistent = EFalse;
    TInt iconId = EEIconEmpty;
    TInt folderId = KErrNotFound;
    iState->GetFolderInfo( currentIndex, folderId, iBuf,
                                         isPersistent, iconId );

    if ( folderId == EEFolderDownload || folderId == EEFolderNullItemOne ||
         folderId == EEFolderNullItemTwo || folderId == KErrNotFound )
        {
        if ( aAttr == EAttrOpen )
            {
            return EFalse;  // these can only be selected, not opened    
            }
        }
        

    if ( folderId == EEFolderDownload )
        {
        *iNullItem = 0;
         // user selected 'Download' item - launch browser
         CWebBrowserLauncher* launcher = CWebBrowserLauncher::NewLC();
         launcher->LaunchBrowserL();
         CleanupStack::PopAndDestroy( launcher );
         return EFalse;
        }

    if ( folderId == EEFolderNullItemOne )
        {
        if ( iNullItemFileNameOne )
            {
            TMFDialogUtil::StrCopy( *iFileName, *iNullItemFileNameOne );
            }
        *iNullItem = 1;
        return ETrue;
        }

    if ( folderId == EEFolderNullItemTwo )
        {
        if ( iNullItemFileNameTwo )
            {
            TMFDialogUtil::StrCopy( *iFileName, *iNullItemFileNameTwo );
            }
        *iNullItem = 2;
        return ETrue;
        }

    if ( folderId == KErrNotFound )
        {
        // user has selected some media file
	    GetSelectedItemFileName( currentIndex, iBuf );
	    
	    TBool ret = ETrue;
	    
	    // protection check not done when item is double checked
	   // if ( aAttr != EAttrDoubleClicked )
	   if (!ifilevalid)
	        {
	    	iFileisvalid = iProtectionHandler->IsFileValid( iBuf, 
	                                        CMFProtectionHandler::ESelect );
	    ifilevalid=ETrue;
	        }
	    
  	   if ( !iFileisvalid)
		   {
		   ifilevalid=EFalse; 
		   }
	    if ( iFileisvalid )
	        {
	        TMFDialogUtil::StrCopy( *iFileName, iBuf );
	        }
	    
	    return iFileisvalid;  // ret is ETrue is selected file's drm is OK
        }

    
    if ( folderId != KErrNotFound && aAttr != EAttrSelect )
        {
        iBuf = KNullDesC;
        // selected list item is folder
         if ( !isPersistent )
            {
            // read non-persistent folder text from database
            GetListItemText( currentIndex, iBuf );
            }
      
        iState->SetQueryInfo( folderId, iBuf, currentIndex, EEQueryDown );
        QueryL( folderId );
        }

    return EFalse;
    }


//------------------------------------------------------------------------------
// CMediaFileDialog::SearchL
//
//------------------------------------------------------------------------------
//
void CMediaFileDialog::SearchL()
    {
    TInt err = KErrNone;
    
    TRAP( err, DoSearchL() );
    
    if ( err != KErrNone )
        {
        ResetDialogL( err );
        }
    }


//------------------------------------------------------------------------------
// CMediaFileDialog::DoSearchL
//
//------------------------------------------------------------------------------
//
void CMediaFileDialog::DoSearchL()
    {
    if ( !FindBox() )
        {
        return;
        }
    
    if ( iPeninputActive)
        {
      return;
        }
    if ( !iMediaFileHandler->QueryReady() )
        {
        FLOG( _L("### CMediaFileDialog::DoSearchL: query in progress ###") );
        return;  // query in progress
        }
    
    TInt currentFolder = iState->CurrentFolder();
    
    if ( FindBox()->TextLength() == 0 )
        {
        return;
        }
    FindBox()->GetSearchText( iSearchText );
    
    _LIT(KSpecialChar, "%");
    _LIT(KReplaceChar, "\\%" );
    TBuf<10> buf;
    buf.Copy( KSpecialChar );
    TInt position = iSearchText.FindF( buf );
    if ( position != KErrNotFound )
        {
        buf.Copy( KReplaceChar );
        iSearchText.Replace( position,1,buf );
        }

    
    if ( currentFolder == EEFolderRoot || currentFolder == EEFolderSearch )
        {
        TInt currentIndex = iListBox->CurrentItemIndex();
        iState->SetQueryInfo( EEFolderSearch, KNullDesC, currentIndex, EEQueryDown );
        
        QueryL( EEFolderSearch );
        }
    else if ( currentFolder == EEFolderMusic || currentFolder == EEFolderSearchMusic )
        {
        TInt currentIndex = iListBox->CurrentItemIndex();
        iState->SetQueryInfo( EEFolderSearchMusic, KNullDesC, currentIndex, EEQueryDown );
        
        QueryL( EEFolderSearchMusic );
        }
    }


//------------------------------------------------------------------------------
// CMediaFileDialog::ResetDialogL
//
//------------------------------------------------------------------------------
//
void CMediaFileDialog::ResetDialogL( TInt aError)
    {
    iMediaFileHandler->EnableObserverCall( EFalse );
    iMediaFileHandler->CancelQuery();
    iMediaFileHandler->EnableObserverCall( ETrue );
    
    iState->Init();  
    UpdateListBoxL();

    if ( aError != KErrNone )
        {
        ShowErrorNoteL( aError );
        }
    }


//------------------------------------------------------------------------------
// CMediaFileDialog::QueryL
//
//------------------------------------------------------------------------------
//
void CMediaFileDialog::QueryL( TInt aFolderId, TBool aDatabaseChanged )
    {
    if ( !iMediaFileHandler->QueryReady() || !iDatabaseOpen )
        {
        FLOG( _L("### CMediaFileDialog::QueryL: query in progress ###") );
        return;
        }

    iMediaFileHandler->EnableObserverCall( ETrue );
    
    TInt folderId = aFolderId;
    
    iMediaFileHandler->SetQueryId( folderId );
    
    if ( folderId == EEFolderAllMusic )
        {
        iMediaFileHandler->QueryMusicL( 0 );
        }
        
    else if ( folderId == EEFolderVideos )
        {
        iMediaFileHandler->QueryVideosL( 0 );
        }

    else if ( folderId == EEFolderArtistRoot ||
              folderId == EEFolderAlbumRoot  ||
              folderId == EEFolderGenreRoot  || 
              folderId == EEFolderComposerRoot )
        {
        if ( iState->UnknownCount( folderId ) == KErrNotFound )
            {
            TInt attr = MapAttribute( folderId );
            iMediaFileHandler->QueryMusicUnknownCountL( attr );
            }
        else
            {
            TInt attr = MapAttribute( folderId );
            iMediaFileHandler->QueryAttributeValuesL( attr );
            }
        }
    
    else if ( folderId == EEFolderAlbum || folderId == EEFolderGenre ||
              folderId == EEFolderComposer )
        {
        TInt attr = MapAttribute( folderId );
        iState->GetText( folderId, iBuf );
        iMediaFileHandler->QueryMusicL( attr, iBuf );
        }
    
    else if ( folderId == EEFolderArtistUnknown || folderId == EEFolderAlbumUnknown ||
              folderId == EEFolderGenreUnknown ||folderId == EEFolderComposerUnknown )
        {
        TInt attr = MapAttribute( folderId );
        iMediaFileHandler->QueryMusicUnknownL( attr );
        }
    
    else if ( folderId == EEFolderArtist )
        {
        iState->GetText( EEFolderArtist, iBuf );
        iMediaFileHandler->QueryArtistL( CMediaFileHandler::EQueryArtist,
                           iBuf, KNullDesC, aDatabaseChanged );
        }

    else if ( folderId == EEFolderArtistAll )
        {
        iState->GetText( EEFolderArtist, iBuf );
        iMediaFileHandler->QueryArtistL( CMediaFileHandler::EQueryArtistAll,
                                         iBuf, KNullDesC, aDatabaseChanged );
        }

    else if ( folderId == EEFolderArtistAlbum )
        {
        TBuf<KBufSize128> buf;
        iState->GetText( EEFolderArtistAlbum, buf );
        iState->GetText( EEFolderArtist, iBuf );
        iMediaFileHandler->QueryArtistL( CMediaFileHandler::EQueryArtistAlbum,
                           iBuf, buf, aDatabaseChanged );
        }

    else if ( folderId == EEFolderArtistAlbumUnknown )
        {
        iState->GetText( EEFolderArtist, iBuf );
        iMediaFileHandler->QueryArtistL( CMediaFileHandler::EQueryArtistUnknown,
                           iBuf, KNullDesC, aDatabaseChanged );
        }

    else if ( folderId == EEFolderTones )
        {
        //iMediaFileHandler->QueryTonesL( 0 );
        iMediaFileHandler->QueryDefaultTonesL( 0 );
        ShowWaitNoteL( EFalse );
        }
    
    else if ( folderId == EEFolderRecordings )
        {
        iMediaFileHandler->QueryRecordingsL( 0 );
        }

    else if ( folderId == EEFolderSearch )
        {
        if ( iSearchText.Length() > 0 )
            {
            iMediaFileHandler->SearchL( iSearchText );    
            }
        else
            {
            iMediaFileHandler->QueryEmptyL();
            }
        }
    else if ( folderId == EEFolderSearchMusic )
        {
        if ( iSearchText.Length() > 0 )
            {
            iMediaFileHandler->SearchAudioL( iSearchText );    
            }
        else
            {
            iMediaFileHandler->QueryEmptyL();
            }
        }
    else
        {
        // make empty query
        iMediaFileHandler->QueryEmptyL();
        }
    
    if ( !iMediaFileHandler->QueryReady() )
        {
        // query is running - show wait note with delay. wait note is
        // removed in HandleMFEventL
        ShowWaitNoteL( aDatabaseChanged );
        }
    
#ifdef _DEBUG    
    iTest->SetTime();
#endif    
    }


// -----------------------------------------------------------------------------
// CMediaFileDialog::MapAttribute
// 
// -----------------------------------------------------------------------------
//
TInt CMediaFileDialog::MapAttribute( TInt aFolderId )
    {
    TInt attr = KErrNotFound;
    
    switch ( aFolderId )
        {
        case EEFolderArtistRoot:
        case EEFolderArtist:
        case EEFolderArtistUnknown:
            {
            attr = CMediaFileHandler::EAttrArtist;
            break;
            }
        case EEFolderAlbumRoot:
        case EEFolderAlbum:
        case EEFolderAlbumUnknown:
            {
            attr = CMediaFileHandler::EAttrAlbum;
            break;
            }
        case EEFolderGenreRoot:
        case EEFolderGenre:
        case EEFolderGenreUnknown:
            {
            attr = CMediaFileHandler::EAttrGenre;
            break;
            }
        case EEFolderComposerRoot:
        case EEFolderComposer:
        case EEFolderComposerUnknown:
            {
            attr = CMediaFileHandler::EAttrComposer;
            break;
            }
        default:
            {
            break;
            }
        }
        
    return attr;
    }


// -----------------------------------------------------------------------------
// CMediaFileDialog::ShowDetailsPopupL
//
// -----------------------------------------------------------------------------
//
void CMediaFileDialog::ShowDetailsPopupL()
    {
    if ( !iMediaFileHandler->QueryReady() )
        {
        return;  // query in progress
        }
        
    TInt currentIndex = CurrentItemListIndex();
    TInt folderId = iState->CurrentFolder();
    
    if ( folderId != EEFolderSearch && folderId != EEFolderSearchMusic )
        {
        return;
        }
    if ( currentIndex < 0 )
        {
        return;
        }
    
    TInt id = iMediaFileHandler->Attribute( currentIndex, 
                                            CMediaFileHandler::EAttrMediaFileId,
                                            iState->CurrentFolder() );
    if ( id == KErrNotFound )
        {
        return;
        }
    
    TInt mediaType = iMediaFileHandler->Attribute( currentIndex, 
            CMediaFileHandler::EAttrMediaType,
            iState->CurrentFolder() );
    
    TInt storageType = iMediaFileHandler->Attribute( currentIndex, 
            CMediaFileHandler::EAttrStorageType,
            iState->CurrentFolder() );
    
    CMediaFileData* data = NULL;

    if ( storageType == CMediaFileHandler::ERomStorage )
        {
        if ( mediaType == CMediaFileHandler::EMediaTypeTone )
            {
            data = iMediaFileHandler->RomMediaFileDataL( id , CMediaFileHandler::EMediaTypeTone );
            }
        if ( mediaType == CMediaFileHandler::EMediaTypeVideo )
            {
            data = iMediaFileHandler->RomMediaFileDataL( id , CMediaFileHandler::EMediaTypeVideo );
            }
        }
    else
    	{
    	data = iMediaFileHandler->ReadMediaFileDataL( id );
    	}

    CleanupStack::PushL( data );

    GetPopupText( data, iPopupText );
    
    CleanupStack::PopAndDestroy( data );
    
    if ( iPopupText.Length() > 0 )
        {
        ShowPopupNoteL( iPopupText );
        }
    }


// -----------------------------------------------------------------------------
// CMediaFileDialog::GetPopupText
//
// -----------------------------------------------------------------------------
//
void CMediaFileDialog::GetPopupText( CMediaFileData* aData, TDes& aText )
    {
    TBuf<KBufSize64> searchText;
    TBuf<KBufSize64> attributeName;
    
    aText = KNullDesC;
    
    if ( FindBox()->TextLength() >= KBufSize64 )
        {
        return;
        }
    FindBox()->GetSearchText( searchText );
   
    aData->GetAttr( CMediaFileHandler::EAttrSongName, aText );
    if ( MatchFound( aText, searchText ) )
        {
        aText = KNullDesC;
        return;
        }
    
    aData->GetAttr( CMediaFileHandler::EAttrArtist, aText );
    if ( MatchFound( aText, searchText ) )
        {
        StringLoader::Load( attributeName, R_QTN_PREVIEW_POPUP_ARTIST );
        aText.Insert( 0, attributeName );
        return;
        }

    aData->GetAttr( CMediaFileHandler::EAttrAlbum, aText );
    if ( MatchFound( aText, searchText ) )
        {
        StringLoader::Load( attributeName, R_QTN_PREVIEW_POPUP_ALBUM );
        aText.Insert( 0, attributeName );
        return;
        }

    aData->GetAttr( CMediaFileHandler::EAttrGenre, aText );
    if ( MatchFound( aText, searchText ) )
        {
        StringLoader::Load( attributeName, R_QTN_PREVIEW_POPUP_GENRE );
        aText.Insert( 0, attributeName );
        return;
        }

    aData->GetAttr( CMediaFileHandler::EAttrComposer, aText );
    if ( MatchFound( aText, searchText ) )
        {
        StringLoader::Load( attributeName, R_QTN_PREVIEW_POPUP_COMPOSER );
        aText.Insert( 0, attributeName );
        return;
        }
    
    aText = KNullDesC;
    }


// -----------------------------------------------------------------------------
// CMediaFileDialog::MatchFound
//
// -----------------------------------------------------------------------------
//
TBool CMediaFileDialog::MatchFound( const TDesC& aAttribute, 
                                    const TDesC& aText )
    {
    _LIT( KSpace, " " );
    
    if ( aText.Length() >= KBufSize64 )
        {
        return EFalse; // too long search text
        }

    if ( aAttribute.FindC( aText ) == 0 )
        {
        // match found from the beginning of the first word
        return ETrue; 
        }

    TBuf<KBufSize64> buf;
    buf = aText;
    buf.Insert( 0, KSpace );
    
    if ( aAttribute.FindC( buf ) != KErrNotFound )
        {
        // match found from the beginning of the word
        return ETrue; 
        }
    
    return EFalse;
    }


// -----------------------------------------------------------------------------
// CMediaFileDialog::PreviewL
//
// -----------------------------------------------------------------------------
//
void CMediaFileDialog::PreviewL( TInt aMediaType )
    {
    //FLOG( _L("CMediaFileDialog::PreviewL START") );
    
    if ( !iMediaFileHandler->QueryReady() )
        {
        FLOG( _L("### CMediaFileDialog::PreviewL: query in progress ###") );
        return;  // query in progress
        }

    if ( !iForeGround )
        {
        return;  // dialog is not visible
        }
        
    TInt currentIndex = CurrentItemListIndex();
    if ( currentIndex == KErrNotFound )
        {
        return;  // list is empty 
        }
    
    TInt folderId = iState->FolderId( currentIndex );
    if ( folderId == EEFolderNullItemOne || folderId == EEFolderNullItemTwo )
        {
        PreviewNullItemL( folderId );
        return;
        }
    
    if ( folderId != KErrNotFound )
        {
        return; // focused list item is folder
        }
     
     
    if ( iState->CurrentFolder() == EEFolderVideos  &&
         aMediaType != CMediaFileList::EMediaFileTypeVideo )
        {
        return;
        }
    
    GetSelectedItemFileName( currentIndex, iBuf );
    if ( iBuf.Length() == 0 )
        {
        return;
        }
        
    if ( aMediaType == CMediaFileList::EMediaFileTypeVideo )
        {
        PreviewVideoL( iBuf );
        return;
        }
    
    if ( iState->CurrentFolder() == EEFolderSearch )
        {
        if ( IsVideoFile( currentIndex ) )
            {
            return; // search folder may contain videos - no automatic preview                
            }
        }
        
    if ( !iProtectionHandler->IsFileValid( iBuf, CMFProtectionHandler::EPlay  ) )
        {
        return;
        }
        
    iAudioPreviewHandler->SetAttrL( TMFDialogUtil::EAttrFileName, iBuf );
    iAudioPreviewHandler->PlayL();
    
    //FLOG( _L("CMediaFileDialog::PreviewL END") );
    }


// -----------------------------------------------------------------------------
// CMediaFileDialog::PreviewVideoL
//
// -----------------------------------------------------------------------------
//
void CMediaFileDialog::PreviewVideoL( const TDesC& aFileName )
    {
    if ( !iProtectionHandler->IsVideoValid( aFileName, CMFProtectionHandler::EPlay ) )
         {
         return;
         }

    // in case "Beep once" one audio beep is played before showing video
    TInt type = iAudioPreviewHandler->Attr( CMediaFileList::EAttrRingingType );
    if ( type == CMFPreviewHandlerBase::ERingingTypeBeepOnce )
        {
        iAudioPreviewHandler->SetAttrL( TMFDialogUtil::EAttrFileName, aFileName );
        iAudioPreviewHandler->PlayL();
        }
     
    iVideoPreviewHandler->SetAttrL( TMFDialogUtil::EAttrFileName, aFileName );
    TInt err = KErrNone;
    if ( iVideoPreviewDlg && iVideoPreviewHandler->IsPlaying() )
        {
        iVideoPreviewDlg->CloseDialogL();
        iVideoPreviewDlg = NULL;
        }
    iVideoPreviewDlg = CVideoPreviewDialog::NewL( iVideoPreviewHandler );
    TBool ret = iVideoPreviewDlg->ShowDialogLD( &err );

    if ( err != KErrNone )
        {
        FLOG( _L("### CVideoPreviewDialog::ShowDialogLD (%d) ###"), err );
        }
     
    ShowPreviewErrorNoteL( err, CMediaFileHandler::EMediaTypeVideo );
    
    if ( ret )
        {
        CloseDialogWithDelayL();
        }
    }


// -----------------------------------------------------------------------------
// CMediaFileDialog::PreviewLNullItemL
//
// -----------------------------------------------------------------------------
//
void CMediaFileDialog::PreviewNullItemL( TInt aFolderId )
    {
    HBufC* fileName = NULL;
    TInt fileType = KErrNotFound;
    
    if ( aFolderId == EEFolderNullItemOne )
        {
        fileType = iNullItemFileTypeOne;
        fileName = iNullItemFileNameOne; 
        }
    if ( aFolderId == EEFolderNullItemTwo )
        {
        fileType = iNullItemFileTypeTwo;
        fileName = iNullItemFileNameTwo; 
        }

    if ( fileType != CMediaFileList::EMediaFileTypeAudio && 
         fileType != CMediaFileList::EMediaFileTypeVideo )
        {
        return;
        }
    if ( !fileName )
        {
        return;
        }
        
    if ( fileType == CMediaFileList::EMediaFileTypeAudio )
        {
        iAudioPreviewHandler->SetAttrL( TMFDialogUtil::EAttrFileName, *fileName );
        iAudioPreviewHandler->PlayL();
        }
    if ( fileType == CMediaFileList::EMediaFileTypeVideo )
        {
        // null item file type cannot be video
        }
    }


//------------------------------------------------------------------------------
// CMediaFileDialog::CancelPreview
//
//------------------------------------------------------------------------------
//
void CMediaFileDialog::CancelPreview()
    {
    iAudioPreviewHandler->Stop();
    iCaller->Cancel();
    }


// -----------------------------------------------------------------------------
// CMediaFileDialog::StartPreviewWithDelay
// 
// -----------------------------------------------------------------------------
//
void CMediaFileDialog::StartPreviewWithDelay()
    {
	const TInt KOneSecond = 1000;
	
	iAudioPreviewHandler->Stop();

    // automatic preview starts after delay. see HandleActiveCallL.
    iCaller->Start( EAttrPreview, KOneSecond );
    }


// -----------------------------------------------------------------------------
// CMediaFileDialog::StartSearchWithDelay
// 
// -----------------------------------------------------------------------------
//
void CMediaFileDialog::StartSearchWithDelay()
    {
    if ( iPeninputActive )
        {
        // search does not start automatically in pen input mode
        return;
        }
    
    const TInt KOneSecond = 1000;
    
    // search starts after delay. see HandleActiveCallL.
    iSearchCaller->Start( EAttrSearch, KOneSecond );
    }


// -----------------------------------------------------------------------------
// CMediaFileDialog::StartRomScan()
// 
// -----------------------------------------------------------------------------
//
void CMediaFileDialog::StartRomScan()
    {
    delete iIdle;
    iIdle = NULL;
    iIdle = CIdle::NewL( CActive::EPriorityIdle );
    
    if ( iIdle )
        {                    
        iIdle->Start( TCallBack( StartRomScaning, iMediaFileHandler ) );
        }
    }

// -----------------------------------------------------------------------------
// CMediaFileDialog::StartRomScaning()
// 
// -----------------------------------------------------------------------------
//

TInt CMediaFileDialog::StartRomScaning( TAny* aInstance )
    {
    CMediaFileHandler* handler = static_cast<CMediaFileHandler*> ( aInstance );
    handler->StartRomScaning( CMediaFileHandler::EQueryTone );
    return 0;
    }

// -----------------------------------------------------------------------------
// CMediaFileDialog::IsVideoFile
// 
// -----------------------------------------------------------------------------
//
TBool CMediaFileDialog::IsVideoFile( TInt aCurrentIndex )
    {
    if ( aCurrentIndex < 0 )
        {
        return EFalse;
        }
    
    TInt current = iState->CurrentFolder();
    
    if ( current == EEFolderVideos )
        {
        return ETrue;
        }
    
    if ( current != EEFolderSearch )
        {
        return EFalse;
        }
    
    TInt mediaType = iMediaFileHandler->Attribute( aCurrentIndex,
                                        CMediaFileHandler::EAttrMediaType,
                                        current );

    if ( mediaType == CMediaFileHandler::EMediaTypeVideo )
        {
        return ETrue;
        }
    
    return EFalse;           
    }
    

// -----------------------------------------------------------------------------
// CMediaFileDialog::IsValidVideoFileL
// 
// -----------------------------------------------------------------------------
//
TBool CMediaFileDialog::IsValidVideoFileL( TInt aCurrentIndex, TInt aIntention )
    {
    TBuf<KBufSize128> buf;
    
    if ( !IsVideoFile( aCurrentIndex ) )
        {
        return EFalse;
        }
    
    CMFProtectionHandler::TIntention intention;
    if ( aIntention == CMFProtectionHandler::EPlay )
        {
        intention = CMFProtectionHandler::EPlay;
        }
    else
        {
        intention = CMFProtectionHandler::ESelect;
        }
    GetSelectedItemFileName( aCurrentIndex, buf );
    
    if ( iProtectionHandler->IsVideoValid( buf, intention ) )
        {
        return ETrue;    
        }
    return EFalse;
    }



// ----------------------------------------------------------------------------
// CMediaFileDialog::UpdateListBoxL
// 
// Update listbox contents (currently open folder items).
// ----------------------------------------------------------------------------
//
void CMediaFileDialog::UpdateListBoxL( TBool aRestFindBox )
    {
    // prevent flickering during listbox update. ResetFindBoxL seems
    // to draw listbox many times
    iListBox->MakeVisible( EFalse );

    TRAP_IGNORE( DoUpdateListBoxL( aRestFindBox ) );
    
    iListBox->MakeVisible( ETrue );
    }


// ----------------------------------------------------------------------------
// CMediaFileDialog::DoUpdateListBoxL
// 
// Update listbox contents (currently open folder items).
// ----------------------------------------------------------------------------
//
void CMediaFileDialog::DoUpdateListBoxL( TBool aRestFindBox )
    {
    iState->SetUnknownFolderPosition( iMediaFileHandler->ResultCount() );
    
    // check current item index in case media file database has changed
    CheckListboxIndex();
    if ( aRestFindBox )
    	{
    	ResetFindBoxL();
    	iListBox->HandleItemAdditionL();
    	}
    
    TInt index = iState->CurrentItemIndex();
    if ( index >= 0 && index < iListBox->Model()->NumberOfItems() )
        {
        iListBox->SetCurrentItemIndex( index );
        }
    else
        {
        iListBox->SetCurrentItemIndex( 0 );    
        }
    
    TBuf<KBufSize128> buf;
    if ( iState->CurrentFolder() == EEFolderSearch || 
         iState->CurrentFolder() == EEFolderSearchMusic )
        {
        TInt count = iListBox->Model()->NumberOfItems();
        iState->GetSearchTitle( buf, count );
        }
    else
        {
        iState->GetTitle( iState->CurrentFolder(), buf );
//#ifdef _DEBUG
        TInt folder = iState->CurrentFolder();
        if ( iState->FindBoxNeeded() && folder != EEFolderMusic && 
             folder != EEFolderRoot )
            {
            TBuf<KBufSize128> buf2( buf );
            TInt count = iListBox->Model()->NumberOfItems();
            buf.Format( _L("%S  (%d)"), &buf2, count );
            }
//#endif
        }
    
    AknTextUtils::DisplayTextLanguageSpecificNumberConversion( buf );
    iStatusPaneHandler->SetNaviPaneTitle( buf );
   
    UpdateCbaL( 0 );
    
    DrawNow();
    
    if ( iState->QueryDirection() != EEQueryUpdate )
        {
        CancelPreview();
        }
    
    ShowDetailsPopupL();       // for search view info popup
    }


// -----------------------------------------------------------------------------
// CMediaFileDialog::UpdateCbaL
// 
// Update cba. NOTE: Commented code (menu cba) is for (possible) future use.
// -----------------------------------------------------------------------------
//
void CMediaFileDialog::UpdateCbaL( TInt /*aAttr*/ )
    {
    TInt itemCount = iListBox->Model()->NumberOfItems();
    TInt currentIndex = CurrentItemListIndex();
    TInt folderId = iState->FolderId( currentIndex );
    TInt currentFolderId = iState->CurrentFolder();
  	  	
	
	CEikButtonGroupContainer& cba = ButtonGroupContainer(); // from eikbtgpc.h
	

	// Videos folder
    if ( currentFolderId == EEFolderVideos )
        {
        if ( itemCount == 0 )
            {
            cba.SetCommandSetL( R_AVKON_SOFTKEYS_BACK );
            //cba.SetCommandSetL( R_AVKON_SOFTKEYS_OPTIONS_BACK );
            }
        else
            {
            if ( IsValidVideoFileL( currentIndex, CMFProtectionHandler::EPlay ) )
                {
                cba.SetCommandSetL( R_MEDIA_FILE_DIALOG_CBA_PREVIEW_SELECT_BACK );    
                }
            else
                {
                cba.SetCommandSetL( R_MEDIA_FILE_DIALOG_CBA_SELECT_SELECT_BACK );
                }
            
            //cba.SetCommandSetL( R_MEDIA_FILE_DIALOG_CBA_OPTIONS_CONTEXT_BACK );
            }
        return;
        }

    
    // Search folder
    if ( currentFolderId == EEFolderSearch || 
         currentFolderId == EEFolderSearchMusic )
        {
        if ( itemCount == 0 )
            {
            cba.SetCommandSetL( R_AVKON_SOFTKEYS_BACK );
            // cba.SetCommandSetL( R_AVKON_SOFTKEYS_OPTIONS_BACK );
            }
        else
            {
            if ( IsValidVideoFileL( currentIndex, CMFProtectionHandler::EPlay ) )
                {
                cba.SetCommandSetL( R_MEDIA_FILE_DIALOG_CBA_PREVIEW_SELECT_BACK );
                }
            else
                {
                cba.SetCommandSetL( R_MEDIA_FILE_DIALOG_CBA_SELECT_SELECT_BACK );
                }
                //cba.SetCommandSetL( R_MEDIA_FILE_DIALOG_CBA_OPTIONS_SELECT_BACK );
            }
        return;
        }
    
    
	// Root folder
	if ( currentFolderId == EEFolderRoot )
	    {
	    if ( folderId == EEFolderDownload || folderId == EEFolderNullItemOne ||
	         folderId == EEFolderNullItemTwo )
	        {
	        //cba.SetCommandSetL( R_MEDIA_FILE_DIALOG_CBA_OPTIONS_SELECT_BACK );
	        cba.SetCommandSetL( R_MEDIA_FILE_DIALOG_CBA_SELECT_SELECT_BACK );
	        }
	    else
	        {
	        //cba.SetCommandSetL( R_MEDIA_FILE_DIALOG_CBA_OPTIONS_OPEN_BACK );
	        cba.SetCommandSetL( R_MEDIA_FILE_DIALOG_CBA_OPEN_OPEN_BACK );
	        }
	    return;
	    }
	
	
	// other folders
    if ( itemCount == 0 )
	    {
	    // folder is empty
	    cba.SetCommandSetL( R_AVKON_SOFTKEYS_BACK );
        //cba.SetCommandSetL( R_AVKON_SOFTKEYS_OPTIONS_BACK );
        }
	else if ( folderId != KErrNotFound )
	    {
	    // list items are folders
        cba.SetCommandSetL( R_MEDIA_FILE_DIALOG_CBA_OPEN_OPEN_BACK );
        //cba.SetCommandSetL( R_MEDIA_FILE_DIALOG_CBA_OPTIONS_OPEN_BACK );
	    }
	else
	    {
	    // list items are files
        cba.SetCommandSetL( R_MEDIA_FILE_DIALOG_CBA_SELECT_SELECT_BACK );
        //cba.SetCommandSetL( R_MEDIA_FILE_DIALOG_CBA_OPTIONS_SELECT_BACK );
	    }
    }
    
    
// -----------------------------------------------------------------------------
// CMediaFileDialog::Panic
//
// -----------------------------------------------------------------------------
//
void CMediaFileDialog::Panic( TInt aReason ) 
    {
	_LIT( KPanicCategory, "CMediaFileDialog" );
	
	User::Panic( KPanicCategory, aReason ); 
    }
    

// -----------------------------------------------------------------------------
// CMediaFileDialog::ShowErrorNoteL
//
// -----------------------------------------------------------------------------
//
void CMediaFileDialog::ShowErrorNoteL( TInt aError )
	{
    TInt err = aError;
    
    if ( err == KErrNone )
        {
        return;
        }

    iBuf = KNullDesC;
    CTextResolver* tr = CTextResolver::NewLC();
	iBuf = tr->ResolveErrorString( err, CTextResolver::ECtxNoCtxNoSeparator );
  	CleanupStack::PopAndDestroy( tr );
	
	if ( iBuf.Length() > 0 )
	    {
  	    TMFDialogUtil::ShowErrorNoteL( iBuf );
	    }
	}


// -----------------------------------------------------------------------------
// CMediaFileDialog::ShowPreviewErrorNoteL
//
// -----------------------------------------------------------------------------
//
void CMediaFileDialog::ShowPreviewErrorNoteL( TInt aError, TInt aMediaType )
    {
    TInt err = KErrNone;
    
    if ( aError == KErrNoMemory || aError == KErrDiskFull )
        {
        CTextResolver* tr = CTextResolver::NewLC();
        iBuf = tr->ResolveErrorString( aError, CTextResolver::ECtxNoCtxNoSeparator );
        CleanupStack::PopAndDestroy( tr );
        err = aError;
        }
    else if ( aError == KErrNotSupported || aError == KErrCorrupt ||aError == KErrMMPartialPlayback)
        {
        if ( aMediaType == CMediaFileHandler::EMediaTypeVideo )
            {
            StringLoader::Load( iBuf, R_QTN_INFO_CLIP_FORMAT_ERROR );
            }
        else
            {
            StringLoader::Load( iBuf, R_QTN_INFO_FILE_FORMAT_ERROR );
            }
        err = aError;
        }
    
    if ( err == KErrNone )
        {
        return;
        }
    
#ifdef _DEBUG
    TMFDialogUtil::ShowErrorNoteL( iBuf, aError );
#else
    TMFDialogUtil::ShowErrorNoteL( iBuf );
#endif
    
    }


//------------------------------------------------------------------------------
// CMediaFileDialog::ShowWaitNoteL
//
//------------------------------------------------------------------------------
//
void CMediaFileDialog::ShowWaitNoteL( TBool aDatabaseChanged )
    {
    TBuf<KBufSize128> buf;
    
    if ( aDatabaseChanged )
        {
        StringLoader::Load( buf, R_QTN_WAIT_NOTE_UPDATE );
        //buf = _L("Refreshing");
        }
    else if ( iState->QueryFolder() == EEFolderSearch ||
              iState->QueryFolder() == EEFolderSearchMusic )
        {
        StringLoader::Load( buf, R_QTN_WAIT_NOTE_SEARCH );
        }
    else
        {
        StringLoader::Load( buf, R_QTN_WAIT_NOTE_OPEN );
        }
    
    iWaitNote->LaunchL( buf );     
    }


// -----------------------------------------------------------------------------
// CMediaFileDialog::DeleteMenuItem
//
// -----------------------------------------------------------------------------
//
void CMediaFileDialog::DeleteMenuItem( CEikMenuPane* aMenuPane, TInt aCommandId )
	{
	TInt pos = KErrNotFound;
    if ( aMenuPane->MenuItemExists( aCommandId, pos ) )
		{
		aMenuPane->DeleteMenuItem( aCommandId );
		}
	}


// -----------------------------------------------------------------------------
// CMediaFileDialog::ShowPopupNote
//
// -----------------------------------------------------------------------------
//
void CMediaFileDialog::ShowPopupNoteL( const TDesC& aText ) 
    {
    const TInt KPopupDisplayTime = 3*1000;
    const TInt KPopupDelayTime = 500;
    
    iPopupNote->SetTextL( aText );
    iPopupNote->SetTimeDelayBeforeShow( KPopupDelayTime );
    iPopupNote->SetTimePopupInView( KPopupDisplayTime );    
    iPopupNote->ShowInfoPopupNote();    
    }


// -----------------------------------------------------------------------------
// CMediaFileDialog::CurrentItemListIndex
// 
// -----------------------------------------------------------------------------
//
TInt CMediaFileDialog::CurrentItemListIndex()
    {
    TInt itemCount = iListBox->Model()->NumberOfItems();
    TInt currentIndex = iListBox->CurrentItemIndex();
     
    if ( itemCount == 0 || currentIndex < 0 )
        {
        return KErrNotFound; // list or find box is empty
        }
     
    // convert from findbox index into original list index
    if ( FindBox() && FindBox()->IsVisible() )
        {
        TInt tempIndex = currentIndex;
        CAknFilteredTextListBoxModel* model =
        STATIC_CAST(CAknFilteredTextListBoxModel*, iListBox->Model());
        
        if ( model->Filter() )
            {
            currentIndex = model->Filter()->FilteredItemIndex( tempIndex );
            }
        }
     
    return currentIndex;
    }


//------------------------------------------------------------------------------
// CMediaFileDialog::GetSelectedItemFileName
//
//------------------------------------------------------------------------------
//
void CMediaFileDialog::GetSelectedItemFileName( TInt aListboxIndex, TDes& aFileName )
    {
    aFileName = KNullDesC;
    
    __ASSERT_DEBUG( iState->QueryNeeded(), Panic( KErrGeneral ) );
            
    TInt id = iState->CurrentFolder();
        
    // read selected list item text from metadata    
    TInt extraIndexes = iState->LeadingPersistentFolderCount();
    TInt queryIndex = aListboxIndex - extraIndexes;

    iMediaFileHandler->GetAttribute( queryIndex, CMediaFileHandler::EAttrFullName, 
                                     aFileName, id );
    }


// -----------------------------------------------------------------------------
// CMediaFileDialog::ConstructListboxItem
// 
// -----------------------------------------------------------------------------
//
TPtrC CMediaFileDialog::ConstructListboxItem( TInt aListboxIndex )
    {
    _LIT( KFormat2, "%d\t%S\t%d" );
  
    TBool isPersistent = EFalse;
    TInt mediaTypeIcon = EEIconEmpty;
    TInt storageTypeIcon = EEIconEmpty;
    TInt folderId = KErrNotFound;
    iState->GetFolderInfo( aListboxIndex, folderId, iListboxItemText1,
                                          isPersistent, mediaTypeIcon );
    if ( folderId != KErrNotFound )
        {
        if ( !isPersistent )
            {
            GetListItemText( aListboxIndex, iListboxItemText1 );
            }
        }
    else
        {
        GetListItemText( aListboxIndex, iListboxItemText1 );
        mediaTypeIcon = MediaTypeIcon( aListboxIndex );
        storageTypeIcon = StorageTypeIcon( aListboxIndex );
        }

    iListboxItemText2.Format( KFormat2, mediaTypeIcon, &iListboxItemText1, storageTypeIcon );
    return iListboxItemText2;
    }


// -----------------------------------------------------------------------------
// CMediaFileDialog::MediaTypeIcon
// 
// -----------------------------------------------------------------------------
//
TInt CMediaFileDialog::MediaTypeIcon( TInt aListboxIndex )
    {
    if ( aListboxIndex < 0 )
        {
        return EEIconEmpty;
        }

    TInt mediaType = iMediaFileHandler->Attribute( aListboxIndex,
                                        CMediaFileHandler::EAttrMediaType,
                                        iState->CurrentFolder() );
    
    TInt iconId = EEIconMusic;
    if ( mediaType == CMediaFileHandler::EMediaTypeTone )
        {
        iconId = EEIconTone;
        }
    else if ( mediaType == CMediaFileHandler::EMediaTypeVideo )
        {
        iconId = EEIconVideo;
        }
    else if ( mediaType == CMediaFileHandler::EMediaTypeRecording )
        {
        iconId = EEIconRecording;
        }
    
    return iconId;
    }


// -----------------------------------------------------------------------------
// CMediaFileDialog::StorageTypeIcon
// 
// -----------------------------------------------------------------------------
//
TInt CMediaFileDialog::StorageTypeIcon( TInt aListboxIndex )
    {
    if ( aListboxIndex < 0 )
        {
        return EEIconEmpty;
        }

    TInt storageType = iMediaFileHandler->Attribute( aListboxIndex,
                                        CMediaFileHandler::EAttrStorageType,
                                        iState->CurrentFolder() );
    TInt iconId = EEIconEmpty;
    if ( storageType == CMediaFileHandler::EMassStorage )
        {
        iconId = EEIconMassStorage;
        }
    
    if ( storageType == CMediaFileHandler::EMemoryCard )
        {
        iconId = EEIconMemoryCard;
        }

    return iconId;
    }


// -----------------------------------------------------------------------------
// CMediaFileDialog::GetListItemText
// 
// -----------------------------------------------------------------------------
//
void CMediaFileDialog::GetListItemText( TInt aListboxIndex, TDes& aText )
    {
    __ASSERT_DEBUG( iState->QueryNeeded(), Panic( KErrGeneral ) );

    TInt id = iState->CurrentFolder();
    
    // read selected list item text from metadata    
    TInt extraIndexes = iState->LeadingPersistentFolderCount();
    TInt queryIndex = aListboxIndex - extraIndexes;
    
    iMediaFileHandler->GetAttribute( queryIndex, 
                       CMediaFileHandler::EAttrSongName, aText, id );
    }



// -----------------------------------------------------------------------------
// CMediaFileDialog::ListboxItemCount (from CMFListHandlerObserver)
// 
// -----------------------------------------------------------------------------
//
TInt CMediaFileDialog::ListboxItemCount()
    {
    TInt count1 = iState->PersistentFolderCount();
    TInt count2 = 0;
    
    if ( iState->QueryNeeded() )
        {
        count2 = iMediaFileHandler->ResultCount();
        }
    return count1 + count2;
    }


// -----------------------------------------------------------------------------
// CMediaFileDialog::HandleMFEventL (from MMediaFileHandlerObserver)
// 
// -----------------------------------------------------------------------------
//
void CMediaFileDialog::HandleMFEventL( TInt aEvent, TInt aError )
    {
    //
    // handle cancel button
    //
    if ( aEvent == MMediaFileHandlerObserver::EQueryCanceled )
        {
        iWaitNote->CancelProgressDialogL();
        iSearchText = KNullDesC;
        UpdateListBoxL();
        return;
        }
    
    //
    // handle error
    //
    if ( aError != KErrNone )
        {
        iWaitNote->CancelProgressDialogL();
        ResetDialogL( aError );

        FLOG( _L("### CMediaFileDialog::HandleMFEventL (%d) ###"), aError );
        return;
        }

    
    if ( aEvent != MMediaFileHandlerObserver::EUnknownQueryComplete )
        {
        // unknown query is always followed by another query
        iWaitNote->CancelProgressDialogL();    
        }
    
    
    if ( aEvent == MMediaFileHandlerObserver::EUnknownQueryComplete )
        {
        TInt count = iMediaFileHandler->ResultCount();
        TInt folderId = iState->QueryFolder();
        iState->SetUnknownCount( folderId, count );
        
        TInt err = KErrNone;
        TRAP( err, QueryL( folderId ) );
        if ( err != KErrNone )
            {
            iWaitNote->CancelProgressDialogL();
            User::Leave( err );
            }
        }

    if ( aEvent == MMediaFileHandlerObserver::EQueryComplete )
        {
        // query was successful - update dialog state
        iState->SetStateAfterQuery();
       
        TInt count = iMediaFileHandler->ResultCount();
        
        if ( iState->CurrentFolder() == EEFolderArtist )
            {
            TInt count = iMediaFileHandler->UnknownArtistCount();
            iState->SetUnknownCount( EEFolderArtist, count );
            }
        iState->HideUnknownFolders();
        UpdateListBoxL();
        
#ifdef _DEBUG
        iTest->PrintTimeL( _L("query took") );  // test code
#endif        
        }

    if ( aEvent == MMediaFileHandlerObserver::EMediaFileChanged )
        {
        HandleMediaFileChanged();
        }
    
    if (  aEvent == MMediaFileHandlerObserver::EScanRomComplete )
        {
        iWaitNote->CancelProgressDialogL(); 
        }
    
    if ( aEvent == MMediaFileHandlerObserver::EInitComplete )
        {
        iDatabaseOpen = ETrue;
    	
    	StartRomScan();
        }
    }


// -----------------------------------------------------------------------------
// CMediaFileDialog::HandleMediaFileChanged
// 
// Handle situation where file system changes and media files are added /
// removed. This function makes a query to update current folder and
// stores name or id of the currently selected item. Current item info
// is needed to restore current item after query. 
// -----------------------------------------------------------------------------
//
void CMediaFileDialog::HandleMediaFileChanged()
    {
    // query is ongoing - ignore this file change event
    if ( !iMediaFileHandler->QueryReady() )
         {
         FLOG( _L("### CMediaFileDialog::HandleMediaFileChanged: query in progress ###") );
         return;
         }
    
    iState->ResetUnknownCount();
    TMFCurrentItemInfo& info = iState->CurrentItemInfo();
    info.Reset();
    info.iIndexCheckNeeded = ETrue; //  set to false when dialog is opened
        
    TInt currentFolder = iState->CurrentFolder();
    TInt currentIndex = CurrentItemListIndex();
         
    TBool isPersistent = EFalse;
    TInt iconId = EEIconEmpty;
    TInt id = KErrNotFound;
    TBuf<KBufSize64> buf;
    iState->GetFolderInfo( currentIndex, id, buf,
                                         isPersistent, iconId );
    if ( id == KErrNotFound )
        {
        // get media file id
        info.iMediaFileId = iMediaFileHandler->Attribute( currentIndex, 
                 CMediaFileHandler::EAttrMediaFileId, currentFolder );
        }
    else if ( !isPersistent )
        {
        // get non-persistent folder name (eg some album name)
        GetListItemText( currentIndex, info.iFolderText );
        }
    else
        {
        // get persistent folder id (eg "Unknown" folder)
        info.iFolderId = id;
        }

    info.iItemIndex = currentIndex;
       
    iState->SetQueryInfo( 0, KNullDesC, 0, EEQueryUpdate );
        
    TBool databaseChanged = ETrue;
    QueryL( currentFolder, databaseChanged );
    }


// -----------------------------------------------------------------------------
// CMediaFileDialog::CheckListboxIndex
// 
// Check that current item selection is correct. Function is used in case
// file system has changed (media files added / removed.
// -----------------------------------------------------------------------------
//
void CMediaFileDialog::CheckListboxIndex()
    {
    TMFCurrentItemInfo& info = iState->CurrentItemInfo();
    if ( !info.iIndexCheckNeeded )
        {
        return;
        }
    
    TInt direction = iState->QueryDirection();
    TInt newListIndex = KErrNotFound;
    
    if ( direction == EEQueryUp )
        {
        FLOG( _L("query up") );
        TInt prevId = iState->PreviousFolderId();
        TInt listIndex = iState->ListIndex( prevId );
        if ( !iState->IsPersistent( prevId ) )
            {
            iState->GetText( prevId, info.iFolderText );
            TInt num = iState->LeadingPersistentFolderCount();
            newListIndex = iMediaFileHandler->ItemIndex(
                           info.iFolderText, listIndex );
            iState->SetCurrentItemIndex( newListIndex + num );
            }
        else
            {
            iState->SetCurrentItemIndex( listIndex );
            }
        }

    if ( direction == EEQueryUpdate )
        {
        if ( info.iMediaFileId != KErrNotFound )
            {
            newListIndex = iMediaFileHandler->ItemIndex(
                           info.iMediaFileId, info.iItemIndex );
            }
        else if ( info.iFolderText.Length() > 0 )
            {
            newListIndex = iMediaFileHandler->ItemIndex(
                    info.iFolderText, info.iItemIndex );
            TInt num = iState->LeadingPersistentFolderCount();
            newListIndex = newListIndex + num;
            }
        else if ( info.iFolderId != KErrNotFound )
            {
            newListIndex = iState->ListIndex( info.iFolderId );
            }
        
        iState->SetCurrentItemIndex( newListIndex );
        }
    }


// -----------------------------------------------------------------------------
// CMediaFileDialog::HandlePreviewEventL (from MPreviewHandlerObserver)
// 
// -----------------------------------------------------------------------------
//
void CMediaFileDialog::HandlePreviewEventL( TInt aEvent, TInt aError )
    {
    if ( aEvent == MPreviewHandlerObserver::EAudioPreviewComplete )
        {
        ShowPreviewErrorNoteL( aError, CMediaFileHandler::EMediaTypeMusic );
        }

    if ( aEvent == MPreviewHandlerObserver::EVideoPreviewComplete )
        {
        ShowPreviewErrorNoteL( aError, CMediaFileHandler::EMediaTypeVideo );        
        }

    if ( aEvent == MPreviewHandlerObserver::EPreviewError )
        {
        FLOG( _L("### CMediaFileDialog::HandlePreviewEventL (%d) ###"), aError );
        
        ShowPreviewErrorNoteL( aError, CMediaFileHandler::EMediaTypeMusic );        
        }
    }


// -----------------------------------------------------------------------------
// CMediaFileDialog::HandleLosingForeground (from MCoeForegroundObserver)
//
// -----------------------------------------------------------------------------
//
void CMediaFileDialog::HandleLosingForeground()
    {
    iForeGround = EFalse;
    //Record the current item info
    if ( iState && iListBox)
    	{
    	iState->SetCurrentItemIndex( CurrentItemListIndex() );
    	}
    
    CancelPreview();
    }


// -----------------------------------------------------------------------------
// CMediaFileDialog::HandleGainingForeground (from MCoeForegroundObserver)
//
// -----------------------------------------------------------------------------
//
void CMediaFileDialog::HandleGainingForeground()
    {
    iForeGround = ETrue;
    if ( iListBox )
    	{
    	UpdateListBoxL( EFalse );
   	 	}
    }


// -----------------------------------------------------------------------------
// CMediaFileDialog::HandleControlEventL (from MCoeControlObserver)
// 
// -----------------------------------------------------------------------------
//
void CMediaFileDialog::HandleControlEventL( CCoeControl* aControl,
                                                      TCoeEvent aEventType )
     {
     TInt currentFolderId = iState->CurrentFolder();
     
     if ( aEventType == EEventStateChanged )
         {
         CancelPreview();
         if ( aControl == iListBox )
             {
             if ( currentFolderId == EEFolderRoot   ||
                  currentFolderId == EEFolderSearch || 
                  currentFolderId == EEFolderVideos )
                {
                UpdateCbaL( 0 );  // only these folders need dynamic cba update
                }
             StartPreviewWithDelay();
             ShowDetailsPopupL();   // show search details in Search view
   
             // forward listbox state change events to this control's
             // observers
             ReportEventL( MCoeControlObserver::EEventStateChanged );
             }
           
         if ( FindBox() && aControl == FindBox() )
             {
             User::InfoPrint(_L("find box  event arrived"));
              
             // this event is received when user enters or deletes
             // chars in find box. Backspace key in empty find box does not
             // generate this event.

             if ( currentFolderId == EEFolderSearch ||
                  currentFolderId == EEFolderRoot ||
                  currentFolderId == EEFolderMusic ||
                  currentFolderId == EEFolderSearchMusic )
                 {
                 StartSearchWithDelay();
                 return;
                 }
            
             // StartPreviewWithDelay();
             
             UpdateCbaL( 0 );
             
//#ifdef _DEBUG
             TBuf<KBufSize128> buf;
             iState->GetTitle( iState->CurrentFolder(), buf );
             TBuf<KBufSize128> buf2( buf );
             TInt count = iListBox->Model()->NumberOfItems();
             buf.Format( _L("%S  (%d)"), &buf2, count );
             iStatusPaneHandler->SetNaviPaneTitle( buf );
//#endif
            
             // Forward listbox state change events to this control's
             // observers
             ReportEventL( MCoeControlObserver::EEventStateChanged );
             }
         }
     }


// -----------------------------------------------------------------------------
// CMediaFileDialog::HandleWaitNoteL (from MMediaFileWaitNoteObserver)
// 
// -----------------------------------------------------------------------------
//
void CMediaFileDialog::HandleWaitNoteL( TInt /*aButtonId*/ )
    {
    iMediaFileHandler->CancelQuery();
    }


// -----------------------------------------------------------------------------
// CMediaFileDialog::AdaptiveSearchTextChanged (from MAdaptiveSearchTextObserver)
// 
// -----------------------------------------------------------------------------
//
void CMediaFileDialog::AdaptiveSearchTextChanged( CAknSearchField* /*aSearchField*/ )
    {
    // not used
    }


// -----------------------------------------------------------------------------
// CMediaFileDialog::OnPeninputUiDeactivated
// 
// -----------------------------------------------------------------------------
//
void CMediaFileDialog::OnPeninputUiDeactivated()
    {
    iPeninputActive = EFalse;
    
    if ( FindBox()->TextLength() == 0 && 
          ( iState->CurrentFolder() == EEFolderSearch ||
            iState->CurrentFolder() == EEFolderSearchMusic ) )
        {
        HandleBackL();
        }
    else
        {
        // search starts immediately after user closes pen input
        TRAP_IGNORE( StartSearchWithDelay() );
        }
    }


// -----------------------------------------------------------------------------
// CMediaFileDialog::OnPeninputUiActivated
// 
// -----------------------------------------------------------------------------
//
void CMediaFileDialog::OnPeninputUiActivated()
    {
    iPeninputActive = ETrue;
    CancelPreview();
    }


// -----------------------------------------------------------------------------
// CMediaFileDialog::HandleListBoxEventL (from MEikListBoxObserver)
// 
// -----------------------------------------------------------------------------
//
void CMediaFileDialog::HandleListBoxEventL( CEikListBox* /*aListBox*/,
                                             TListBoxEvent aEventType )
    {
    TInt current = iState->CurrentFolder();
    if ( aEventType == EEventPenDownOnItem)
         {
         iListBox->View()->ItemDrawer()->SetFlags( CListItemDrawer::EPressedDownState );
         return;
         }
    
    if ( aEventType == EEventItemClicked )
        {
        	  iIsDoubleClicked = EFalse;
        if ( current != KErrNotFound )
            {
            HandleOKL( EAttrOpen );  // open folder item
            }
        return;
        }

    if ( aEventType == EEventItemSingleClicked  || aEventType == EEventEnterKeyPressed )
        {
        TBool closeDialog = HandleOKL( EAttrDoubleClicked );
        
        if ( closeDialog )
            {
            // close after short delay (dialog cannot be closed from this function)
            CloseDialogWithDelayL();
            }
        iIsDoubleClicked = ETrue;
        }
    if ( aEventType == EEventItemDraggingActioned )
    	{
    	UpdateCbaL( KErrNone );
    	DrawNow();
    	}
    }


// -----------------------------------------------------------------------------
// CMediaFileDialog::CloseDialogWithDelayL
//
// -----------------------------------------------------------------------------
//
void CMediaFileDialog::CloseDialogWithDelayL()
    {    
    delete iIdle;
    iIdle = NULL;
    iIdle = CIdle::NewL( CActive::EPriorityIdle );
    
    if ( iIdle )
        {                    
        iIdle->Start( TCallBack( CloseDialog, this ) );
        }
    delete iInputBlock;
    iInputBlock = NULL;
    iInputBlock = CAknInputBlock::NewLC();
    CleanupStack::Pop( iInputBlock );
    }


// -----------------------------------------------------------------------------
// CMediaFileDialog::CloseDialog
//
// -----------------------------------------------------------------------------
//
TInt CMediaFileDialog::CloseDialog( TAny *aObj )
    {    
    CMediaFileDialog *dlg = (CMediaFileDialog*) aObj;
    delete dlg->iInputBlock;
    dlg->iInputBlock = NULL;
    
    TRAP_IGNORE( dlg->TryExitL( EAknSoftkeyOk ) );
    
    return 0;
    }


// -----------------------------------------------------------------------------
// CMediaFileDialog::HandleActiveCallL (from MMFActiveCallerObserver)
//
// -----------------------------------------------------------------------------
//
void CMediaFileDialog::HandleActiveCallL( TInt aCallId )
    {
    if ( aCallId == EAttrPreview )
        {
        //double click the item of the listbox to select the tones, do not preview it.
        if ( iIsDoubleClicked )
        	{
        	iIsDoubleClicked = EFalse;
        	}
        else
        	{
            // start preview after delay
            PreviewL( CMediaFileList::EMediaFileTypeAudio ); 
        	}
        }

    if ( aCallId == EAttrSearch )
        {
        // start search after delay
        SearchL();    
        }
    }




/******************************************************************************
 *  class CMFListHandler
 ******************************************************************************/

// -----------------------------------------------------------------------------
// CMFListHandler::NewL
// 
// -----------------------------------------------------------------------------
//
CMFListHandler* CMFListHandler::NewL()
    {
	CMFListHandler* self = new (ELeave) CMFListHandler();
    CleanupStack::PushL(self);
	self->ConstructL();
    CleanupStack::Pop(self);
	return self;
    }


// -----------------------------------------------------------------------------
// CMFListHandler::~CMFListHandler
// 
// -----------------------------------------------------------------------------
//
CMFListHandler::~CMFListHandler()
    {
    }


// -----------------------------------------------------------------------------
// CMFListHandler::ConstructL
// 
// -----------------------------------------------------------------------------
//
void CMFListHandler::ConstructL( void )
    {
    }


// -----------------------------------------------------------------------------
// CMFListHandler::CMFListHandler
// 
// -----------------------------------------------------------------------------
//
CMFListHandler::CMFListHandler()
    {
    }


// -----------------------------------------------------------------------------
// CMFListHandler::SetObserver
// 
// -----------------------------------------------------------------------------
//
void CMFListHandler::SetObserver( CMFListHandlerObserver* aObserver )
    {
    iObserver = aObserver;
    }
    
    
// -----------------------------------------------------------------------------
// CMFListHandler::MdcaCount (from MDesCArray)
// 
// -----------------------------------------------------------------------------
//
TInt CMFListHandler::MdcaCount() const
    {
    return iObserver->ListboxItemCount();
    }


// -----------------------------------------------------------------------------
// CMFListHandler::MdcaPoint (from MDesCArray)
//
// CEikListBox calls this function just before it draws list box item.
// -----------------------------------------------------------------------------
//
TPtrC16 CMFListHandler::MdcaPoint( TInt aIndex ) const
    {
    return iObserver->ConstructListboxItem( aIndex );
    }


// -----------------------------------------------------------------------------
// CMFListHandler::Panic
//
// -----------------------------------------------------------------------------
//
void CMFListHandler::Panic( TInt aReason ) 
    {
	_LIT( KPanicCategory, "CMFListHandler" );
	
	User::Panic( KPanicCategory, aReason ); 
    }








//  End of File  
