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


// INCLUDE FILES
#include "C3DAudioXMLParser.h"
#include "tdaudioconstants.h"	// K3DAudioRepositoryUid
#include <f32file.h>			// RFs
#include <s32mem.h>				// RBufWriteStream
#include <centralrepository.h>	// CRepository


#include "RingingTone3DPluginDebug.h"
#include <ProfileInternal.hrh> // for TProfile3DToneEffect and TProfile3DToneEcho

#include <pathinfo.h>
#include <driveinfo.h>
#include <data_caging_path_literals.hrh>


//CONSTANTS
const TInt32 KRingingTone3DAudioEComPluginUID = 0x10208ADF;


// 3DTone pattern dir
_LIT( K3DTonesDir, "3DTones" );

// 3DTone pattern files
_LIT( KPatternCircle,     "1circular.xml" );
_LIT( KPatternFlyBy,      "2flyby.xml" );
_LIT( KPatternZigZag,     "3zigzag.xml" );
_LIT( KPatternRandomWalk, "4meander.xml" );
_LIT( KPatternChaos,      "5random.xml" );


// Elements
_LIT8( KInitElement, "initialization");
_LIT8( KPositionElement, "position" );
_LIT8( KUpdateElement, "update" );
_LIT8( KVelocityElement, "velocity" );
_LIT8( KLoopElement, "loop" );

// Attribute names.
_LIT8( KPatternName, "name" );
_LIT8( KPatternReverb, "reverb" );
_LIT8( KPatternDoppler, "doppler" );
_LIT8( KXCoordinate, "x" );
_LIT8( KYCoordinate, "y" );
_LIT8( KZCoordinate, "z" );
_LIT8( KDTime, "dTime" );
_LIT8( KScalarVelocity, "scalar" );
_LIT8( KLoopCount, "count" );

// Attribute values
_LIT8( KReverbPresetOne, "Livingroom" );
_LIT8( KReverbPresetTwo, "Cave" );
_LIT8( KReverbPresetThree, "Stone Corridor" );
_LIT8( KReverbPresetFour, "Forest" );
_LIT8( KReverbPresetFive, "Sewerpipe" );
_LIT8( KReverbPresetSix, "Underwater" );
_LIT8( KBooleanTrue, "true" );



/******************************************************************************
 * class C3DAudioXMLParser
 ******************************************************************************/



// -----------------------------------------------------------------------------
// C3DAudioXMLParser::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//		
C3DAudioXMLParser* C3DAudioXMLParser::NewL()
	{
	C3DAudioXMLParser* self = C3DAudioXMLParser::NewLC();
    CleanupStack::Pop( self );
    return self;
	}


// -----------------------------------------------------------------------------
// C3DAudioXMLParser::NewLC
// Two-phased constructor. Leaves pointer on cleanup stack.
// -----------------------------------------------------------------------------
//		
C3DAudioXMLParser* C3DAudioXMLParser::NewLC()
	{
	C3DAudioXMLParser * self = new ( ELeave ) C3DAudioXMLParser();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;	
	}


// Destructor
C3DAudioXMLParser::~C3DAudioXMLParser()
	{
	delete iParser;
	delete iParsedPattern;
   	iFsSession.Close();
	}


// -----------------------------------------------------------------------------
// C3DAudioXMLParser::C3DAudioXMLParser
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//		
C3DAudioXMLParser::C3DAudioXMLParser():
	iParser( NULL ),
	iParsedPattern( NULL )
	{
	}


// -----------------------------------------------------------------------------
// C3DAudioXMLParser::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void C3DAudioXMLParser::ConstructL()
    {
    _LIT8( KParserMimeType, "text/xml" );
    
	iPatternId = KErrNotFound;
	
	iParser = Xml::CParser::NewL( KParserMimeType, *this );
	
	User::LeaveIfError( iFsSession.Connect() );
    }	
    

// -----------------------------------------------------------------------------
// C3DAudioXMLParser::Count
//
// -----------------------------------------------------------------------------
//		
TInt C3DAudioXMLParser::CountL()
    {
    User::LeaveIfNull( iParsedPattern );
    const RArray<T3DScriptUpdate>& script = iParsedPattern->Script();
    
    return script.Count();
    }


// -----------------------------------------------------------------------------
// C3DAudioXMLParser::PositionL
//
// -----------------------------------------------------------------------------
//		
const T3DPosition& C3DAudioXMLParser::PositionL( TInt aIndex )
    {
    User::LeaveIfNull( iParsedPattern );
    const RArray<T3DScriptUpdate>& script = iParsedPattern->Script();
    
    if ( aIndex < 0 || aIndex >= script.Count() )
        {
        User::Leave( KErrArgument );
        }
        
    return script[ aIndex ].iPosition;
    }


// -----------------------------------------------------------------------------
// C3DAudioXMLParser::VelocityL
//
// -----------------------------------------------------------------------------
//		
const T3DVelocity& C3DAudioXMLParser::VelocityL( TInt aIndex )
    {
    User::LeaveIfNull( iParsedPattern );
    const RArray<T3DScriptUpdate>& script = iParsedPattern->Script();
    
    if ( aIndex < 0 || aIndex >= script.Count() )
        {
        User::Leave( KErrArgument );
        }
        
    return script[ aIndex ].iVelocity;
    }


// -----------------------------------------------------------------------------
// C3DAudioXMLParser::TimeL
//
// -----------------------------------------------------------------------------
//		
const TInt& C3DAudioXMLParser::TimeL( TInt aIndex )
    {
    User::LeaveIfNull( iParsedPattern );    
    const RArray<T3DScriptUpdate>& script = iParsedPattern->Script();
    
    if ( aIndex < 0 || aIndex >= script.Count() )
        {
        User::Leave( KErrArgument );
        }
        
    return script[ aIndex ].iDTime;
    }


// -----------------------------------------------------------------------------
// C3DAudioXMLParser::Attribute
//
// -----------------------------------------------------------------------------
//		
TInt C3DAudioXMLParser::Attribute( TInt aKey )
    {
    TInt num = KErrNotFound;
    
    switch ( aKey )
        {
        case EKeyPatternId:
            {
            num = iPatternId;
            break;
            }
        case EKeyDopplerUsed:
            {
            num = iParsedPattern->DopplerAvailable();
            break;
            }
        case EKeyVelocity:
            {
            num = KMotionPatternDefaultVelocity;
            break;
            }
        default:
            {
            break;    
            }
        }
        
    return num;
    }


// -----------------------------------------------------------------------------
// C3DAudioXMLParser::ParseL
//
// -----------------------------------------------------------------------------
//		
void C3DAudioXMLParser::ParseL( TInt aPatternId )
	{
	FLOG( _L("C3DAudioXMLParser::ParseL START") );

	if ( ReadFromPatternCacheL( aPatternId ) )
	    {

#ifdef _DEBUG
        // parsed pattern data found in cache (publish and subcribe)
        FlogPatternL( aPatternId );
#endif

	    }
	else
	    {
    	GetPatternFullPathL( iFullPath, aPatternId );
	    ParseL( iFullPath );

	    WriteToPatternCacheL( aPatternId );
	    
	    FLOG( _L("pattern '%S' parsed and written to cache"), &iFullPath );
	    }
	    
	iPatternId = aPatternId;
	
#ifdef _DEBUG                         
    AssertParseDataL( aPatternId );
#endif

	
	FLOG( _L("C3DAudioXMLParser::ParseL END") );
	}


