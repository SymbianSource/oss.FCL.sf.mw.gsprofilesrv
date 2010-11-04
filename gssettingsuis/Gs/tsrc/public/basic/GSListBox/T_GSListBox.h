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


#ifndef __T_GSLISTBOX_H__
#define __T_GSLISTBOX_H__

//  EXTERNAL INCLUDES
#include <CEUnitTestSuiteClass.h>
#include <EUnitDecorators.h>
#include <GSListBox.h>

//  INTERNAL INCLUDES

//  FORWARD DECLARATIONS


//  CLASS DEFINITION
NONSHARABLE_CLASS( T_GSListBox )
	: public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static T_GSListBox* NewL();
        static T_GSListBox* NewLC();
        /**
         * Destructor
         */
        ~T_GSListBox();

    private:    // Constructors and destructors

        T_GSListBox();
        void ConstructL();

    private:    // New methods

         void SetupL();
         void Teardown();
         
         // CGSListboxEntry test cases
         void SetupListBoxEntryL();
         void T_Global_GSListBox_NewLC();
         void T_Global_GSListBox_New2LC();
         void T_Global_GSListBox_New3LC();
         void T_Global_GSListBox_FeatureIdL();
         void T_Global_GSListBox_TypeL();
         void T_Global_GSListBox_CaptionL();
         void T_Global_GSListBox_SetDynamicTextL();
         void T_Global_GSListBox_SetDynamicTextDoublePaneL();
         
         // CGSItemTextArray test cases
         void SetupGSItemTextArrayL();
         void T_Global_GSItemTextArray_SetupL();
         void T_Global_GSItemTextArray_MdcaCountL();
         void T_Global_GSItemTextArray_MdcaPointL();
         void T_Global_GSItemTextArray_SetDynamicTextL();
         void T_Global_GSItemTextArray_SetDynamicTextDoublePaneL();
         void T_Global_GSItemTextArray_SetItemVisibilityL();
         void T_Global_GSItemTextArray_CurrentFeatureL();
         void T_Global_GSItemTextArray_IndexForFeatureIdL();
         void T_Global_GSItemTextArray_FeatureIdByIndexL();
         void T_Global_GSItemTextArray_FeatureByIdL();
         void T_Global_GSItemTextArray_GetItemCaptionFromFeatureIdLC();
         
         // CGSRadioButtonSettingPageItemTextArray test cases
         void SetupResourceL();
         void T_Global_GSRBSPItemTextArray_NewL();
         void Setup_GSRBSPItemTextArrayL();
         void T_Global_GSRBSPItemTextArray_CurrentFeatureL();
         void T_Global_GSRBSPItemTextArray_SetRadioButtonSettingPageL();
         
         // CGSListBoxItemTextArray test cases
         void Setup_CGSLBITextArrayL();
         void T_Global_CGSLBITextArray_NewL();
         void T_Global_CGSLBITextArray_New2L();
         void T_Global_CGSLBITextArray_CurrentFeatureL();
         
         

    private:    // Data
        CGSListboxEntry* iListboxEntry;
        CGSItemTextArray* iItemTextArray;
        CGSRadioButtonSettingPageItemTextArray* iRBSPArray;
        CGSListBoxItemTextArray* iLBITextArray;
        CEikListBox* iListBox;
        
        TInt iOffset;
        
        EUNIT_DECLARE_TEST_TABLE; 

    };

#endif      //  __T_GSLISTBOX_H__

// End of file
