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
* Description:  Barrings sub-menu in General Settings.
*
*/


#ifndef GSCALLBARRINGPLUGIN_H
#define GSCALLBARRINGPLUGIN_H

// INCLUDES
#include <gsbaseview.h>
#include <aknview.h>
#include <gspubsubslistener.h>
#include <mgssettingpsobserver.h>
#include <PsetCSP.h>
#include <PsetCallBarring.h>
#include <etelmm.h>
#include <eikclb.h>

#include "GSLocalBaseView.h"
#include "CallBarringPlugin.hrh"


// FORWARD DECLARATIONS
class CGSBaseContainer;
class CGSCallBarringPluginContainer;
class CPsetCustomerServiceProfile;
class CAknNavigationDecorator;
class CPsetContainer;
class CPsuiContainer;
class CPsuiBarringObs;
class CAknStaticNoteDialog;
class CAknNoteDialog;

// CONSTANTS
_LIT( KGSCallBarringPluginIconDirAndName, "z:GSCallBarringPlugin.mbm");


// CLASS DECLARATION
/***************************************************
*     CGSCallBarringPlugin class                   *
****************************************************/
class CGSCallBarringPlugin : public CGSBaseView,
                             public MGSSettingPSObserver
    {
    public: // Constructors and destructor

        /* C++ Constructor */
        CGSCallBarringPlugin();

        /* Symbian OS two phase constructor - puts self into stack. 
        *
        * @return CGSCallBarringPlugin object
        */
        static CGSCallBarringPlugin* NewLC();

        /**
        * Destructor.
        */
        ~CGSCallBarringPlugin();
        
        /**
        * Symbian OS two-phased constructor
        * @return GS call barring view.
        */
        static CGSCallBarringPlugin* NewL( TAny* aInitParams );
        
        /**
        *
        */
        void HandleClientRectChange();

    public: // Functions from base classes

        /**
        * From CAknView, returns the views id.
        *
        * @return Returns TUid, which is the id of the view.
        */
        TUid Id() const;

        /**
        * From MEikCommandObserver, handles the commands given through menu.
        *
        * @param aCommand is the given command.
        */
        void HandleCommandL( TInt aCommand );
        
    public: // From CGSPluginInterface

        /**
        * @see CGSPluginInterface header file.
        */
        void GetCaptionL( TDes& aCaption ) const;

        /**
        * See base class.
        */
        CGulIcon* CreateIconL( const TUid aIconType );

        /**
        * @see CGSPluginInterface header file.
        */
        TInt PluginProviderCategory() const;

        /**
        * @see CGSPluginInterface header file.
        */
        TBool Visible() const;

    public: //new
        
        /**
        * This is a locally owned method used to activate
        * other local views seen from GSTelPlugin
        * @param aFeatureId feature ID of the folder
        */
        void ActivateInternalViewL();

        /**
        * This is a locally owned method used to initialize
        * other local views seen from GSTelPlugin
        * @param aLocalViewId view ID of local views to this plugin
        */
        void CreateLocalViewL( TUid aLocalViewId );
        
        /**
        * Removes sub-views owned by this plugin to make sure sub-view UIDs do
        * not clash.
        */
        void RemoveLocalViews();
        
        /**
        *Is VoIP feature supported 
        */
        TBool VoipSupported() const;

    protected: // from CAknView

        void DoActivateL( const TVwsViewId& aPrevViewId,
                          TUid aCustomMessageId,
                          const TDesC8& aCustomMessage );
    
        void DoDeactivate();

    private: // from base class

        /**
        * Symbian OS default constructor. 
        *
        */
        void ConstructL();
    
        /**
        * From CGSBaseView, handles selection of list box.
        */
        void HandleListBoxSelectionL();

        void NewContainerL();
    
    private: // From MGSSettingPSObserver
        /**
        * Callback from MGSSettingPSObserver
        */
        void HandleNotifyPSL( const TUid aUid, const TInt& aKey, 
                              const TRequestStatus& aStatus );
        
    private: //new
        /**
        * Retrieves container pointer.
        */
        CGSCallBarringPluginContainer*  Container();

        /**
        * Customer Service Profile status setting
        */
        void SetCSPActiveL( TBool aValue );

    private: // Member variables
        //Navi pane
        CAknNavigationDecorator* iNaviPaneContext;
        //PubSub object for BT SAP state
        CGSPubSubsListener* iBtSapListener;
        //check if CSP is active
        CPsetCustomerServiceProfile* iCSP;
        // Call barring view. Not owned by this - owned by iAppUi.
        CGSLocalBaseView* iBarringView;
        // check if voip supported
        TBool iVoipSupported;
    };

#endif //GSCALLBARRINGPLUGIN_H

// End of File
