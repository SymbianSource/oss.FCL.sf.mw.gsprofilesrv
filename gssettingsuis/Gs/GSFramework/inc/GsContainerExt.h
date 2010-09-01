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
* Description:   Container for GSParentPlugin.
*
*/

#ifndef GSCONTAINEREXT_H
#define GSCONTAINEREXT_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

// CLASS DECLARATION
class MGSWatchDog;

/*
 * Double click preventer.
 */
class CGsDoubleClickPreventer : public CTimer
    {
public:
    IMPORT_C static CGsDoubleClickPreventer* NewL();
    
    CGsDoubleClickPreventer();
    IMPORT_C void Start();
private:
    void ConstructL();
protected: 
    void RunL() {};    
    };

/**
 *  Extension for container.
 * 
 */
class CGsContainerExt : public CBase
    {
public:
    IMPORT_C static CGsContainerExt* NewL();
    ~CGsContainerExt() 
        {
        iDblClickPreventer->Cancel(); 
        delete iDblClickPreventer;
        }
private:    
    void ConstructL();
public:    
    MGSWatchDog* iGSWatchDog;
    CGsDoubleClickPreventer* iDblClickPreventer;
    
    };

#endif // GSCONTAINEREXT_H