// -----------------------------------------------------------------------------
// C3DAudioXMLParser::ParseL
//
// -----------------------------------------------------------------------------
//		
void C3DAudioXMLParser::ParseL( const TDesC& aFileName )
	{
	delete iParsedPattern;
    iParsedPattern = NULL;
	iParsedPattern = C3DAudioPattern::NewL();
	
	//
	// read xml-format pattern file into descriptor
	//
	CBufFlat* buf = ReadPatternFileL( aFileName );
	CleanupStack::PushL( buf );
	
	//
	// parse xml-format pattern into C3DAudioPattern
	//
	TPtr8 ptr = buf->Ptr( 0 );
    iParser->ParseL( ptr );
    iParser->ParseEndL();
    
    RArray<T3DScriptUpdate> script = iParsedPattern->Script();
	if ( script.Count() == 0 )
	    {
        User::Leave( KErrNotFound );
	    }

   	CleanupStack::PopAndDestroy( buf );
	}


// -----------------------------------------------------------------------------
// C3DAudioXMLParser::ReadFromPatternCacheL
//
// -----------------------------------------------------------------------------
//		
TBool C3DAudioXMLParser::ReadFromPatternCacheL( TInt aPatternId )
	{
   	C3DPatternPS* pubSub = C3DPatternPS::NewLC( KRingingTone3DAudioEComPluginUID );
    
    TInt patternId = pubSub->IntL( C3DPatternPS::EKeyPatternId );
    if ( patternId == aPatternId )
        {
        // buffer for 3D pattern data
        CBufFlat* buf = pubSub->DataLC();
                
        // readstream to construct the pattern.
        RBufReadStream readstream( *buf );
        CleanupClosePushL( readstream );
                
        // create C3DAudioPattern for accessing 3D ringing tone data 
        delete iParsedPattern;
        iParsedPattern = NULL;
        iParsedPattern = C3DAudioPattern::NewL( readstream );
    
	    CleanupStack::PopAndDestroy( &readstream );
	    CleanupStack::PopAndDestroy( buf );
        }
       
	CleanupStack::PopAndDestroy( pubSub );
	
	if ( patternId == aPatternId )
	    {
	    return ETrue;
	    }
	return EFalse;
	}


// -----------------------------------------------------------------------------
// C3DAudioXMLParser::WriteToPatternCacheL
//
// -----------------------------------------------------------------------------
//		
void C3DAudioXMLParser::WriteToPatternCacheL( TInt aPatternId )
	{
    const TInt KGranularity = 1024;
	
   	C3DPatternPS* pubSub = C3DPatternPS::NewLC( KRingingTone3DAudioEComPluginUID );

    //
    // write binary-format (parsed) pattern data into descriptor
    //
    CBufFlat* buf = CBufFlat::NewL( KGranularity );
    CleanupStack::PushL( buf );
    
    RBufWriteStream writestream( *buf );
    CleanupClosePushL( writestream );
    iParsedPattern->ExternalizeL( writestream );
   	writestream.CommitL();
	CleanupStack::PopAndDestroy( &writestream );

#ifdef _DEBUG
    TInt oldPatternId = pubSub->IntL( C3DPatternPS::EKeyPatternId );
    FlogPatternsL( oldPatternId, aPatternId );
#endif    
    
    //
    // write pattern data and attributes into public and subscribe
    //
    pubSub->SetIntL( C3DPatternPS::EKeyPatternId, KErrNotFound );
	pubSub->SetIntL( C3DPatternPS::EKeyDopplerUsed, iParsedPattern->DopplerAvailable() );
	pubSub->SetIntL( C3DPatternPS::EKeyVelocity, KMotionPatternDefaultVelocity );
	pubSub->SetDataL( buf->Ptr( 0 ) );
	pubSub->SetIntL( C3DPatternPS::EKeyPatternId, aPatternId );
	
	CleanupStack::PopAndDestroy( buf );
	
	CleanupStack::PopAndDestroy( pubSub );
	}



// -----------------------------------------------------------------------------
// C3DAudioXMLParser::OnStartDocumentL
// Leave if error, otherwise nothing to do.
// -----------------------------------------------------------------------------
//	
void C3DAudioXMLParser::OnStartDocumentL( 
	const Xml::RDocumentParameters& /*aDocParam*/, 
	TInt aErrorCode )
	{
	// Leave if error occurred.
	User::LeaveIfError( aErrorCode );
	}


// -----------------------------------------------------------------------------
// C3DAudioXMLParser::OnEndDocumentL
// Leave if error, otherwise nothing to do.
// -----------------------------------------------------------------------------
//		
void C3DAudioXMLParser::OnEndDocumentL( 
	TInt aErrorCode )
	{
	// Leave if error occurred.
	User::LeaveIfError( aErrorCode );
	}


// -----------------------------------------------------------------------------
// C3DAudioXMLParser::OnStartElementL
// Proceed depending on the element.
// -----------------------------------------------------------------------------
//		
void C3DAudioXMLParser::OnStartElementL( 
	const Xml::RTagInfo& aElement, 
	const Xml::RAttributeArray& aAttributes, 
	TInt aErrorCode )
	{
	// Leave if error occurred.
	User::LeaveIfError( aErrorCode );
	
	CopyL( iElementName, aElement.LocalName().DesC() );
	
	// Proceed depending of the element.
	if ( iElementName.CompareC( KUpdateElement ) == 0 )
		{
		ProcessUpdateAttributesL( aAttributes );
		iLastTag = EUpdate;
		}
	else if ( iElementName.CompareC( KPositionElement ) == 0 )
		{
		// Do not take position in initialization in account.
		if ( iLastTag == EUpdate )
			{
			ProcessPositionAttributesL( aAttributes );
			}
		}
	else if ( iElementName.CompareC( KVelocityElement ) == 0 )
		{
		// Do not take velocity in initialization in account.
		if ( iLastTag == EUpdate )
			{
			ProcessVelocityAttributesL( aAttributes );	
			}
		}
	else if ( iElementName.CompareC( KInitElement ) == 0 )
		{
		// pasi
		// Create new motion pattern and process the attributes of it.
		delete iParsedPattern;
    	iParsedPattern = NULL;
		iParsedPattern = C3DAudioPattern::NewL();
		
		ProcessInitAttributesL( aAttributes );
		iLastTag = EInitialization;
		}
	else if ( iElementName.CompareC( KLoopElement ) == 0 )
		{
		ProcessLoopAttributesL( aAttributes );
		}
	}


// -----------------------------------------------------------------------------
// C3DAudioXMLParser::OnEndElementL
// Proceed for elements that need processing.
// -----------------------------------------------------------------------------
//		
void C3DAudioXMLParser::OnEndElementL( const Xml::RTagInfo& aElement, TInt aErrorCode )
	{
	// Leave if error occurred.
	User::LeaveIfError( aErrorCode );
	
	CopyL( iElementName, aElement.LocalName().DesC() );
	
	if ( iElementName.CompareC( KUpdateElement ) == 0 )
		{
		// Update-element ended, add the update to script.
		iParsedPattern->AddUpdateL( iCurrentUpdate );
		}
	else if ( iElementName.CompareC( KLoopElement ) == 0 )
		{
		// Loop-element ended, end the loop.
		iParsedPattern->EndLoop();
		}
	}


// -----------------------------------------------------------------------------
// C3DAudioXMLParser::OnContentL
// Leave if error, otherwise nothing to do.
// -----------------------------------------------------------------------------
//		
void C3DAudioXMLParser::OnContentL( const TDesC8& /*aBytes*/, TInt aErrorCode )
	{
	// Leave if error occurred.
	User::LeaveIfError( aErrorCode );
	}


