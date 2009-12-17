/*
* Copyright (c) 2003-2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Class declaration for CGSListboxEntry & CGSItemTextArray.
*
*/

// INCLUDES
#include "gslistbox.h"
#include "gscommon.hrh"

#include <eiklbx.h>
#include <aknradiobuttonsettingpage.h>


// CONSTANTS
const TInt KGSListboxModelGranularity = 5;
_LIT(KGSEngineItemArrayPanic, "GSEngItemArray");

// Panic enum
enum TGSEngineItemArrayPanic
    {
    EGSEngineItemArrayPanicNoRadioSettingsPage = 0,
    };

/*================================================
*
*   class declaration for CGSListboxEntry
*
*================================================*/


//------------------------------------------------------------------------------
// CGSListboxEntry::NewLC()
// Symbian OS two-phased constructor
//------------------------------------------------------------------------------
EXPORT_C CGSListboxEntry* CGSListboxEntry::NewLC( TResourceReader& aReader )
    {
    CGSListboxEntry* self = new (ELeave) CGSListboxEntry;
    CleanupStack::PushL( self );
    self->ConstructL( aReader );
    return self;
    }

//------------------------------------------------------------------------------
// CGSListboxEntry::NewLC()
// Symbian OS two-phased constructor
//------------------------------------------------------------------------------
EXPORT_C CGSListboxEntry* CGSListboxEntry::NewLC( TInt aFeatureId )
    {
    CGSListboxEntry* self = new (ELeave) CGSListboxEntry;
    CleanupStack::PushL( self );
    self->iFeatureId = aFeatureId;
    return self;
    }

//------------------------------------------------
// CGSListboxEntry::NewLC()
// Symbian OS two-phased constructor
//------------------------------------------------
EXPORT_C CGSListboxEntry* CGSListboxEntry::NewLC(
    TUint32 aResId, CCoeEnv& aEnv, TInt aFeatureId )
    {
    CGSListboxEntry* self = new (ELeave) CGSListboxEntry;
    CleanupStack::PushL( self );
    self->ConstructL( aResId, aEnv, aFeatureId );
    return self;
    }

//------------------------------------------------
// CGSListboxEntry::ConstructL()
// Symbian OS 2nd phase constructor
//------------------------------------------------
void CGSListboxEntry::ConstructL(
    TUint32 aResId, CCoeEnv& aEnv, TInt aFeatureId )
    {
    // Create resource reader
    TResourceReader reader;
    aEnv.CreateResourceReaderLC( reader, aResId );

    // Just read, not actually needed anywhere
    reader.ReadInt16();

    // Take base settings from resources
    iCaption = reader.ReadHBufCL();
    iStaticTextLength = iCaption->Length();
    iFeatureId = aFeatureId + reader.ReadInt16();
    iType = reader.ReadInt8();

    CleanupStack::PopAndDestroy(); //reader
    }

//------------------------------------------------------------------------------
// CGSListboxEntry::ConstructL()
// Symbian OS 2nd phase constructor
//------------------------------------------------------------------------------
void CGSListboxEntry::ConstructL( TResourceReader& aReader )
    {
    iCaption = aReader.ReadHBufCL();
    iStaticTextLength = iCaption->Length();
    iFeatureId = aReader.ReadInt16();
    iType = aReader.ReadInt8();
    }

//------------------------------------------------------------------------------
// CGSListboxEntry()
// c++ default constructor
//------------------------------------------------------------------------------
CGSListboxEntry::CGSListboxEntry()
    {
    }

//------------------------------------------------------------------------------
// ~CGSListboxEntry()
// destructor
//------------------------------------------------------------------------------
EXPORT_C CGSListboxEntry::~CGSListboxEntry()
    {
    delete iCaption;
    }

//------------------------------------------------------------------------------
// CGSListboxEntry::Feature()
// Provides feature for the selected entry
// @return feature
//------------------------------------------------------------------------------
EXPORT_C TInt CGSListboxEntry::FeatureId() const
    {
    return iFeatureId;
    }

