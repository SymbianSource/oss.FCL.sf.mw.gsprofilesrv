/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of the CFLDPopupList.
*
*/



// CLASS HEADER
#include    "CFLDPopupList.h"

// INTERNAL INCLUDES
#include    "CFLDFileListModel.h"
#include    "MFLDFileObserver.h"
#include    "MFLDFileListBoxObserver.h"

// EXTERNAL INCLUDES
#include <AknIconArray.h>
#include <aknmemorycardui.mbg>
#include <AknsUtils.h>
#include <gulicon.h>
#include <aknconsts.h> // KAvkonBitmapFile, KAvkonVariatedBitmapsFile
#include <variatedbitmaps.mbg> // MBM indices for seamless link
#include <avkon.mbg>
#include <data_caging_path_literals.hrh>
#include <aknlists.h>
#include <commondialogs.mbg>
#include <filelist.mbg>

// CONSTANTS
namespace
	{
    _LIT( KFLDMassStorageBitmapFile, "z:filelist.mbm" );
    _LIT( KFLDMemoryCardUiBitmapFile, "z:aknmemorycardui.mbm" );

	const TInt KIconGranularity( 3 );
	}


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CFLDPopupList::CFLDPopupList
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CFLDPopupList::CFLDPopupList(
	MFLDFileObserver& aFileObserver,
    MFLDFileListBoxObserver* aListBoxObserver,
	CFLDFileListModel& aModel )
	:	CAknPopupList(),
		iFileObserver( aFileObserver ),
        iListBoxObserver( aListBoxObserver ),
		iModel( aModel ),
		iPoint(-1,-1)
    {
    }


void CFLDPopupList::ConstructL(
 CEikListBox* aListBox, TInt aCbaResource, AknPopupLayouts::TAknPopupLayouts aType )
	{
	// Notify foreground lost
    CCoeEnv* coeEnv = CCoeEnv::Static();
    coeEnv->AddForegroundObserverL( *this );
    coeEnv->AddFocusObserverL( *this );
    
    CAknPopupList::ConstructL( aListBox, aCbaResource, aType );
	}

// -----------------------------------------------------------------------------
// CFLDPopupList::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CFLDPopupList* CFLDPopupList::NewL(
	CEikListBox* aListBox,
	TInt aCbaResource,
	MFLDFileObserver& aFileObserver,
    MFLDFileListBoxObserver* aListBoxObserver,
	CFLDFileListModel& aModel,
	AknPopupLayouts::TAknPopupLayouts aType )
	{
	CFLDPopupList* self = new( ELeave )
        CFLDPopupList( aFileObserver, aListBoxObserver, aModel );
	CleanupStack::PushL( self );
	self->ConstructL( aListBox, aCbaResource, aType );
	CleanupStack::Pop( self );
	return self;
	}

// Destructor
 CFLDPopupList::~CFLDPopupList()
    {
    CCoeEnv::Static()->RemoveForegroundObserver( *this );
    CCoeEnv::Static()->RemoveFocusObserver( *this );
    }
    
 // -----------------------------------------------------------------------------
 // CFLDPopupList::HandlePointerEventL
 // (other items were commented in a header).
 // -----------------------------------------------------------------------------
 //
 void CFLDPopupList::HandlePointerEventL(const TPointerEvent& aPointerEvent)
	 {
	 if ( aPointerEvent.iType == TPointerEvent::EButton1Down)
		 {
		 iPoint = aPointerEvent.iPosition;
		 CancelPreview();
		 }
	 if ( Rect().Contains( iPoint ) )
		 {
		 CCoeControl::HandlePointerEventL( aPointerEvent );
		 }
	 else
		 {
		 AttemptExitL(EFalse);    
		 }
	 }