// -----------------------------------------------------------------------------
// C3DAudioXMLParser::OnStartPrefixMappingL
// Leave if error, otherwise nothing to do.
// -----------------------------------------------------------------------------
//	
void C3DAudioXMLParser::OnStartPrefixMappingL( const RString& /*aPrefix*/, 
	                                           const RString& /*aUri*/, 
	                                           TInt aErrorCode )
	{
	// Leave if error occurred.
	User::LeaveIfError( aErrorCode );
	}


// -----------------------------------------------------------------------------
// C3DAudioXMLParser::OnEndPrefixMappingL
// Leave if error, otherwise nothing to do.
// -----------------------------------------------------------------------------
//		
void C3DAudioXMLParser::OnEndPrefixMappingL( const RString& /*aPrefix*/, 
	                                         TInt aErrorCode )
	{
	// Leave if error occurred.
	User::LeaveIfError( aErrorCode );
	}


// -----------------------------------------------------------------------------
// C3DAudioXMLParser::OnIgnorableWhiteSpaceL
// Leave if error, otherwise nothing to do.
// -----------------------------------------------------------------------------
//		
void C3DAudioXMLParser::OnIgnorableWhiteSpaceL(	const TDesC8& /*aBytes*/, 
	                                            TInt aErrorCode )
	{
	// Leave if error occurred.
	User::LeaveIfError( aErrorCode );
	}


// -----------------------------------------------------------------------------
// C3DAudioXMLParser::OnSkippedEntityL
// Leave if error, otherwise nothing to do.
// -----------------------------------------------------------------------------
//		
void C3DAudioXMLParser::OnSkippedEntityL( const RString& /*aName*/, 
	                                      TInt aErrorCode )
	{
	// Leave if error occurred.
	User::LeaveIfError( aErrorCode );
	}


// -----------------------------------------------------------------------------
// C3DAudioXMLParser::OnProcessingInstructionL
// Leave if error, otherwise nothing to do.
// -----------------------------------------------------------------------------
//		
void C3DAudioXMLParser::OnProcessingInstructionL( const TDesC8& /*aTarget*/, 
	                                              const TDesC8& /*aData*/, 
	                                              TInt aErrorCode )
	{
	// Leave if error occurred.
	User::LeaveIfError( aErrorCode );
	}


// -----------------------------------------------------------------------------
// C3DAudioXMLParser::OnError
// Print error code to debug.
// -----------------------------------------------------------------------------
//		
void C3DAudioXMLParser::OnError( TInt aErrorCode )
	{
	if ( aErrorCode != KErrNone )
	    {
	    FLOG( _L("### C3DAudioXMLParser::OnError (%d) ###"), aErrorCode );    
	    }
	}


// -----------------------------------------------------------------------------
// C3DAudioXMLParser::GetExtendedInterface
// Not implemented
// -----------------------------------------------------------------------------
//
TAny* C3DAudioXMLParser::GetExtendedInterface( const TInt32 /*aUid*/ )
	{
	return NULL;
	}


// -----------------------------------------------------------------------------
// C3DAudioXMLParser::ProcessInitAttributesL
// Process the initialization attributes.
// -----------------------------------------------------------------------------
//
void C3DAudioXMLParser::ProcessInitAttributesL( const Xml::RAttributeArray& aAttributes )
	{
	// Check each attribute.	
	for ( TUint i = 0; i < aAttributes.Count(); ++i )
		{
		CopyL( iAttributeName, aAttributes[i].Attribute().LocalName().DesC() );
		CopyL( iAttributeValue, aAttributes[i].Value().DesC() );
		
		// Set the name of the pattern.
		if ( iAttributeName.CompareC( KPatternName ) == 0 )
			{
			iParsedPattern->SetNameL( iAttributeValue );
			}
		else if ( iAttributeName.CompareC( KPatternReverb ) == 0 )
			{
			TUint selectedReverb = EReverbNotEnabled;

			// Determine the reverberation.			
			if ( iAttributeValue.CompareC( KReverbPresetOne ) == 0 )
				{
				selectedReverb = EReverbPresetOne;	
				}
			else if ( iAttributeValue.CompareC( KReverbPresetTwo ) == 0 )
				{
				selectedReverb = EReverbPresetTwo;
				}
			else if ( iAttributeValue.CompareC( KReverbPresetThree ) == 0 )
				{
				selectedReverb = EReverbPresetThree;
				}
			else if ( iAttributeValue.CompareC( KReverbPresetFour ) == 0 )
				{
				selectedReverb = EReverbPresetFour;
				}
			else if ( iAttributeValue.CompareC( KReverbPresetFive ) == 0 )
				{
				selectedReverb = EReverbPresetFive;
				}
			else if ( iAttributeValue.CompareC( KReverbPresetSix ) == 0 )
				{
				selectedReverb = EReverbPresetSix;
				}
				
			// Set the reverberation to pattern.				
			iParsedPattern->SetReverb( selectedReverb );
			}
		else if ( iAttributeName.CompareC( KPatternDoppler ) == 0)
			{
			// Set doppler.			
			if ( iAttributeValue.CompareC( KBooleanTrue ) == 0 )
				{
				iParsedPattern->SetDoppler( ETrue );
				}
			}
		}
	}


// -----------------------------------------------------------------------------
// C3DAudioXMLParser::ProcessLoopAttributes
// Process the loop-element attributes.
// -----------------------------------------------------------------------------
//
void C3DAudioXMLParser::ProcessLoopAttributesL( const Xml::RAttributeArray& aAttributes )
	{
	TLex8 lex;
			
	for ( TUint i = 0; i < aAttributes.Count(); ++i )
		{
		CopyL( iAttributeName, aAttributes[i].Attribute().LocalName().DesC() );
		CopyL( iAttributeValue, aAttributes[i].Value().DesC() );

		lex.Assign( iAttributeValue );
		
		if ( iAttributeName.CompareC( KLoopCount ) == 0 )
			{
			TUint loopCount;
			User::LeaveIfError ( lex.Val( loopCount ) );
			
			// Start the loop in pattern.
			iParsedPattern->StartLoopL( loopCount );
			}
		}
	}


// -----------------------------------------------------------------------------
// C3DAudioXMLParser::ProcessPositionAttributes
// Process the position-element attributes.
// -----------------------------------------------------------------------------
//
void C3DAudioXMLParser::ProcessPositionAttributesL( const Xml::RAttributeArray& aAttributes )
	{
	TLex8 lex;
	T3DPosition position = T3DPosition();
		
    TInt count = aAttributes.Count();
	for ( TUint i = 0; i < count; ++i )
		{
		CopyL( iAttributeName, aAttributes[i].Attribute().LocalName().DesC() );
		CopyL( iAttributeValue, aAttributes[i].Value().DesC() );

		lex.Assign( iAttributeValue );
		
		if ( iAttributeName.CompareC( KXCoordinate ) == 0 )
			{
			User::LeaveIfError( lex.Val( position.iX ) );
			}
		else if ( iAttributeName.CompareC( KYCoordinate ) == 0 )
			{
			User::LeaveIfError( lex.Val( position.iY ) );
			}
		else if ( iAttributeName.CompareC( KZCoordinate ) == 0)
			{
			User::LeaveIfError( lex.Val( position.iZ ) );
			}
		}
		
	// Set position for current update.
	iCurrentUpdate.iPosition = position;	
	}


