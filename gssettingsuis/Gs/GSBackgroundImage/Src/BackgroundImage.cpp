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
* Description: 
*
*/



// INCLUDE FILES
#include    "backgroundimage.h"
#include    "GsLogger.h"
#include    "settingsinternalcrkeys.h"
#include    "SettingsPrivateCRKeys.h"

#include    <aknappui.h>
#include    <aknlayoutscalable_apps.cdl.h>
#include    <AknUtils.h>
#include    <apgcli.h>
#include    <applayout.cdl.h>
#include    <DRMCommon.h>
#include    <Oma1DcfCreator.h>  //DRM v2 migration
#include    <s32file.h>
#include    <startupdomaincrkeys.h>
#include    <sysutil.h>
#include    <graphics/bitmapuid.h>



// CONSTANTS
// Setting values for Background and Welcome note image
const   TInt    KGSBackgroundModeImage = 1;
// estimated image max size in bytes
const   TInt    KGSMaxBackgroundImgSize = 520000;
// estimated image max size in bytes
const   TInt    KGSMaxWelcomeNoteImgSize = 100000;
const   TInt    KGSVTStillImageWidth = 176;
const   TInt    KGSVTStillImageHeight = 144;

//Personalisation application is availble on ROM (1) or not (0)
const   TInt    KGSPlsnOnRom = 1;
const   TInt    KGSPlsnNotOnRom = 0;

// other constants
_LIT( KGSBackgroundImgPath, "c:\\resource\\apps\\backgroundimage.mbm");
_LIT( KGSVTStillImgPath, "c:\\resource\\apps\\vtstillimage.mbm");




// FORWARD DECLARATIONS


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CGSBackgroundImage::CGSBackgroundImage
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CGSBackgroundImage::CGSBackgroundImage()
    {
    }

// -----------------------------------------------------------------------------
// CGSBackgroundImage::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CGSBackgroundImage::ConstructL()
    {
    iPersonalizationRepository =
        CRepository::NewL( KCRUidPersonalizationSettings );
    iStartupConfRepository = CRepository::NewL( KCRUidStartupConf );
    iTelephonyRepository = CRepository::NewL( KCRUidTelephonySettings );

    iDisplayMode = CEikonEnv::Static()->ScreenDevice()->DisplayMode();
    }


// -----------------------------------------------------------------------------
// CGSBackgroundImage::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CGSBackgroundImage* CGSBackgroundImage::NewL()
    {
    CGSBackgroundImage* self = new( ELeave ) CGSBackgroundImage;

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }


// ----------------------------------------------------------------------------
// Destructor
//
//-----------------------------------------------------------------------------
//
CGSBackgroundImage::~CGSBackgroundImage()
    {
    delete iPersonalizationRepository;
    delete iStartupConfRepository;
    delete iTelephonyRepository;
    }

// ----------------------------------------------------------------------------
// CGSBackgroundImage::BackgroundImageL
//
// Returns background image mode.
// DEPRECATED
// ----------------------------------------------------------------------------
//
EXPORT_C TInt CGSBackgroundImage::BackgroundImageL()
    {
    TInt image;
    User::LeaveIfError( iPersonalizationRepository->
        Get( KSettingsBackgroundImage, image ) );
    return image;
    }

// ----------------------------------------------------------------------------
// CGSBackgroundImage::SetBackgroundImageL
//
// Sets background image mode.
// DEPRECATED
// ----------------------------------------------------------------------------
//
EXPORT_C void CGSBackgroundImage::SetBackgroundImageL(const TInt aImage)
    {
    User::LeaveIfError( iPersonalizationRepository->
        Set( KSettingsBackgroundImage, aImage ) );
    if (aImage != KGSBackgroundModeImage)
        {
        // It doesn't matter much if the image deletion fails,
        // so no need to handle the error.
        TRAP_IGNORE( DeleteImageL( KGSBackgroundImgPath ) );
        }

    }

