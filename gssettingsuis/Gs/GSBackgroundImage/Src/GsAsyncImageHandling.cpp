/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies).
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
#include "gsasyncimagehandling.h"
#include <aknappui.h>
#include <AknUtils.h>
#include <applayout.cdl.h>
#include <aknlayoutscalable_apps.cdl.h>
#include <startupdomaincrkeys.h>

#include "mgsasyncimagehandlingobserver.h"
#include "GsLogger.h"

#include <CPhCltImageHandler.h>
#include <CPhCltImageParams.h>
#include <CPhCltBaseImageParams.h>

//_LIT( KGSWelcomeNoteImgPath, "c:\\private\\100058ec\\welcomeimage.mbm");

const   TInt    KGSVTStillImageWidth = 176;
const   TInt    KGSVTStillImageHeight = 144;


// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------------------------
// CGSAsyncImageHandling::CGSAsyncImageHandling
// C++ constructor.
//
// ---------------------------------------------------------------------------
//
CGSAsyncImageHandling::CGSAsyncImageHandling( RFs& aFs,
                                  MGSAsyncImageHandlingObserver* aObserver,
                                  const TDesC& aDestinationPath )
        : CActive( EPriorityNormal ),
          iObserver( aObserver ),
          iFs( aFs ),
          iDestinationPath( aDestinationPath )
          
    {
    CActiveScheduler::Add( this );
    }


// ---------------------------------------------------------------------------
// CGSAsyncImageHandling::NewL()
// C++ constructor.
//
// ---------------------------------------------------------------------------
//
EXPORT_C CGSAsyncImageHandling* CGSAsyncImageHandling::NewL( RFs& aFs,
                                   MGSAsyncImageHandlingObserver* aObserver,
                                   const TDesC& aDestinationPath )
    {
    CGSAsyncImageHandling* self =
        new( ELeave ) CGSAsyncImageHandling( aFs, aObserver,
                                                  aDestinationPath );
    CleanupStack::PushL( self );                                              
    self->ConstructL();
    CleanupStack::Pop();
    
    return self;
    }


// -----------------------------------------------------------------------------
// CGSAsyncImageHandling::ConstructL
// Symbian 2nd phase constructor can leave.
//
// -----------------------------------------------------------------------------
//
void CGSAsyncImageHandling::ConstructL()
    {
    iStartupRepository = CRepository::NewL( KCRUidStartupConf );
    iBitmapScaler = CBitmapScaler::NewL();
    }


// ---------------------------------------------------------------------------
// CGSAsyncImageHandling::~CGSAsyncImageHandling
// destructor.
//
// ---------------------------------------------------------------------------
//
CGSAsyncImageHandling::~CGSAsyncImageHandling()
    {
    Cancel();

    if ( iStartupRepository )
        {
        delete iStartupRepository;
        }

    if( iDecoder )
        {
        delete iDecoder;
        }

    if ( iBitmapScaler )
        {
        delete iBitmapScaler;
        }

    if ( iBitmap )
        {
        delete iBitmap;
        }
    }

// ---------------------------------------------------------------------------
// CGSAsyncImageHandling::ProcessImageL
//
// Processing the image asynchronously
// ---------------------------------------------------------------------------
//
void CGSAsyncImageHandling::ProcessImageL( const TDesC& aFileName,
                                           TInt aScreenWidth,
                                           TInt aScreenHeight,
                                           TGSBgImageType aBgImageType )
    {
__GSLOGSTRING1( "[CGSAsyncImageHandling] process image %S", &aFileName );

    TInt frameNumber = 0; //for JPEG & bmp images
    iBgImageType = aBgImageType;

    // make sure there is no memory leaks because of iBitmap and iDimmedBmp
    if (iBitmap)
        {
        delete iBitmap;
        iBitmap = NULL;
        }
    iBitmap = new(ELeave) CFbsBitmap;

    // Loading image from file into CFbsBitmap
    CImageDecoder::GetMimeTypeFileL( iFs, aFileName, iMimeString );
    iDecoder = CImageDecoder::FileNewL( iFs, aFileName,
                                        CImageDecoder::EOptionNone );

    TFrameInfo frameInfo = iDecoder->FrameInfo();
    TSize imgSize = frameInfo.iOverallSizeInPixels;


    TDisplayMode displayMode = GetDisplayMode( frameInfo );

    TSize screenSize( aScreenWidth, aScreenHeight );
    TSize loadSize( CalculateLoadSize( frameInfo, screenSize ) );
    iScaleSize = loadSize;

    iBitmap->Reset();
    User::LeaveIfError( iBitmap->Create( imgSize, displayMode ) );
    iDecoder->Convert( &iStatus, *iBitmap, frameNumber );

    if ( aBgImageType == EGSWelcomeNoteImage )
      {
      iState = EGSWelcomeConversion;
      }
    else if ( aBgImageType == EGSVtStillImage )
      {
      iState = EGSVTConversion;
      }

    SetActive();
    }

