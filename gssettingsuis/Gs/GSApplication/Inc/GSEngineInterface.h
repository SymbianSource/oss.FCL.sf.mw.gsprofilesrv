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
* Description:  GS engine interface.
*
*/


#ifndef GS_ENGINE_INTERFACE_H
#define GS_ENGINE_INTERFACE_H

// INCLUDES
#include <e32base.h>

/**
* Interface to GS engine. Provides functions for creating, deleting, getting 
* and setting settings data. Supported data types are Tint, Treal, TDesC8 and 
* TDesC16.
*/
class MGSEngine
    {
public:

    virtual void MGSEngine() = 0;
    
    /**
    * Adds an observer. Does not take the ownership of the observer. The 
    * observer will get notification whenever the observed data is changed.
    */
    virtual void AddObserverL(MGSEngineObserver* aObserver, TUnt32 aObservedId) = 0;
    
    /**
    * Removes an observer.
    */
    virtual void RemoveObserver(MGSEngineObserver* aObserver) = 0;
    
    virtual TInt Create(TUint32 aId, TInt aVal) = 0;
	virtual TInt Create(TUint32 aId, const TReal& aVal) = 0;
	virtual TInt Create(TUint32 aId, const TDesC8& aVal) = 0;
	virtual TInt Create(TUint32 aId, const TDesC16& aVal) = 0;
	
	virtual TInt Delete(TUint32 aId) = 0;

    virtual TInt Get(TUint32 aId, TInt& aVal) = 0;
	virtual TInt Set(TUint32 aId, TInt aVal) = 0;
	
	virtual TInt Get(TUint32 aId, TReal& aVal) = 0;
	virtual TInt Set(TUint32 aId, const TReal& aVal) = 0;

	virtual TInt Get(TUint32 aId, TDes8& aVal) = 0;
	virtual TInt Set(TUint32 aId, const TDesC8& aVal) = 0;

	virtual TInt Get(TUint32 aId, TDes16& aVal) = 0;
	virtual TInt Set(TUint32 aId, const TDesC16& aVal) = 0;
    
    
    };

/**
* Observer for GS engine. 
*/
class MGSEngineObserver
    {
    virtual void ValueChanged(TUint32 aId) = 0;
    };

#endif //GS_ENGINE_INTERFACE_H

// End of File