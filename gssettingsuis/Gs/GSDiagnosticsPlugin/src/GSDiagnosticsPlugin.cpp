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
* Description:  GSDiagnosticsPlugin implementation.
*
*/


// User includes
#include    "GSDiagnosticsPlugin.h"
#include    <GSParentPlugin.h>
#include    <GSCommon.hrh>
#include    <gsdiagnosticspluginrsc.rsg> // GUI Resource
#include    <GSPrivatePluginProviderIds.h>
#include    <GSFWViewUIDs.h>
#include    "GSLogger.h"

// System includes
#include    <AknWaitDialog.h>
#include    <aknnotewrappers.h>
#include    <AknRadioButtonSettingPage.h>
#include    <AknTextSettingPage.h>
#include    <AknViewAppUi.h>
#include    <bautils.h>
#include    <FeatMgr.h>
#include    <hlplch.h> // HlpLauncher
#include    <StringLoader.h>

#include    <ImplementationInformation.h>
#include    <UTF.H> // CnvUtfConverter

// Constants

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSDiagnosticsPlugin::CGSDiagnosticsPlugin()
// Constructor
//
// ---------------------------------------------------------------------------
//
CGSDiagnosticsPlugin::CGSDiagnosticsPlugin()
    : iResources( *iCoeEnv )
    {
    }


// ---------------------------------------------------------------------------
// CGSDiagnosticsPlugin::~CGSDiagnosticsPlugin()
// Destructor
//
// ---------------------------------------------------------------------------
//
CGSDiagnosticsPlugin::~CGSDiagnosticsPlugin()
    {
    iResources.Close();
    }


// ---------------------------------------------------------------------------
// CGSDiagnosticsPlugin::ConstructL(const TRect& aRect)
// Symbian OS two-phased constructor
//
// ---------------------------------------------------------------------------
//
void CGSDiagnosticsPlugin::ConstructL()
    {
    OpenLocalizedResourceFileL( KOperatorLogoResourceFileName, iResources );
    BaseConstructL( R_GS_DIAGNOSTICS_VIEW );
    __GSLOGSTRING( "[CGSDiagnosticsPlugin] ConstructL()" );
    }


// ---------------------------------------------------------------------------
// CGSDiagnosticsPlugin::NewL()
// Static constructor
//
// ---------------------------------------------------------------------------
//
CGSDiagnosticsPlugin* CGSDiagnosticsPlugin::NewL( TAny* /*aInitParams*/ )
    {
    CGSDiagnosticsPlugin* self = new( ELeave ) CGSDiagnosticsPlugin();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }


// -----------------------------------------------------------------------------
// CGSDiagnosticsPlugin::Id
//
//
// -----------------------------------------------------------------------------
//
TUid CGSDiagnosticsPlugin::Id() const
    {
    return KGSDiagnosticsPluginUID;
    }


// -----------------------------------------------------------------------------
// CGSDiagnosticsPlugin::DoActivateL
//
//
// -----------------------------------------------------------------------------
//
void CGSDiagnosticsPlugin::DoActivateL( const TVwsViewId& /*aPrevViewId*/,
                                  TUid /*aCustomMessageId*/,
                                  const TDesC8& /*aCustomMessage*/ )
    {
    // Update these values to define what is tested.
    iPrintInfo  = ETrue;
    iTestLoad   = EFalse;
    iCheckHeap  = EFalse;

    CheckInfoL( KGSPluginInterfaceUid );
    }


// -----------------------------------------------------------------------------
// CGSDiagnosticsPlugin::DoDeactivate
//
//
// -----------------------------------------------------------------------------
//
void CGSDiagnosticsPlugin::DoDeactivate()
    {
    }


// ========================= From CGSPluginInterface ==================


// -----------------------------------------------------------------------------
// CGSDiagnosticsPlugin::GetCaptionL
//
//
// -----------------------------------------------------------------------------
//
void CGSDiagnosticsPlugin::GetCaptionL( TDes& aCaption ) const
    {
    HBufC* result = StringLoader::LoadL( R_GS_DIAGNOSTICS_PLUGIN_CAPTION );
    aCaption.Copy( *result );
    delete result;
    }


// -----------------------------------------------------------------------------
// CGSDiagnosticsPlugin::PluginProviderCategory
//
//
// -----------------------------------------------------------------------------
//
TInt CGSDiagnosticsPlugin::PluginProviderCategory() const
    {
    return KGSPluginProviderInternal;
    }


// -----------------------------------------------------------------------------
// CGSDiagnosticsPlugin::ItemType()
//
//
// -----------------------------------------------------------------------------
//
TGSListboxItemTypes CGSDiagnosticsPlugin::ItemType()
    {
    return EGSItemTypeSettingDialog;
    }


