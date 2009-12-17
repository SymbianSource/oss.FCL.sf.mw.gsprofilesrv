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
* Description:  View handling for Call Divert.
*
*/


#ifndef CFCALLDIVERTVIEW_H
#define CFCALLDIVERTVIEW_H

// INCLUDES
#include "GSLocalBaseView.h"
#include "CallDivertPlugin.hrh"

#include <aknlists.h>
#include <AiwContactAssignDataTypes.h>
#include <AiwCommon.h>
#include <MVPbkSingleContactOperationObserver.h>
#include <MVPbkContactStoreListObserver.h>
#include <nwdefs.h>

// FORWARD DECLARATIONS
class CAiwServiceHandler;
class CGSCallDivertModel;
class CGSCallDivertPluginContainer;
class CGSCallDivertQuery;
class CVPbkContactLinkArray;
class CVPbkContactManager;
class MVPbkContactLink;


// CLASS DECLARATION

/**
*  CGSCallDivertView view class.
*/
class CGSCallDivertView : public CGSLocalBaseView,
                          public MAiwNotifyCallback,
                          public MVPbkSingleContactOperationObserver,
                          public MVPbkContactStoreListObserver
    {
    public: // Constructors and destructor

        /**
        * Symbian OS default constructor
        */
        void ConstructL();

        /**
        * Destructor
        */
        ~CGSCallDivertView();


        /**
        * Symbian OS two-phase Constructor
        * @return pointer to CGSCallDivertView object
        */
        static CGSCallDivertView* NewLC( CGSCallDivertModel* aModel );



    public: // Functions from base class CAknView

        /* C++ Constructor */
        CGSCallDivertView( CGSCallDivertModel* aModel );

        /**
        * From CAknView, gives the id of the view.
        *
        * @return Returns the TUid of the view.
        */
        TUid Id() const;

        /**
        * From MEikCommandObserver, handles the commands.
        *
        * @param aCommand identifies the command given.
        */
        void HandleCommandL( TInt aCommand );
        /**
        * From MEikCommandObserver, handles the commands.
        *
        * @param aCommand identifies the command given.
        */
        void ProcessCommandL( TInt aCommand );

    public: // MAiwNotifyCallback

        /**
        * From MAiwNotifyCallback. Handles notifications caused by an
        * asynchronous Execute*CmdL call or an event.
        *
        * @param aCmdId The service command associated to the event.
        * @param aEventId Occured event, see AiwCommon.hrh.
        * @param aEventParamList Event parameters, if any, as defined per
        *        each event.
        * @param aInParamList Input parameters, if any, given in the
        *        related HandleCommmandL.
        * @return Error code for the callback.
        */
        TInt HandleNotifyL(
            TInt aCmdId,
            TInt aEventId,
            CAiwGenericParamList& aEventParamList,
            const CAiwGenericParamList& aInParamList);

    public: // From MVPbkSingleContactOperationObserver

        /**
        * This is called when user has selected a contact from pbk2
        * contacts-dialog.
        * @param aOperation pbk2 related operation. Not used here.
        * @param aContact Selected contact data.
        */
        void VPbkSingleContactOperationComplete(
            MVPbkContactOperationBase& aOperation,
            MVPbkStoreContact* aContact );
        /**
        * See MVPbkSingleContactOperationObserver.
        */
        void VPbkSingleContactOperationFailed(
            MVPbkContactOperationBase& aOperation,
            TInt aError );

    public: // From MVPbkContactStoreListObserver

        /**
        * See MVPbkContactStoreListObserver.
        */
        void OpenComplete();

        /**
        * See MVPbkContactStoreListObserver.
        */
        void StoreReady( MVPbkContactStore& aContactStore );

        /**
        * See MVPbkContactStoreListObserver.
        */
        void StoreUnavailable( MVPbkContactStore& aContactStore,
                               TInt aReason );

        /**
        * See MVPbkContactStoreListObserver.
        */
        void HandleStoreEventL( MVPbkContactStore& aContactStore,
                                TVPbkContactStoreEvent aStoreEvent );

    public: // New

        /**
        * Creates time list to be used in "not available" cf service.
        * @return Returns user keypress of the list
        */
        TInt CreateTimeListL();

    private: // From base class CGSBaseView

        /**
        * See CGSBaseView.
        */
        void NewContainerL();

        /**
        * See CGSBaseView.
        */
        void HandleListBoxSelectionL();

    private: //new

        /**
        * Executes divert query dialog.
        * --> voice calls -> all voice calls -> activate -> this popup.
        *                   OR
        * --> data calls  -> all data calls  -> activate -> this popup.
        *
        * @param aTelNumber user selected existing divert-to number
        * @param aTitle gives title to the list
        * @param aVmbxValidNumber divert is to a voicemail box number
        * @return Returns TTelnumber to be the target for operation.
        */
        void ExecuteDivertQueryL( TTelNumber& aTelNumber,
                                  const TPtrC& aTitle,
                                  TBool& aVmbxValidNumber );

        /**
        * Displays a query for divert number. Used for all divert types:
        * - voice
        * - data
        * - fax
        * diverts.
        *
        * @param aValue text in the editor
        * @param aType Dataquery resource ID text
        * @return Returns the number.
        */
        void ExecuteDivertNumberQueryL( TDes& aValue, TInt aType );

        /**
        * Single item fetch from the phonebook
        * @param aNumber fetched phonenumber
        */
        void StartAsyncPhoneNumberFetchL( /*TDes& aNumber*/ );


        /**
        * Checks if the user given telephone number contain
        * illegal ('#', 'p', 'w', ...) characters
        *
        *   @param  aDivertTo   user given telephone number
        */
        static TInt ValidatePhoneNumberL( const TDesC& aDivertTo );

        /**
        *   Appends to array a delay time in steps of five (5,10,15,...).
        *   Delay time is figured out by multiplying index by five.
        *
        *   @param   aArrayToAppend
        *   @param  aIndex
        */
        void AppendDelayItemL( CDesCArrayFlat& aArrayToAppend, TInt aIndex );


        /**
        * Seeks '+' from given number.
        *   @param  aDivertTo   the number checked for
        *   @return Either KErrNone (no plus) or KCFErrIllegalNumber.
        */
        static TInt FindPlus( const TDesC& aDivertTo );

        /**
        * Sets prompt text to a query.
        */
        void SetPromptToQueryL( CGSCallDivertQuery& aQuery,
                                const TInt& aType );

        /**
        * Creates items for Delay time list
        */
        void CreateDelayTimeItemsL( CDesCArrayFlat& aItems );

        /**
        * Creates items for Delay time list for restricted call divert
        */
        void CreateRestrictedCallDivertDelayTimeItemsL( CDesCArrayFlat& aItems );

        /**
        * Displays invalid number note.
        */
        static void ShowErrorNoteL();

        /**
        * Updates most recently used voice divert numbers to cenrep
        *
        *   @param  aItems          array in which all items are
        *   @param  aNumber         telNumber to be updated
        */
        void UpdateDefaultVoiceDivertNumbersL( TDes& aNumber );

        /**
        * Reads 3 previously used voice divert numbers.
        *
        *   @param  aItems          array in which the items are put into
        *   @param  aDefaultNumbers array from which default items are read
        */
        void ReadDefaultVoiceDivertNumbersL( CDesCArray& aItems,
                                             CDesC16ArrayFlat& aDefaultNumbers );

        /**
        * Creates items for the target list ('To voicemailbox', ...)
        *
        *   @param  aItems          array in which the items are put into
        *   @param  aDefaultNumbers array from which default items are read
        */
        void CreateVoiceDivertItemsL( CDesCArray& aItems,
                                      CDesC16ArrayFlat& aDefaultNumbers );

        /**
        * Creates items for the target list ('To voicemailbox', ...)
        *
        *   @param  aItems          array in which the items are put into
        */
        void CreateVideoDivertItemsL( CDesCArray& aItems );

        /**
        * Handles users selection from Target menu
        */
        TTelNumber HandleVoiceDivertSelectionL(
            CAknSinglePopupMenuStyleListBox& aList,
            CDesC16ArrayFlat& aDefNumbers,
            TBool& aVmbxDivert );

        /**
        * Handles users selection from Target menu
        */
        TTelNumber HandleVideoDivertSelectionL(
                CAknSinglePopupMenuStyleListBox& aList );
        
        /**
         * Displays data call number query and activates data divert
         * @param aTelNumber Phone number for call divert
         */
        void DisplayDataCallDivertNumberQueryL( TTelNumber& aTelNumber );

    private:

        void DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane );

    private:
        // Has selection key been pressed
        TBool               iRockerPress;

        // Array of setting item titles
        CDesCArray*         iTitles;

        /**
        * Member variables related to AIW service handler used by pbk2 contact
        * selection functionality. Former functionality was synchronous dialog
        * based but pbk2 framework required refactoring this to asynchronous.
        * As a result a bucket load of observers and functionality otherwise
        * unused is needed here. Basically the call divert plugin is now
        * changed into a state machine because of pbk2 requirement.
        */
        // AIW service for pbk2.
        CAiwServiceHandler* iServiceHandler;
        // Handles contact stores.
        CVPbkContactManager* iContactManager;
        // Pointer to both a contact and a contact field
        CVPbkContactLinkArray* iContactLinks;
        // Pbk stores are opened.
        TBool iStoresOpened;
        // Stores pbk2 operations.
        MVPbkContactOperationBase* iRetrieveOperation;
        // Used pbk2 number query which is asynchronously.
        TBool iNumberFetchedAsynchronously;


        // Call divert request is for voice mailbox.
        TBool iVoiceMailboxDivert;

        //Context menu items for call forwarding
        CDesCArrayFlat* iContextMenuItems;

        // Reference to call divert model. NOT owned.
        CGSCallDivertModel* iModel;
        TBool iVideoMailboxSupported;

    };



#endif //CFCALLDIVERTVIEW_H
// End of File