// ---------------------------------------------------------------------------
// CGSAsyncImageHandling::GetDisplayMode
//
// Dithers the image to attain the required dithering according to the display
// mode settings
// ---------------------------------------------------------------------------
//
TDisplayMode CGSAsyncImageHandling::GetDisplayMode( const TFrameInfo& aFrameInfo )
    {
    if( aFrameInfo.iFlags & TFrameInfo::ECanDither )
        {
        if( aFrameInfo.iFrameDisplayMode < EColor64K )
            {
            return aFrameInfo.iFrameDisplayMode;
            }
        else
            {
            return EColor64K;
            }
        }
    else
        {
        return aFrameInfo.iFrameDisplayMode;
        }
    }


// -----------------------------------------------------------------------------
// CGSAsyncImageHandling::RunL
//
//
// ---------------------------------------------------------------------------
//
void CGSAsyncImageHandling::RunL()
    {
    if( iStatus.Int() == KErrNone )
      {
      switch( iState )
        {
        case EGSWelcomeConversion:
          iBitmapScaler->Scale(&iStatus, *iBitmap, iScaleSize, ETrue );
          iState = EGSWelcomeScaling;
          SetActive();
          break;

        case EGSWelcomeScaling:
            {
            TInt err = iBitmap->Save( iDestinationPath );
            iStartupRepository->Set( KStartupWelcomeNoteImage,
                                     iDestinationPath );

            //raising completion event
            iObserver->ImageHandlingCompleteL( iStatus.Int() );
            Cancel();
            }
          break;

        case EGSVTConversion:
          iBitmapScaler->Scale(&iStatus, *iBitmap, iScaleSize, ETrue );
          iState = EGSVTScaling;
          SetActive();
          break;

        case EGSVTScaling:
          SaveVTStillImageL();
          iObserver->ImageHandlingCompleteL( iStatus.Int() );
          Cancel();
          break;

        default:
          break;
        }
      }
    else
      {
      iObserver->ImageHandlingCompleteL( iStatus.Int() );
      Cancel();
      }
    }

// -----------------------------------------------------------------------------
// CGSAsyncImageHandling::DoCancel
//
//
// ---------------------------------------------------------------------------
//
void CGSAsyncImageHandling::DoCancel()
    {
    if ( iDecoder )
        {
        iDecoder->Cancel();
        }

    if ( iBitmapScaler )
        {
        iBitmapScaler->Cancel();
        }

    delete iDecoder;
    iDecoder = NULL;
    }

// -----------------------------------------------------------------------------
// CGSAsyncImageHandling::RunError
//
//
// ---------------------------------------------------------------------------
//
TInt CGSAsyncImageHandling::RunError( TInt aError )
    {
    TRAP_IGNORE( iObserver->ImageHandlingCompleteL( aError ) );
    DoCancel();
    return aError;
    }