// -----------------------------------------------------------------------------
// C3DAudioXMLParser::ProcessUpdateAttributes
// Process the update-element attributes.
// -----------------------------------------------------------------------------
//
void C3DAudioXMLParser::ProcessUpdateAttributesL( const Xml::RAttributeArray& aAttributes )
	{
	TLex8 lex;
	// New update-element.
	iCurrentUpdate = T3DScriptUpdate();
		
	for ( TUint i = 0; i < aAttributes.Count(); ++i )
		{
		CopyL( iAttributeName, aAttributes[i].Attribute().LocalName().DesC() );
		CopyL( iAttributeValue, aAttributes[i].Value().DesC() );

		lex.Assign( iAttributeValue );
		
		if ( iAttributeName.CompareC( KDTime ) == 0 )
			{
			// Set the dTime for the update.			
			User::LeaveIfError( lex.Val( iCurrentUpdate.iDTime ) );
			}
		}
	}


// -----------------------------------------------------------------------------
// C3DAudioXMLParser::ProcessVelocityAttributes
// Process the velocity-element attributes.
// -----------------------------------------------------------------------------
//
void C3DAudioXMLParser::ProcessVelocityAttributesL( const Xml::RAttributeArray& aAttributes )
	{
	TLex8 lex;
	T3DVelocity velocity = T3DVelocity();
		
	for ( TUint i = 0; i < aAttributes.Count(); ++i )
		{
		CopyL( iAttributeName, aAttributes[i].Attribute().LocalName().DesC() );
		CopyL( iAttributeValue, aAttributes[i].Value().DesC() );

		lex.Assign( iAttributeValue );
		
		if ( iAttributeName.CompareC( KScalarVelocity ) == 0 )
			{
			velocity.iScalar = ETrue;
			User::LeaveIfError( lex.Val( velocity.iScalarVelocity ) );
			velocity.iXVector = 0;
			velocity.iYVector = 0;
			velocity.iZVector = 0;
			break;
			}
		else if ( iAttributeName.CompareC( KXCoordinate ) == 0 )
			{
			User::LeaveIfError( lex.Val( velocity.iXVector ) );
			}
		else if ( iAttributeName.CompareC( KYCoordinate ) == 0 )
			{
			User::LeaveIfError( lex.Val( velocity.iYVector ) );
			}
		else if ( iAttributeName.CompareC( KZCoordinate ) == 0)
			{
			User::LeaveIfError( lex.Val( velocity.iZVector ) );
			}
		}
	// Set velocity for current update.	
	iCurrentUpdate.iVelocity = velocity;
	}


// -----------------------------------------------------------------------------
// C3DAudioXMLParser::CopyL
// 
// -----------------------------------------------------------------------------
//
void C3DAudioXMLParser::CopyL( TDes8& aTarget, const TDesC8& aSource )
	{
	if ( aSource.Length() > aTarget.MaxLength() )
	    {

#ifdef _DEBUG			                
        Panic( KErrArgument );
#endif        

	    User::Leave( KErrTooBig );
	    }
	    
	aTarget.Copy( aSource );
	}


// -----------------------------------------------------------------------------
// C3DAudioXMLParser::Panic()
//
// -----------------------------------------------------------------------------
//    
void C3DAudioXMLParser::Panic(TInt aReason)
    {
	_LIT(KPanicCategory,"RingingTone3DAudioPlugin");
	
	User::Panic(KPanicCategory, aReason); 
    }
    

// -------------------------------------------------------------------------------
// C3DAudioXMLParser::ReadPatternFileL
//
// -------------------------------------------------------------------------------
//
CBufFlat* C3DAudioXMLParser::ReadPatternFileL( const TDesC& aFullName )
    {
    const TInt KGranularity = 1024;
    
    RFile file;
	User::LeaveIfError( file.Open( iFsSession, aFullName, EFileRead ) );
	CleanupClosePushL( file );

  	CBufFlat* buf = CBufFlat::NewL( KGranularity );
	CleanupStack::PushL( buf );
	
	TInt size = 0;
	file.Size( size );
	buf->ResizeL( size );
	
	TPtr8 ptr = buf->Ptr( 0 );
	User::LeaveIfError( file.Read( ptr ) );

	CleanupStack::Pop( buf );
	CleanupStack::PopAndDestroy( &file );
	
	return buf;
    }
    
    
// -------------------------------------------------------------------------------
// C3DAudioXMLParser::GetPatternFullPathL
//
// -------------------------------------------------------------------------------
//
void C3DAudioXMLParser::GetPatternFullPathL(TDes& aText, TInt aPatternId)
    {
    _LIT( KFormatString, "%S%S%S\\%S");

    TBuf<KBufSize32> pattern;

    // emulator dir  "\epoc32\release\winscw\udeb\Z\resource\3DTones"
    // hardware dir  "z:\data\Z\resource\3DTones"

    switch (aPatternId)
        {
        case EProfile3DEffectCircle:
            {
            pattern = KPatternCircle;
            break;
            }
        case EProfile3DEffectFlyby:
            {
            pattern = KPatternFlyBy;
            break;
            }
        case EProfile3DEffectZigZag:
            {
            pattern = KPatternZigZag;
            break;
            }
        case EProfile3DEffectRandomWalk:
            {
            pattern = KPatternRandomWalk;
            break;
            }
        case EProfile3DEffectChaos:
            {
            pattern = KPatternChaos;
            break;
            }
        default:
            {
            User::Leave( KErrNotSupported );
            break;
            }
        }
        
  	TDriveUnit drive( EDriveZ );
	TDriveName driveName = drive.Name();
	
	aText.Format(KFormatString, &driveName, &KDC_RESOURCE_FILES_DIR, &K3DTonesDir, &pattern);
    }
    





#ifdef _DEBUG

// -----------------------------------------------------------------------------
// C3DAudioXMLParser::AssertParseDataL
//
// -----------------------------------------------------------------------------
//
void C3DAudioXMLParser::AssertParseDataL( TInt aPatternId )
	{
	const TInt KGranularity = 1024;
	
   	C3DPatternPS* ps = C3DPatternPS::NewLC( KRingingTone3DAudioEComPluginUID );
	   	
    CBufFlat* buf = CBufFlat::NewL( KGranularity );
    CleanupStack::PushL( buf );
    
    RBufWriteStream writestream( *buf );
    CleanupClosePushL( writestream );
    iParsedPattern->ExternalizeL( writestream );
   	writestream.CommitL();
	CleanupStack::PopAndDestroy( &writestream );
	
	CBufFlat* buf2 = ps->DataLC();


    TInt id = ps->IntL(C3DPatternPS::EKeyPatternId);
    if ( id != aPatternId )
        {
        Panic( KErrGeneral );
        }
    
    TPtr8 p1 = buf->Ptr( 0 );
    TPtr8 p2 = buf2->Ptr( 0 );
    
    TInt ret = p1.Compare( p2 );
    if ( ret != 0 )
        {
        Panic( KErrGeneral );
        }
    TInt doppler = ps->IntL( C3DPatternPS::EKeyDopplerUsed );
    if ( doppler != iParsedPattern->DopplerAvailable() )
        {
        Panic( KErrGeneral );
        }
    TInt velocity = ps->IntL( C3DPatternPS::EKeyVelocity );
    if (velocity != KMotionPatternDefaultVelocity)
        {
        Panic( KErrGeneral );
        }
    
    CleanupStack::PopAndDestroy( buf2 );
    CleanupStack::PopAndDestroy( buf );
    CleanupStack::PopAndDestroy( ps );
	}