// -----------------------------------------------------------------------------
// CFLDPopupList::AttemptExitL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDPopupList::AttemptExitL( TBool aAccept )
	{
	if( iExitChecksDone )
	    {
	    CAknPopupList::AttemptExitL( aAccept );
	    return;
	    }

	TFileName fileName( KNullDesC );
	iModel.GetFileName( fileName, iListBox->CurrentItemIndex() );

#ifdef RD_VIDEO_AS_RINGING_TONE
	// Get current softkey state
	if( iListBoxObserver )
    	{
		iListBoxObserver->HandleSoftKeyState( iSoftKeyState );
    	}
	TBool isVideo( EFalse );
	if ( iModel.MediaFileType( fileName ) == ECLFMediaTypeVideo )
		{
		isVideo = ETrue;
		}
#endif

	if( aAccept )
		{
		// User made a selection (didn't press cancel).

#ifdef RD_VIDEO_AS_RINGING_TONE
		if( isVideo )
			{
			if( iSoftKeyState ==
			 MFLDFileListBoxObserver::EToneSelectionSoftKeyState )
				{
				// Cancel listbox timer
        		if( iListBoxObserver )
            		{
            		iListBoxObserver->HandleFileListBoxEventL(
                		MFLDFileListBoxObserver::EOtherKeyEvent,
                		fileName );
            		}
				}
			else
				{
				// No need to cancel timer
				// (it has been already cancelled)
				}
			}
		else
			{
        	// Cancel listbox timer
        	if( iListBoxObserver )
            	{
            	iListBoxObserver->HandleFileListBoxEventL(
                	MFLDFileListBoxObserver::EOtherKeyEvent,
                	fileName );
            	}
			}
#else
		// Cancel listbox timer
        if( iListBoxObserver )
        	{
            iListBoxObserver->HandleFileListBoxEventL(
            	MFLDFileListBoxObserver::EOtherKeyEvent,
                fileName );
            }
#endif

#ifdef RD_VIDEO_AS_RINGING_TONE
	if( !isVideo )	// Only perform file validity check for audio files here
		{
		// Ask the observer if the file is valid.
        if( !iFileObserver.IsFileValidL(
         fileName, MFLDFileObserver::ESelect ) )
			{
            // Not ok to close the pop-up.
            // Return. Don't call CAknPopupList::AttemptExitL.
			return;
			}
		}

#else
        // Ask the observer if the file is valid.
    if( !iFileObserver.IsFileValidL(
     fileName, MFLDFileObserver::ESelect ) )
    	{
	    // Not ok to close the pop-up.
        // Return. Don't call CAknPopupList::AttemptExitL.
		return;
		}
#endif


#ifdef RD_VIDEO_AS_RINGING_TONE
		if( iListBoxObserver )
			{
			// Ask the observer if the file is video
			if( isVideo )
				{// File is a video

				// Change the softkeys back to normal
				iListBoxObserver->HandleFileListBoxEventL(
 						MFLDFileListBoxObserver::EVideoPreviewCanceled,
 						 fileName );
 						
				if( iSoftKeyState
				 == MFLDFileListBoxObserver::EToneSelectionSoftKeyState )
			 		{	// File is a video and we are in tone selection list

			 		// Ask the observer if the file is valid.
        			if( !iFileObserver.IsFileValidL(
        			 fileName, MFLDFileObserver::ESelect ) )
        				{
						// Not ok to close the pop-up.
            			// Return. Don't call CAknPopupList::AttemptExitL.
						return;
        				}
			 		}
			 	else if( iSoftKeyState
			 	 == MFLDFileListBoxObserver::EPreviewSoftKeyState )
			 		{	// File is a video and it has been focused for 1s

			 		// Ask the observer if the file is valid.
        			if( !iFileObserver.IsFileValidL(
        			 fileName, MFLDFileObserver::EPlay ) )
        				{
						// Not ok to close the pop-up.
            			// Return. Don't call CAknPopupList::AttemptExitL.
						return;
        				}

			 		iListBoxObserver->HandleFileListBoxEventL(
                		MFLDFileListBoxObserver::EVideoPreview,
                		fileName );
                	
			 		// Disable scrollbar
			 		ListBox()->ScrollBarFrame()->VerticalScrollBar()->SetDimmed( ETrue );

			 		// Remove the focus from listbox
                	ListBox()->SetDimmed( ETrue );

                	// Not ok to close the pop-up.
            		// Return. Don't call CAknPopupList::AttemptExitL.
                	return;
			 		}
			 	else if( iSoftKeyState
			 	 == MFLDFileListBoxObserver::EPreviewSelectSoftKeyState )
			 		{	// File is a video and it is in preview state

			 		// Ask the observer if the file is valid.
        			if( !iFileObserver.IsFileValidL(
        			 fileName, MFLDFileObserver::ESelect ) )
						{
						// When video screen is closed,
						// listbox must be forced to be refreshed
						// ( otherwise scrollbar is not redrawn)
						ListBox()->SetDimmed( EFalse );
	                    ListBox()->ScrollBarFrame()->VerticalScrollBar()->SetDimmed( EFalse );
						DrawNow( CEikonEnv::Static()->EikAppUi()->ApplicationRect() );
						// Not ok to close the pop-up.
            			// Return. Don't call CAknPopupList::AttemptExitL.
						return;
						}

					// When video screen is closed, listbox must be refreshed
					DrawDeferred();
	            	}
				}
			}
		}
	else	// 	if( aAccept )
		{
		// User canceled selection
		if( iListBoxObserver )
			{
			if( isVideo )
				{	// File is a video
				
				// Change the softkeys back to normal
				iListBoxObserver->HandleFileListBoxEventL(
	       			MFLDFileListBoxObserver::EVideoPreviewCanceled,
	       			 fileName );
				
	 			if( iSoftKeyState ==
	 			 MFLDFileListBoxObserver::EPreviewSelectSoftKeyState )
					{	// File is a video and it is in preview state

					// When video screen is closed,
					// listbox must be forced to be refreshed
					// ( otherwise scrollbar is not redrawn)
					ListBox()->SetDimmed( EFalse );
                    ListBox()->ScrollBarFrame()->VerticalScrollBar()->SetDimmed( EFalse );
					DrawNow( CEikonEnv::Static()->EikAppUi()->ApplicationRect() );
					// Not ok to close the pop-up.
            		// Return. Don't call CAknPopupList::AttemptExitL.
					return;
					}
	 			}
			}
#endif

		}

	iExitChecksDone = ETrue;
	CAknPopupList::AttemptExitL( aAccept );
	}