//------------------------------------------------------------------------------
// CGSListboxEntry::Caption
// Provides caption for the selected entry
// @return caption
//------------------------------------------------------------------------------
EXPORT_C const TDesC& CGSListboxEntry::Caption() const
    {
    return *iCaption;
    }

//------------------------------------------------------------------------------
// CGSListboxEntry::Type
// Provides type for the selected entry
// @return type
//------------------------------------------------------------------------------
EXPORT_C TInt CGSListboxEntry::Type() const
    {
    return iType;
    }

//------------------------------------------------------------------------------
// CGSListboxEntry::SetDynamicTextL
// Appends dynamic text to the static text read
// from the resource.
//------------------------------------------------------------------------------
EXPORT_C void CGSListboxEntry::SetDynamicTextL( const TDesC& aText )
    {
    const TInt requiredLength = StaticTextLength() + aText.Length();
    //
    TPtr pCaption( iCaption->Des() );
    const TInt maxLength = pCaption.MaxLength();
    //
    if( requiredLength >= maxLength )
        {
        // Make enough room for static text + new dynamic text
        iCaption = iCaption->ReAllocL( requiredLength );
        }

    // Have to re-get a new modifiable descriptor for the HBufC* since the
    // memory address may have changed after the realloc.
    pCaption.Set( iCaption->Des() );

    // Set the length back to only the static part
    pCaption.SetLength( StaticTextLength() );
    pCaption.Append( aText );
    }


//------------------------------------------------------------------------------
// CGSListboxEntry::SetDynamicTextDoublePaneL
// Appends dynamic text to the static text read
// from the resource.
//------------------------------------------------------------------------------
EXPORT_C void CGSListboxEntry::SetDynamicTextDoublePaneL( const TDesC& aText )
  {
  const TInt requiredLength = StaticTextLength() + aText.Length();
  //
  TPtr pCaption( iCaption->Des() );
  const TInt maxLength = pCaption.MaxLength();
  const TInt textSize = aText.Length();
  TInt location;
  
  // Here if it is the NULL string then we track of the 
  // Default text size in iDynamicTextSize
  if ( textSize > 0 )
    {
    iDynamicTextSize = textSize;
    // this location is also preserved for the same purpose
    location = aText.Locate( aText[0] );  
    }
  else
    {
    location = KErrNotFound;
    }
  //
  if ( location == KErrNotFound )
    {
    // We take the value from the end of the string
    location = pCaption.LocateReverse('\t');
    // We then delete the string;
    // iDynamicTextSize is the size of the "Default" text i.e. 7
    // 1 is added bcos \tText\t\t for the last tab
    pCaption.Delete( location - ( iDynamicTextSize + 1 ), iDynamicTextSize );
    }
  else
    {
    if( requiredLength >= maxLength )
        {
        // Make enough room for static text + new dynamic text
        iCaption = iCaption->ReAllocL( requiredLength );
        }

    // Have to re-get a new modifiable descriptor for the HBufC* since the
    // memory address may have changed after the realloc.
    pCaption.Set( iCaption->Des() );
    location = pCaption.LocateReverse('\t'); 
    pCaption.Insert( ( location - 1 ), aText );  
    }
  }
//... HERE IT ENDS
/*================================================
*
*   class declaration for CGSItemTextArray
*
*================================================*/

//------------------------------------------------------------------------------
// CGSItemTextArray::NewLC()
// Symbian OS two-phased constructor
//------------------------------------------------------------------------------
EXPORT_C CGSItemTextArray* CGSItemTextArray::NewL( TUint32 aResId,
                                                   CCoeEnv& aEnv )
    {
    CGSItemTextArray* self =
        new (ELeave) CGSItemTextArray( );
    CleanupStack::PushL( self );
    self->ConstructL( aResId, aEnv );
    CleanupStack::Pop( self );

    return self;
    }


//------------------------------------------------------------------------------
// CGSItemTextArray()
// constructor
//------------------------------------------------------------------------------
CGSItemTextArray::CGSItemTextArray()
:    iAllEntries( KGSListboxModelGranularity ),
    iCurrentlyDisplayedEntries( KGSListboxModelGranularity )
    {
    }


