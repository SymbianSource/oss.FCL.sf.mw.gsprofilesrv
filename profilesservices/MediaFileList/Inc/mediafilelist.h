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
* Description:   Interface of MediaFileList. Provide some API for 
*                the user to use mediafilelist
*
*/




#ifndef MEDIALFILELIST_H
#define MEDIALFILELIST_H

//  INCLUDES
#include <e32base.h>
#include <s32file.h>


// FORWARD DECLARATIONS
class CMediaFileDialog;



// CLASS DECLARATION

/**
*  Public API for displaying media file list. List is used for 
*  selecting/previewing audio and video files. 
* 
*  @lib mediafilelist.lib
*  @since Series60_5.1
*  
*/
class CMediaFileList : public CBase
    {
    public:
    enum TMediaFileListAttribute
        {
        // max media file size
        EAttrFileSize = 0,
        // ringing volume (TProfileRingingVolume from Profile.hrh)
        EAttrVolume,
        // ringing type,  (TProfileRingingType from Profile.hrh)
        EAttrRingingType,
        // vibra on/off   (Boolean) 
        EAttrVibra,
        // 3D effect      (TProfile3DToneEffect from ProfileInternal.hrh)
        EAttr3DEffect,
        // 3D echo        (TProfile3DToneEcho from ProfileInternal.hrh)
        EAttr3DEcho,
        // excluded mime type text
        EAttrExcludeMimeType,
        // for file protection checking
        EAttrAutomatedType,
        // media file dialog title
        EAttrTitle,
        // excluded folder (see enum TFolderType)
        EAttrExcludeFolder
        };

    enum TMediaFileType
        {
        EMediaFileTypeAudio = 0,
        EMediaFileTypeVideo
        };

    enum TNullItemIcon
        {
        ENullItemIconNone = 0,
        ENullItemIconDefaultTone,
        ENullItemIconOff
        };

    enum TFolderType
        {
        EFolderDownload = 0,
        EFolderVideos
        };

    public:
        /**
        * Two-phased constructor.
        */
        IMPORT_C static CMediaFileList* NewL();

        /**
        * Destructor.
        */
	    IMPORT_C virtual ~CMediaFileList();

    private:
        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * C++ default constructor.
        */
        CMediaFileList();

    public:

       /**
        * Shows Media file list.
        * @since Series60_5.1
        * @param aFileName Selected media file name full path.
        * @param aNullItem Selected null item index. Possible values:
        *        KErrNotFound - item is not null item or Download item
        *        0 - Download item
        *        1 - first null item
        *        2 - second null item
        *  
        * @param aInfo Extra information, set to NULL if not used.
        * @param aDesInfo Extra information, set to KNullDesC if not used.
        * @return ETrue if user selected media file, EFalse otherwise.
        */
	    IMPORT_C TBool ShowMediaFileListL( TDes* aFileName, TInt* aNullItem,
	                                       TInt* aInfo, TDes* aDesInfo );

        /**
        * Set Media file list attribute. See enum TMediaFileListAttribute for
        * list of supported attributes.
        * @since Series60_5.1
        * @param aAttr Attribute id. 
        * @param aValue Attribute value. 
        * @return None.
        */
       	IMPORT_C void SetAttrL( TInt aAttr, TInt aValue );

        /**
        * Set Media file list attribute. See enum TMediaFileListAttribute for
        * list of supported attributes.
        * @since Series60_5.1
        * @param aAttr Attribute id. 
        * @param aValue Attribute value. 
        * @return None.
        */
	    IMPORT_C void SetAttrL( TInt aAttr, const TDesC& aValue );

        /**
        * Set Media file list attribute. Currently not in use.
        * @since Series60_5.1
        * @param aAttr Attribute id. 
        * @param aValue Attribute value. 
        * @return None.
        */
	    IMPORT_C void SetAttrL( TInt aAttr, TAny* aValue );

        /**
        * Set Media file list null item. Null items are custom list items
        * that appear at the end of the media file list main view.
        * @since Series60_5.1
        * @param aText Null item text.
        * @param aMediaFile File that is played when null items gets focused.
        *        Use KNullDesC if not used.
        * @param aMediaFileType Media file type. See enum TMediaFileType for
                 supported media types.
        * @param aIconId Null item icon id. See enum TNullItemIcon for
                 supported icons.
        * @return None.
        */
	    IMPORT_C void SetNullItemL( const TDesC& aText, const TDesC& aMediaFile,
	                                TInt aMediaFileType, TInt aIconId );

	

    private:
		CMediaFileDialog* iDialog;
    };



#endif      // MEDIALFILELIST_H
            
// End of File