// -----------------------------------------------------------------------------
// C3DAudioXMLParser::FlogPatternsL()
//
// -----------------------------------------------------------------------------
//    
void C3DAudioXMLParser::FlogPatternsL(TInt aOldPattern, TInt aNewPattern)
    {
    TBuf<128> buf; TBuf<128> buf2;
    
    GetPatternString(buf, aOldPattern);
    GetPatternString(buf2, aNewPattern);
    FLOG( _L("pattern changed from '%S' to '%S'"), &buf, &buf2 );
    }


// -----------------------------------------------------------------------------
// C3DAudioXMLParser::FlogPatternL()
//
// -----------------------------------------------------------------------------
//    
void C3DAudioXMLParser::FlogPatternL(TInt aPattern)
    {
    TBuf<128> buf;
    
    GetPatternString(buf, aPattern);
    FLOG( _L("pattern '%S' found in cache"), &buf );
    }


// -----------------------------------------------------------------------------
// C3DAudioXMLParser::GetPatternString()
//
// -----------------------------------------------------------------------------
//    
void C3DAudioXMLParser::GetPatternString(TDes& aText, TInt aPattern)
    {
	switch (aPattern)
		{
		case EProfile3DEffectOff:
			aText = _L("EProfile3DEffectOff");
			break;
		case EProfile3DEffectCircle:
			aText = _L("EProfile3DEffectCircle");
			break;
		case EProfile3DEffectFlyby:
			aText = _L("EProfile3DEffectFlyby");
			break;
		case EProfile3DEffectZigZag:
			aText = _L("EProfile3DEffectZigZag");
			break;
		case EProfile3DEffectRandomWalk:
			aText = _L("EProfile3DEffectRandomWalk");
			break;
		case EProfile3DEffectChaos:
			aText = _L("EProfile3DEffectChaos");
			break;
		case EProfile3DEffectStereoWidening:
			aText = _L("EProfile3DEffectStereoWidening");
			break;
		case KErrNotFound:
			aText = _L("KErrNotFound");
			break;

		default:
			aText = _L("### unknown pattern ###");
			break;
		}
    }


// -----------------------------------------------------------------------------
// C3DAudioXMLParser::FlogPubSubL()
//
// -----------------------------------------------------------------------------
//    
void C3DAudioXMLParser::FlogPubSubL(TInt aEcho)
    {
    FLOG( _L("------ start pubsub data ------") );
    
    TInt num = 0;
    TBuf<128> buf;
    
    C3DPatternPS* pubSub = C3DPatternPS::NewLC( KRingingTone3DAudioEComPluginUID );
	
	num = pubSub->IntL(C3DPatternPS::EKeyPatternId);
    C3DAudioXMLParser::GetPatternString(buf, num);
    FLOG( _L("pattern: %S (%d)"), &buf, num );
    
    GetEchoString(buf, aEcho);
    FLOG( _L("echo: %S (%d)"), &buf, aEcho );
 
    num = pubSub->IntL(C3DPatternPS::EKeyVelocity);
    FLOG( _L("velocity: %d"), num );
    
    num = pubSub->IntL(C3DPatternPS::EKeyDopplerUsed);
    FLOG( _L("doppler: %d"), num );
    
    num = pubSub->IntL(C3DPatternPS::EKeyDataSize);
    FLOG( _L("data size: %d"), num );
    
    CleanupStack::PopAndDestroy( pubSub );
    
    FLOG( _L("------ end pubsub data ------") );
    }


// -----------------------------------------------------------------------------
// C3DAudioXMLParser::GetEchoString()
//
// -----------------------------------------------------------------------------
//    
void C3DAudioXMLParser::GetEchoString(TDes& aText, TInt aEcho)
    {
	switch (aEcho)
		{
		case EProfile3DEchoOff:
			aText = _L("EProfile3DEchoOff");
			break;
		case EProfile3DEchoLivingRoom:
			aText = _L("EProfile3DEchoLivingRoom");
			break;
		case EProfile3DEchoCave:
			aText = _L("EProfile3DEchoCave");
			break;
		case EProfile3DEchoStoneCorridor:
			aText = _L("EProfile3DEchoStoneCorridor");
			break;
		case EProfile3DEchoForest:
			aText = _L("EProfile3DEchoForest");
			break;
		case EProfile3DEchoSewerPipe:
			aText = _L("EProfile3DEchoSewerPipe");
			break;
		case EProfile3DEchoUnderwater:
			aText = _L("EProfile3DEchoUnderwater");
			break;
		default:
			aText = _L("### unknown echo ###");
			break;
		}
    }

#endif // _DEBUG




/******************************************************************************
 * class C3DPatternPS
 ******************************************************************************/



// -----------------------------------------------------------------------------
// C3DPatternPS::NewLC
//
// -----------------------------------------------------------------------------
//		
C3DPatternPS* C3DPatternPS::NewLC( TInt aStoreId )
	{
	C3DPatternPS* self = new ( ELeave ) C3DPatternPS( aStoreId );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;	
	}


// Destructor
C3DPatternPS::~C3DPatternPS()
	{
	iProperty.Close();
	}


// -----------------------------------------------------------------------------
// C3DPatternPS::C3DPatternPS
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//		
C3DPatternPS::C3DPatternPS( TInt aStoreId )
	{
	iStoreUid.iUid = aStoreId;
	}


// -----------------------------------------------------------------------------
// C3DPatternPS::ConstructL
//
// -----------------------------------------------------------------------------
//
void C3DPatternPS::ConstructL()
    {
    TBool ret = DefineKeyL( EKeyPatternId, RProperty::EInt );
    if ( ret )
        {
        // first time initialization
        TInt err = iProperty.Set( iStoreUid, EKeyPatternId, KErrNotFound );
        User::LeaveIfError( err );
        }
        
    DefineKeyL( EKeyDopplerUsed, RProperty::EInt );
    DefineKeyL( EKeyVelocity, RProperty::EInt );
    DefineKeyL( EKeyDataSize, RProperty::EInt );
    DefineKeyL( EKeyPatternData, RProperty::ELargeText );
    }	


// -----------------------------------------------------------------------------
// C3DPatternPS::DefineKeyL
//
// -----------------------------------------------------------------------------
//
TBool C3DPatternPS::DefineKeyL( TInt aKeyId, TInt aKeyType )
    {
    TInt err = iProperty.Define( iStoreUid, aKeyId, aKeyType );

    if ( err != KErrNone && err != KErrAlreadyExists )
        {
        User::Leave( err );    
        }
        
    if ( err == KErrNone )
        {
        return ETrue; // key was created
        }
        
    return EFalse;    // key already existed
    }



// -----------------------------------------------------------------------------
// C3DPatternPS::IntL
//
// -----------------------------------------------------------------------------
//
TInt C3DPatternPS::IntL( TInt aKey )
    {
    TInt num = KErrNotFound;
    TInt err = iProperty.Get( iStoreUid, aKey, num );
    User::LeaveIfError( err );
    
    return num;
    }


// -----------------------------------------------------------------------------
// C3DPatternPS::SetIntL
//
// -----------------------------------------------------------------------------
//
void C3DPatternPS::SetIntL( TInt aKey, TInt aValue )
    {
    TInt err = iProperty.Set( iStoreUid, aKey, aValue );
    User::LeaveIfError( err );
    }


