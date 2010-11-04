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
* Description:   Dialog create for video preview
*
*/



#include <aknappui.h>
#include "mediafilevideopreviewdialog.h"
#include "mediafilepreview.h"
#include <mediafilelist.rsg>



/******************************************************************************
 * class CVideoPreviewDialog
 ******************************************************************************/


// -----------------------------------------------------------------------------
// CVideoPreviewDialog::ShowDialogLD
// 
// NOTE: ExecuteLD deletes dialog object.
// -----------------------------------------------------------------------------
TBool CVideoPreviewDialog::ShowDialogLD( TInt* aError )
    {
    iError = aError;
    *iError = KErrNone;
    
    TBool ret = ExecuteLD( R_VIDEO_PREVIEW_DIALOG );

    return ret;
    }

// ----------------------------------------------------------------------------
// Destructor
//
// ----------------------------------------------------------------------------
//
CVideoPreviewDialog::~CVideoPreviewDialog()
    {
    CCoeEnv::Static()->RemoveForegroundObserver( *this );
    
    delete iIdle;
    
    if ( iVideoHandlerCreated )
        {
        delete iVideoHandler;    
        }
   
    if ( iAvkonAppUi )
        {
        iAvkonAppUi->RemoveFromStack( this );
        }
    }


// -----------------------------------------------------------------------------
// CVideoPreviewDialog::CVideoPreviewDialog
// 
// -----------------------------------------------------------------------------
//
CVideoPreviewDialog::CVideoPreviewDialog( TAny* aVideoHandler )
    {
    if ( aVideoHandler )
        {
        iVideoHandler = static_cast<CMFVideoPreviewHandler*>( aVideoHandler );
        //iVideoHandler = aVideoHandler;
        }
    iVideoHandlerCreated = EFalse;
    }


// -----------------------------------------------------------------------------
// CVideoPreviewDialog::NewL
//
// -----------------------------------------------------------------------------
CVideoPreviewDialog* CVideoPreviewDialog::NewL( TAny* aVideoHandler )
    {
    CVideoPreviewDialog* self = new (ELeave) CVideoPreviewDialog( aVideoHandler );
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }


// -----------------------------------------------------------------------------
// CVideoPreviewDialog::ConstructL
//
// -----------------------------------------------------------------------------
//
void CVideoPreviewDialog::ConstructL()
    {
    CCoeEnv::Static()->AddForegroundObserverL( *this );
    
    CAknDialog::ConstructL( R_VIDEO_PREVIEW_DIALOG_MENU );

    if ( !iVideoHandler )
        {
        iVideoHandler = CMFVideoPreviewHandler::NewL();
        iVideoHandlerCreated = ETrue;
        }
    
    iVideoHandler->SetObserver( this );
    }


// -----------------------------------------------------------------------------
// CVideoPreviewDialog::SetAttrL
// 
// -----------------------------------------------------------------------------
//
void CVideoPreviewDialog::SetAttrL( TInt aAttr, TInt aValue )
    {
    iVideoHandler->SetAttrL( aAttr, aValue );
    }


// -----------------------------------------------------------------------------
// CVideoPreviewDialog::SetAttrL
// 
// -----------------------------------------------------------------------------
//
void CVideoPreviewDialog::SetAttrL( TInt aAttr, const TDesC& aValue )
    {
    iVideoHandler->SetAttrL( aAttr, aValue );
    }


// -----------------------------------------------------------------------------
// CVideoPreviewDialog::SetAttr
// 
// -----------------------------------------------------------------------------
//
void CVideoPreviewDialog::SetAttrL( TInt aAttr, TAny* aValue )
    {
    iVideoHandler->SetAttrL( aAttr, aValue );
    }


// -----------------------------------------------------------------------------
// CVideoPreviewDialog::PreLayoutDynInitL
// 
// -----------------------------------------------------------------------------
//
void CVideoPreviewDialog::PreLayoutDynInitL()
    {
    }


// -----------------------------------------------------------------------------
// CVideoPreviewDialog::PostLayoutDynInitL
// 
// -----------------------------------------------------------------------------
//
void CVideoPreviewDialog::PostLayoutDynInitL()
    {
    // dialog has two lines (see .rss). Listbox line id is 1
    //CEikCaptionedControl *ctrl = Line( 1 );
    //iVideoPreviewHandler->SetAttrL( TMFDialogUtil::EAttrDrawingWindow, (ctrl->iControl->DrawableWindow()) );
    
    iVideoHandler->SetAttrL( TMFDialogUtil::EAttrDrawingWindow, DrawableWindow() );
    iVideoHandler->PlayL();
    }


