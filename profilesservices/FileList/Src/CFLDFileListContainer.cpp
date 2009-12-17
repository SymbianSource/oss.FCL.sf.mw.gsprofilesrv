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
* Description: 
*     A wrapper class, which is part of an example implementation of tone
*     selection list. It owns the list box, the model, and the controller,
*     and it is used to launch the popup list.
*     If you need to use a different list box style or a different file,
*     processor replace this class with your own container.
*
*
*/


// CLASS HEADER
#include "CFLDFileListContainer.h"

// INTERNAL INCLUDES
#include "CFLDPopupList.h"
#include "CFLDDRMImplementation.h"
#include "CFLDBrowserLauncher.h"
#include "CFLDSingleGraphicEntryFormatter.h"
#include "FLDListBoxTemplate.h"
#include "CFLDController.h"
#include "CFLDFileListModel.h"

// EXTERNAL INCLUDES
#include <ConeResLoader.h>
#include <StringLoader.h>
#include <FileList.rsg>
#include <featmgr.h>
#include <pathinfo.h>
#include <centralrepository.h>
#include <ProfileEngineInternalCRKeys.h> // Profiles engine internal Central Repository keys
#include <aknlists.h>

// CONSTANTS
namespace
	{
	// MIME types
	_LIT( KFLDAMRMimeType, "audio/amr" );
#ifndef __WMA
	_LIT( KFLDAudioWMA, "audio/x-ms-wma" );
#endif

    _LIT( KFLDResourceFileName, "FileList.RSC" );

	// Default delay 1000000 = 1sec
	const TInt KDefaultDelay( 1000000 );
	}

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CFLDFileListContainer::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CFLDFileListContainer* CFLDFileListContainer::NewL()
    {
    CFLDFileListContainer* self = CFLDFileListContainer::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CFLDFileListContainer::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CFLDFileListContainer* CFLDFileListContainer::NewLC()
    {
    CFLDFileListContainer* self =
        new( ELeave ) CFLDFileListContainer();
    CleanupStack::PushL( self );
    self->ConstructL( R_FLD_LIST_MODEL, R_FLD_DIRECTORIES );
    return self;
    }

// -----------------------------------------------------------------------------
// CFLDFileListContainer::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CFLDFileListContainer* CFLDFileListContainer::NewL(
    const TInt aResourceId )
    {
    CFLDFileListContainer* self = CFLDFileListContainer::NewLC(
        aResourceId, R_FLD_DIRECTORIES );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CFLDFileListContainer::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CFLDFileListContainer* CFLDFileListContainer::NewLC(
    const TInt aResourceId )
    {
    CFLDFileListContainer* self =
        new( ELeave ) CFLDFileListContainer();
    CleanupStack::PushL( self );
    self->ConstructL( aResourceId, R_FLD_DIRECTORIES );
    return self;
    }

// -----------------------------------------------------------------------------
// CFLDFileListContainer::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CFLDFileListContainer* CFLDFileListContainer::NewL(
    const TInt aResourceId, const TInt aDirectoriesResourceId )
    {
    CFLDFileListContainer* self = CFLDFileListContainer::NewLC(
        aResourceId, aDirectoriesResourceId );
    CleanupStack::Pop( self );
    return self;
    }
    
// -----------------------------------------------------------------------------
// CFLDFileListContainer::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CFLDFileListContainer* CFLDFileListContainer::NewLC(
    const TInt aResourceId, const TInt aDirectoriesResourceId )
    {
    CFLDFileListContainer* self =
        new( ELeave ) CFLDFileListContainer();
    CleanupStack::PushL( self );
    self->ConstructL( aResourceId, aDirectoriesResourceId );
    return self;
    }
    
// Destructor
EXPORT_C CFLDFileListContainer::~CFLDFileListContainer()
    {
    iResourceLoader.Close();
    
    delete iDRMImplementation;
    delete iModel;
    delete iController;
    FeatureManager::UnInitializeLib();
    }

// -----------------------------------------------------------------------------
// CFLDFileListContainer::CFLDFileListContainer
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CFLDFileListContainer::CFLDFileListContainer()
	: iResourceLoader( *( CCoeEnv::Static() ) )
    {
    }

