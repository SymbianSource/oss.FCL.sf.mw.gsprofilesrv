/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Container for Call Divert Plugin.
*
*/


#ifndef GSDIVERTPLUGINCONTAINER_H
#define GSDIVERTPLUGINCONTAINER_H

// INCLUDES
#include "gsbasecontainer.h"

// CLASS DECLARATION
class CGSListBoxItemTextArray;

// CLASS DECLARATION
/**
*  CGSCallDivertPluginContainer container class
*/
class CGSCallDivertPluginContainer : public CGSBaseContainer
    {
    public: // Constructors and destructor

        /**
        * Symbian OS default constructor.
        *
        * @param aRect gives the size of container
        */
        void ConstructL( const TRect& aRect );

        /** Destructor */
        ~CGSCallDivertPluginContainer();

    private:

        void GetHelpContext( TCoeHelpContext& aContext ) const;

    public:
        /**
        * Updates the listbox items
        * @param aFeatureId: selected listbox item ID
        */
        void UpdateListBoxL( TInt aFeatureId );

        /**
        * Retrieves the feature id for the selected item in the listbox
        * @return listbox item array's current feature.
        */
        TGSCallDivertServiceGroups CurrentFeatureId() const;


    protected:
        /* Creates listbox
        *   @param aResLbxId
        */
        void ConstructListBoxL( TInt aResLbxId );

    private: // Functions
        void MakeVoiceDivertsItemL();
        void MakeVideoDivertsItemL();

    private: // Member variables
		CGSListBoxItemTextArray* iListboxItemArray;

    };

#endif // GSDIVERTPLUGINCONTAINER_H

// End of File
