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
* Description:  XML-Parser used to parse motion patterns.
*
*/


#ifndef C3DAUDIOXMLPARSER_H
#define C3DAUDIOXMLPARSER_H

// INCLUDES
#include <eikenv.h>
#include "C3DAudioPattern.h"	// C3DAudioPatter, T3DScriptPosition
#include <xml/contenthandler.h>	// Xml::MContentHandler
#include <xml/parser.h>			// Xml::CParser

#include <e32property.h>	    // RProperty


//CONSTANTS

// Maximum length for names of element and attribute and for value of attribute.
const TUint KMaxDataLength = 20;
const TUint KBufSize32 = 32;


// DATA TYPES

// Enumeration to use in determining the last tag.
enum TLastTag 
	{
	EInitialization = 0,
	EUpdate
	};
	
	
// Identifiers for reverberation presets. Used in mapping.
enum TReverbPresets
	{
	EReverbNotEnabled = 0,
	EReverbPresetOne = 14,		// Livingroom
	EReverbPresetTwo = 5,		// Cave
    EReverbPresetThree = 26,	// Stone Corridor
    EReverbPresetFour = 8,		// Forest
    EReverbPresetFive = 24,		// Sewerpipe
    EReverbPresetSix = 28		// Underwater
	};
	

// FORWARD DECLARATIONS

class C3DPatternPS;
	

// CLASS DECLARATION