// ----------------------------------------------------------------------------
// CGSBackgroundImage::SetBackgroundImagePathL
//
// Sets background image.
// DEPRECATED
// ----------------------------------------------------------------------------
//
EXPORT_C void CGSBackgroundImage::SetBackgroundImagePathL(
                                                const TDesC& aImagePath )
    {
    // Check if there is enough space in disk to store background image
    if (SysUtil::FFSSpaceBelowCriticalLevelL(NULL, KGSMaxBackgroundImgSize))
        {
        // don't show any own notes here
        User::Leave(KErrDiskFull);
        }

    // make sure there is no memory leaks because of iBitmap and iDimmedBmp
    if (iBitmap)
        {
        delete iBitmap;
        iBitmap = NULL;
        }

    iBitmap = new(ELeave) CFbsBitmap;

    TRect appRect = iAvkonAppUi->ApplicationRect();
    TAknLayoutRect layout;

    layout.LayoutRect(appRect, AknLayout::wallpaper_pane());
    LoadImageL(aImagePath, layout.Rect().Width(), layout.Rect().Height(),
            ETrue, EGSWallpaperIdle);
    TBuf<1> KGSNullParam;
    SaveBitmapsL( KGSNullParam, KGSNullParam );

    delete iBitmap;
    iBitmap = NULL;
    User::LeaveIfError( iPersonalizationRepository->
        Set( KSettingsBackgroundImagePath, KGSBackgroundImgPath ) );
    }

// ----------------------------------------------------------------------------
// CGSBackgroundImage::SetBackgroundImagePathL
// Sets background image. Does not store image to SD
// Overloaded function, takes 2 parameters
// DEPRECATED
// ----------------------------------------------------------------------------
//
EXPORT_C void CGSBackgroundImage::SetBackgroundImagePathL(
    const TDesC& aImagePath,
    TDes& aBmpFileName )
    {
    SetBackgroundImagePathL( aImagePath, aBmpFileName, EGSWallpaperIdle );
    }

// ----------------------------------------------------------------------------
// CGSBackgroundImage::SetBackgroundImagePathL
// Sets background image. Does not store image to SD
// Overloaded function, takes 3 parameters
// DEPRECATED
// ----------------------------------------------------------------------------
//
EXPORT_C void CGSBackgroundImage::SetBackgroundImagePathL(
    const TDesC& aImagePath,
    TDes& aBmpFileName,
    TGSWallpaperType aWallpaperType )
    {
    // Check if there is enough space in disk to store background image
    if (SysUtil::FFSSpaceBelowCriticalLevelL(NULL, KGSMaxBackgroundImgSize))
        {
        // don't show any own notes here
        User::Leave(KErrDiskFull);
        }

    // Make sure there is no memory leaks because of iBitmap and iDimmedBmp
    if (iBitmap)
        {
        delete iBitmap;
        iBitmap = NULL;
        }

    iBitmap = new(ELeave) CFbsBitmap;

    if( aWallpaperType == EGSWallpaperIdle )
        {
        //Reading wallpaper size
        __GSLOGSTRING(
            "[CGSBackgroundImage] SetBackgroundImagePathL |->");
        TRect screenRect( CEikonEnv::Static()->ScreenDevice()->SizeInPixels() );
        TAknLayoutRect appRect;
        appRect.LayoutRect(
            screenRect,
            AknLayout::application_window( screenRect ) );
        TAknLayoutRect mainRectSmall;
        mainRectSmall.LayoutRect(
            appRect.Rect(),
            AknLayout::main_pane( appRect.Rect(), 0, 1, 1 ) );
        TAknLayoutRect layout;
        layout.LayoutRect( mainRectSmall.Rect(), AknLayout::wallpaper_pane() );

        __GSLOGSTRING("[CGSBackgroundImage] SetBackgroundImagePathL");
        LoadImageL(aImagePath, layout.Rect().Width(), layout.Rect().Height(),
            ETrue, EGSWallpaperIdle);
        }
    else if( aWallpaperType == EGSWallpaperPinboard )
        {
        //Reading wallpaper size
        __GSLOGSTRING("[CGSBackgroundImage] SetBackgroundImagePathL 1");

        TRect screenRect( CEikonEnv::Static()->ScreenDevice()->SizeInPixels() );
        TAknLayoutRect appRect;
        appRect.LayoutRect(
            screenRect,
            AknLayout::application_window( screenRect ) );
        TAknLayoutRect mainRectSmall;
        mainRectSmall.LayoutRect(
            appRect.Rect(),
            AknLayout::main_pane( appRect.Rect(), 0, 0, 1 ) );
        TAknLayoutRect layout;

        layout.LayoutRect(
            mainRectSmall.Rect(), AknLayoutScalable_Apps::bg_pinb_pane(0) );

        __GSLOGSTRING("[CGSBackgroundImage] SetBackgroundImagePathL 2");
        LoadImageL(aImagePath, layout.Rect().Width(), layout.Rect().Height(),
            ETrue, EGSWallpaperPinboard);
        __GSLOGSTRING("[CGSBackgroundImage] SetBackgroundImagePathL -|");
        }

    SaveBitmapsL( aBmpFileName, aImagePath );

    delete iBitmap;
    iBitmap = NULL;
    }