// -----------------------------------------------------------------------------
// C3DPatternPS::SetDataL
//
// -----------------------------------------------------------------------------
//
void C3DPatternPS::SetDataL( const TDesC8& aData )
    {
    const TInt KMaxSize = 65536;
    
    if ( aData.Size() > KMaxSize )
        {
        User::Leave( KErrOverflow );
        }

    TInt err = iProperty.Set( iStoreUid, EKeyPatternData, aData );
    User::LeaveIfError( err );
    
    TInt size = aData.Size();
    err = iProperty.Set( iStoreUid, EKeyDataSize, size );
    User::LeaveIfError( err );
    }


// -----------------------------------------------------------------------------
// C3DPatternPS::GetDataL
//
// -----------------------------------------------------------------------------
//
void C3DPatternPS::GetDataL( TDes8& aData )
    {
    TInt err = iProperty.Get( iStoreUid, EKeyPatternData, aData );
    User::LeaveIfError( err );
    }


// -----------------------------------------------------------------------------
// C3DPatternPS::DataLC
//
// -----------------------------------------------------------------------------
//
CBufFlat* C3DPatternPS::DataLC()
    {
    const TInt KGranularity = 1024;

    CBufFlat* buf = CBufFlat::NewL( KGranularity );
    CleanupStack::PushL( buf );

    TInt size = IntL( C3DPatternPS::EKeyDataSize );
    buf->ResizeL( size );
    TPtr8 ptr8 = buf->Ptr( 0 );
    GetDataL( ptr8 );
    
    return buf;
    }





    
////////////////////////////////////// START TEST CODE //////////////////////////

// element names
_LIT(KKInitElement, "initialization");
_LIT(KKUpdateElement, "update");
_LIT(KKPositionElement, "position");
_LIT(KKVelocityElement, "velocity");

// attribute names
_LIT( KKXCoordinate, "x" );
_LIT( KKYCoordinate, "y" );
_LIT( KKZCoordinate, "z" );

_LIT( KKPatternName, "name" );
_LIT( KKEchoName, "reverb" );
_LIT( KKDoppler, "doppler" );
_LIT( KKDTime, "dTime" );

// attribute values
_LIT( KKBooleanTrue, "true" );




// "Z:\\resource\\10208ADF\\3zigzag.xml"


// -----------------------------------------------------------------------------
// C3DPatternParser::NewL
// 
// -----------------------------------------------------------------------------
//
C3DPatternParser* C3DPatternParser::NewL()
    {
    C3DPatternParser* self = new (ELeave) C3DPatternParser();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);

	return self;
    }


// -----------------------------------------------------------------------------
// Destructor
// 
// -----------------------------------------------------------------------------
//
C3DPatternParser::~C3DPatternParser()
    {
    iPositionList.Close();
    iVelocityList.Close();
   	delete iParser;
   	iFsSession.Close();
    }


// -----------------------------------------------------------------------------
// C3DPatternParser::C3DPatternParser
// 
// -----------------------------------------------------------------------------
//
C3DPatternParser::C3DPatternParser()
    {
    }


// -----------------------------------------------------------------------------
// C3DPatternParser::ConstructL
// 
// -----------------------------------------------------------------------------
//
void C3DPatternParser::ConstructL()
    {
    _LIT8(KMimeType, "text/xml");
    
	iParser = Xml::CParser::NewL( KMimeType, *this );
	
	User::LeaveIfError( iFsSession.Connect() );
    }


// -----------------------------------------------------------------------------
// C3DPatternParser::ReadFileL
// 
// -----------------------------------------------------------------------------
//
CBufFlat* C3DPatternParser::ReadDocumentL(const TDesC& aFullName)
    {
    const TInt KGranularity = 1024;
    
    RFile file;
	User::LeaveIfError( file.Open(iFsSession, aFullName, EFileRead) );
	CleanupClosePushL(file);

  	CBufFlat* buffer = CBufFlat::NewL(KGranularity);
	CleanupStack::PushL(buffer);
	
	TInt size = 0;
	file.Size(size);
	buffer->ResizeL(size);
	
	TPtr8 ptr = buffer->Ptr(0);
	User::LeaveIfError( file.Read(ptr) );

	CleanupStack::Pop(buffer);
	CleanupStack::PopAndDestroy(&file);
	
	return buffer;
    }


// -----------------------------------------------------------------------------
// C3DPatternParser::ParseDocumentL
// 
// -----------------------------------------------------------------------------
//
void C3DPatternParser::ParseDocumentL(const TDesC& aFullName)
    {
    iVelocityUsed = EFalse;
    iDopplerUsed = EFalse;
    
    iPositionList.Reset();
    iVelocityList.Reset();

    
    CBufFlat* buf = ReadDocumentL(aFullName);
    CleanupStack::PushL(buf);
    TPtr8 ptr = buf->Ptr(0);
    
    iParser->ParseL(ptr);
    
    iParser->ParseEndL();
    
    ///Print();
    
    CleanupStack::PopAndDestroy(buf);
    }


// -----------------------------------------------------------------------------
// C3DPatternParser::ParseDocumentL
// 
// -----------------------------------------------------------------------------
//
void C3DPatternParser::ParseDocumentL(TInt aPatternId)
    {
    TFileName name;
    GetPatternFileNameL(name, aPatternId);
    
    ParseDocumentL(name);
    }


// -----------------------------------------------------------------------------
// C3DPatternParser::GetPatternName
// 
// -----------------------------------------------------------------------------
//
void C3DPatternParser::GetPatternName(TDes& aText)
    {
    StrCopy(aText, iPatternName);
    }


// -----------------------------------------------------------------------------
// C3DPatternParser::GetEchoName
// 
// -----------------------------------------------------------------------------
//
void C3DPatternParser::GetEchoName(TDes& aText)
    {
    StrCopy(aText, iEchoName);
    }


// -----------------------------------------------------------------------------
// C3DPatternParser::GetEchoName
// 
// -----------------------------------------------------------------------------
//
TBool C3DPatternParser::DopplerEnabled()
    {
    return iDopplerUsed;
    }


// -----------------------------------------------------------------------------
// C3DPatternParser::PositionCount
// 
// -----------------------------------------------------------------------------
//
TInt C3DPatternParser::PositionCount()
    {
    return iPositionList.Count();
    }


// -----------------------------------------------------------------------------
// C3DPatternParser::Position
// 
// -----------------------------------------------------------------------------
//
T3DPatternData& C3DPatternParser::Position(TInt aIndex)
    {
    if (aIndex < 0 || aIndex >= iPositionList.Count())
        {
        Panic(KErrArgument);
        }
        
    return iPositionList[aIndex];
    }


// -----------------------------------------------------------------------------
// C3DPatternParser::Velocity
// 
// -----------------------------------------------------------------------------
//
T3DPatternData& C3DPatternParser::Velocity(TInt aIndex)
    {
    if (aIndex < 0 || aIndex >= iVelocityList.Count())
        {
        Panic(KErrArgument);
        }
        
    return iVelocityList[aIndex];
    }


// -----------------------------------------------------------------------------
// C3DPatternParser::AssertPosition
// 
// -----------------------------------------------------------------------------
//
void C3DPatternParser::AssertPosition(TInt aIndex, TInt aX, TInt aY, TInt aZ)
    {
    T3DPatternData d = Position(aIndex);
    
    if (d.iX != aX || d.iY != aY || d.iZ != aZ)
        {
        Panic(KErrArgument);
        }
    }


