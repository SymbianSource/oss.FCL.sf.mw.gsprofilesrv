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
* Description:   Interface of MediaFileList. Provide some API for 
*                the user to use mediafilelist
*
*/




// INCLUDE FILES

#include "mediafilelist.h"
#include "mediafiledialog.h"



// -----------------------------------------------------------------------------
// CMediaFileList::NewL
//
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CMediaFileList* CMediaFileList::NewL()
    {
    CMediaFileList* self = new (ELeave) CMediaFileList();
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
EXPORT_C CMediaFileList::~CMediaFileList()
    {
    delete iDialog;
    }


// -----------------------------------------------------------------------------
// CMediaFileList::ConstructL
//
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMediaFileList::ConstructL()
    {
    iDialog = CMediaFileDialog::NewL();
    }


// -----------------------------------------------------------------------------
// CMediaFileList::CMediaFileList
//
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMediaFileList::CMediaFileList()
    {
    }


// -----------------------------------------------------------------------------
// CMediaFileList::ShowCMediaFileListL
//
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CMediaFileList::ShowMediaFileListL( TDes* aFileName, 
                                                   TInt* aNullItem,
                                                   TInt* aInfo,
                                                   TDes* aDesInfo )
    {
    TInt err = KErrNone;
    TInt ret = EFalse;
    TRAP( err, ret = iDialog->ShowDialogL( aFileName, aNullItem, aInfo, aDesInfo ) );
    
    iDialog = NULL;
    
    User::LeaveIfError( err );
    
    if ( !ret )
        {
    	  return EFalse;
    	  }
    
    if ( aFileName->Length() == 0 )
        {
        return EFalse;
        }
    return ETrue;
    }


// -----------------------------------------------------------------------------
// CMediaFileList::SetAttrL
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMediaFileList::SetAttrL( TInt aAttr, TInt aValue )
    {
    if ( iDialog )
        {
        iDialog->SetAttrL( aAttr, aValue );
        }
    }

// -----------------------------------------------------------------------------
// CMediaFileList::SetAttrL
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMediaFileList::SetAttrL( TInt aAttr, const TDesC& aValue )
    {
    if ( iDialog )
        {
        iDialog->SetAttrL( aAttr, aValue );
        }
    }


// -----------------------------------------------------------------------------
// CMediaFileList::SetAttrL
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMediaFileList::SetAttrL( TInt aAttr, TAny* aValue )
    {
    if ( iDialog )
        {
        iDialog->SetAttrL( aAttr, aValue );
        }
    }


// -----------------------------------------------------------------------------
// CMediaFileList::SetNullItemL
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMediaFileList::SetNullItemL( const TDesC& aText, 
                                            const TDesC& aMediaFile,
	                                        TInt aMediaFileType, TInt aIconId )
    {
    if ( iDialog )
        {
        iDialog->SetNullItemL( aText, aMediaFile, aMediaFileType, aIconId );
        }
    }



// End of file
