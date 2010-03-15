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
* Description:  Container class for main view.
*
*/

#ifndef GSCONTAINER_H
#define GSCONTAINER_H

#include    "gsbasecontainer.h"
#include    <bldvariant.hrh>
#include    <AknIconArray.h>
#include    <AknsUtils.h>     // For loading icons
#include    <gsmainview.h>
// CLASS DECLARATION
class CAknViewAppUi;
class CAknSingleLargeStyleListBox;
class CGSPluginInterface;

/**
* Container class for the GS Main view.
* @since Series60_3.1
*/
class CGSMainContainer : public CCoeControl, public MEikListBoxObserver
{
    public: // Constructors and destructor

        /**
        * Symbian OS constructor.
        * Required by the base class.
        *
        */
        void ConstructL(
            const TRect& aRect,
            CAknViewAppUi* aAppUi,
            CArrayPtrFlat<CGSPluginInterface>* aPluginArray );

        /**
        * Destructor.
        */
        ~CGSMainContainer();

    public: // New

        /**
        * Updates listbox.
        */
        void UpdateListBoxL();

        /**
        * @return pointer to owned listbox.
        */
        CAknSingleLargeStyleListBox* ListBox();

        /**
        * @return Plugin which is selected in the lbx.
        */
        CGSPluginInterface* SelectedPlugin();

        /**
        * Sets selected lbx item using the UID of the item. If the item is not
        * existing or visible anymore, selected item is the first item in the
        * lbx.
        */
        void SetSelectedItem( TUid aSelectedItemUid );
        
        /**
         * Set the empty text of list box.
         */
        void SetListBoxEmptyTextL(const TDes& aEmpty);

        /**
         * Store listbox's exact position.
         * @param aPosition includes the exact position of the listbox.
         */
        void StoreListBoxPositionL( CGSMainView::TListBoxPosition& aPosition );

        /**
         * Restore listbox's exact position.
         * @param aPosition includes the exact position of the listbox.
         * @param aScreenModeChanged indicates whether the screen mode is changed.
         */
        void RestoreListBoxPositionL( const CGSMainView::TListBoxPosition& aPosition, TBool aScreenModeChanged );
        

    public: // From CCoeControl

        /**
        * See base class.
        */
        void HandleResourceChange( TInt aType );

        /**
        * Set focus on the selected listbox. For animated skins feature.
        */
        void FocusChanged( TDrawNow aDrawNow );

    protected: // New

        /**
        * Required for help.
        */
        void GetHelpContext( TCoeHelpContext& aContext ) const;

    protected: // From MEikListBoxObserver
        /**
        * Handles listbox events.
        * @param aListBox Listbox where the event occurred.
        * @param aEventType Event type.
        */
        void HandleListBoxEventL( CEikListBox* aListBox,
                                  TListBoxEvent aEventType );
    private: // New

        /**
        * Adds plugin data to listbox.
        * @param aPlugin A plugin whose data is to be used.
        * @param aIndex Index of the plugin in the listbox.
        * @param aItemTextArray Array for plugin texts and format strings used
        *        by lbx.
        * @param aIconArray Array for icons used in lbx.
        */
        void AddPluginDataToLbxL( CGSPluginInterface* aPlugin,
                                  TInt aIndex,
                                  CDesCArray* aItemTextArray,
                                  CAknIconArray* aIconArray );

    private: // From CCoeControl

        /**
        * See base class.
        */
        void SizeChanged();

        /**
        * See base class.
        */
        TInt CountComponentControls() const;

        /**
        * See base class.
        */
        CCoeControl* ComponentControl( TInt /*aIndex*/ ) const;

        /**
        * See base class.
        */
        TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent,
                                     TEventCode aType );

    private: // Data

        // Owned listbox.
        CAknSingleLargeStyleListBox* iListBox;

        // Pointer to Application UI. Not owned.
        CAknViewAppUi* iAppUi;

        /**
        * Pointer to listbox model's item texts. Note that the indexes of
        * listbox items are mapped directly to the indexes in plugin array.
        * Plugin which maps to selected listbox item index in plugin array is
        * activated when item is selected in listbox.
        *
        * Owned by iListBox.
        */
        CDesCArray* iItemTextArray;

        // Given from CGSMainView - not owned
        CArrayPtrFlat<CGSPluginInterface>* iPluginArray;

        // Array of pointers to iPluginArray plugins which are visible.
        // This is the actual array which is displayed by the listbox.
        // It is needed because iPluginArray also contains non-visible plugins
        // and therefore the indexes do not match to lbx items.
        CArrayPtrFlat<CGSPluginInterface>* iVisiblePlugins;
};


#endif // GSCONTAINER_H
// End of File