// ----------------------------------------------------------------------------
// CGSBackgroundImage::SetWelcomeNoteImageL
//
// Sets welcome note image's path
// ----------------------------------------------------------------------------
//
EXPORT_C void CGSBackgroundImage::SetWelcomeNoteImageL(const TDesC& aImagePath,
                          CGSAsyncImageHandling& aImgHandler )
    {
    // Check if there is enough space in disk to store welcome note image
    if ( SysUtil::FFSSpaceBelowCriticalLevelL( NULL, KGSMaxWelcomeNoteImgSize ) )
        {
        // don't show any own notes here
        User::Leave( KErrDiskFull );
        }



    TRect screen = iAvkonAppUi->ApplicationRect();
    TInt screenWidth = screen.Width() - 2;
    TInt screenHeight = screen.Height() - 2;

    //rescale
    aImgHandler.ProcessImageL( aImagePath, screenWidth, screenHeight,
                                                        EGSWelcomeNoteImage );
    }

// ----------------------------------------------------------------------------
// CGSBackgroundImage::LoadImageL
// Loads an image to bitmap and scales it if it's too large.
// Deprecated
// ----------------------------------------------------------------------------
//
void CGSBackgroundImage::LoadImageL(const TDesC& /*aImagePath*/,
                        TInt /*aWidth*/, TInt /*aHeight*/,
                        TBool /*aBgImage*/, TGSWallpaperType /*aWallpaperType*/)
    {
    }


// ----------------------------------------------------------------------------
// CGSBackgroundImage::SaveBitmapsL
// Stores background image and dimmed image to the same mbm file.
// ----------------------------------------------------------------------------
//
void CGSBackgroundImage::SaveBitmapsL( TDes& aBmpFileName,
                                            const TDesC& aOriginalFullname )
    {
    RFs fs;
    User::LeaveIfError( fs.Connect() );
    CleanupClosePushL( fs );

    CDirectFileStore* fileStore;
    if( aBmpFileName != KNullDesC )
        {
        fileStore = CDirectFileStore::ReplaceLC(
            fs, aBmpFileName, EFileWrite );
        }
    else
        {
        fileStore = CDirectFileStore::ReplaceLC(
            fs, KGSBackgroundImgPath, EFileWrite );
        }

    TUidType uidType( KDirectFileStoreLayoutUid, KMultiBitmapFileImageUid );
    fileStore->SetTypeL( uidType );

    RStoreWriteStream bmpStream;
    TStreamId imgId = bmpStream.CreateLC( *fileStore );
    iBitmap->ExternalizeL( bmpStream );

    CleanupStack::PopAndDestroy(); //bmpStream

    RStoreWriteStream rootStream;
    // create root stream
    TStreamId rootStreamId = rootStream.CreateLC( *fileStore );
    rootStream.WriteInt32L( 1 ); // number of bitmaps

    rootStream << imgId; // stream id of bitmap

    CleanupStack::PopAndDestroy(); //rootStream

    fileStore->SetRootL( rootStreamId );
    CleanupStack::PopAndDestroy(); // filestore;

    TBool isprotected( EFalse );

    DRMCommon* drmclient = DRMCommon::NewL();

    CleanupStack::PushL( drmclient );
    TInt error = drmclient->Connect();
    if ( error != DRMCommon::EOk )
        {
        User::Leave( KErrCorrupt );
        }

    if ( aOriginalFullname != KNullDesC )
        {
        error = drmclient->IsProtectedFile( aOriginalFullname, isprotected );
        if ( error != DRMCommon::EOk )
            {
            User::Leave( KErrCorrupt );
            }
        }


    if ( isprotected )
        {
        HBufC8* contenturi = NULL;
        HBufC8* mimetype = NULL;
        DRMCommon::TContentProtection contentProtection;
        TUint datalen( 0 );
        CDRMRights* rights = NULL;


        error = drmclient->GetFileInfo(
            aOriginalFullname, contentProtection,
            mimetype, contenturi, datalen );
        CleanupStack::PushL( mimetype );
        CleanupStack::PushL( contenturi );
        if ( error != DRMCommon::EOk )
            {
            User::Leave( KErrCorrupt );
            }

        drmclient->GetActiveRights(
            *contenturi, DRMCommon::EDisplay, rights );
        CleanupStack::PushL( rights );

        RFile file;
        User::LeaveIfError( file.Open( fs, aBmpFileName,
                                        EFileWrite|EFileShareAny ) );
        CleanupClosePushL( file );

        TInt filesize( 0 );
        User::LeaveIfError( file.Size( filesize ) );
        HBufC8* content = HBufC8::NewL( filesize ); // use actual filesize

        CleanupStack::PushL( content );

        TPtr8 readPtr( content->Des() );
        User::LeaveIfError( file.Read( readPtr ) );

        CleanupStack::Pop(); // content
        COma1DcfCreator* dcfCreator = COma1DcfCreator::NewL();
        dcfCreator->EncryptContentL( content, *mimetype, rights );
        CleanupStack::PushL( content );

        TInt offset = 0;
        User::LeaveIfError( file.Seek( ESeekStart,offset ) );
        TPtr8 writePtr( content->Des() );
        User::LeaveIfError( file.Write( writePtr ) );
        User::LeaveIfError( file.Flush() );
        // mimetype, contenturi, rights, file, content
        CleanupStack::PopAndDestroy(5);
        delete dcfCreator;
        }

    drmclient->Disconnect();
    CleanupStack::PopAndDestroy(2); // fs & drmclient
    }


