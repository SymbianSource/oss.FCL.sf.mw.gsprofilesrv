/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Class create for testing.
*
*/


#ifndef MEDIAFILETEST_H_
#define MEDIAFILETEST_H_


//  INCLUDES
#include "mediafiledialogutils.h"

#include <e32base.h>
#include <AknDialog.h>
#include <e32math.h>  // Math::FRand
#include <AknInfoPopupNoteController.h> // CAknInfoPopupNoteController

// for metadata engine
#include <harvesterclient.h>



#ifdef _DEBUG

// CLASS DECLARATION
class CMFVideoPreviewHandler;
class CMediaFileDialog;
class CMdeTest;
 

/**
* CRandomNum
*
* CRandomNum is used for generating random numbers.
*/
NONSHARABLE_CLASS (CRandomNum) :  public CBase
    {
    public:
        static CRandomNum* NewLC();
        static CRandomNum* NewL();
        ~CRandomNum();
    private:
        CRandomNum();

    public:
        TInt Random(TInt aMax);
        TInt Random( TInt aMin, TInt aMax );
        TBool Match(TInt aPercentage);

    private:
        TInt64 iSeed;
    };



/**
* CMFDialogTest
*/
NONSHARABLE_CLASS (CMFDialogTest) :  public CBase,
                                     public MMFActiveCallerObserver
    {
    enum TNextStep
        {
        ENextFolder = 0,
        ENextListItem = 1,
        EMovePosition = 2
        };

    public:// Constructors and destructor

        static CMFDialogTest* NewL( CMediaFileDialog* aDialog );
        virtual ~CMFDialogTest();
    
    private:

        CMFDialogTest( CMediaFileDialog* aDialog );
        void ConstructL();
        
    public:
        
        TBool TestL( TInt aAttr );
        void Test1L( TInt aAttr );
        void Test2L( TInt aAttr );
        void PrintTimeL( const TDesC& aText );
        void SetTime();
        
    private:
        void NextListItemL();
        void MovePositionL();
        void NextFolderL();
        
    private:
        void SimulateKeyEventL( TInt aEvent );
        void SimulateOpenEventL();
        void SimulateBackEventL();
        void SimulateSearchEventL();
        
    private:   // from MMFActiveCallerObserver
        void HandleActiveCallL( TInt aCallId );

    private:
        void SetScreenSaverPeriod( TInt aPeriod );
        void SetScreenSaverPeriodL( TInt aPeriod );
        TInt ScreenSaverPeriodL();
    private:
        static TInt64 TimeAfter( TTime aBefore );
        static TTime TimeBefore();
    private:
        void ShowPopupNoteL( const TDesC& aText );
        
        
    public:
        TTime iTime;
        TInt iTestCounter;
        TInt iBottomVisited;
        TInt* iDestroyedPtr;
    private:
        CMediaFileDialog* iDlg;
        CRandomNum* iRandomNum;
        CMFActiveCaller* iTestCaller;
        TInt iNewIndex;
        TInt iScreenSaverPeriod;
        
        // popup note
        CAknInfoPopupNoteController* iPopupNote;

    };




/**
* CMediaFileDialog3
*
*/
/*
NONSHARABLE_CLASS (CMediaFileDialog3) : public CAknDialog
    {
    
    public:// Constructors and destructor
        static CMediaFileDialog3* NewL();
        virtual ~CMediaFileDialog3();
    
    public:
        static TBool ShowDialogL();
    private:

        CMediaFileDialog3();
    
        void ConstructL();
        
    private:
        void PreLayoutDynInitL();
        TBool OkToExitL( TInt aButtonId );
        void ProcessCommandL(TInt aCommandId);
        
    private:
        CMFVideoPreviewHandler* iVideoPreviewHandler;
    };
    
*/


/**
* TMediaFileHistoryItem
*
*/
NONSHARABLE_CLASS (TMediaFileHistoryItem)
    {
    public:
        void SetId( TInt64 aId );
        TInt64 Id();
        void SetTime( TTime aTime );
        TTime Time();
        void InternalizeL( RReadStream& aStream );
        void ExternalizeL( RWriteStream& aStream );


    public:
        // media file id 
        TInt64 iId;
        
        // time stamp
        TTime iTime;
        
        // media file name and path
        TFileName iFullName;
        
        // does item exist
        TBool iPresent;
    };



/**
* CMediaFileHistory
*
* CMediaFileHistory is used for storing list of mediafile ids
* (and time stapms). These ids refer to most recently selected
* ringing tones. Notice that each media file list client has its
* own most recent list (in its private folder).
*   
*/
NONSHARABLE_CLASS (CMediaFileHistory) : public CBase
    {
    static const TInt KMaxHistoryItemCount = 3;
    
    public:
        static CMediaFileHistory* NewL();
        virtual ~CMediaFileHistory();
    
    private:
        CMediaFileHistory();
        void ConstructL();

    public:  // public api
        void AddItemL( TInt64 aId );
        TInt Count();
        void Sort();
        void Clean();
        void SetPresent( TBool aPresent, TInt aIndex );
        void SetFileNameL( const TDesC& aFullName, TInt aIndex );
        const TDesC& FileName( TInt aIndex );
        TInt64 Id( TInt aIndex );
        
    private:
        void WriteHistoryL();
        void ReadHistoryL();
        TInt ItemIndex( TInt aId );
        void AddItemL( TMediaFileHistoryItem aItem );
        TMediaFileHistoryItem& Item( TInt index );

        TBool RemoveOldItem();
        void Reset();
    
    private:
        void InternalizeL( RReadStream& aStream );
        void ExternalizeL( RWriteStream& aStream );
        
    
    private:
        static TInt CompareItems( const TMediaFileHistoryItem& aFirst, 
                                  const TMediaFileHistoryItem& aSecond );

    private:
        static void GetTimeText( TDes& aText, TTime aTime );
        
    private:
        // list of query items
        RArray<TMediaFileHistoryItem> iList;
        
        // file server session
        RFs iFsSession;
        
        // history file path
        TFileName iPath;
    };


#endif // #ifdef _DEBUG

#endif /*MEDIAFILETEST_H_*/