// ----------------------------------------------------------------------------
// TSize CGSAsyncImageHandling::CalculateLoadSize
//
// Calculates the load size
// ----------------------------------------------------------------------------
//
TSize CGSAsyncImageHandling::CalculateLoadSize( TFrameInfo& aFrameInfo,
                                               const TSize aScreenSize )
    {
    TReal perfectAspect = ( TReal )(
    		( TReal )aScreenSize.iWidth/
    		( TReal )aScreenSize.iHeight );
    TReal aspect = ( TReal )(
        ( TReal )aFrameInfo.iFrameCoordsInPixels.Width()/
        ( TReal )aFrameInfo.iFrameCoordsInPixels.Height() );
    TSize size( aScreenSize.iWidth, aScreenSize.iHeight );
    TSize cropsize( aScreenSize.iWidth, aScreenSize.iHeight );

    TAknWindowLineLayout layout = AknLayout::wallpaper_pane();
    if( aFrameInfo.iFrameCoordsInPixels.Width() > aScreenSize.iWidth ||
        aFrameInfo.iFrameCoordsInPixels.Height() > aScreenSize.iHeight )
        {
        //calculating dynamically range for image aspect ratio close to screen
        TReal maxAspect = ((TReal)aScreenSize.iWidth/(TReal)aScreenSize.iHeight) + 0.12;
        TReal minAspect = ((TReal)aScreenSize.iWidth/(TReal)aScreenSize.iHeight) - 0.12;

        if (aspect >= minAspect && aspect <= maxAspect )
            {
            if( aspect < perfectAspect )
                {
                size.iWidth = aScreenSize.iWidth;
                size.iHeight = (TInt)((TReal)aScreenSize.iWidth/
                    (TReal)aFrameInfo.iFrameCoordsInPixels.Width()*
                    (TReal)aFrameInfo.iFrameCoordsInPixels.Height());
                }
            else
                {
                size.iWidth = (TInt)((TReal)aScreenSize.iHeight/
                    (TReal)aFrameInfo.iFrameCoordsInPixels.Height()*
                    (TReal)aFrameInfo.iFrameCoordsInPixels.Width());
                size.iHeight = aScreenSize.iHeight;
                }
            if (size.iWidth > aScreenSize.iWidth)
                {
                cropsize.iWidth = aScreenSize.iWidth;
                }
            else
                {
                cropsize.iWidth = size.iWidth;
                }
            if (size.iHeight > aScreenSize.iHeight)
                {
                cropsize.iHeight = aScreenSize.iHeight;
                }
            else
                {
                cropsize.iHeight = size.iHeight;
                }
            }
        else
            {
            // Scale and maintain aspect ratio
            if( aspect < perfectAspect )
                {
                if (aFrameInfo.iFrameCoordsInPixels.Height() > aScreenSize.iHeight)
                    {
                    size.iWidth = (TInt)((TReal)aScreenSize.iHeight/
                        (TReal)aFrameInfo.iFrameCoordsInPixels.Height()*
                        (TReal)aFrameInfo.iFrameCoordsInPixels.Width());
                    size.iHeight = aScreenSize.iHeight;
                    }
                }
            else
                {
                if (aFrameInfo.iFrameCoordsInPixels.Width() > aScreenSize.iWidth)
                    {
                    size.iWidth = aScreenSize.iWidth;
                    size.iHeight = (TInt)((TReal)aScreenSize.iWidth/
                        (TReal)aFrameInfo.iFrameCoordsInPixels.Width()*
                        (TReal)aFrameInfo.iFrameCoordsInPixels.Height());
                    }
                }
            }
        }

    return size;
    }

// -----------------------------------------------------------------------------
// CGSAsyncImageHandling::SaveVTStillImageL
//
//
// ---------------------------------------------------------------------------
//
void CGSAsyncImageHandling::SaveVTStillImageL()
    {
    //creating new and merging
    const TSize KStillImageSize( KGSVTStillImageWidth, KGSVTStillImageHeight );

    CFbsBitmap* newBitmap = new ( ELeave ) CFbsBitmap;
    CleanupStack::PushL( newBitmap );

    User::LeaveIfError(
        newBitmap->Create( KStillImageSize, iBitmap->DisplayMode() ) );

    CFbsBitmapDevice* device = CFbsBitmapDevice::NewL( newBitmap );
    CleanupStack::PushL( device );

    CFbsBitGc* context = NULL;
    User::LeaveIfError( device->CreateContext( context ) );
    User::LeaveIfNull( context );

    context->SetPenStyle( CGraphicsContext::ENullPen );
    context->SetBrushColor( TRgb( 0, 0, 0 ) ); // black color
    context->SetBrushStyle( CGraphicsContext::ESolidBrush );
    context->Clear();

    const TSize scaledImageSize( iBitmap->SizeInPixels() );

    TInt xPos = ( KStillImageSize.iWidth - scaledImageSize.iWidth ) / 2;
    TInt yPos = ( KStillImageSize.iHeight - scaledImageSize.iHeight ) / 2;

    context->BitBlt( TPoint( xPos, yPos ), iBitmap );

    delete context;
    CleanupStack::PopAndDestroy( device );

    //saving bitmap
    CPhCltImageHandler* phCltImageHandler = CPhCltImageHandler::NewL();
    CleanupStack::PushL( phCltImageHandler );

    // Acquires ownership of *imageParams
    // Bad naming for create-function CPhCltBaseImageParamsL().
    CPhCltImageParams* imageParams = phCltImageHandler->
                       CPhCltBaseImageParamsL( EPhCltTypeVTStill );
    CleanupStack::PushL( imageParams );
    TInt ret = newBitmap->Handle();
    imageParams->AddImageL( ret );
    phCltImageHandler->SaveImages( *imageParams );
    CleanupStack::PopAndDestroy( imageParams );
    CleanupStack::PopAndDestroy( phCltImageHandler );

    delete iBitmap;
    iBitmap = NULL;

    CleanupStack::PopAndDestroy( newBitmap );
    }


// End of File