//------------------------------------------------------------------------------
// ~CGSItemTextArray()
// destructor
//------------------------------------------------------------------------------
CGSItemTextArray::~CGSItemTextArray()
    {
    iCurrentlyDisplayedEntries.Close();
    iAllEntries.ResetAndDestroy();
    iAllEntries.Close();
    }


//------------------------------------------------------------------------------
// CGSItemTextArray::ConstructL()
// Symbian OS two-phased constructor
//------------------------------------------------------------------------------
void CGSItemTextArray::ConstructL( TUint32 aResId, CCoeEnv& aEnv )
    {
    TResourceReader reader;
    aEnv.CreateResourceReaderLC( reader, aResId );
    //
    const TInt count = reader.ReadInt16();
    for( TInt i = 0; i < count; i++ )
        {
        CGSListboxEntry* entry = CGSListboxEntry::NewLC( reader );
        User::LeaveIfError( iAllEntries.Append( entry ) );

        // Always visible items are added here and never removed
        if    ( entry->Type() == EGSListBoxItemTypeIsAlwaysVisible )
            {
            User::LeaveIfError( iCurrentlyDisplayedEntries.Append( entry ) );
            }
        //
        CleanupStack::Pop( entry );
        }

    CleanupStack::PopAndDestroy(); //reader
    }

//------------------------------------------------
// CGSItemTextArray::ConstructL()
// Symbian OS two-phased constructor
//------------------------------------------------
void CGSItemTextArray::ConstructL(
    TUint32 aResId, CCoeEnv& aEnv, TInt aCount )
    {
    // Create entries and append them to entrylist.
    for( TInt i = 0; i < aCount; i++ )
        {
        CGSListboxEntry* entry = CGSListboxEntry::NewLC( aResId, aEnv, i );
        User::LeaveIfError( iAllEntries.Append( entry ) );
        CleanupStack::Pop( entry );
        }
    }

//------------------------------------------------------------------------------
// CGSItemTextArray::MdcaCount()
// from MDesCArray
// @return count
//------------------------------------------------------------------------------
EXPORT_C TInt CGSItemTextArray::MdcaCount() const
    {
    const TInt count = iCurrentlyDisplayedEntries.Count();
    return count;
    }

//------------------------------------------------------------------------------
// CGSItemTextArray::MdcaPoint()
// from MDesCArray
// @return item text
//------------------------------------------------------------------------------
EXPORT_C TPtrC CGSItemTextArray::MdcaPoint( TInt aIndex ) const
    {
    const TPtrC ret( iCurrentlyDisplayedEntries[aIndex]->Caption() );
    return ret;
    }


//------------------------------------------------------------------------------
// CGSItemTextArray::SetDynamicTextL()
// Appends dynamic text to a selected listbox item
//
//------------------------------------------------------------------------------
EXPORT_C void CGSItemTextArray::SetDynamicTextL( TInt aFeatureId,
                                              const TDesC& aAdditionalText )
    {
    TIdentityRelation<CGSListboxEntry> relation(CompareEntries);
    //
    CGSListboxEntry* entryToSearchFor = CGSListboxEntry::NewLC( aFeatureId );
    const TInt index = iAllEntries.Find( entryToSearchFor, relation );
    CleanupStack::PopAndDestroy( entryToSearchFor );
    User::LeaveIfError( index );
    //
    CGSListboxEntry* entry = iAllEntries[index];
    entry->SetDynamicTextL( aAdditionalText );
    }

//------------------------------------------------------------------------------
// CGSItemTextArray::SetDynamicTextDoublePaneL()
// Appends dynamic text to a selected listbox item
//
//------------------------------------------------------------------------------
EXPORT_C void CGSItemTextArray::SetDynamicTextDoublePaneL( TInt aFeatureId,
                        const TDesC& aAdditionalText )
  {
  TIdentityRelation<CGSListboxEntry> relation(CompareEntries);
  //
  CGSListboxEntry* entryToSearchFor = CGSListboxEntry::NewLC( aFeatureId );
  const TInt index = iAllEntries.Find( entryToSearchFor, relation );
  CleanupStack::PopAndDestroy( entryToSearchFor );
  User::LeaveIfError( index );
  //
  CGSListboxEntry* entry = iAllEntries[index];
  entry->SetDynamicTextDoublePaneL( aAdditionalText );
  }
