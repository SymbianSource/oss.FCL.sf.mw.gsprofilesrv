/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef __MT_GSFRAMEWORK_H__
#define __MT_GSFRAMEWORK_H__

//  EXTERNAL INCLUDES
#include <CEUnitTestSuiteClass.h>
#include <EUnitDecorators.h>
#include <GSTabHelper.h>
#include <GSPluginLoader.h>
#include <GSPubSubsListener.h>
#include <MGSSettingPSObserver.h>
#include <GSParentPlugin.h>

//  INTERNAL INCLUDES
#include "TestGSBaseView.h"

//  FORWARD DECLARATIONS


//  CLASS DEFINITION
NONSHARABLE_CLASS( MT_GSFramework )
  : public CEUnitTestSuiteClass, MGSTabbedViewOwner, MGSPluginLoadObserver, MGSSettingPSObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static MT_GSFramework* NewL();
        static MT_GSFramework* NewLC();
        /**
         * Destructor
         */
        ~MT_GSFramework();

    public:
        // From MGSTabbedViewOwner
      CArrayPtrFlat<CGSPluginInterface>* TabbedViews();
      void TabChangedL( TUid selectedTabUid );

      // From MGSPluginLoadObserver
        void HandlePluginLoaded( KGSPluginLoaderStatus aStatus );

        // From MGSSettingPSObserver
        void HandleNotifyPSL( const TUid aUid,
                              const TInt& aKey,
                              const TRequestStatus& aStatus );


    private:    // Constructors and destructors

        MT_GSFramework();
        void ConstructL();

    private:
    // Test setup and teardown functions

    // CGSTabHelper
         void SetupTabHelperL();
         void TeardownTabHelper();

    // CGSTabHelper test cases
         void T_CGSTabHelper_NewLL();
         void T_CGSTabHelper_NewLCL();
         //~Destructor
         void T_CGSTabHelper__CGSTabHelperL();
         void T_CGSTabHelper_CreateTabGroupLL();
         void T_CGSTabHelper_CreateTabGroupL2L();
         void T_CGSTabHelper_CreateTabGroupL3L();
         void T_CGSTabHelper_RemoveTabGroupL();
         void T_CGSTabHelper_TabChangedLL();

     // CGSParentContainer test cases
         void SetupParentContainerL();
         void TearDownParentContainer();

         void T_CGSParentContainer_FocusChanged();

     // CGSPluginLoader test cases
         void SetupPluginLoaderL();
         void TearDownPluginLoader();

         void T_CGSPluginLoader_NewL();
         void T_CGSPluginLoader__CGSPluginLoader();
         void T_CGSPluginLoader_LoadAsyncL();
         void T_CGSPluginLoader_SetObserver();
         void T_CGSPluginLoader_AbortAsyncLoad();
         void T_CGSPluginLoader_SortPluginsL();


     // CGSPubSubsListener test cases
         void SetupPubSubsListenerL();
         void TearDownPubSubsListener();

         void T_CGSPubSubsListener_NewL();
         void T_CGSPubSubsListener__CGSPubSubsListener();
         void T_CGSPubSubsListener_Get1();
         void T_CGSPubSubsListener_Get2();
         void T_CGSPubSubsListener_Get3();


     // Empty Setup & TearDown
         void SetupNone();
         void TeardownNone();


         void SetupLaunch();
         void TeardownLaunch();

     static TInt CloseTopMostWindow( TAny* aPtr );
     void DoCloseTopMostWindowL();

     void LaunchGsApp1L();
     TBool GetKeyCodeCase1( TKeyCode& aKeyCode );

     void LaunchGsApp2L();
     TBool GetKeyCodeCase2( TKeyCode& aKeyCode );

     void LaunchGsApp3L();
     TBool GetKeyCodeCase3( TKeyCode& aKeyCode );

     void LaunchGsApp4L();
     TBool GetKeyCodeCase4( TKeyCode& aKeyCode );

     static TInt SimulateKeyEvent( TAny* aPtr );
     void DoSimulateKeyEventL();

     TBool GetKeyCodeCase0( TKeyCode& aKeyCode );


     // CGSBaseView test cases
     void SetupCGSBaseViewL();
     void TeardownCGSBaseView();
     void MT_CGSBaseView_SetCurrentItemL();
     void MT_CGSBaseView_ContainerL();
     void MT_CGSBaseView_SetNaviPaneLL();
     void MT_CGSBaseView_CreateNaviPaneContextLL();

     // CGSParentPlugin test cases
     void SetupCGSParentPluginL();
     void TeardownCGSParentPlugin();
     void MT_CGSParentPlugin_UpperLevelViewUidL();
     void MT_CGSParentPlugin_UpdateViewL();
     void MT_CGSParentPlugin_SetOptionFlagsL();
     void MT_CGSParentPlugin_OptionFlagsL();
     void MT_CGSParentPlugin_GetHelpContextL();
     void MT_CGSParentPlugin_SelectedPluginL();
     void MT_CGSParentPlugin_TransferDynamicPluginLL();

     // CGSPluginInterface test cases
     void MT_CGSPluginInterface_ItemTypeL();
     void MT_CGSPluginInterface_HandleSelectionL();
     void MT_CGSPluginInterface_PluginProviderCategoryL();
     void MT_CGSPluginInterface_CustomOperationLL();
     void MT_CGSPluginInterface_SetOrderL();
     void MT_CGSPluginInterface_CreateIconLL();
     void MT_CGSPluginInterface_GetEcomDestructorKey();


    private:    // Data

      // For CGSTabHelper
        CGSTabHelper* iCGSTabHelper;
      CArrayPtrFlat<CGSPluginInterface>* iPlugins;
        CArrayPtrFlat<MGSTabbedView>* iTabbedViews;

        // For CGSPluginLoader
        CGSPluginLoader* iCGSPluginLoader;

        // For CGSPubSubsListener
        CGSPubSubsListener* iCGSPubSubsListener;

        // For launching Gs application test cases
        CPeriodic* iPeriodic;
        TInt ikeySimulateCase;
        TInt iKeyNumber;

        // For CTestGSBaseView
        CTestGSBaseView* iTestBaseView;

        // For CGSParentPlugin
        CGSParentPlugin* iParentPlugin;

        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __MT_GSFRAMEWORK_H__

// End of file
