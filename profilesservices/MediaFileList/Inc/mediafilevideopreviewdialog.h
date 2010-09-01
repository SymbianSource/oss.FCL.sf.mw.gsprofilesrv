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




#ifndef VIDEOPREVIEWDIALOG_H_
#define VIDEOPREVIEWDIALOG_H_


//  INCLUDES
#include <AknDialog.h>

#include "mediafilepreview.h"

// CLASS DECLARATION


/**
* CVideoPreviewDialog
*
*/
NONSHARABLE_CLASS (CVideoPreviewDialog) : public CAknDialog,
                                          public MPreviewHandlerObserver,
                                          public MCoeForegroundObserver
    {
    public:// Constructors and destructor
        static CVideoPreviewDialog* NewL( TAny* aVideoHandler );
        virtual ~CVideoPreviewDialog();
    
    public:
        TBool ShowDialogLD( TInt* aError );
        void SetAttrL( TInt aAttr, TInt aValue );
        void SetAttrL( TInt aAttr, const TDesC& aValue );
        void SetAttrL( TInt aAttr, TAny* aValue );
        // Close the dialogue to cancel the preview
        void CloseDialogL();


    private:
        CVideoPreviewDialog( TAny* aVideoHandler );
        void ConstructL();
        
    private:
        void PreLayoutDynInitL();
        void PostLayoutDynInitL();
        TBool OkToExitL( TInt aButtonId );
        void ProcessCommandL(TInt aCommandId);
        void HandleResourceChange(TInt aType);
        
    private:
        void CloseDialogWithDelayL();
        static TInt CloseDialog( TAny *aObj );
        
    private:   // from MPreviewHandlerObserver
        void HandlePreviewEventL( TInt aEvent, TInt aError );
        
    private:
        void HandleGainingForeground();
        void HandleLosingForeground();
        
        
    private:
        CMFVideoPreviewHandler* iVideoHandler;
        TBool iVideoHandlerCreated;
        
        // for closing dialog
        CIdle* iIdle;
        
        TInt* iError;
        
    };


#endif /*VIDEOPREVIEWDIALOG_H_*/