//*****ENDS HERE

//------------------------------------------------------------------------------
// CGSItemTextArray::SetItemVisibilityL()
// Indicates whether an item is to be shown/hidden
//
//------------------------------------------------------------------------------
EXPORT_C void CGSItemTextArray::SetItemVisibilityL( TInt aFeatureId,
                                                 TVisibility aVisibility )
    {
    TIdentityRelation<CGSListboxEntry> relation( CompareEntries );
    CGSListboxEntry* entryToSearchFor = CGSListboxEntry::NewLC( aFeatureId );
    //
    if    ( aVisibility == EInvisible )
        {
        const TInt index = iCurrentlyDisplayedEntries.Find( entryToSearchFor,
                                                      relation );
        if    ( index != KErrNotFound )
            {
            // Remove the item from the currently visible items array
            iCurrentlyDisplayedEntries.Remove( index );
            }
        }
    else if ( aVisibility == EVisible )
        {
        // Check its not already visible
        const TInt alreadyVisibleIndex = iCurrentlyDisplayedEntries.Find(
                                             entryToSearchFor, relation );
        if    ( alreadyVisibleIndex == KErrNotFound )
            {
            const TInt index = iAllEntries.Find( entryToSearchFor, relation );
            User::LeaveIfError( index );

            // This is the entry we are going to make visible
            CGSListboxEntry* entry = iAllEntries[index];

            // Now insert it at the correct location.
            TBool foundInsertionPoint = EFalse;
            const TInt currentEntriesCount = iCurrentlyDisplayedEntries.Count();
            for( TInt allEntriesIndex=index-1; allEntriesIndex>=0 &&
                !foundInsertionPoint; allEntriesIndex-- )
                {
                // We are going to try and insert the new entry after the entry
                // specified as the previous item in the 'all entries' array,
                // in the iCurrentlyDisplayedEntries array.
                const TInt featureIdOfPreviousItem =
                           iAllEntries[allEntriesIndex]->FeatureId();

                for( TInt currentEntriesIndex=0; !foundInsertionPoint &&
                    currentEntriesIndex<currentEntriesCount;
                    currentEntriesIndex++ )
                    {
                    CGSListboxEntry* visibleEntry = iCurrentlyDisplayedEntries[
                                                           currentEntriesIndex];
                    if    ( visibleEntry->FeatureId() == featureIdOfPreviousItem )
                        {
                        foundInsertionPoint = ETrue;
                        // Insert after this item
                        User::LeaveIfError( iCurrentlyDisplayedEntries.Insert(
                                            entry, currentEntriesIndex+1 ) );
                        }
                    }

                }

            if    (!foundInsertionPoint)
                {
                // Just insert the item at the top of the list
                User::LeaveIfError(
                    iCurrentlyDisplayedEntries.Insert( entry, 0 ) );
                }
            }
        }
    CleanupStack::PopAndDestroy( entryToSearchFor );
    }


//------------------------------------------------------------------------------
// CGSItemTextArray::CurrentFeature()
// Obtains feature id for a selected listbox item (feature)
// @return feature id
//------------------------------------------------------------------------------
EXPORT_C TInt CGSItemTextArray::CurrentFeature() const
    {
    return KErrNotFound;
    }


//------------------------------------------------------------------------------
// CGSItemTextArray::IndexForFeatureId()
//
// @return index
//------------------------------------------------------------------------------
EXPORT_C TInt CGSItemTextArray::IndexForFeatureIdL(TInt aFeatureId) const
    {
    TIdentityRelation<CGSListboxEntry> relation( CompareEntries );
    CGSListboxEntry* entryToSearchFor = CGSListboxEntry::NewLC( aFeatureId );
    //
    const TInt index = iCurrentlyDisplayedEntries.Find( entryToSearchFor,
                                                        relation );
    CleanupStack::PopAndDestroy( entryToSearchFor );
    //
    return index;
    }


