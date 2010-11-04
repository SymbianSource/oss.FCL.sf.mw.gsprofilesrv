/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Container for Call Divert view.
*
*/


#ifndef GSCALLDIVERTCONTAINER_H
#define GSCALLDIVERTCONTAINER_H

// INCLUDES
#include "gsbasecontainer.h"
#include "CallDivertPlugin.hrh"

// FORWARD DECLARATION
class CGSListBoxItemTextArray;

// CLASS DECLARATION
/**
*  GSCallDivertContainer container class
*/
class CGSCallDivertContainer : public CGSBaseContainer
    {
    public: // Constructors and destructor

        /**
        * Symbian OS default constructor.
        *
        * @param aRect gives the size of container
        */
        void ConstructL( const TRect& aRect );

        /* Destructor */
        ~CGSCallDivertContainer();

        /* Constructor */
        CGSCallDivertContainer( TGSCallDivertServiceGroups aItemId );

    public:
        /**
        * Retrieves the feature id for the selected item in the listbox
        * @return listbox item array's current feature.
        */
        TInt CurrentFeatureId() const;

    private:

        void GetHelpContext( TCoeHelpContext& aContext ) const;

        /* Updates listbox
        *   @param aItemId
        */
        void UpdateListBoxL( TInt aItemId );

        /* Creates listbox
        *   @param aResLbxId
        */
        void ConstructListBoxL( TInt aResLbxId );

        /* Sets title to container */
        void SetTitleL();

        void CreateListBoxItemsL();

    private: //new
        void MakeAllVoiceCallDivertsItemL();
        void MakeAllVideoCallDivertsItemL();
        void MakeIfBusyItemL();
        void MakeIfNotAnsweredItemL();
        void MakeIfOutOfReachItemL();
        void MakeIfNotAvailableItemL();

	private:
        // listbox item array
		CGSListBoxItemTextArray* iListboxItemArray;
        //variable to store the main view selection
		TGSCallDivertServiceGroups iItemId;

    };

#endif //GSCALLDIVERTCONTAINER_H
// End of File