// -----------------------------------------------------------------------------
// CFLDPopupList::HandleResourceChange
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDPopupList::HandleResourceChange( TInt aType )
    {
#ifdef RD_VIDEO_AS_RINGING_TONE
    if( aType == KEikDynamicLayoutVariantSwitch ||
         aType == KAknsMessageSkinChange )
        {
		if( iListBoxObserver )
			{
 			TRAP_IGNORE( iListBoxObserver->HandleFileListBoxEventL(
 				MFLDFileListBoxObserver::EVideoPreviewCanceled ) );
 		    if( ListBox()->IsFocused() )
 		        {
 		        ListBox()->SetDimmed( EFalse );
                ListBox()->ScrollBarFrame()->VerticalScrollBar()->SetDimmed( EFalse );
 		        }
			}
        }
#endif
    if( aType == KAknsMessageSkinChange )
        {
        TRAP_IGNORE( PopulateIconArrayL() );
        }

    CAknPopupList::HandleResourceChange( aType );
    }

// -----------------------------------------------------------------------------
// CFLDPopupList::GetFileListBoxObserver
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MFLDFileListBoxObserver* CFLDPopupList::GetFileListBoxObserver()
	{
	return iListBoxObserver;
	}

// -----------------------------------------------------------------------------
// CFLDPopupList::HandleListBoxEventL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDPopupList::HandleListBoxEventL( CEikListBox* aListBox,
	TListBoxEvent aEventType )
    {
	switch( aEventType )
		{
		// Special handling for touch: first click just starts preview;
		// second click (double click) will select the tone.
		// Fall through.
		case EEventItemClicked:
			{
			iModel.GetFileName( iCurrentFileName, iListBox->CurrentItemIndex() );
			iListBoxObserver->HandleFileListBoxEventL( 
				MFLDFileListBoxObserver::EFocusChanged, iCurrentFileName );
			return;
			}

		case EEventItemDoubleClicked:
			AttemptExitL( ETrue );
			return;

		default:
			break;
		}

	CAknPopupList::HandleListBoxEventL( aListBox, aEventType );
    }