//------------------------------------------------------------------------------
// CGSItemTextArray::FeatureIdByIndexL()
//
// @return index
//------------------------------------------------------------------------------
EXPORT_C TInt CGSItemTextArray::FeatureIdByIndex(TInt aIndex) const
    {
    TInt featureId = KErrNotFound;
    //
    const TInt currentEntriesCount = iCurrentlyDisplayedEntries.Count();
    if  (aIndex >= 0 && aIndex < currentEntriesCount)
        {
        featureId = iCurrentlyDisplayedEntries[ aIndex ]->FeatureId();
        }
    //
    return featureId;
    }


//------------------------------------------------------------------------------
// CGSItemTextArray::FeatureById()
//
// @return feature
//------------------------------------------------------------------------------
EXPORT_C const CGSListboxEntry* CGSItemTextArray::FeatureByIdL(
                                                  TInt aFeatureId ) const
    {
    const CGSListboxEntry* locatedEntry = NULL;
    //
    TIdentityRelation<CGSListboxEntry> relation( CompareEntries );
    CGSListboxEntry* entryToSearchFor = CGSListboxEntry::NewLC( aFeatureId );
    //
    const TInt index = iAllEntries.Find( entryToSearchFor,
                                         relation );
    if    ( index != KErrNotFound )
        {
        locatedEntry = iAllEntries[ index ];
        }
    CleanupStack::PopAndDestroy( entryToSearchFor );
    //
    return locatedEntry;
    }


//------------------------------------------------------------------------------
// CGSItemTextArray::GetItemCaptionFromFeatureIdLC()
//
// @return feature's caption
//------------------------------------------------------------------------------
EXPORT_C HBufC* CGSItemTextArray::GetItemCaptionFromFeatureIdLC(
                                                        TUint32 aResourceId,
                                                        TInt aFeatureId,
                                                        CCoeEnv& aCone )
    {
    HBufC* caption = NULL;
    //
    CGSItemTextArray* items = CGSItemTextArray::NewL( aResourceId, aCone );
    CleanupStack::PushL( items );
    const CGSListboxEntry* entry = items->FeatureByIdL( aFeatureId );
    //
    if ( entry )
        {
        caption = entry->Caption().AllocL();
        }
    else
        {
        caption = KNullDesC().AllocL();
        }
    //
    CleanupStack::PopAndDestroy( items );
    CleanupStack::PushL( caption );
    return caption;
    }


//------------------------------------------------------------------------------
// CGSItemTextArray::CompareEntries()
// Compares feature id's to check if they are the same
// @return boolean
//------------------------------------------------------------------------------
TBool CGSItemTextArray::CompareEntries(const CGSListboxEntry& aLeft,
                                              const CGSListboxEntry& aRight)
    {
    return aLeft.FeatureId() == aRight.FeatureId();
    }







/*================================================
*
*   class declaration for CGSListBoxItemTextArray
*
*================================================*/

//------------------------------------------------------------------------------
// CGSListBoxItemTextArray::NewLC()
// Symbian OS two-phased constructor
//------------------------------------------------------------------------------
EXPORT_C CGSListBoxItemTextArray* CGSListBoxItemTextArray::NewL( TUint32 aResId,
                                                       CEikListBox& aListBox,
                                                       CCoeEnv& aEnv )
    {
    CGSListBoxItemTextArray* self =
        new (ELeave) CGSListBoxItemTextArray( aListBox );
    CleanupStack::PushL( self );
    self->ConstructL( aResId, aEnv );
    CleanupStack::Pop( self );

    return self;
    }

//------------------------------------------------
// CGSListBoxItemTextArray::NewL()
// Symbian OS two-phased constructor
//------------------------------------------------
EXPORT_C CGSListBoxItemTextArray* CGSListBoxItemTextArray::NewL(
    TUint32 aResId, CEikListBox& aListBox, CCoeEnv& aEnv, TInt aCount )
    {
    CGSListBoxItemTextArray* self =
        new (ELeave) CGSListBoxItemTextArray( aListBox );
    CleanupStack::PushL( self );
    self->ConstructL( aResId, aEnv, aCount );
    CleanupStack::Pop( self );

    return self;
    }

