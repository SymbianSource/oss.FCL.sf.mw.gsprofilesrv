/*
* Copyright (c) 2005-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  CGSPluginInteface implementation.
*
*/

#include <gsplugininterface.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CGSPluginInterface::CGSPluginInterface()
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
EXPORT_C CGSPluginInterface::CGSPluginInterface()
    : iOrder( KGSPluginNotIndexed )
    {
    }


// -----------------------------------------------------------------------------
// CGSPluginInterface::~CGSPluginInterface()
// Destructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CGSPluginInterface::~CGSPluginInterface()
    {
    // We don't unload the plugin object here. The loader
    // has to do this for us. Without this kind of destruction idiom,
    // the view framework can potentially unload an ECOM plugin dll whilst
    // there are still child views (Created by the plugin) that are registered
    // with the view framework. If this occurs, the plugin code segment isn't
    // loaded anymore and so there is no way to run the subsequent destructor
    // code => exception.
    }


// -----------------------------------------------------------------------------
// CGSPluginInterface::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CGSPluginInterface* CGSPluginInterface::NewL( TUid aImplementationUid,
                                                       TAny* aInitParams )
    {
    TAny* ptr;
    TInt32 keyOffset = _FOFF( CGSPluginInterface, iDtor_ID_Key );
    ptr = REComSession::CreateImplementationL( aImplementationUid,
                                               keyOffset,
                                               aInitParams );

    CGSPluginInterface* plugin = REINTERPRET_CAST( CGSPluginInterface*, ptr );

    return plugin;
    }


// -----------------------------------------------------------------------------
// CGSPluginInterface::PluginProviderCategory()
//
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CGSPluginInterface::PluginProviderCategory() const
    {
    return EGSPluginProvider3rdParty;
    }

// -----------------------------------------------------------------------------
// CGSPluginInterface::CustomOperationL()
//
//
// -----------------------------------------------------------------------------
//
EXPORT_C TAny* CGSPluginInterface::CustomOperationL(
    TAny* /*aParam1*/,
    TAny* /*aParam2*/ )
    {
    return NULL;
    };


// -----------------------------------------------------------------------------
// CGSPluginInterface::Visible()
//
//
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CGSPluginInterface::Visible() const
    {
    return ETrue;
    }


// -----------------------------------------------------------------------------
// CGSPluginInterface::CreateIconL()
//
//
// -----------------------------------------------------------------------------
//
EXPORT_C CGulIcon* CGSPluginInterface::CreateIconL(
    const TUid aIconType )
    {
    TParse parse;
    parse.Set( KGSDefaultIconFileName, &KDC_BITMAP_DIR, NULL );
    TFileName fileName( parse.FullName() );
    CGulIcon* icon = NULL;

    if( aIconType == KGSIconTypeLbxItem )
        {
        icon = AknsUtils::CreateGulIconL(
            AknsUtils::SkinInstance(),
            KAknsIIDQgnPropSetDeviceSub,
            fileName,
            EMbmGsfwiconQgn_prop_set_default_sub,
            EMbmGsfwiconQgn_prop_set_default_sub_mask );
        }
    else if( aIconType == KGSIconTypeTab )
        {
        }
    else if( aIconType == KGSIconTypeDColumn )
        {
        }
    else // Unknown type
        {
        }
    return icon;
    }


// -----------------------------------------------------------------------------
// CGSPluginInterface::GetValue()
//
// Empty implementation just to minimize plugin implementor work.
// -----------------------------------------------------------------------------
//
EXPORT_C void CGSPluginInterface::GetValue( const TGSPluginValueKeys aKey,
                                            TDes& aValue )
    {
    switch( aKey )
        {
        case EGSPluginKeySettingsItemValueString:

            _LIT( KStr, "" );
            aValue.Append( KStr );
            break;

        default:
            _LIT( KEmptyStr, "" );
            aValue.Append( KEmptyStr );
            break;
        }
    }


// -----------------------------------------------------------------------------
// CGSPluginInterface::HandleSelection()
//
// Default implementation just to minimize plugin implementor work.
// On default activate plugin.
// -----------------------------------------------------------------------------
//
EXPORT_C void CGSPluginInterface::HandleSelection(
    const TGSSelectionTypes /*aSelectionType*/ )
    {
    // There is nothing pluginInterface can do if activating view fails.
    TRAP_IGNORE( AppUi()->ActivateLocalViewL( Id() ) );

    }


// -----------------------------------------------------------------------------
// CGSPluginInterface::ItemType()
//
// Default implementation just to minimize plugin implementor work.
// Default value EGSItemTypeSingleLarge
// -----------------------------------------------------------------------------
//
EXPORT_C TGSListboxItemTypes CGSPluginInterface::ItemType()
    {
    return EGSItemTypeSingleLarge;
    }


// -----------------------------------------------------------------------------
// CGSPluginInterface::ResetSelectedItemIndex()
//
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CGSPluginInterface::ResetSelectedItemIndex()
    {
    // Default implementation does nothing.
    }


// -----------------------------------------------------------------------------
// CGSPluginInterface::TGSMenuActivationItems()
//
//
// -----------------------------------------------------------------------------
//
EXPORT_C TGSMenuActivationItems CGSPluginInterface::MenuActivationItem()
    {
    // This default item will be the one read from the resource file.
    return EGSMenuActivationItemDefault;
    }

// -----------------------------------------------------------------------------
// CGSPluginInterface::SetOrder()
//
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CGSPluginInterface::SetOrder( TInt aOrder )
    {
    iOrder = aOrder;
    }


// -----------------------------------------------------------------------------
// CGSPluginInterface::GetEcomDestructorKey()
//
//
// -----------------------------------------------------------------------------
//
EXPORT_C TUid CGSPluginInterface::GetEcomDestructorKey()
    {
    return iDtor_ID_Key;
    }

//  End of File
