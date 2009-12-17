/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Container for the Barring folder
*
*/


// INCLUDE FILES
#include "GSCallBarringPluginContainer.h"

#include <aknlists.h>
#include <StringLoader.h>
#include <GSCallBarringPluginRsc.rsg>
#include <csxhelp/cp.hlp.hrh>
#include <featmgr.h>
#include <gsfwviewuids.h>
#include <gslistbox.h>

#include <centralrepository.h>
#include <settingsinternalcrkeys.h>
#if defined(__VOIP) && defined(RD_VOIP_REL_2_2)
#include <spsettings.h>
#endif // __VOIP && RD_VOIP_REL_2_2
// ========================= MEMBER FUNCTIONS ================================
// ---------------------------------------------------------------------------
// 
// Symbian OS two phased constructor
// 
// ---------------------------------------------------------------------------
void CGSCallBarringPluginContainer::ConstructL( const TRect& aRect )
    {
    iListBox = new ( ELeave ) CAknSingleLargeStyleListBox;
    BaseConstructL( aRect, R_GS_BARR_VIEW_TITLE, R_INTERNET_BARRING_LBX );

#if defined(__VOIP) && defined(RD_VOIP_REL_2_2)
    iVoIPSupported = EFalse;
    CSPSettings* spSettings = CSPSettings::NewL();
    if ( spSettings->IsFeatureSupported( ESupportVoIPFeature ) &&
        spSettings->IsFeatureSupported( ESupportVoIPSSFeature ) )
        {
        iVoIPSupported = ETrue;
        }
#else // __VOIP && RD_VOIP_REL_2_2
    if ( FeatureManager::FeatureSupported ( KFeatureIdCommonVoip ) )
        {
        TInt supported( KGSSettingOff );
        CRepository* telephonyrep = 
            CRepository::NewL( KCRUidTelephonySettings );
        telephonyrep->Get( KDynamicVoIP, supported );
        iVoIPSupported = KGSSettingOff != supported;
        delete telephonyrep;
        }
#endif // __VOIP && RD_VOIP_REL_2_2
    }

// ---------------------------------------------------------------------------
// 
// Destructor
//  
// ---------------------------------------------------------------------------
CGSCallBarringPluginContainer::~CGSCallBarringPluginContainer()
    {
    if ( iListboxItemArray )
        {
        delete iListboxItemArray;
        }
    }
// ---------------------------------------------------------------------------
// 
// Creates list box
//  
// ---------------------------------------------------------------------------
void CGSCallBarringPluginContainer::ConstructListBoxL( TInt aResLbxId )
    {
    iListBox->ConstructL( this );
    iListboxItemArray = CGSListBoxItemTextArray::NewL( aResLbxId,
        *iListBox, *iCoeEnv );
    iListBox->Model()->SetItemTextArray( iListboxItemArray );
    iListBox->Model()->SetOwnershipType( ELbmDoesNotOwnItemArray );
    }

// ---------------------------------------------------------------------------
// CGSCallBarringPluginContainer::GetHelpContext
// Gets Help Context
//  
// ---------------------------------------------------------------------------
//
void CGSCallBarringPluginContainer::GetHelpContext(
    TCoeHelpContext& aContext ) const
    {
    if ( iVoIPSupported )
        {
        aContext.iMajor = KUidGS;
        aContext.iContext = KDIV_HLP_BARRING_MAIN;
        }
    else
        {
        aContext.iMajor = KUidGS;
        aContext.iContext = KSET_HLP_BARRING_SETTINGS;
        }
    }
//End of File
