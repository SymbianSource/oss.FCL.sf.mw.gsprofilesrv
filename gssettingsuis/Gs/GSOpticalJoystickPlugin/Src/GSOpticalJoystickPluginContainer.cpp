/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Container for GSOpticalJoystickPlugin
*
*/


// INCLUDE FILES
#include "GSOpticalJoystickPluginContainer.h"
#include "GSOpticalJoystickPlugin.hrh"
#include "GsLogger.h"
#include <bldvariant.hrh>
#include <GSOpticalJoystickPluginRsc.rsg>
#include <centralrepository.h>
#include <aknlists.h>
#include <StringLoader.h>
#include <AknUtils.h>
#include <csxhelp/cp.hlp.hrh>
#include <gsfwviewuids.h>
#include <gslistbox.h>
#include <settingsinternalcrkeys.h>

// These enumations could be found in ofn_hal_groups.h
/**
 * The OFN HAL group
*/
enum
    {
    EHalGroupOFN = 28
    };

/**
 * Supported HAL functions
 *
*/
enum TOFNHalFunction
    {
    EOFNSetPowerON,     // OFN Powering (ON/OFF)
    EOFNGetPowerState   // For reading power state
    };

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSOpticalJoystickPluginContainer::ConstructL()
// 
// Symbian OS two phased constructor
// ---------------------------------------------------------------------------
//
void CGSOpticalJoystickPluginContainer::ConstructL( const TRect& aRect )
    {
    iPersonalizationRepository = CRepository::NewL( KCRUidPersonalizationSettings );

    iListBox = new( ELeave ) CAknSettingStyleListBox;

    BaseConstructL( aRect, R_GS_OPTICALJOYSTICK_VIEW_TITLE, R_OPTICALJOYSTICK_LBX );
    }

// ---------------------------------------------------------------------------
// CGSOpticalJoystickPluginContainer::~CGSOpticalJoystickPluginContainer()
// 
// Destructor 
// ---------------------------------------------------------------------------
//
CGSOpticalJoystickPluginContainer::~CGSOpticalJoystickPluginContainer()
    {
    delete iAutodisconValue;
    delete iListboxItemArray;
    if ( iPersonalizationRepository )
        {
        delete iPersonalizationRepository;
        iPersonalizationRepository = NULL;
        }
    }


// ---------------------------------------------------------------------------
// CGSOpticalJoystickPluginContainer::ConstructListBoxL()
// 
// Construct the listbox from resource array.
// ---------------------------------------------------------------------------
//
void CGSOpticalJoystickPluginContainer::ConstructListBoxL( TInt aResLbxId )
    {
    iListBox->ConstructL( this, EAknListBoxSelectionList );

    iListboxItemArray = CGSListBoxItemTextArray::NewL( aResLbxId, 
                                                 *iListBox, *iCoeEnv );
    iListBox->Model()->SetItemTextArray( iListboxItemArray );
    iListBox->Model()->SetOwnershipType( ELbmDoesNotOwnItemArray );

    CreateListBoxItemsL();
    }


// ---------------------------------------------------------------------------
// CGSOpticalJoystickPluginContainer::CreateListBoxItemsL()
// 
// Create listbox items.
// ---------------------------------------------------------------------------
//
void CGSOpticalJoystickPluginContainer::CreateListBoxItemsL()
    {
    MakeFocusedItemContentL();
    }


// ---------------------------------------------------------------------------
// CGSOpticalJoystickPluginContainer::UpdateListBoxL()
// 
// Update listbox item.
// ---------------------------------------------------------------------------
//
void CGSOpticalJoystickPluginContainer::UpdateListBoxL( TInt aFeatureId )
    {
    switch( aFeatureId )
        {
        case KGSSettIdOptJoyst:
            MakeFocusedItemContentL();
            break;
        default:
            break;
        }

    iListBox->HandleItemAdditionL();
    }


// ---------------------------------------------------------------------------
// CGSOpticalJoystickPluginContainer::MakeAutodisconItemL()
// 
// Create auto disconnect list item 
// ---------------------------------------------------------------------------
//
void CGSOpticalJoystickPluginContainer::MakeFocusedItemContentL()
    {
    TInt currentStatus = 0;
    GetOpticalJoystickStatus( currentStatus);
    __GSLOGSTRING1("[CGSOpticalJoystickPluginContainer]--> MakeFocusedItemContentL(),  value: %d", currentStatus);

    HBufC* buf = NULL;
    switch( currentStatus )
        {
        case 1:
            buf = iEikonEnv->AllocReadResourceL( R_OPTICAL_JOYSTICK_ON );
            break;
        case 0:
            buf = iEikonEnv->AllocReadResourceL( R_OPTICAL_JOYSTICK_OFF );
            break;
        default:
            buf = iEikonEnv->AllocReadResourceL( R_OPTICAL_JOYSTICK_OFF );
            break;
        }

    TPtr bufPtr( buf->Des() );
    iListboxItemArray->SetDynamicTextL( KGSSettIdOptJoyst, bufPtr );

    // And add to listbox
    iListboxItemArray->SetItemVisibilityL( KGSSettIdOptJoyst, CGSListBoxItemTextArray::EVisible );

    delete buf;
    }

// ---------------------------------------------------------------------------
// CGSOpticalJoystickPluginContainer::GetHelpContext() const
//  
// Gets Help 
// ---------------------------------------------------------------------------
//
void CGSOpticalJoystickPluginContainer::GetHelpContext( TCoeHelpContext& aContext ) const
    {
    aContext.iMajor = KUidGS;
    aContext.iContext = KCP_HLP_DEVICE_MANAGEMENT;
    }

// ---------------------------------------------------------------------------
// CGSOpticalJoystickPluginContainer::CurrentFeatureId()
//
// Return the feature id of selected listitem  
// ---------------------------------------------------------------------------
//
TInt CGSOpticalJoystickPluginContainer::CurrentFeatureId( ) const
    {
    return iListboxItemArray->CurrentFeature( );
    }

// ---------------------------------------------------------------------------
// CGSOpticalJoystickPluginContainer::GetOpticalJoystickStatus()
//
// Get the status of optical joystick
// ---------------------------------------------------------------------------
//
void CGSOpticalJoystickPluginContainer::GetOpticalJoystickStatus(TInt& aStatus)
    {
    TInt err = UserSvr::HalFunction( EHalGroupOFN, EOFNGetPowerState,
                            &aStatus, reinterpret_cast<TAny*>( ETrue ) );
    __GSLOGSTRING2( "[CGSOpticalJoystickPluginContainer::GetOpticalJoystickStatusL] err: %d, status: %d", err, aStatus );
    }

// ---------------------------------------------------------------------------
// CGSOpticalJoystickPluginContainer::SetOpticalJoystickStatus()
//
// Set the status of optical joystick
// ---------------------------------------------------------------------------
//
void CGSOpticalJoystickPluginContainer::SetOpticalJoystickStatusL(TInt aStatus)
    {
    TInt err = UserSvr::HalFunction( EHalGroupOFN, EOFNSetPowerON, 
                            reinterpret_cast<TAny*>( aStatus ), 
                            reinterpret_cast<TAny*>( aStatus ) );
    __GSLOGSTRING2( "[CGSOpticalJoystickPluginContainer::SetOpticalJoystickStatusL] err: %d, status: %d", err, aStatus );
    User::LeaveIfError( iPersonalizationRepository->Set( KSettingsOpticalJoystickStatus, aStatus ) );
    }

// End of File