// -----------------------------------------------------------------------------
// CFLDPopupList::CancelPreview
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDPopupList::CancelPreview()
    {
    if( iListBoxObserver )
    	{
        TRAP_IGNORE( iListBoxObserver->HandleFileListBoxEventL(
         				MFLDFileListBoxObserver::EOtherKeyEvent, KNullDesC ) );
    	}
    }

// -----------------------------------------------------------------------------
// CFLDPopupList::PopupListWindow::
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
RWindow& CFLDPopupList::PopupListWindow() const
	{
	return Window();
	}

// -----------------------------------------------------------------------------
// CFLDPopupList::HandleGainingForeground
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDPopupList::HandleGainingForeground()
	{
	TRAP_IGNORE( ListBox()->HandleItemAdditionL() );
	}
// -----------------------------------------------------------------------------
// CFLDPopupList::HandleLosingForeground
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDPopupList::HandleLosingForeground()
	{
    if( iListBoxObserver )
        {
        iListBoxObserver->HandleSoftKeyState( iSoftKeyState );
        if( iSoftKeyState == MFLDFileListBoxObserver::EPreviewSelectSoftKeyState )
            {
            TRAP_IGNORE( iListBoxObserver->HandleFileListBoxEventL(
                MFLDFileListBoxObserver::EVideoPreviewCanceled, KNullDesC ) );
            DrawNow();
            }

        // Set the vertical scroll bar and listbox to not dimmed.        
        if ( ListBox()->IsFocused() )
            {
            ListBox()->SetDimmed( EFalse );
            ListBox()->ScrollBarFrame()->VerticalScrollBar()->SetDimmed( EFalse );
            }
        }
	}	

// -----------------------------------------------------------------------------
// CFLDPopupList::HandleChangeInFocus
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDPopupList::HandleChangeInFocus()
	{
	if( ListBox()->IsFocused() )
		{
		ListBox()->SetDimmed( EFalse );
		}
	}	
// -----------------------------------------------------------------------------
// CFLDPopupList::HandleDestructionOfFocusedItem
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDPopupList::HandleDestructionOfFocusedItem()
	{
	}	

