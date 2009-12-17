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
* Description:   Basic functionality of MediaFileList.
*
*/



// INCLUDE FILES

#include "mediafiledialogutils.h"


#include <bautils.h>  // bafl.lib
#include <StringLoader.h>
#include <driveinfo.h>
#include <pathinfo.h>

#include <aknnavide.h>          // CAknNavigationDecorator
#include <aknnavilabel.h>       // CAknNaviLabel
#include <akntitle.h>           // CAknTitlePane
#include <AknQueryDialog.h>
#include <aknnotewrappers.h>
#include <AknsUtils.h>          // AknsUtils

// for CWebBrowserLauncher
#include <BrowserLauncher.h>
#include <DocumentHandler.h>
#include <AiwCommon.h>


#include <mediafilelist.rsg>
#include <data_caging_path_literals.hrh> // KDC_APP_RESOURCE_DIR




/******************************************************************************
 * class TMFDialogUtil
 ******************************************************************************/


// -----------------------------------------------------------------------------
// TMFDialogUtil::StrCopy
//
// -----------------------------------------------------------------------------
//
void TMFDialogUtil::StrCopy( TDes& aTarget, const TDesC& aSource )
    {
	TInt len = aTarget.MaxLength();
    if( len < aSource.Length() ) 
	    {
		aTarget.Copy( aSource.Left(len) );
		return;
	    }
	aTarget.Copy( aSource );
    }


// -----------------------------------------------------------------------------
// TMFDialogUtil::Panic
//
// -----------------------------------------------------------------------------
//
void TMFDialogUtil::Panic( TInt aReason ) 
    {
	_LIT( KPanicCategory, "TMFDialogUtil" );
	
	User::Panic( KPanicCategory, aReason ); 
    }


//------------------------------------------------------------------------------
// TMFDialogUtil::ShowInformationNoteL
//
//------------------------------------------------------------------------------
//
void TMFDialogUtil::ShowInformationNoteL( TInt aResource )
    {
    HBufC* hBuf = StringLoader::LoadLC( aResource );
    ShowInformationNoteL( hBuf->Des() );
    CleanupStack::PopAndDestroy( hBuf );
    }


//------------------------------------------------------------------------------
// TMFDialogUtil::ShowInformationNoteL
//
//------------------------------------------------------------------------------
//
void TMFDialogUtil::ShowInformationNoteL( const TDesC& aText )
    {
    CAknInformationNote* queryDialog = new (ELeave) CAknInformationNote( ETrue );
    queryDialog->ExecuteLD( aText );
    }


//------------------------------------------------------------------------------
// TMFDialogUtil::ShowInformationQueryL
//
//------------------------------------------------------------------------------
//
void TMFDialogUtil::ShowInformationQueryL( const TDesC& aText )
    {
    CAknQueryDialog* queryDialog = new (ELeave) CAknQueryDialog;
    queryDialog->ExecuteLD( R_INFORMATION_QUERY, aText );
    }


//------------------------------------------------------------------------------
// TMFDialogUtil::ShowInformationQueryL
//
//------------------------------------------------------------------------------
//
void TMFDialogUtil::ShowInformationQueryL( TInt aResource )
    {
    HBufC* hBuf = StringLoader::LoadLC( aResource );
    ShowInformationQueryL( hBuf->Des() );
    CleanupStack::PopAndDestroy( hBuf );
    }

//------------------------------------------------------------------------------
// TMFDialogUtil::ShowErrorNoteL
//
//------------------------------------------------------------------------------
//
void TMFDialogUtil::ShowErrorNoteL( TInt aResource )
    {
    HBufC* hBuf = StringLoader::LoadLC( aResource );
    ShowErrorNoteL( hBuf->Des() );
    CleanupStack::PopAndDestroy( hBuf );
    }


//------------------------------------------------------------------------------
// TMFDialogUtil::ShowErrorNoteL
//
//------------------------------------------------------------------------------
//
void TMFDialogUtil::ShowErrorNoteL( const TDesC& aText )
    {
    CAknErrorNote* errorNote = new (ELeave) CAknErrorNote(ETrue);
    errorNote->ExecuteLD( aText );
    }