//------------------------------------------------------------------------------
// CGSListBoxItemTextArray()
// constructor
//------------------------------------------------------------------------------
CGSListBoxItemTextArray::CGSListBoxItemTextArray( CEikListBox& aListBox )
:    iListBox( aListBox )
    {
    }


//------------------------------------------------------------------------------
// CGSListBoxItemTextArray::CurrentFeature()
//
// Obtains feature id for a selected listbox item (feature)
// @return feature id
//------------------------------------------------------------------------------
EXPORT_C TInt CGSListBoxItemTextArray::CurrentFeature() const
    {
    TInt featureId = KErrNotFound;
    const TInt listBoxCurrentItemIndex = iListBox.CurrentItemIndex();
    const TInt count = iCurrentlyDisplayedEntries.Count();
    if    ( listBoxCurrentItemIndex >= 0 && listBoxCurrentItemIndex < count )
        {
        const CGSListboxEntry* entry =
                        iCurrentlyDisplayedEntries[ listBoxCurrentItemIndex ];
        featureId = entry->FeatureId();
        }
    return featureId;
    }









/*================================================
*
*   class declaration for CGSRadioButtonSettingPageItemTextArray
*
*================================================*/

//------------------------------------------------------------------------------
// CGSRadioButtonSettingPageItemTextArray::NewL()
//
// Symbian OS two-phased constructor
//------------------------------------------------------------------------------
EXPORT_C CGSRadioButtonSettingPageItemTextArray*
         CGSRadioButtonSettingPageItemTextArray::NewL( TUint32 aResId,
                                                       CCoeEnv& aEnv,
                             CAknRadioButtonSettingPage* aSettingPage )
    {
    CGSRadioButtonSettingPageItemTextArray* self =
        new (ELeave) CGSRadioButtonSettingPageItemTextArray( aSettingPage );
    CleanupStack::PushL( self );
    self->ConstructL( aResId, aEnv );
    CleanupStack::Pop( self );

    return self;
    }


//------------------------------------------------------------------------------
// CGSRadioButtonSettingPageItemTextArray()
//
// constructor
//------------------------------------------------------------------------------
CGSRadioButtonSettingPageItemTextArray::CGSRadioButtonSettingPageItemTextArray (
                                     CAknRadioButtonSettingPage* aSettingPage )
:   iSettingPage( aSettingPage )
    {
    }


//------------------------------------------------------------------------------
// CGSRadioButtonSettingPageItemTextArray::SetRadioButtonSettingPage()
//
//
//------------------------------------------------------------------------------
EXPORT_C void CGSRadioButtonSettingPageItemTextArray::SetRadioButtonSettingPage (
                                     CAknRadioButtonSettingPage& aSettingPage )
    {
    iSettingPage = &aSettingPage;
    }


//------------------------------------------------------------------------------
// CGSRadioButtonSettingPageItemTextArray::CurrentFeature()
// Obtains feature id for a selected listbox item (feature)
// @return feature id
//------------------------------------------------------------------------------
EXPORT_C TInt CGSRadioButtonSettingPageItemTextArray::CurrentFeature() const
    {
    __ASSERT_ALWAYS(iSettingPage, User::Panic( KGSEngineItemArrayPanic,
                         EGSEngineItemArrayPanicNoRadioSettingsPage ) );

    TInt featureId = KErrNotFound;

    const TInt listBoxCurrentItemIndex =
                         iSettingPage->ListBoxControl()->CurrentItemIndex();
    const TInt count = iCurrentlyDisplayedEntries.Count();
    if    ( listBoxCurrentItemIndex >= 0 && listBoxCurrentItemIndex < count )
        {
        const CGSListboxEntry* entry =
                         iCurrentlyDisplayedEntries[ listBoxCurrentItemIndex ];
        featureId = entry->FeatureId();
        }
    return featureId;
    }

//End of File