/**
* XML-parser used to parse motion patterns.
*
* @lib RingingTone3DPlugin.dll
* @since 5.0
*/ 	
NONSHARABLE_CLASS (C3DAudioXMLParser) : public CBase, public Xml::MContentHandler
	{
    public:
    
    enum TPatternAttributeKeys
        {
        EKeyPatternId = 1,
        EKeyDopplerUsed = 2,
        EKeyVelocity = 3
        };
	
	public:
		
	   /**
        * Two-phased constructor.
        */
		static C3DAudioXMLParser* NewL();
		
	   /**
        * Two-phased constructor.
        */
		static C3DAudioXMLParser* NewLC();
		
	   /**
        * Destructor.
        */
		virtual ~C3DAudioXMLParser();
		
	private:

       /**
        * C++ default constructor.
        */
        C3DAudioXMLParser();
        
       /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
		
		
	public:
		
       /**
        * Parses the motion pattern from XML-file.
        * @param aPatternNum Motion pattern to be parsed.
        */
		void ParseL( TInt aPatternId );

       /**
        * Parses the motion pattern from XML-file.
        * @param aPatternNum Motion pattern to be parsed.
        */
		void ParseL( const TDesC& aFileName );
		
	   /**
        * Returns single pattern attribute.
        * @param Pattern attribute key.
        * @return Pattern attribute value.
        */
		TInt Attribute( TInt aKey );
		
		const T3DPosition& PositionL( TInt aIndex );
		const T3DVelocity& VelocityL( TInt aIndex );
		const TInt& TimeL( TInt aIndex );
		TInt CountL();
		
    private:
	   /**
        * Reads 3D ringing tone pattern data from cache (PubSub)
        * @param aPatternId Pattern id.
        */
        TBool ReadFromPatternCacheL( TInt aPatternId );

	   /**
        * Writes 3D ringing tone pattern data into cache (PubSub)
        * @param aPatternId Pattern id.
        */
        void WriteToPatternCacheL( TInt aPatternId );
        
#ifdef _DEBUG
    public:        
        void AssertParseDataL( TInt aPatternId );
        static void GetPatternString(TDes& aText, TInt aPattern);
        static void FlogPatternsL(TInt aOldPattern, TInt aNewPattern);
        static void FlogPatternL(TInt aPattern);
        static void FlogPubSubL(TInt aEcho);
        static void GetEchoString(TDes& aText, TInt aEcho);
#endif        


	public: // from MContentHandler
	
	   /**
        * From MContentHandler This method is a callback to indicate the start of the document. 
        * @param aDocParam Specifies the various parameters of the document.
        * @param aErrorCode The error code.
        */
		void OnStartDocumentL( const Xml::RDocumentParameters& aDocParam, TInt aErrorCode );
		
       /**
        * This method is a callback to indicate the end of the document. 
        * @param aErrorCode The error code.
        */
		void OnEndDocumentL( TInt aErrorCode );
		
	   /**
        * This method is a callback to indicate an element has been parsed. 
        * @param aElement Handle to the element's details.
        * @param aAttributes Attributes for the element. 
        * @param aErrorCode The error code.
        */
		void OnStartElementL( const Xml::RTagInfo& aElement, const Xml::RAttributeArray& aAttributes, TInt aErrorCode );
		
	   /**
        * This method is a callback to indicate the end of the element has been reached. 
        * @param aElement Handle to the element's details.
        * @param aErrorCode The error code.
        */
		void OnEndElementL( const Xml::RTagInfo& aElement, TInt aErrorCode );
		
       /**
        * This method is a callback that sends the content of the element.
        * @param aBytes Raw content data for the element
        * @param aErrorCode The error code.
        */
		void OnContentL( const TDesC8& aBytes, TInt aErrorCode );
	
	   /**
        * This method is a notification of the beginning of the scope of a prefix-URI Namespace mapping.
        * @param aPrefix Namespace prefix being declared. 
        * @param aUri Namespace URI the prefix is mapped to. 
        * @param aErrorCode The error code.
        */
		void OnStartPrefixMappingL( const RString& aPrefix, const RString& aUri, TInt aErrorCode );
		
	   /**
        * This method is a notification of the end of the scope of a prefix-URI mapping.
        * @param aPrefix Namespace prefix that was mapped. 
        * @param aErrorCode The error code.
        */
		void OnEndPrefixMappingL( const RString& aPrefix, TInt aErrorCode );
		
	   /**
        * This method is a notification of ignorable whitespace in element content. 
        * @since Series ?XX ?SeriesXX_version
        * @param aBytes Ignored bytes from the document being parsed. 
        * @param aErrorCode The error code.
        */
		void OnIgnorableWhiteSpaceL( const TDesC8& aBytes, TInt aErrorCode );
		
	   /**
        * This method is a notification of a skipped entity.
        * @param aName Name of the skipped entity.
        * @param aErrorCode The error code.
        */
		void OnSkippedEntityL( const RString& aName, TInt aErrorCode );
		
       /**
        * This method is a receive notification of a processing instruction. 
        * @param aTarget Processing instruction target. 
        * @param aData Processing instruction data. If empty none was supplied.
        * @param aErrorCode The error code.
        */
		void OnProcessingInstructionL( const TDesC8& aTarget, const TDesC8& aData, TInt aErrorCode );
		
       /**
        * This method indicates an error has occurred.
        * @param aErrorCode The error code.
        */
		void OnError( TInt aErrorCode );
	
	   /**
        * This method obtains the interface matching the specified uid. 
        * @param aUid UID identifying the required interface. 
        */
		TAny* GetExtendedInterface( const TInt32 aUid );
		
		
	private:
		
	   /**
        * Processes the attributes of initialization-element.
        * @param aAttributes Array of attributes.
        */	
		void ProcessInitAttributesL( const Xml::RAttributeArray& aAttributes );
		
	   /**
		* Processes the attributes of loop-element.
        * @param aAttributes Array of attributes.
        */	
		void ProcessLoopAttributesL( const Xml::RAttributeArray& aAttributes );
		
	   /**
		* Processes the attributes of position-element.
        * @param aAttributes Array of attributes.
        */
		void ProcessPositionAttributesL( const Xml::RAttributeArray& aAttributes );
		
	   /**
		* Processes the attributes of update-element.
        * @param aAttributes Array of attributes.
        */
		void ProcessUpdateAttributesL( const Xml::RAttributeArray& aAttributes );
		
	   /**
		* Processes the attributes of velocity-element.
        * @param aAttributes Array of attributes.
        */
		void ProcessVelocityAttributesL( const Xml::RAttributeArray& aAttributes );
        
    private:

	   /**
		* String copy with length check.
        * @param aTarget.
        * @param aSource.
        */
        void CopyL( TDes8& aTarget, const TDesC8& aSource );
        
	   /**
        * Panics plugin dll.
        * @param aReason Panic code.
        */		        
        void Panic(TInt aReason);
        
        void GetPatternFullPathL(TDes& aText, TInt aPatternId);
        CBufFlat* ReadPatternFileL( const TDesC& aFullName );
        CBufFlat* ReadPatternFileL( TInt aPatternId );
        
        
	private:	// Data
	
		// Xml::CParser
		Xml::CParser* iParser;
		
		// file server session
		RFs iFsSession;
		
		// Parsed pattern
		C3DAudioPattern* iParsedPattern;  
		
		// element name
		TBuf8<KMaxDataLength> iElementName;
		
		// attribute name
		TBuf8<KMaxDataLength> iAttributeName;
		
		// attribute value
		TBuf8<KMaxDataLength> iAttributeValue;
			
		// Currently parsed pattern update.
		T3DScriptUpdate iCurrentUpdate;
		
		// Last tag occurred.
		TLastTag iLastTag;
		
		// 3DTone file name and path
		TFileName iFullPath;
		
		// Pattern id
		TInt iPatternId;
	};



