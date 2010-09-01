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
* Description:  Container for Int. Barrings sub-menu in General Settings.
*
*/


#ifndef __GSSETTLISTINTERNETBARRINGCONTAINER_H__
#define __GSSETTLISTINTERNETBARRINGCONTAINER_H__

// INCLUDES
#include "gsbasecontainer.h"
#include "GSPhoneSettingConstants.h"
#include "CallBarringPlugin.hrh"
#include <centralrepository.h>
// CLASS DECLARATION
class CGSListBoxItemTextArray;

/**
* CGSSettListInternetBarringContainer container class
* @since 3.0
*/
class CGSSettListInternetBarringContainer : public CGSBaseContainer
    {
    public: // Constructors and destructor
        
        /**
        * Symbian OS default constructor. 
        *
        * @param aRect gives the correct TRect for construction.
        */
        void ConstructL( const TRect& aRect );

        /* Destructor */
        ~CGSSettListInternetBarringContainer();

    public:  //from CGSBaseContainer

        /* 
        * Creates list box object.
        * @param aResLbxId resource list id to create list box.
        */
        void ConstructListBoxL( TInt aResLbxId );
        
        void UpdateListBoxL();
    
    public: // New
    
    		/** 
        * Returns one of the VoIP call related values from Cent. Repository
        * @param aVoipSettingId name of the Cent. Repository key to fetch
        * @param aVoipId value of Cent. Repository key
        * @return KErrNone if successful
        */
        TInt GetVoIPCallSettingValue( 
            const TGSVoIPSetting aVoipSettingId, 
            TInt& aVoipId );        
                
        /** 
        * Sets one of the VoIP call related values from Cent. Repository
        * @param aVoipSettingId name of the Cent. Repository key to set
        * @param aVoipId value of Cent. Repository key
        * @return KErrNone if successful
        */
        TInt SetVoIPCallSettingValue( 
            const TGSVoIPSetting aVoipSettingId, 
            TInt aVoipId ); 
            
        /**
        * Gets SCCP status from Central Repository
        * @param aStatus value of Cent. Repository key
        * @return KErrNone if successful
        */    
        TInt GetSCCPStatus( TInt& aStatus );

    private: //new

        /*
        * Creates items for the list box.
        */
        void CreateListBoxItemsL();

        /**
        * Required for help.
        */
        void GetHelpContext(TCoeHelpContext& aContext) const;
        
        /*
        * Creates anonymous item.
        */
        void MakeAnonymousItemL();
        
        /*
        * Converting value
        */
        void SwitchValue( TInt& aValue );


    private: // member variables
    
        //Resource texts for setting items
        CDesCArrayFlat*          iItems;
        // Listbox item array.
        CGSListBoxItemTextArray* iListboxItemArray;
        //CenRep handle
        CRepository* iRichCallRepository;
        CRepository* iTelephonyRepository;
    };

#endif //GSINTERNETBARRINGCONTAINER_H

// End of File