// ----------------------------------------------------------------------------
// CGSBackgroundImage::DeleteImageL
// Deletes file specified in the parameter.
// ----------------------------------------------------------------------------
//
EXPORT_C void CGSBackgroundImage::DeleteImageL(const TDesC& aImagePath)
    {
    RFs fs;
    User::LeaveIfError(fs.Connect());
    CleanupClosePushL(fs);

    User::LeaveIfError(fs.Delete(aImagePath));

    CleanupStack::PopAndDestroy(); // fs
    }

// ----------------------------------------------------------------------------
// CGSBackgroundImage::SetVTStillImagePathL
//
// Sets VT still image.
// ----------------------------------------------------------------------------
//
EXPORT_C void CGSBackgroundImage::SetVTStillImagePathL(
                                           const TDesC& aImagePath,
                                           CGSAsyncImageHandling& aImgHandler )
    {
    // Check if there is enough space in disk to store still image
    if (SysUtil::FFSSpaceBelowCriticalLevelL(NULL, KGSMaxBackgroundImgSize))
        {
        // don't show any own notes here
        User::Leave(KErrDiskFull);
        }
    aImgHandler.ProcessImageL( aImagePath, KGSVTStillImageWidth,
                    KGSVTStillImageHeight, EGSVtStillImage );
    User::LeaveIfError( iTelephonyRepository->
            Set( KSettingsVTStillImagePath, KGSVTStillImgPath ) );
    }

// ----------------------------------------------------------------------------
// CGSBackgroundImage::GetPlnsStatus
//
// Check if Personalisation application is on the device
// ----------------------------------------------------------------------------
//
EXPORT_C TInt CGSBackgroundImage::GetPlnsStatus()
    {
    TRAP_IGNORE( IsPersonalisationApplicationL() );
    return iPlnsInDevice;
    }

// ----------------------------------------------------------------------------
// CGSBackgroundImage::IsPersonalisationApplication
//
//
// ----------------------------------------------------------------------------
//
EXPORT_C void CGSBackgroundImage::IsPersonalisationApplicationL()
    {
    RApaLsSession session;
    User::LeaveIfError( session.Connect() );

    const TUid KUidPsln = { 0x10005A32 };
    TApaAppInfo appInfo;
    TInt search = session.GetAppInfo( appInfo, KUidPsln );
    session.Close();

    if ( search == KErrNone )
        {
        TParse fileInfo;
        fileInfo.Set( appInfo.iFullName, NULL, NULL );
        TPtrC drive = fileInfo.Drive();
        _LIT( KZDrive, "Z:" );
        if ( drive == KZDrive )
            {
            iPlnsInDevice = KGSPlsnOnRom;
            }
        else
            {
            iPlnsInDevice = KGSPlsnNotOnRom;
            }
        }
    else
        {
        iPlnsInDevice = KErrNotFound;
        }
    }


//  End of File
