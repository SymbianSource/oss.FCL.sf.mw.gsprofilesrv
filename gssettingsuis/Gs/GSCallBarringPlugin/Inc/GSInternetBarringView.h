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
* Description:  Internet Barrings sub-menu in General Settings.
*
*/


#ifndef __GSSETTLISTINTERNETBARRINGVIEW_H
#define __GSSETTLISTINTERNETBARRINGVIEW_H

// INCLUDES
#include "GSLocalBaseView.h"

// FORWARD DECLARATIONS
class CGSSettListInternetBarringContainer;
class CAknRadioButtonSettingPage;
class CAknNoteDialog;
class CAknPopupList;
class CGSTelPluginModel;

// CLASS DECLARATION

/**
* CGSSettListInternetBarringView view class
* @since 3.0
*/

class CGSSettListInternetBarringView : public CGSLocalBaseView
    {
    public: // Constructors and destructor

        /* C++ Constructor */
        CGSSettListInternetBarringView();

        /**
        * Symbian OS two phased constructor.
        */
        static CGSSettListInternetBarringView* NewL();

        /* Symbian OS two phase constructor - puts self into stack. 
        *
        * @return CGSSettListInternetBarringView object
        */
        static CGSSettListInternetBarringView* NewLC();

        /**
        * Destructor.
        */
        ~CGSSettListInternetBarringView();

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

    private:
        /**
        * Symbian OS default constructor. 
        *
        */
        void ConstructL();
    
        /**
        * From CGSBaseView, handles selection of list box.
        */
        void HandleListBoxSelectionL();
        
        /**
        * Show Internet barring setting page.
        */
        void ShowSettingPageL();
        
        /**
        * Adds item to a list
        * @param aList reference to a list
        * @param aItem resource ID of the item to be added
        */
        void AppendItemL( CDesCArrayFlat& aList, TInt aItem );

    private: // From MEikMenuObserver
        
        void DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane );
    
    private: // from CGSBaseView

        void NewContainerL();
        

    private: // from CAknView
    
        void DoActivateL( const TVwsViewId& aPrevViewId,
                          TUid aCustomMessageId,
                          const TDesC8& aCustomMessage );
    
        void DoDeactivate();

    private: // Member variables
    
        //GSTelPluginModel for settings from Central Repository
        CGSTelPluginModel* iModel;
    };

#endif // GSINTERNETBARRINGVIEW_H

//End of File