// -----------------------------------------------------------------------------
// C3DPatternParser::AssertVelocity
// 
// -----------------------------------------------------------------------------
//
void C3DPatternParser::AssertVelocity(TInt aIndex, TInt aX, TInt aY, TInt aZ)
    {
    T3DPatternData d = Velocity(aIndex);
    
    if (d.iX != aX || d.iY != aY || d.iZ != aZ)
        {
        Panic(KErrArgument);
        }
    }


// -----------------------------------------------------------------------------
// C3DPatternParser::AssertTime
// 
// -----------------------------------------------------------------------------
//
void C3DPatternParser::AssertTime(TInt aIndex, TInt aTime)
    {
    T3DPatternData d = Position(aIndex);
    
    if (d.iTime != aTime)
        {
        Panic(KErrArgument);
        }
    }


// -------------------------------------------------------------------------------
// C3DPatternParser::StrCopy
//
// String copy with lenght check.
// -------------------------------------------------------------------------------
//
void C3DPatternParser::StrCopy(TDes& aTarget, const TDesC8& aSource)
    {
	TInt len = aTarget.MaxLength();
    if(len < aSource.Length()) 
	    {
		aTarget.Copy(aSource.Left(len));
		return;
	    }
	aTarget.Copy(aSource);
    }


// -------------------------------------------------------------------------------
// C3DPatternParser::StrCopy
//
// String copy with lenght check.
// -------------------------------------------------------------------------------
//
void C3DPatternParser::StrCopy(TDes& aTarget, const TDesC& aSource)
    {
	TInt len = aTarget.MaxLength();
    if(len < aSource.Length()) 
	    {
		aTarget.Copy(aSource.Left(len));
		return;
	    }
	aTarget.Copy(aSource);
    }


// -------------------------------------------------------------------------------
// C3DPatternParser::StrToIntL
//
// -------------------------------------------------------------------------------
//
TInt C3DPatternParser::StrToIntL(const TDesC& aText)
	{
	TInt num = KErrNotFound;
	
    TLex lex(aText);

    User::LeaveIfError( lex.Val(num) ); 
	return num;
	}

// -----------------------------------------------------------------------------
// C3DPatternParser::Panic
//
// -----------------------------------------------------------------------------
//    
void C3DPatternParser::Panic(TInt aReason)
    {
	_LIT(KPanicCategory,"C3DPatternParser");
	
	User::Panic(KPanicCategory, aReason); 
    }


// -------------------------------------------------------------------------------
// C3DPatternParser::GetPatternFileNameL
//
// -------------------------------------------------------------------------------
//
void C3DPatternParser::GetPatternFileNameL(TDes& aText, TInt aPatternId)
    {
    _LIT( KFormatString, "%S%S%S%S");
    _LIT( KPatternDir, "3DTones\\" );

    _LIT( KPatternCircle,     "1circular.xml" );
    _LIT( KPatternFlyBy,      "2flyby.xml" );
    _LIT( KPatternZigZag,     "3zigzag.xml" );
    _LIT( KPatternRandomWalk, "4meander.xml" );
    _LIT( KPatternChaos,      "5random.xml" );

    TBuf<KBufSize32> pattern;

    switch (aPatternId)
        {
        case EProfile3DEffectCircle:
            {
            pattern = KPatternCircle;
            break;
            }
        case EProfile3DEffectFlyby:
            {
            pattern = KPatternFlyBy;
            break;
            }
        case EProfile3DEffectZigZag:
            {
            pattern = KPatternZigZag;
            break;
            }
        case EProfile3DEffectRandomWalk:
            {
            pattern = KPatternRandomWalk;
            break;
            }
        case EProfile3DEffectChaos:
            {
            pattern = KPatternChaos;
            break;
            }
        default:
            {
            User::Leave( KErrNotSupported );
            break;
            }
        }
        
  	TDriveUnit drive(EDriveZ);
	TDriveName driveName = drive.Name();
	
	aText.Format(KFormatString, &driveName, &KDC_RESOURCE_FILES_DIR, &KPatternDir, &pattern);
    }


// -------------------------------------------------------------------------------
// C3DPatternParser::ReadPositionAttributesL
//
// -------------------------------------------------------------------------------
//
void C3DPatternParser::ReadPositionAttributesL(const Xml::RAttributeArray& aAttributes)
	{
    TInt count = aAttributes.Count();
	
	for (TUint i=0; i<count; ++i)
		{
    	StrCopy(iAttributeName, aAttributes[i].Attribute().LocalName().DesC());
		StrCopy(iAttributeValue, aAttributes[i].Value().DesC());

		if (iAttributeName.CompareC(KKXCoordinate) == 0)
			{
			iCurrentPosition.iX = StrToIntL(iAttributeValue);
			}
		else if (iAttributeName.CompareC(KKYCoordinate) == 0)
			{
			iCurrentPosition.iY = StrToIntL(iAttributeValue);
			}
		else if (iAttributeName.CompareC(KKZCoordinate) == 0)
			{
			iCurrentPosition.iZ = StrToIntL(iAttributeValue);
			}
		}
	}


// -------------------------------------------------------------------------------
// C3DPatternParser::ReadVelocityAttributesL
//
// -------------------------------------------------------------------------------
//
void C3DPatternParser::ReadVelocityAttributesL(const Xml::RAttributeArray& aAttributes)
	{
    TInt count = aAttributes.Count();
	
	for (TUint i=0; i<count; ++i)
		{
		
		StrCopy(iAttributeName, aAttributes[i].Attribute().LocalName().DesC());
		StrCopy(iAttributeValue, aAttributes[i].Value().DesC());

		if (iAttributeName.CompareC(KKXCoordinate) == 0)
			{
			iCurrentVelocity.iX = StrToIntL(iAttributeValue);
			}
		else if (iAttributeName.CompareC(KKYCoordinate) == 0)
			{
			iCurrentVelocity.iY = StrToIntL(iAttributeValue);
			}
		else if (iAttributeName.CompareC(KKZCoordinate) == 0)
			{
			iCurrentVelocity.iZ = StrToIntL(iAttributeValue);
			}
		}
		
    iVelocityUsed = ETrue;
	}


// -------------------------------------------------------------------------------
// C3DPatternParser::ReadInitAttributesL
//
// -------------------------------------------------------------------------------
//
void C3DPatternParser::ReadInitAttributesL(const Xml::RAttributeArray& aAttributes)
	{
    TInt count = aAttributes.Count();
	for ( TUint i = 0; i < count; ++i )
		{
		StrCopy(iAttributeName, aAttributes[i].Attribute().LocalName().DesC());
		StrCopy(iAttributeValue, aAttributes[i].Value().DesC());
		
		if (iAttributeName.CompareC(KKPatternName) == 0)
			{
			StrCopy(iPatternName, iAttributeValue);
			}
		else if (iAttributeName.CompareC(KKEchoName) == 0)
			{
			StrCopy(iEchoName, iAttributeValue);
			}
		else if (iAttributeName.CompareC(KKDoppler) == 0)
			{
			if (iAttributeValue.CompareC(KKBooleanTrue) == 0)
				{
				iDopplerUsed = ETrue;
				}
		    else
		        {
		        iDopplerUsed = EFalse;
		        }
			}
		}
	}