//------------------------------------------------------------------------------
// TMFDialogUtil::ShowErrorNoteL
//
//------------------------------------------------------------------------------
//
void TMFDialogUtil::ShowErrorNoteL( const TDesC& aText, TInt aError )
    {
    _LIT( KFormat, "%S (%d)" );
    
    TInt len = aText.Length();
    HBufC* hBuf = HBufC::NewLC( len + KBufSize64 );
    
    TPtr ptr = hBuf->Des();
    ptr.Format( KFormat, &aText, aError);
    
    ShowErrorNoteL( ptr );
    
    CleanupStack::PopAndDestroy( hBuf );
    }

//------------------------------------------------------------------------------
// ShowNumberQueryL
//
//------------------------------------------------------------------------------
//
TBool TMFDialogUtil::ShowNumberQueryL( TInt& aNumber, const TDesC& aTitle )
    {
    CAknNumberQueryDialog* queryDialog = CAknNumberQueryDialog::NewL(aNumber);

    if ( aTitle.Length() > 1 )
        {
        CleanupStack::PushL( queryDialog );
        queryDialog->SetPromptL( aTitle );
        CleanupStack::Pop( queryDialog );
        }

    return queryDialog->ExecuteLD( R_NUMBER_QUERY );
    }


// -----------------------------------------------------------------------------
// TMFDialogUtil::ShowListQueryL
// 
// -----------------------------------------------------------------------------
TBool TMFDialogUtil::ShowListQueryL( const TDesC& aTitle, CDesCArray* aList,
                                     TInt* aSelectedIndex )
    {
    CAknListQueryDialog* query = new (ELeave) CAknListQueryDialog( aSelectedIndex );
    CleanupStack::PushL( query );

    query->PrepareLC( R_LIST_QUERY );
    query->SetItemTextArray( aList );
    query->SetOwnershipType( ELbmDoesNotOwnItemArray );
    CAknPopupHeadingPane* headingPane = query->QueryHeading();
    headingPane->SetTextL( aTitle );
    
    CleanupStack::Pop( query );

    return query->RunLD();
    }


// -----------------------------------------------------------------------------
// TMFDialogUtil::AddResFileL
//
// -----------------------------------------------------------------------------
//
TInt TMFDialogUtil::AddResFileL()
    {
    _LIT( KDriveZ, "z:" );
    _LIT( KResFileName, "mediafilelist.rsc" );

    TFileName fileName;
    fileName.Append( KDriveZ );
    fileName.Append( KDC_RESOURCE_FILES_DIR );
    fileName.Append( KResFileName );

    CEikonEnv* env = CEikonEnv::Static();
    BaflUtils::NearestLanguageFile( env->FsSession(), fileName );

    return env->AddResourceFileL( fileName );
    }


// ----------------------------------------------------------------------------
// TMFDialogUtil::SetIconFileNameL
//
// ----------------------------------------------------------------------------
//
void TMFDialogUtil::SetIconFileNameL( const TDesC& aName, TDes& aFullName )
    {
    _LIT( KDriveZ, "z:" );
    
    aFullName = KNullDesC;
    
    aFullName.Append( KDriveZ );
    aFullName.Append( KDC_APP_RESOURCE_DIR );
    aFullName.Append( aName );
    }


// -----------------------------------------------------------------------------
// TMFDialogUtil::CreateImageL
//
// -----------------------------------------------------------------------------
//
CEikImage* TMFDialogUtil::CreateImageLC( TAknsItemID aId, const TDesC& aFileName,
                                         TInt aFileBitmapId, TInt aFileMaskId )
    {
    CFbsBitmap* bitmap1 = NULL;
    CFbsBitmap* bitmap2 = NULL;
    
    CEikImage* image = new (ELeave) CEikImage;
    CleanupStack::PushL( image );

    AknsUtils::CreateIconLC( AknsUtils::SkinInstance(), aId, bitmap1, bitmap2,
                             aFileName, aFileBitmapId, aFileMaskId );
    image->SetPicture( bitmap1, bitmap2 );
    CleanupStack::Pop(2); // bitmap1 bitmap2

    return image;
    }

