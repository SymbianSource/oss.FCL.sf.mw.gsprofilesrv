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
* Description:  Main Barrings sub-menu in General Settings.
*
*/


#ifndef GSSETTLISTMAINBARRINGVIEW_H
#define GSSETTLISTMAINBARRINGVIEW_H

// INCLUDES
#include "GSLocalBaseView.h"
#include <aknview.h>

// FORWARD DECLARATIONS
class CGSSettListMainBarringContainer;
class CAknView;

// CLASS DECLARATION

/**
* CGSSettListMainBarringView view class
* @since 3.0
*/
class CGSSettListMainBarringView : public CGSLocalBaseView
    {
    public: // Constructors and destructor

        /* C++ Constructor */
        CGSSettListMainBarringView();

        /* Symbian OS two phase constructor - puts self into stack. 
        *
        * @return CGSSettListMainBarringView object
        */
        static CGSSettListMainBarringView* NewLC();

        /**
        * Destructor.
        */
        ~CGSSettListMainBarringView();

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

    private:

        /**
        * Symbian OS default constructor. 
        *
        */
        void ConstructL();
    
        /**
        * From CGSLocalBaseView, handles selection of list box.
        */
        void HandleListBoxSelectionL();
    
        /**
        * Retrieves container pointer.
        */
        CGSSettListMainBarringContainer*  Container();


    private: // From MEikMenuObserver
        
        void DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane );
    
    private: // from CGSLocalBaseView

        void NewContainerL();

    private: // from CAknView
    
        void DoActivateL( const TVwsViewId& aPrevViewId,
                          TUid aCustomMessageId,
                          const TDesC8& aCustomMessage );
    
        void DoDeactivate();

    private: // Member variables
    
        // Call barring view. Not owned by this - owned by iAppUi.
        CAknView* iBarringView;
        
        // Internet call barring view. Not owned by this - owned by iAppUi.
        CAknView* iInternetBarringView;
            
    };

#endif //GSSETTLISTMAINBARRINGVIEW_H

//End of File