// -----------------------------------------------------------------------------
// CGSDiagnosticsPlugin::GetValue()
//
//
// -----------------------------------------------------------------------------
//
void CGSDiagnosticsPlugin::GetValue( const TGSPluginValueKeys /*aKey*/,
                                      TDes& /*aValue*/ )
    {

    }


// -----------------------------------------------------------------------------
// CGSDiagnosticsPlugin::HandleSelection()
//
//
// -----------------------------------------------------------------------------
//
void CGSDiagnosticsPlugin::HandleSelection(
    const TGSSelectionTypes aSelectionType )
    {
    TRAP_IGNORE( CheckInfoL( KGSPluginInterfaceUid ) );
    }


// -----------------------------------------------------------------------------
// CGSDiagnosticsPlugin::Visible()
//
//
// -----------------------------------------------------------------------------
//
TBool CGSDiagnosticsPlugin::Visible() const
    {
    return ETrue;
    }


// -----------------------------------------------------------------------------
// CGSDiagnosticsPlugin::OpenLocalizedResourceFileL()
//
//
// -----------------------------------------------------------------------------
//
void CGSDiagnosticsPlugin::OpenLocalizedResourceFileL(
    const TDesC& aResourceFileName,
    RConeResourceLoader& aResourceLoader )
    {
    RFs fsSession;
    User::LeaveIfError( fsSession.Connect() );

    // Find the resource file:
    TParse parse;
    parse.Set( aResourceFileName, &KDC_RESOURCE_FILES_DIR, NULL );
    TFileName fileName( parse.FullName() );

    // Get language of resource file:
    BaflUtils::NearestLanguageFile( fsSession, fileName );

    // Open resource file:
    aResourceLoader.OpenL( fileName );

    // If leave occurs before this, close is called automatically when the
    // thread exits.
    fsSession.Close();
    }


// ----------------------------------------------------------------------------
// CGSDiagnosticsPlugin::PrintInfoDebug
// Print CImplementationInformation to log. Used for debugging.
//
// ----------------------------------------------------------------------------
//
void CGSDiagnosticsPlugin::PrintInfoDebug( CImplementationInformation* aInfo )
    {
    TBuf16<256> dataType;
    TBuf16<256> opaqueData;
    CnvUtfConverter::ConvertToUnicodeFromUtf8(
        dataType, aInfo->DataType() );
    CnvUtfConverter::ConvertToUnicodeFromUtf8(
        opaqueData, aInfo->OpaqueData() );

    __GSLOGSTRING( "." );
    __GSLOGSTRING1( "   DisplayName:        %S", &aInfo->DisplayName() );
    __GSLOGSTRING1( "   ImplementationUid:  0x%X", aInfo->ImplementationUid() );
    __GSLOGSTRING1( "   DataType:           %S", &dataType );
    __GSLOGSTRING1( "   OpaqueData:         %S", &opaqueData );

    }


// -----------------------------------------------------------------------------
// CGSDiagnosticsPlugin::CheckInfoL()
//
//
// -----------------------------------------------------------------------------
//
void CGSDiagnosticsPlugin::CheckInfoL( TUid aInterfaceUid )
    {
    RImplInfoPtrArray implInfoArray;

    REComSession::ListImplementationsL( aInterfaceUid, implInfoArray );

    if( iPrintInfo )
        {
        __GSLOGSTRING( "[CGSDiagnosticsPlugin implementation information]" );
        for( TInt i = 0; i < implInfoArray.Count(); i++ )
            {
            PrintInfoDebug( implInfoArray[i] );
            }
        __GSLOGSTRING( "[End of implementation information]" );
        }

    CImplementationInformation* info;
    CGSPluginInterface* plugin;

    if( iTestLoad )
        {
        __GSLOGSTRING( "[CGSDiagnosticsPlugin plugin load/delete checks...]" );
        for( TInt i = 0; i < implInfoArray.Count(); i++ )
            {
            info = implInfoArray[i];
            __GSLOGSTRING1( "[CGSDiagnosticsPlugin] Checking: %S",
                            &info->DisplayName() );

            if ( iCheckHeap )
              {
              __UHEAP_MARK;
              }

            //Load plugin:
            plugin = CGSPluginInterface::NewL(
                info->ImplementationUid(),
                AppUi() );

            // Delete plugin:
            delete plugin;
            plugin = NULL;

            if ( iCheckHeap )
                {
                __UHEAP_MARKEND;
                }

            __GSLOGSTRING1( "[CGSDiagnosticsPlugin]     %S OK",
                            &info->DisplayName() );
            }
        }

    implInfoArray.ResetAndDestroy();// This is needed
    }


// End of file