/**
* C3DPatternPS is used for storing 3D pattern data in
* Public and Subcribe. Max pattern size is 65536 bytes.
*
* @lib RingingTone3DPlugin.dll
* @since 5.0
*/ 	
NONSHARABLE_CLASS (C3DPatternPS) : public CBase
	{
    public:
    
    enum TPatternStoreKeys
        {
        EKeyPatternId = 1,
        EKeyDopplerUsed = 2,
        EKeyVelocity = 3,
        EKeyDataSize = 4,
        EKeyPatternData = 5
        };
	
	public:
		static C3DPatternPS* NewLC( TInt aStoreId );
		virtual ~C3DPatternPS();
		
	private:
        C3DPatternPS( TInt aStoreId );
        void ConstructL();
		
	public:
	    void SetIntL( TInt aKey, TInt aValue );
	    TInt IntL( TInt aKey );
	    
	    CBufFlat* DataLC();
		void GetDataL( TDes8& aData );
		void SetDataL( const TDesC8& aData );
		
		void TestL();

	private:
	    TBool DefineKeyL( TInt aKeyId, TInt aKeyType );
        
	private:
	    RProperty iProperty;
	    TUid iStoreUid;
	};



////////////////////////////////////// START TEST CODE //////////////////////////

#include <eikenv.h>

const TInt KBufSize128 = 128;
const TInt KBufSize64 = 64;


class T3DPatternData
    {
    public:
        TInt iTime;
        TInt iX;
        TInt iY;
        TInt iZ;
    };


/**
* C3DPatternParser
*
* Used to parse xml files.
*/
NONSHARABLE_CLASS (C3DPatternParser) : public CBase, public Xml::MContentHandler 
    {
    public:
        static C3DPatternParser* NewL();
        virtual ~C3DPatternParser();

    private:
	    C3DPatternParser();
        void ConstructL();
        
    public:
        void ParseDocumentL(const TDesC& aFullName);
        void ParseDocumentL(TInt aPatternId);
        TInt PositionCount();
       	T3DPatternData& Position(TInt aIndex);
       	T3DPatternData& Velocity(TInt aIndex);
        void GetPatternName(TDes& aText);
        void GetEchoName(TDes& aText);
        TBool DopplerEnabled();
        
        void AssertPosition(TInt aIndex, TInt aX, TInt aY, TInt aZ);
        void AssertVelocity(TInt aIndex, TInt aX, TInt aY, TInt aZ);
        void AssertTime(TInt aIndex, TInt aTime);

        
        
    private:
        void Print();
        void PrintCoordinates(T3DPatternData& aData, TBool aVelocity);
        
    
    private: // from MContentHandler
    
        void OnStartDocumentL(const Xml::RDocumentParameters& aDocParam, TInt aErrorCode);
        void OnEndDocumentL(TInt aErrorCode);
        void OnStartElementL(const Xml::RTagInfo& aElement, 
                             const Xml::RAttributeArray& aAttributes, 
							 TInt aErrorCode);
		void OnEndElementL(const Xml::RTagInfo& aElement, TInt aErrorCode);
		void OnContentL(const TDesC8& aBytes, TInt aErrorCode);
		void OnStartPrefixMappingL(const RString& aPrefix, const RString& aUri, 
								   TInt aErrorCode);
		void OnEndPrefixMappingL(const RString& aPrefix, TInt aErrorCode);
		void OnIgnorableWhiteSpaceL(const TDesC8& aBytes, TInt aErrorCode);
		void OnSkippedEntityL(const RString& aName, TInt aErrorCode);
		void OnProcessingInstructionL(const TDesC8& aTarget, const TDesC8& aData, 
		    						  TInt aErrorCode);
        void OnError(TInt aErrorCode);
        TAny* GetExtendedInterface(const TInt32 aUid);
        
    private:
        CBufFlat* ReadDocumentL(const TDesC& aFullName);
        void ReadPositionAttributesL(const Xml::RAttributeArray& aAttributes);
        void ReadVelocityAttributesL(const Xml::RAttributeArray& aAttributes);
        void ReadInitAttributesL(const Xml::RAttributeArray& aAttributes);
        void ReadUpdateAttributesL(const Xml::RAttributeArray& aAttributes);
        
    private:
        void StrCopy(TDes& aTarget, const TDesC8& aSource);
        void StrCopy(TDes& aTarget, const TDesC& aSource);
        TInt StrToIntL(const TDesC& aText);
        void Panic(TInt aReason);
        void GetPatternFileNameL(TDes& aText, TInt aPatternId);
        
    private:
        
		// Xml parser
		Xml::CParser* iParser;
		
		// file server session
		RFs iFsSession;
		
		TBuf<KBufSize64> iElementName;
		TBuf<KBufSize64> iAttributeName;
		TBuf<KBufSize64> iAttributeValue;
		TBuf<KBufSize64> iEchoName;
		TBuf<KBufSize64> iPatternName;
		
		TBool iDopplerUsed;
		TBool iVelocityUsed;
		
		T3DPatternData iCurrentPosition;
		T3DPatternData iCurrentVelocity;
		
		T3DPatternData iInitPosition;
		T3DPatternData iInitVelocity;

		
		
		// list for parsed 3d pattern position coordinates
		RArray<T3DPatternData> iPositionList;
		
		// list for parsed 3d pattern velocity coordinates
		RArray<T3DPatternData> iVelocityList;
    };

////////////////////////////////////// END TEST CODE ////////////////////////////


#endif	// C3DAUDIOXMLPARSER_H

// End of File
