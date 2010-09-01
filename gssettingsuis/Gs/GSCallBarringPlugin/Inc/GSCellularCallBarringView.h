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


#ifndef GSSETTLISTBARRINGVIEW_H
#define GSSETTLISTBARRINGVIEW_H

// INCLUDES
#include <aknview.h>
#include <PsetCallBarring.h>
#include <etelmm.h>
#include <eikclb.h>

#include "GSLocalBaseView.h"
#include "CallBarringPlugin.hrh"

// FORWARD DECLARATIONS
class CGSBaseContainer;
class CGSSettListBarringContainer;
class CAknNavigationDecorator;
class CPsetContainer;
class CPsuiContainer;
class CPsuiBarringObs;
class CAknStaticNoteDialog;
class CAknNoteDialog;

// CLASS DECLARATION
/***************************************************
*     CGSSettListBarringView view class            *
***************************************************/
class CGSSettListBarringView : public CGSLocalBaseView
    {
    public: // Constructors and destructor

        /* C++ Constructor */
        CGSSettListBarringView();

        /**
        * Symbian OS two phased constructor.
        */
        static CGSSettListBarringView* NewL();

        /* Symbian OS two phase constructor - puts self into stack. 
        *
        * @return CGSSettListBarringView object
        */
        static CGSSettListBarringView* NewLC();

        /**
        * Destructor.
        */
        ~CGSSettListBarringView();

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
		/**
        * From MEikCommandObserver, handles the commands.
        *
        * @param aCommand identifies the command given.
        */
        void ProcessCommandL( TInt aCommand );

    public: //new
        
        /**
        * Creates the barring password query. 
        * 
        * @param aBarringPassword is user inputed password
        * @param aTitleID is type of password query 
        * @return softkey pressed in pwd query
        */
        TInt BarringPasswordQueryL( TBarringPassword& aBarringPassword,    
                                    const TInt& aTitleID );

    protected: // from CAknView

        void DoActivateL( const TVwsViewId& aPrevViewId,
                          TUid aCustomMessageId,
                          const TDesC8& aCustomMessage );
    
        void DoDeactivate();

       
    private:
    
        /**
        * Adds item to a list
        * @param aList pointer to a list
        * @param aItem resource ID of the item text
        */
        void AppendItemL( CDesCArrayFlat& aList, const TInt aItem );

        /**
        * Selects barring type
        * @param aCommand user action
        * @param aBarringSetting barring type is set into this
        * @param aType value used for mapping the current type
        */
        void SetBarringType( TInt& aCommand, 
            TCallBarringSetting& aBarringSetting, 
            TGSBarringSetting aType );

        /**
        * Handles barring operation requests
        * @param aBarringSetting barring settings (e.g. pwd) passed to TSY
        * @param aBsc basic service groups
        */
        void HandleBarringOperationL( TCallBarringSetting aBarringSetting,
            TBasicServiceGroups aBsc );

        /** 
        * Creates a request to cancel all barrings.
        *
        */
        void CancelAllBarringsL();

        /**
        * Handles changing of barring password.
        *
        */
        void ChangeBarringPwdL();

    private: // From MEikMenuObserver
        
        void DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane );
    
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

    private: // Member variables
        //Navi pane
        CAknNavigationDecorator* iNaviPaneContext;
        //Pointer to PhoneSetting's barring object
        CPsetCallBarring*   iBarring;
        //Pointer to PhoneSetting's container
        CPsetContainer*     iSettings;
        //Pointer to PSUI's container
        CPsuiContainer*     iObsContainer;
        //Pointer to PSUI's barring object
        CPsuiBarringObs*    iCbObserver;
        //Has selection key been pressed
        TBool               iRockerPress;
        //Has context menu initialized
        TBool               iContextMenuInitialized;
    };

#endif //GSSETTLISTBARRINGVIEW_H

// End of File