// -------------------------------------------------------------------------------
// C3DPatternParser::ReadUpdateAttributesL
//
// -------------------------------------------------------------------------------
//
void C3DPatternParser::ReadUpdateAttributesL( const Xml::RAttributeArray& aAttributes )
	{
    TInt count = aAttributes.Count();
	for ( TUint i = 0; i < aAttributes.Count(); ++i )
		{
		StrCopy(iAttributeName, aAttributes[i].Attribute().LocalName().DesC());
		StrCopy(iAttributeValue, aAttributes[i].Value().DesC());

    	if (iAttributeName.CompareC(KKDTime) == 0)
			{
			iCurrentPosition.iTime = StrToIntL(iAttributeValue);
			}
		}
	}


// -------------------------------------------------------------------------------
// C3DPatternParser::Print
//
// -------------------------------------------------------------------------------
//
void C3DPatternParser::Print()
	{
	FLOG( _L("----- start pattern data -----") );
	
	TBuf<KBufSize128> buf; TBuf<KBufSize128> buf2;
	
	GetPatternName(buf);
	GetEchoName(buf2);
	FLOG( _L("pattern: '%S'  echo: '%S' doppler: %d"), &buf, &buf2, iDopplerUsed);
	
	FLOG( _L("init position and velocity:") );
	PrintCoordinates(iInitPosition, EFalse);
	PrintCoordinates(iInitVelocity, ETrue);
	
	FLOG( _L("positions and velocities:") );
	
	TInt count = PositionCount();
	for (TInt i=0; i<count; i++)
	    {
	    T3DPatternData data = Position(i);
	    PrintCoordinates(data, EFalse);
	    
	    if (iVelocityUsed)
	        {
    	    T3DPatternData data = Velocity(i);
	        PrintCoordinates(data, ETrue);
	        }
	    }
	    
	    
	FLOG( _L("----- end pattern data -----") );
	}


// -------------------------------------------------------------------------------
// C3DPatternParser::PrintCoordinates
//
// -------------------------------------------------------------------------------
//
void C3DPatternParser::PrintCoordinates(T3DPatternData& aData, TBool aVelocity)
	{
	if (aVelocity)
	    {
	    FLOG( _L("velocity: x=%5d   y=%5d   z=%5d"), aData.iX, aData.iY, aData.iZ);
	    }
    else
        {
        FLOG( _L("position: x=%5d   y=%5d   z=%5d   time=%4d"), aData.iX, aData.iY, aData.iZ, aData.iTime);
        }	
	}


// -----------------------------------------------------------------------------
// C3DPatternParser::OnStartDocumentL  (from MContentHandler)
// 
// -----------------------------------------------------------------------------
//
void C3DPatternParser::OnStartDocumentL(const Xml::RDocumentParameters& /*aDocParam*/, TInt /*aErrorCode*/)
    {
    }


// -----------------------------------------------------------------------------
// C3DPatternParser::OnEndDocumentL  (from MContentHandler)
// 
// -----------------------------------------------------------------------------
//
void C3DPatternParser::OnEndDocumentL(TInt /*aErrorCode*/)
    {
    }
    
    
// -----------------------------------------------------------------------------
// C3DPatternParser::OnStartElementL  (from MContentHandler)
// 
// -----------------------------------------------------------------------------
//
void C3DPatternParser::OnStartElementL(const Xml::RTagInfo& aElement, 
                                 const Xml::RAttributeArray& aAttributes, 
							     TInt /*aErrorCode*/)
    {
    StrCopy(iElementName, aElement.LocalName().DesC());
    
    if (iElementName.Compare(KKInitElement) == 0)
        {
        ReadInitAttributesL(aAttributes);
        }
    else if (iElementName.Compare(KKUpdateElement) == 0)
        {
        ReadUpdateAttributesL(aAttributes);
        }
    else if (iElementName.Compare(KKPositionElement) == 0)
        {
        ReadPositionAttributesL(aAttributes);
        }
    else if (iElementName.Compare(KKVelocityElement) == 0)
        {
        ReadVelocityAttributesL(aAttributes);
        }
    }
    
    
// -----------------------------------------------------------------------------
// C3DPatternParser::OnEndElementL  (from MContentHandler)
// 
// -----------------------------------------------------------------------------
//
void C3DPatternParser::OnEndElementL(const Xml::RTagInfo& aElement, TInt /*aErrorCode*/)
    {
    StrCopy(iElementName, aElement.LocalName().DesC());
    
    if (iElementName.Compare(KKUpdateElement) == 0)
        {
        iPositionList.AppendL(iCurrentPosition);
        if (iVelocityUsed)
            {
            iVelocityList.AppendL(iCurrentVelocity);
            }
        }
        
    if (iElementName.Compare(KKInitElement) == 0)
        {
        iInitPosition = iCurrentPosition;
        if (iVelocityUsed)
            {
            iInitVelocity = iCurrentVelocity;
            }
        }
    }
    
    
// -----------------------------------------------------------------------------
// C3DPatternParser::OnContentL  (from MContentHandler)
// 
// -----------------------------------------------------------------------------
//
void C3DPatternParser::OnContentL(const TDesC8& /*aBytes*/, TInt /*aErrorCode*/)
    {
    }
    
    
// -----------------------------------------------------------------------------
// C3DPatternParser::OnStartPrefixMappingL  (from MContentHandler)
// 
// -----------------------------------------------------------------------------
//
void C3DPatternParser::OnStartPrefixMappingL(const RString& /*aPrefix*/, 
                                             const RString& /*aUri*/, 
				     				         TInt /*aErrorCode*/)
    {
    }
    
    
// -----------------------------------------------------------------------------
// C3DPatternParser::OnEndPrefixMappingL  (from MContentHandler)
// 
// -----------------------------------------------------------------------------
//
void C3DPatternParser::OnEndPrefixMappingL(const RString& /*aPrefix*/, TInt /*aErrorCode*/)
    {
    }
    
    
// -----------------------------------------------------------------------------
// C3DPatternParser::OnIgnorableWhiteSpaceL  (from MContentHandler)
// 
// -----------------------------------------------------------------------------
//
void C3DPatternParser::OnIgnorableWhiteSpaceL(const TDesC8& /*aBytes*/, TInt /*aErrorCode*/)
    {
    }
    
    
// -----------------------------------------------------------------------------
// C3DPatternParser::OnSkippedEntityL  (from MContentHandler)
// 
// -----------------------------------------------------------------------------
//
void C3DPatternParser::OnSkippedEntityL(const RString& /*aName*/, TInt /*aErrorCode*/)
    {
    }
    
    
// -----------------------------------------------------------------------------
// C3DPatternParser::OnProcessingInstructionL  (from MContentHandler)
// 
// -----------------------------------------------------------------------------
//
void C3DPatternParser::OnProcessingInstructionL(const TDesC8& /*aTarget*/, 
                                          const TDesC8& /*aData*/, TInt /*aErrorCode*/)
    {
    }
    
    
// -----------------------------------------------------------------------------
// C3DPatternParser::OnError  (from MContentHandler)
// 
// -----------------------------------------------------------------------------
//
void C3DPatternParser::OnError(TInt aErrorCode)
    {
    if ( aErrorCode != KErrNone )
        {
        FLOG( _L("### C3DPatternParser::OnError (%d) ###"), aErrorCode);
        }
    }
    

// -----------------------------------------------------------------------------
// C3DPatternParser::GetExtendedInterface  (from MContentHandler)
// 
// -----------------------------------------------------------------------------
//
TAny* C3DPatternParser::GetExtendedInterface(const TInt32 /*aUid*/)
    {
    return 0;
    }
    
    
////////////////////////////////////// END TEST CODE ////////////////////////////




    

// End of File