// -------------------------------------------------------------------------------
// TMFDialogUtil::IsEmpty
//
// Function returns ETrue if string only contains white space or has
// no characters.
// -------------------------------------------------------------------------------
//
TBool TMFDialogUtil::IsEmpty( const TDesC& aText )
    {
    TInt len = aText.Length();
    
    for ( TInt i=0; i<len; i++ )
        {
        TChar c = aText[i];
        if ( !c.IsSpace() )
            {
            return EFalse;
            }
        }
    return ETrue;
    }




/******************************************************************************
 * class CMFActiveCaller
 ******************************************************************************/


// -----------------------------------------------------------------------------
// CMFActiveCaller::NewL
// 
// -----------------------------------------------------------------------------
//
CMFActiveCaller* CMFActiveCaller::NewL( MMFActiveCallerObserver* aObserver )
    {
    CMFActiveCaller* self = new (ELeave) CMFActiveCaller( aObserver );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );

 	return self;
    }


// -----------------------------------------------------------------------------
// Destructor
// 
// -----------------------------------------------------------------------------
//
CMFActiveCaller::~CMFActiveCaller()
    {
 	Cancel();
	iTimer.Close();
    }


// -----------------------------------------------------------------------------
// CMFActiveCaller::CMFActiveCaller
// 
// -----------------------------------------------------------------------------
//
CMFActiveCaller::CMFActiveCaller(MMFActiveCallerObserver* aObserver) : CActive(CActive::EPriorityStandard)
    {
	iObserver = aObserver;
    }


// -----------------------------------------------------------------------------
// CMFActiveCaller::ConstructL
// 
// -----------------------------------------------------------------------------
//
void CMFActiveCaller::ConstructL()
    {
	User::LeaveIfError( iTimer.CreateLocal() );
	CActiveScheduler::Add( this );
    }


// -----------------------------------------------------------------------------
// CMFActiveCaller::DoCancel
//
// -----------------------------------------------------------------------------
//
void CMFActiveCaller::DoCancel()
    {
 	iTimer.Cancel();
    }


// -----------------------------------------------------------------------------
// CMFActiveCaller::RunL
//
// -----------------------------------------------------------------------------
//
void CMFActiveCaller::RunL()
    {
    TRAP_IGNORE( iObserver->HandleActiveCallL( iCallId ) );
    }


// -----------------------------------------------------------------------------
// CMFActiveCaller::Request
//
// This function calls this class RunL.
// -----------------------------------------------------------------------------
//
void CMFActiveCaller::Request()
    {
	Cancel();
	SetActive();
	TRequestStatus* status = &iStatus;
	User::RequestComplete( status, KErrNone );
    }


// -----------------------------------------------------------------------------
// CMFActiveCaller::Start
//
// -----------------------------------------------------------------------------
//
void CMFActiveCaller::Start( TInt aCallId, TInt aMilliseconds )
    {
    const TInt KThousand = 1000;
    Cancel();
	
	iCallId = aCallId;

	if ( aMilliseconds <= 0 )
		{
		Request();  // no delay - complete asap
		}
	else
		{
	    iTimer.After( iStatus, aMilliseconds*KThousand );
	    SetActive();
		}
    }


// -----------------------------------------------------------------------------
// CMFActiveCaller::Stop
//
// -----------------------------------------------------------------------------
//
void CMFActiveCaller::Stop()
    {
    Cancel();
    }




/*******************************************************************************
 * class CNaviPaneHandler
 *******************************************************************************/


// -----------------------------------------------------------------------------
// Destructor
//
// -----------------------------------------------------------------------------
//
CNaviPaneHandler::~CNaviPaneHandler()
    {
    if ( iNaviDecorator )
        {
        if ( iNaviPane && iNavidecoratorPushed )
            {
            iNaviPane->Pop( iNaviDecorator );
            }
    
        delete iNaviDecorator;
        }
        
    if ( iNaviPane && iNavipanePushed )
        {
        iNaviPane->Pop(); // restore previous navi pane
        }
    }


// -----------------------------------------------------------------------------
// CNaviPaneHandler::CNaviPaneHandler
//
// -----------------------------------------------------------------------------
//
CNaviPaneHandler::CNaviPaneHandler( CEikStatusPane* aStatusPane ) : iStatusPane( aStatusPane )
    {
    iNavidecoratorPushed = EFalse;
    iNavipanePushed = EFalse;
    
    if ( iStatusPane )
        {
        TRAP_IGNORE( iNaviPane = (CAknNavigationControlContainer*)iStatusPane->ControlL(TUid::Uid(EEikStatusPaneUidNavi)) );
        }
    }


