/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Template for a listbox class which forwards listbox events
*                to an observer.
*
*/


#ifndef FLDLISTBOXTEMPLATE_H
#define FLDLISTBOXTEMPLATE_H

#include <coedef.h> // TKeyResponse
#include <w32std.h> // TKeyEvent, TEventCode
#include "MFLDFileListBoxObserver.h"
#include "CFLDFileListModel.h"
#include "CFLDPopupList.h"

// CLASS DECLARATION

/**
*  Template for a listbox class which forwards listbox events to an observer.
*
*  @lib filelist.lib
*  @since Series 60 2.6
*/
template<class T>
class FLDListBoxTemplate : public T
    {
    public:     // Constructors and destructor

        /**
        * Constructor
        */
        FLDListBoxTemplate(
            MFLDFileListBoxObserver& aObserver, CFLDFileListModel& aModel )
            : iObserver( aObserver ), iModel( aModel ) {}

        virtual ~FLDListBoxTemplate()
            {
            iObserver.HandleFileListBoxEventL(
                MFLDFileListBoxObserver::EListBoxClosed );
            }

	public:		// New functions
	
		/**
		* Sets populist
		*/
		void SetListBox( CFLDPopupList* aPopupList )
			{
			iPopupList = aPopupList;
			}

    public:     // Functions from base classes

        /**
        * Method modified to send listbox events to an observer, when
        * focus changes from one list item to another
        */
        TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType )
            {
            /// Current softkeystate
			MFLDFileListBoxObserver::TFileListSoftKeyState softKeyState;
	        iObserver.HandleSoftKeyState( softKeyState );
	        
            if( aType == EEventKey )
                {
                // Get current item index
                TInt oldIndex( T::CurrentItemIndex() );
                // Call OfferKeyEventL
                TKeyResponse response( T::OfferKeyEventL( aKeyEvent, aType ) );
                // Get new item index
                TInt newIndex( T::CurrentItemIndex() );
                // Get current filename
                TFileName filename;
            	iModel.GetFileName( filename, T::CurrentItemIndex() );
                // Compare new and old index
                if( oldIndex != newIndex )
                    {
                    // Notify focus change if focus changes
                    iObserver.HandleFileListBoxEventL(
                        MFLDFileListBoxObserver::EFocusChanged, filename );

#ifdef RD_VIDEO_AS_RINGING_TONE	             
					// When video is drawn over popuplist and user changes focus
					// or interrupts the video by using other key
					//  screen must be forced to be redrawn	                
					if( ( iPopupList ) &&
					 ( softKeyState == MFLDFileListBoxObserver::EPreviewSelectSoftKeyState ) )
						{
                        // Set the vertical scroll bar and listbox to not dimmed.
                        if ( iPopupList )
                            {
                            iPopupList->ListBox()->SetDimmed( EFalse );
                            iPopupList->ListBox()->ScrollBarFrame()->
                                VerticalScrollBar()->SetDimmed( EFalse );                            
                            }

						iPopupList->DrawNow( CEikonEnv::Static()->EikAppUi()->ApplicationRect() );
						}	
#endif                        
                    }
                else
                    {
#ifdef RD_VIDEO_AS_RINGING_TONE	                    
	                if( ( iModel.MediaFileType( filename ) == ECLFMediaTypeVideo ) &&
	                    ( aKeyEvent.iScanCode == EStdKeyDevice3 ) )
    	            	{
    	                // Do not notify observer
    	                // when joystick key is pressed
    	                // and current file is a video file
	                    }
	                else
	                	{
   	                	iObserver.HandleFileListBoxEventL(
	   	                	MFLDFileListBoxObserver::EOtherKeyEvent );
	   	                	
 						// When video is drawn over popuplist and user changes focus
						// or interrupts the video by using other key
						//  screen must be forced to be redrawn	                
						if( ( iPopupList ) &&
					 	 ( softKeyState == MFLDFileListBoxObserver::EPreviewSelectSoftKeyState ) )
							{
                            // Set the vertical scroll bar and listbox to not dimmed.
                            if ( iPopupList )
                                {
                                iPopupList->ListBox()->SetDimmed( EFalse );
                                iPopupList->ListBox()->ScrollBarFrame()->
                                    VerticalScrollBar()->SetDimmed( EFalse );                            
                                }

							iPopupList->DrawNow( CEikonEnv::Static()->EikAppUi()->ApplicationRect() );
							}		                	
	                	}
            
#else
					iObserver.HandleFileListBoxEventL(
	   	                	MFLDFileListBoxObserver::EOtherKeyEvent );
#endif	                	
	                }
                return response;
                }
            return T::OfferKeyEventL( aKeyEvent, aType );
            }

    private:    // Data

        /// Ref: The list box observer, which gets notified about list item focus changes
        MFLDFileListBoxObserver& iObserver;

        /// Ref: File list model
        CFLDFileListModel& iModel;
        
        /// Ref: File list popuplist
        CFLDPopupList* iPopupList;

    };

#endif // FLDLISTBOXTEMPLATE_H

// End of File
