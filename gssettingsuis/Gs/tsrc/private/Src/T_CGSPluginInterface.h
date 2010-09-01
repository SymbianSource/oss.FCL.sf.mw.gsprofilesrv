/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
*
*/


#ifndef __T_CGSPLUGININTERFACE_H__
#define __T_CGSPLUGININTERFACE_H__

//  EXTERNAL INCLUDES
#include <CEUnitTestSuiteClass.h>
#include <EUnitDecorators.h>


//  INTERNAL INCLUDES
#include <GSPluginInterface.h>

//  FORWARD DECLARATIONS


//  CLASS DEFINITION

/**
 * Utility go through all ECOM plugins for a certain interface.
 */
NONSHARABLE_CLASS( CGSPluginIndexer ) : public CBase
    {
    public:

        /**
         * @param aInterface Interface Id for which the plugins should be 
         * listed.
         */
        static CGSPluginIndexer* NewL( TUid aInterface );
        
        ~CGSPluginIndexer();
        void ConstructL();

        /**
         * @return ETrue if has still items and NextUid() can be called.
         */
        TBool HasNext();
        
        /**
         * @return Next Uid of the listed plugins.
         */
        TUid NextUid();
        
        /**
         * Resets the interator location to the first item [0].
         */
        void ResetIndex();
        
        /**
         * @return Current position in the list.
         */
        TInt Index();
        
        /**
         * @return Number of items in the list.
         */
        TInt Count();

    private:
    
        CGSPluginIndexer( TUid aInterface );
        TUid iInterfaceUid;
        TInt iIndex;
        RImplInfoPtrArray iImplInfoArray;
    };


/**
 * Auto-generated EUnit test suite
 *
 */
NONSHARABLE_CLASS( T_CGSPluginInterface )
	: public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static T_CGSPluginInterface* NewL();
        static T_CGSPluginInterface* NewLC();
        /**
         * Destructor
         */
        ~T_CGSPluginInterface();

    private:    // Constructors and destructors

        T_CGSPluginInterface();
        void ConstructL();

    private:    // New methods

         void SetupL();

         void Teardown();

         /**
          * Functions for testing memory leaks...
          */
         void T_CGSPluginInterface_CheckHeapL();
         void T_CGSPluginInterface_ResetBlacklistL();
         void T_CGSPluginInterface_SetToBlacklistL();
  
         void T_CGSPluginInterface__CGSPluginInterfaceL();

         void T_CGSPluginInterface_GetCaptionLL();

         void T_CGSPluginInterface_GetValueL();

         void T_CGSPluginInterface_HandleSelectionL();

         void T_CGSPluginInterface_ItemTypeL();

         void T_CGSPluginInterface_MenuActivationItemL();

         void T_CGSPluginInterface_CreateIconLL();

         void T_CGSPluginInterface_PluginProviderCategoryL();

         void T_CGSPluginInterface_CustomOperationLL();

         void T_CGSPluginInterface_VisibleL();

         void T_CGSPluginInterface_ResetSelectedItemIndexL();

         void T_CGSPluginInterface_SetOrderL();


    private:    // Data

        CGSPluginInterface* iCGSPluginInterface;
        CGSPluginIndexer* iPluginIndexer;
        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __T_CGSPLUGININTERFACE_H__

// End of file