// ----------------------------------------------------------------------------
// CNaviPaneHandler::SetNaviPaneTitleL
// 
// ----------------------------------------------------------------------------
//
void CNaviPaneHandler::SetNaviPaneTitleL(const TDesC& aTitle)
    {
    if ( !iStatusPane || !iNaviPane )
        {
        return;
        }

    if ( iNaviDecorator )
        {
        CAknNaviLabel* naviLabel = static_cast<CAknNaviLabel*>(iNaviDecorator->DecoratedControl());
        if ( naviLabel )
            {
            naviLabel->SetTextL( aTitle );
            iNaviDecorator->DrawDeferred();
            }
        }
     else
        {
        iNaviDecorator = iNaviPane->CreateNavigationLabelL( aTitle ) ;
        iNaviPane->PushL( *iNaviDecorator ); // activate navi label in navi pane
        iNavidecoratorPushed = ETrue;
        }
    }


// ----------------------------------------------------------------------------
// CNaviPaneHandler::StoreNavipaneL
// 
// ----------------------------------------------------------------------------
//
void CNaviPaneHandler::StoreNavipaneL()
    {
    if ( !iNaviPane || !iStatusPane || iNavipanePushed )
        {
        return;
        }

    if ( iStatusPane->PaneCapabilities(TUid::Uid(EEikStatusPaneUidTitle)).IsPresent() )
        {
        iNaviPane->PushDefaultL();
        iNavipanePushed = ETrue;
        }
    }




/*******************************************************************************
 * class CStatusPaneHandler
 *******************************************************************************/