// -----------------------------------------------------------------------------
// CFLDFileListContainer::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CFLDFileListContainer::ConstructL(
 const TInt aResourceId, const TInt aDirectoriesResourceId )
    {
    // Open Filelist default resource
	TFileName* fn = new (ELeave) TFileName
		( TParsePtrC( PathInfo::RomRootPath() ).Drive() );
	CleanupStack::PushL( fn );

	fn->Append( KDC_RESOURCE_FILES_DIR );
	fn->Append( KFLDResourceFileName );
	iResourceLoader.OpenL( *fn );
	CleanupStack::PopAndDestroy( fn );

    iModel = CFLDFileListModel::NewL( aResourceId, aDirectoriesResourceId );
    CFLDSingleGraphicEntryFormatter* entryFormatter =
    	new ( ELeave ) CFLDSingleGraphicEntryFormatter();
    iModel->SetEntryFormatter( entryFormatter );

	// Add 'download tones' item to model
    FeatureManager::InitializeLibL();
 	if( FeatureManager::FeatureSupported( KFeatureIdSeamlessLinks ) )
        {
        // Load 'download tones' text and format it
		HBufC* text = StringLoader::LoadLC(
			R_FLD_QTN_PROFILES_DOWNLOAD_TONES );
		_LIT( KIconIndexAndTab, "1\t" );
        HBufC* newText =
            text->ReAllocL( text->Length() + KIconIndexAndTab().Length() );
        CleanupStack::Pop( text );
        CleanupStack::PushL( newText );
        TPtr des( newText->Des() );
        des.Insert( 0, KIconIndexAndTab );
		// Insert formatted 'download tones' text to model
        iModel->InsertNullItemL( des, KNullDesC );
        CleanupStack::PopAndDestroy( newText );
        }

	iDRMImplementation = CFLDDRMImplementation::NewL( iModel );

	TBool showErrorMsgs( ETrue );
    iController = CFLDController::NewL( showErrorMsgs, KDefaultDelay );
 	iController->SetFileObserver( iDRMImplementation );

	// Following implementation enables disabling recordable ringing tones.
	TInt recEnable( 0 );
	CRepository* cenrep = CRepository::NewL( KCRUidProfileEngine );
    CleanupStack::PushL( cenrep );
    User::LeaveIfError( cenrep->Get( KProEngRecordedRingingTones, recEnable ) );
    CleanupStack::PopAndDestroy( cenrep );

    if( !recEnable )
        {
        // Recorded tones as ringing tones should be disabled.
        // Exclude AMR files.
        iModel->AddExclusiveMimeTypeL( KFLDAMRMimeType );
        }
    }

// -----------------------------------------------------------------------------
// CFLDFileListContainer::SetAutomatedType()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CFLDFileListContainer::SetAutomatedType(
 CDRMHelper::TDRMHelperAutomatedType aAutomatedType )
	{
	iDRMImplementation->SetAutomatedType( aAutomatedType );
	}

// -----------------------------------------------------------------------------
// CFLDFileListContainer::LaunchL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CFLDFileListContainer::LaunchL( TDes& aFileName, const TDesC& aPopupTitle )
    {
    // Update the list
	iModel->RefreshEntryListL();

    CEikFormattedCellListBox* listBox = NULL;
        listBox = new( ELeave ) FLDListBoxTemplate
        	<CAknSingleGraphicBtPopupMenuStyleListBox>( *iController, *iModel );
    CleanupStack::PushL( listBox );

    // Create the popup list
    CFLDPopupList* popup = CFLDPopupList::NewL( listBox,
        R_AVKON_SOFTKEYS_SELECT_CANCEL__SELECT,
        *iDRMImplementation, iController,
        *iModel, AknPopupLayouts::EMenuGraphicWindow );
    // EMenuGraphicWindow for CAknSingleGraphicBtPopupMenuStyleListBox
    CleanupStack::PushL( popup );

	static_cast<FLDListBoxTemplate
		 <CAknSingleGraphicBtPopupMenuStyleListBox> *>( listBox )
		 ->SetListBox( popup );

	// Video player can be fully constructed only when popup list
	// is constructed
	iController->CompleteConstructionL( popup->PopupListWindow() );

	// Set popup to Model
	iModel->SetPopupList( popup );

    popup->SetTitleL( aPopupTitle );

    listBox->ConstructL( popup,
        EAknListBoxSelectionList | EAknListBoxLoopScrolling );

    // Create the scroll indicator
    listBox->CreateScrollBarFrameL( ETrue );
    listBox->ScrollBarFrame()->SetScrollBarVisibilityL(
        CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto);

    // Set the listbox to use the the file list model
    listBox->Model()->SetItemTextArray( iModel );
    listBox->Model()->SetOwnershipType( ELbmDoesNotOwnItemArray );

    // Refresh the listbox due to model change
    listBox->HandleItemAdditionL();

	popup->PopulateIconArrayL();

    // If a filename was given, set focus on it
    if ( aFileName != KNullDesC )
        {
        TInt index = iModel->FindFileL( aFileName );
        if ( index != KErrNotFound )
            {
            listBox->SetCurrentItemIndex( index );
            }
        }

    TBool result = popup->ExecuteLD();
	// Note, CAknPopupList must NOT be popped out
	// before ExecuteLD (like dialogs do) but after.
    CleanupStack::Pop( popup );

	// Reset popup pointer
	iModel->SetPopupList( NULL );

    if ( result )
        {
        TInt selectedIndex( listBox->CurrentItemIndex() );

		// Check if the selected item is 'Download tones' or regular ringingtone
		TFileName selectedFileName( KNullDesC );
		iModel->GetFileName( selectedFileName, selectedIndex );

        // If there is such a model that has no 'Download tones' item but
        // seamless links are supported, we have to check that selectedFileName
        // is really ringingtone (it is not a null item)
        if( selectedIndex == 0 &&
            FeatureManager::FeatureSupported( KFeatureIdSeamlessLinks ) &&
            selectedFileName.Length() == 0 )
            {
            // User selected the first item "Download tones". Launch browser.
            CFLDBrowserLauncher* launcher = CFLDBrowserLauncher::NewLC();
            launcher->LaunchBrowserL();
            CleanupStack::PopAndDestroy(); // launcher
            result = EFalse; // No tone was selected
            }
        else
            {
            // Any other choice. Copy selected file name to aFileName.
            iModel->GetFileName( aFileName, selectedIndex );
            }
        }

    CleanupStack::PopAndDestroy(); // listBox

    return result;
    }