//------------------------------------------------------------------------------
// CVideoPreviewDialog::OkToExitL
//
//------------------------------------------------------------------------------
//
TBool CVideoPreviewDialog::OkToExitL(TInt aButtonId)
    {
    if ( aButtonId == EEikBidCancel )
        {
        if ( iVideoHandler->IsPlaying() )
            {
            iVideoHandler->Stop();
            }

        return ETrue;  // close dialog
        }

    if ( aButtonId == EAknSoftkeySelect || aButtonId == EEikBidOk )
        {
        if ( iVideoHandler->IsPlaying() )
            {
            iVideoHandler->Stop();
            }
        return ETrue;  // close dialog
        }

    if ( aButtonId == EAknSoftkeyBack )
        {
        if ( iVideoHandler->IsPlaying() )
            {
            iVideoHandler->Stop();
            }
        return ETrue;  // close dialog
        }

    return CAknDialog::OkToExitL( aButtonId );
    }


// ----------------------------------------------------------------------------
// CVideoPreviewDialog::HandleResourceChange
// 
// ----------------------------------------------------------------------------
//
void CVideoPreviewDialog::HandleResourceChange(TInt aType)
    {   
    if ( aType == KEikDynamicLayoutVariantSwitch )
        {
        // handle change in layout orientation
        
        CAknDialog::HandleResourceChange( aType );
        
        TRAP_IGNORE( iVideoHandler->SetDisplayWindowL() );
        return;
        }
    }

    
//------------------------------------------------------------------------------
// CVideoPreviewDialog::ProcessCommandL
//
// Handle commands from menu.
//------------------------------------------------------------------------------
//
void CVideoPreviewDialog::ProcessCommandL(TInt aCommandId)
    {
    HideMenu();
    
    switch (aCommandId)
        {
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


// -----------------------------------------------------------------------------
// CVideoPreviewDialog::CloseDialogWithDelayL
//
// -----------------------------------------------------------------------------
//
void CVideoPreviewDialog::CloseDialogWithDelayL()
    {    
    // close after short delay (dialog cannot be closed from this function)
    delete iIdle;
    iIdle = NULL;
    iIdle = CIdle::NewL( CActive::EPriorityIdle );                
    if ( iIdle )
        {                    
        iIdle->Start( TCallBack( CloseDialog, this ) );
        }
    }


// -----------------------------------------------------------------------------
// CVideoPreviewDialog::CloseDialogL()
//
// -----------------------------------------------------------------------------
//
void CVideoPreviewDialog::CloseDialogL()
    {    
    // close the dialogue immediately
    CloseDialog( this );
    }

// -----------------------------------------------------------------------------
// CVideoPreviewDialog::CloseDialog
//
// -----------------------------------------------------------------------------
//
TInt CVideoPreviewDialog::CloseDialog( TAny *aObj )
    {    
    CVideoPreviewDialog *dlg = (CVideoPreviewDialog*) aObj;
    
    TRAP_IGNORE( dlg->TryExitL( EAknSoftkeyBack ) );
    
    return 0;
    }


// -----------------------------------------------------------------------------
// CVideoPreviewDialog::HandlePreviewEventL (from MPreviewHandlerObserver)
// 
// -----------------------------------------------------------------------------
//
void CVideoPreviewDialog::HandlePreviewEventL( TInt aEvent, TInt aError )
    {
    if ( aEvent == MPreviewHandlerObserver::EVideoPreviewComplete )
        {
        *iError = aError;
        CVideoPreviewDialog::CloseDialogWithDelayL();
        }

    if ( aEvent == MPreviewHandlerObserver::EPreviewError )
        {
        *iError = aError;
        CVideoPreviewDialog::CloseDialogWithDelayL();
        }
    }


// -----------------------------------------------------------------------------
// CVideoPreviewDialog::HandleLosingForeground (from MCoeForegroundObserver)
//
// -----------------------------------------------------------------------------
//
void CVideoPreviewDialog::HandleLosingForeground()
    {
    TRAP_IGNORE( CVideoPreviewDialog::CloseDialogWithDelayL() );
    }


// -----------------------------------------------------------------------------
// CVideoPreviewDialog::HandleGainingForeground (from MCoeForegroundObserver)
//
// -----------------------------------------------------------------------------
//
void CVideoPreviewDialog::HandleGainingForeground()
    {
    }

//  End of File  