// -----------------------------------------------------------------------------
// CFLDPopupList::PopulateIconArrayL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CFLDPopupList::PopulateIconArrayL()
    {
    CAknSingleGraphicBtPopupMenuStyleListBox* listBox = 
    	static_cast<CAknSingleGraphicBtPopupMenuStyleListBox*>( ListBox() );

	if( !listBox )
		{
		return;
		}

    // Create new icon array
    CArrayPtr<CGulIcon>* iconArray = new( ELeave ) CAknIconArray( KIconGranularity );
    CleanupStack::PushL( iconArray );

    MAknsSkinInstance* skin = AknsUtils::SkinInstance();

    //    
    // Create an empty icon in icon array position 0
    // 
    CGulIcon* icon =
        AknsUtils::CreateGulIconL( skin, KAknsIIDQgnPropEmpty, KAvkonBitmapFile,
        EMbmAvkonQgn_prop_empty, EMbmAvkonQgn_prop_empty_mask  );
    CleanupStack::PushL( icon );
    iconArray->AppendL( icon );
    CleanupStack::Pop( icon );

    //
    // Create an URL icon for download tones in icon array position 1
    //
    icon = AknsUtils::CreateGulIconL( skin, KAknsIIDQgnPropLinkEmbdSmall,
        KAvkonVariatedBitmapsFile,
        EMbmVariatedbitmapsQgn_prop_link_embd_small,
        EMbmVariatedbitmapsQgn_prop_link_embd_small_mask );
    CleanupStack::PushL( icon );
    iconArray->AppendL( icon );
    CleanupStack::Pop( icon );

    //
    // Create an memory card icon in icon array position 2
    //		
	TParse* fp = new(ELeave) TParse();
	fp->Set(KFLDMemoryCardUiBitmapFile, &KDC_APP_BITMAP_DIR, NULL);
	TFileName resourceFileName( fp->FullName() );
	delete fp;
        
    TRgb defaultColour( KRgbBlack );
    CFbsBitmap* bmap = NULL;
    CFbsBitmap* mask = NULL;

    AknsUtils::GetCachedColor( skin, defaultColour,
               KAknsIIDQsnIconColors, EAknsCIQsnIconColorsCG13 );
    AknsUtils::CreateColorIconLC( skin, KAknsIIDQgnPropLinkEmbdSmall,
                KAknsIIDQsnIconColors, EAknsCIQsnIconColorsCG13, bmap, mask,
                resourceFileName, EMbmAknmemorycarduiQgn_indi_mmc_add,
                EMbmAknmemorycarduiQgn_indi_mmc_add_mask,
                defaultColour );
    icon = CGulIcon::NewL( bmap, mask );
    icon->SetBitmapsOwnedExternally( EFalse );
    CleanupStack::Pop( 2 ); // icon owns the bitmaps now
        
    CleanupStack::PushL( icon );
    iconArray->AppendL( icon );
    CleanupStack::Pop( icon );
     
    //
    // Create an audio file icon in icon array position 3
    //
    icon = AknsUtils::CreateGulIconL( skin, KAknsIIDQgnPropFmgrFileSound,
        KCommonDialogsBitmapFile,
        EMbmCommondialogsQgn_prop_fmgr_file_sound,
        EMbmCommondialogsQgn_prop_fmgr_file_sound_mask );
    CleanupStack::PushL( icon );
    iconArray->AppendL( icon );
    CleanupStack::Pop( icon );

    //
    // Create a video file icon in icon array position 4
    //
    icon = AknsUtils::CreateGulIconL( skin, KAknsIIDQgnPropFmgrFileVideo,
        KCommonDialogsBitmapFile,
        EMbmCommondialogsQgn_prop_fmgr_file_video,
        EMbmCommondialogsQgn_prop_fmgr_file_video_mask );
    CleanupStack::PushL( icon );
    iconArray->AppendL( icon );
    CleanupStack::Pop( icon );

    //
    // create mass storage icon in icon array position 5
    //
	TParse* fp2 = new(ELeave) TParse();
	fp2->Set(KFLDMassStorageBitmapFile, &KDC_APP_BITMAP_DIR, NULL);
	resourceFileName = fp2->FullName();
	delete fp2;

    TRgb defaultColour2( KRgbBlack );
    CFbsBitmap* bmap2 = NULL;
    CFbsBitmap* mask2 = NULL;

    AknsUtils::GetCachedColor( skin, defaultColour2,
               KAknsIIDQsnIconColors, EAknsCIQsnIconColorsCG13 );
    AknsUtils::CreateColorIconLC( skin, KAknsIIDQgnPropLinkEmbdSmall,
                KAknsIIDQsnIconColors, EAknsCIQsnIconColorsCG13, bmap2, mask2,
                resourceFileName, EMbmFilelistQgn_indi_fmgr_ms_add,
                EMbmFilelistQgn_indi_fmgr_ms_add_mask,
                defaultColour2 );
    
    icon = CGulIcon::NewL( bmap2, mask2 );
    icon->SetBitmapsOwnedExternally( EFalse );
    CleanupStack::Pop( 2 ); // icon owns the bitmaps now
        
    CleanupStack::PushL( icon );
    iconArray->AppendL( icon );
    CleanupStack::Pop( icon );


    // Delete old icons
    CArrayPtr<CGulIcon>* arr = listBox->ItemDrawer()->ColumnData()->IconArray();
    if( arr )
    	{
    	arr->ResetAndDestroy();
	    delete arr;
	    arr = NULL;
    	}

	listBox->ItemDrawer()->ColumnData()->SetIconArrayL( iconArray );
	CleanupStack::Pop( iconArray );

    }


//  End of File