// -----------------------------------------------------------------------------
// CFLDFileListContainer::InsertNullItemL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CFLDFileListContainer::InsertNullItemL(
 const TDesC& aItemText )
    {
    iModel->InsertNullItemL( aItemText );
    }

// -----------------------------------------------------------------------------
// CFLDFileListContainer::InsertNullItemL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CFLDFileListContainer::InsertNullItemL(
 const TDesC& aItemText, const TDesC& aFileName )
    {
    iModel->InsertNullItemL( aItemText, aFileName );
    }

// -----------------------------------------------------------------------------
// CFLDFileListContainer::InsertEndNullItemL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CFLDFileListContainer::InsertEndNullItemL(
 const TDesC& aItemText )
    {
    iModel->InsertEndNullItemL( aItemText );
    }

// -----------------------------------------------------------------------------
// CFLDFileListContainer::InsertEndNullItemL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CFLDFileListContainer::InsertEndNullItemL(
 const TDesC& aItemText, const TDesC& aFileName )
    {
    iModel->InsertEndNullItemL( aItemText, aFileName );
    }

// -----------------------------------------------------------------------------
// CFLDFileListContainer::SetDelay()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CFLDFileListContainer::SetDelay( TTimeIntervalMicroSeconds32 aDelay )
    {
    iController->SetDelay( aDelay );
    }

// -----------------------------------------------------------------------------
// CFLDFileListContainer::SetVolume()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CFLDFileListContainer::SetVolume( TInt aVolume )
    {
    iController->SetVolume( aVolume );
    }

// -----------------------------------------------------------------------------
// CFLDFileListContainer::SetRingingType()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CFLDFileListContainer::SetRingingType( TInt aRingingType )
    {
    iController->SetRingingType( aRingingType );
    }

// -----------------------------------------------------------------------------
// CFLDFileListContainer::SetVibra()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CFLDFileListContainer::SetVibra( TBool aVibra )
    {
    iController->SetVibra( aVibra );
    }

// -----------------------------------------------------------------------------
// CFLDFileListContainer::Set3dEffects()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C void CFLDFileListContainer::Set3dEffects( TBool a3dEffects )
	{
	iController->Set3dEffects( a3dEffects );
	}

// -----------------------------------------------------------------------------
// CFLDFileListContainer::AddExclusiveMimeTypeL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CFLDFileListContainer::AddExclusiveMimeTypeL( const TDesC& aMimeType )
	{
	iModel->AddExclusiveMimeTypeL( aMimeType );
	}

// -----------------------------------------------------------------------------
// CFLDFileListContainer::AddExclusiveMediaTypeL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CFLDFileListContainer::AddExclusiveMediaTypeL( const TInt32 aMediaType )
	{
	iModel->AddExclusiveMediaTypeL( aMediaType );
	}

// -----------------------------------------------------------------------------
// CFLDFileListContainer::SetWantedMimeTypesL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CFLDFileListContainer::SetWantedMimeTypesL( const MDesCArray& aMimeTypes )
	{
	iModel->SetWantedMimeTypesL( aMimeTypes );
	}

// -----------------------------------------------------------------------------
// CFLDFileListContainer::SetWantedMediaTypesL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CFLDFileListContainer::SetWantedMediaTypesL( const TArray<TCLFMediaType>& aMediaTypes )
	{
	iModel->SetWantedMediaTypesL( aMediaTypes );
	}

// -----------------------------------------------------------------------------
// CFLDFileListContainer::SetWantedMimeTypesL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CFLDFileListContainer::ResetExclusiveMimeTypes()
	{
	iModel->ResetExclusiveMimeTypes();
	}

// -----------------------------------------------------------------------------
// CFLDFileListContainer::SetWantedMimeTypesL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CFLDFileListContainer::ResetExclusiveMediaTypes()
	{
	iModel->ResetExclusiveMediaTypes();
	}

// -----------------------------------------------------------------------------
// CFLDFileListContainer::SetMaxFileSize()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CFLDFileListContainer::SetMaxFileSize( const TInt aMaxFileSize )
    {
    iModel->SetMaxFileSize( aMaxFileSize );
    }	


//  End of File
