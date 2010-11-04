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
* Description:  Base vew in GSCallBarringPlugin.
*
*/


#ifndef GSLOCALBASEVIEW_H
#define GSLOCALBASEVIEW_H

// INCLUDES
#include <aknview.h>
#include <etelmm.h>
#include <eikclb.h>

// FORWARD DECLARATIONS
class CGSBaseContainer;
class CAknNavigationDecorator;
class CAknViewAppUi;

// CLASS DECLARATION
/***************************************************
*     CGSLocalBaseView view class            *
***************************************************/
class CGSLocalBaseView : public CAknView,
                         public MEikListBoxObserver
    {
    public:
        // Panic code used in this class
        enum KGSViewPanicCodes
            {
            EGSViewPanicNullPtr = 1
            };

    public: // Constructors and destructor

        /**
        * Symbian OS default constructor. 
        *
        */
        virtual void ConstructL()=0;

        /* C++ Constructor */
        CGSLocalBaseView();

        /**
        * Destructor.
        */
        ~CGSLocalBaseView();

    public:

        /**
        * This function is used to set the current item in the listbox.
        * @param aIndex Current item. Default value is 0 i.e. the first item of the list.
        * 
        */
        virtual void SetCurrentItem( TInt aIndex = 0 );

    protected: // from CAknView

        /**
        * Handles client rect change.
        * 
        */
        void HandleClientRectChange();

        void DoActivateL( const TVwsViewId& aPrevViewId,
                          TUid aCustomMessageId,
                          const TDesC8& aCustomMessage );
    
        void DoDeactivate();

    protected: // from MEikListBoxObserver

        /**
        * Handles listbox events.
        * @param aListBox Listbox where the event occurred.
        * @param aEventType Event type.
        * 
        */
        void HandleListBoxEventL( CEikListBox* aListBox,
                                  TListBoxEvent aEventType );
        
    protected: //new

        /**
        * Sets the correct navi pane for the view (eg. tabs, empty, string etc.)
        * 
        */
        virtual void SetNaviPaneL();

        /**
        * Creates context for view's navi pane.
        * @param aResourceId Resource id for navi pane context's resource.
        * 
        */
        virtual void CreateNaviPaneContextL( TInt aResourceId );

    protected:
        // Called by DoActivateL
        virtual void NewContainerL() = 0;
        virtual void CreateContainerL();

    private:
    
        /**
        * Handles selection of list box.
        * Called by HandleListBoxEventL
        */
        virtual void HandleListBoxSelectionL() = 0;        

    protected: // Member variables
        //Navi pane
        CAknNavigationDecorator* iNaviPaneContext;
        //Handle to get the view
        CAknViewAppUi*      iAppUi;
        // currently selected listbox item
        TInt                iCurrentItem;
        // first item in the listbox
        TInt                iTopItemIndex;
        // pointer to the container
        CGSBaseContainer*   iContainer;
    };
//end of file
#endif //GSLOCALBASEVIEW_H