// -----------------------------------------------------------------------------
// CStatusPaneHandler::NewL
//
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CStatusPaneHandler* CStatusPaneHandler::NewL( CAknAppUi* aAppUi )
    {
    CStatusPaneHandler* self = new (ELeave) CStatusPaneHandler( aAppUi );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// Destructor
//
// -----------------------------------------------------------------------------
//
CStatusPaneHandler::~CStatusPaneHandler()
    {
    TRAP_IGNORE( RestoreOriginalTitleL() );
    
    delete iNaviPaneHandler;
    }


// -----------------------------------------------------------------------------
// CStatusPaneHandler::ConstructL
//
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CStatusPaneHandler::ConstructL()
    {
    iNaviPaneHandler = new (ELeave) CNaviPaneHandler( iAppUi->StatusPane() );
    }


// -----------------------------------------------------------------------------
// CStatusPaneHandler::CStatusPaneHandler
//
// -----------------------------------------------------------------------------
//
CStatusPaneHandler::CStatusPaneHandler( CAknAppUi* aAppUi ) : iAppUi(aAppUi )
    {
    __ASSERT_ALWAYS( iAppUi, TMFDialogUtil::Panic( KErrGeneral ) );

    iOriginalTitleStored = EFalse;
    iOriginalTitle = KNullDesC;
    }


// ----------------------------------------------------------------------------
// CStatusPaneHandler::GetTitleL
// 
// ----------------------------------------------------------------------------
//
TBool CStatusPaneHandler::GetTitleL( CAknAppUi* aAppUi, TDes& aTitle )
    {
    aTitle = KNullDesC;

    CEikStatusPane* statusPane = aAppUi->StatusPane();
    if (statusPane && statusPane->PaneCapabilities( TUid::Uid( EEikStatusPaneUidTitle ) ).IsPresent() )
        {
        CAknTitlePane* titlePane = static_cast<CAknTitlePane*>
            ( statusPane->ControlL( TUid::Uid( EEikStatusPaneUidTitle ) ) );
        if ( titlePane->Text() ) 
            {
            TMFDialogUtil::StrCopy( aTitle, *titlePane->Text() );
            return ETrue;
            }
        }

    return EFalse;
    }


// ----------------------------------------------------------------------------
// CStatusPaneHandler::SetTitleL
// 
// ----------------------------------------------------------------------------
//
TBool CStatusPaneHandler::SetTitleL( CAknAppUi* aAppUi, const TDesC& aTitle )
    {
    CEikStatusPane* statusPane = aAppUi->StatusPane();
    if ( statusPane && statusPane->PaneCapabilities( TUid::Uid( EEikStatusPaneUidTitle ) ).IsPresent() )
        {
        CAknTitlePane* titlePane = static_cast<CAknTitlePane*>
            ( statusPane->ControlL( TUid::Uid( EEikStatusPaneUidTitle ) ) );
        if ( titlePane->Text() ) 
            {
            titlePane->SetTextL( aTitle );
            return ETrue;
            }
        }
    return EFalse;
    }


// ----------------------------------------------------------------------------
// CStatusPaneHandler::SetNaviPaneTitleL
// 
// ----------------------------------------------------------------------------
//
void CStatusPaneHandler::SetNaviPaneTitleL( const TDesC& aTitle )
    {
    iNaviPaneHandler->SetNaviPaneTitleL( aTitle );
    }


// ----------------------------------------------------------------------------
// CStatusPaneHandler::SetNaviPaneTitle
// 
// ----------------------------------------------------------------------------
//
void CStatusPaneHandler::SetNaviPaneTitle( const TDesC& aTitle )
    {
    TRAP_IGNORE( iNaviPaneHandler->SetNaviPaneTitleL( aTitle ) );
    }


// ----------------------------------------------------------------------------
// CStatusPaneHandler::SetNaviPaneTitleL
// 
// ----------------------------------------------------------------------------
//
void CStatusPaneHandler::SetNaviPaneTitleL( TInt aResource )
    {
    HBufC* hBuf = StringLoader::LoadLC( aResource) ;
    SetNaviPaneTitleL( hBuf->Des() );
    CleanupStack::PopAndDestroy( hBuf );
    }


// ----------------------------------------------------------------------------
// CStatusPaneHandler::StoreNavipaneL
// 
// ----------------------------------------------------------------------------
//
void CStatusPaneHandler::StoreNavipaneL()
    {
    iNaviPaneHandler->StoreNavipaneL();
    }


// ----------------------------------------------------------------------------
// CStatusPaneHandler::StoreOriginalTitleL
// 
// ----------------------------------------------------------------------------
//
void CStatusPaneHandler::StoreOriginalTitleL()
    {
    TBool ret = GetTitleL( iAppUi, iOriginalTitle );
    
    if ( ret )
        {
        iOriginalTitleStored = ETrue;
        }
    }


// ----------------------------------------------------------------------------
// CStatusPaneHandler::RestoreOriginalTitleL
// 
// ----------------------------------------------------------------------------
//
void CStatusPaneHandler::RestoreOriginalTitleL()
    {
    if ( iOriginalTitleStored )
        {
        SetTitleL( iAppUi, iOriginalTitle );
        }
    }


// ----------------------------------------------------------------------------
// CStatusPaneHandler::SetTitleL
// 
// ----------------------------------------------------------------------------
//
void CStatusPaneHandler::SetTitleL( const TDesC& aText )
    {
    if ( iOriginalTitleStored )
        {
        SetTitleL( iAppUi, aText );
        }
    }


// ----------------------------------------------------------------------------
// CStatusPaneHandler::SetTitleL
// 
// ----------------------------------------------------------------------------
//
void CStatusPaneHandler::SetTitleL( TInt aResourceId )
    {
    HBufC* hBuf = StringLoader::LoadLC( aResourceId );
    SetTitleL( hBuf->Des() );
    CleanupStack::PopAndDestroy( hBuf );
    }



/******************************************************************************
 * class CDriveUtil
 ******************************************************************************/


// -----------------------------------------------------------------------------
// CDriveUtil::NewL
// 
// -----------------------------------------------------------------------------
//
CDriveUtil* CDriveUtil::NewL()
    {
    CDriveUtil* self = new (ELeave) CDriveUtil();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }


// -----------------------------------------------------------------------------
// Destructor
// 
// -----------------------------------------------------------------------------
//
CDriveUtil::~CDriveUtil()
    {
    iFsSession.Close();
    }


// -----------------------------------------------------------------------------
// CDriveUtil::CDriveUtil
// 
// -----------------------------------------------------------------------------
//
CDriveUtil::CDriveUtil()
    {
    }


// -----------------------------------------------------------------------------
// CDriveUtil::ConstructL
// 
// -----------------------------------------------------------------------------
//
void CDriveUtil::ConstructL()
    {
    User::LeaveIfError( iFsSession.Connect() );
    }


// -----------------------------------------------------------------------------
// CDriveUtil::IsRom
// 
// -----------------------------------------------------------------------------
//
TBool CDriveUtil::IsRom( const TDesC& aFullPath )
    {
    const TUint KMassStorageBits = DriveInfo::EDriveRom;

    TDriveUnit drive( aFullPath );
    TUint driveStatus( 0 );
    
    TInt err = DriveInfo::GetDriveStatus( iFsSession, drive, driveStatus );
    if ( err != KErrNone )
        {
        return EFalse;
        }
        
    if ( (driveStatus & KMassStorageBits) == KMassStorageBits )
        {
        return ETrue;
        }
        
    return EFalse;
    }


// -----------------------------------------------------------------------------
// CDriveUtil::IsMassStorage
// 
// -----------------------------------------------------------------------------
//
TBool CDriveUtil::IsMassStorage( const TDesC& aFullPath )
    {
    const TUint KMassStorageBits = DriveInfo::EDriveInternal |
                                   DriveInfo::EDriveExternallyMountable;

    TDriveUnit drive( aFullPath );
    TUint driveStatus( 0 );
    
    TInt err = DriveInfo::GetDriveStatus( iFsSession, drive, driveStatus );
    if ( err != KErrNone )
        {
        return EFalse;
        }
        
    if ( (driveStatus & KMassStorageBits) == KMassStorageBits )
        {
        return ETrue;
        }
        
    return EFalse;
    }


// -----------------------------------------------------------------------------
// CDriveUtil::IsMemoryCard
// 
// -----------------------------------------------------------------------------
//
TBool CDriveUtil::IsMemoryCard( const TDesC& aFullPath )
    {
    const TUint KMemoryCardBits = DriveInfo::EDriveRemovable |
                                  DriveInfo::EDriveExternallyMountable;

    TDriveUnit drive( aFullPath );
    TUint driveStatus( 0 );
    
    TInt err = DriveInfo::GetDriveStatus( iFsSession, drive, driveStatus );
    if ( err != KErrNone )
        {
        return EFalse;
        }
        
    if ( (driveStatus & KMemoryCardBits) == KMemoryCardBits )
        {
        return ETrue;
        }
        
    return EFalse;
    }



/******************************************************************************
 * class CMimeTypeList
 ******************************************************************************/



// -----------------------------------------------------------------------------
// CMimeTypeList::NewL
// 
// -----------------------------------------------------------------------------
//
CMimeTypeList* CMimeTypeList::NewL()
    {
    CMimeTypeList* object = new (ELeave) CMimeTypeList;
    return object;
    }

// -----------------------------------------------------------------------------
// CMimeTypeList::CMimeTypeList
// 
// -----------------------------------------------------------------------------
//
CMimeTypeList::CMimeTypeList()
    {
    }

// -----------------------------------------------------------------------------
// CMimeTypeList::~CMimeTypeList
// 
// -----------------------------------------------------------------------------
//
CMimeTypeList::~CMimeTypeList()
    {
    iList.Close();
    }


// -----------------------------------------------------------------------------
// CMimeTypeList::ConstructL
// 
// -----------------------------------------------------------------------------
//
void CMimeTypeList::ConstructL()
    {
    }



// -----------------------------------------------------------------------------
// CMimeTypeList::ResetL
// 
// -----------------------------------------------------------------------------
//
void CMimeTypeList::ResetL()
    {
    iList.Reset();
    }


// -----------------------------------------------------------------------------
// CMimeTypeList::Count
// 
// -----------------------------------------------------------------------------
//
TInt CMimeTypeList::Count()
    {
    return iList.Count();
    }


// -----------------------------------------------------------------------------
// CMimeTypeList::Compare
//
// -----------------------------------------------------------------------------
//
TInt CMimeTypeList::Compare(const TFileName& aFirst, const TFileName& aSecond)
    {
    return aFirst.Compare( aSecond );
    }


// -----------------------------------------------------------------------------
// CMimeTypeList::AddMimeTypeL
//
// -----------------------------------------------------------------------------
//
void CMimeTypeList::AddMimeTypeL( const TDesC& aText )
    {
    if ( aText.Length() > KMaxFileName || aText.Length() == 0 )
        {
        User::Leave( KErrArgument );
        }

    TLinearOrder<TFileName> sortFunction( Compare );

    TInt err = iList.InsertInOrder( aText, sortFunction );
    
    if ( err != KErrNone && err != KErrAlreadyExists )
        {
        User::Leave( err );
        }
    }


// -----------------------------------------------------------------------------
// CMimeTypeList::FindMimeTypeL
//
// -----------------------------------------------------------------------------
//
TBool CMimeTypeList::FindMimeTypeL( const TDesC& aText )
    {
    TInt count = Count();
    
    for ( TInt i=0; i<count; i++ )
        {
        TPtrC ptr = MimeType( i );
        if ( ptr.Compare( aText ) == 0 )
            {
            return ETrue;
            }
        }
    
    return EFalse;
    }


// -----------------------------------------------------------------------------
// CMimeTypeList::MimeType
//
// -----------------------------------------------------------------------------
//
const TDesC& CMimeTypeList::MimeType( TInt aIndex )
    {
    TInt count = iList.Count();
    __ASSERT_ALWAYS( aIndex >= 0 && aIndex < count, TMFDialogUtil::Panic( KErrGeneral ) );
    
    return iList[aIndex];
    }




/******************************************************************************
 * class CWebBrowserLauncher
 ******************************************************************************/


// -----------------------------------------------------------------------------
// CWebBrowserLauncher::CWebBrowserLauncher
//
// -----------------------------------------------------------------------------
//
CWebBrowserLauncher::CWebBrowserLauncher()
    {
    }

// -----------------------------------------------------------------------------
// CWebBrowserLauncher::ConstructL
//
// -----------------------------------------------------------------------------
//
void CWebBrowserLauncher::ConstructL()
    {
    iBrowserLauncher = CBrowserLauncher::NewL();
    iDocumentHandler = CDocumentHandler::NewL( CEikonEnv::Static()->Process() );
    }

// -----------------------------------------------------------------------------
// CWebBrowserLauncher::NewLC
//
// -----------------------------------------------------------------------------
//
CWebBrowserLauncher* CWebBrowserLauncher::NewLC()
    {
    CWebBrowserLauncher* self = new( ELeave ) CWebBrowserLauncher();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
// CWebBrowserLauncher::~CWebBrowserLauncher
//
// -----------------------------------------------------------------------------
//
CWebBrowserLauncher::~CWebBrowserLauncher()
    {
    delete iDocumentHandler;
    delete iBrowserLauncher;
    }


// -----------------------------------------------------------------------------
// CWebBrowserLauncher::HandleContentL
//
// -----------------------------------------------------------------------------
//
TBool CWebBrowserLauncher::HandleContentL( const TDesC& aFileName,
                                           const CAiwGenericParamList& aParamList,
                                           TBool& aContinue )
    {
    TBool isSaved = EFalse;

    if ( aParamList.Count() > 0 )
        {
        TInt index = 0;
        aParamList.FindFirst( index, EGenericParamFileSaved );

        if( index != KErrNotFound )
            {
            isSaved = ( aParamList[ index ].Value().AsTInt32() );
            }
        }

    // Tone is NOT yet saved by a browser, save it by using DocHandler
    if( !isSaved )
        {
        //Let documenthandler to find out the datatype
        TDataType nullType;
        iDocumentHandler->CopyL( aFileName, KNullDesC, nullType, NULL );
        }
    
    aContinue = ETrue;
    
    return ETrue;
    }


// -----------------------------------------------------------------------------
// CWebBrowserLauncher::HandleContentL
//
// -----------------------------------------------------------------------------
//
TAny* CWebBrowserLauncher::DownloadedContentHandlerReserved1( TAny* /*aAnyParam*/ )
    {
    return NULL;
    }

// -----------------------------------------------------------------------------
// CWebBrowserLauncher::LaunchBrowserL
//
// -----------------------------------------------------------------------------
//
void CWebBrowserLauncher::LaunchBrowserL()
    {
    iBrowserLauncher->LaunchBrowserSyncEmbeddedL( KFavouritesAudioContextId );
    }



//  End of File  
