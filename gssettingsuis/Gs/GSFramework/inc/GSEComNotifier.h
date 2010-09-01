/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  GS utilities.
*
*/


#ifndef GSECOMNOTIFIER_H
#define GSECOMNOTIFIER_H

#include <e32std.h>
#include <ecom/ecom.h>



/**
 *  Utility class to capsulate active object observing ECOM events.
 *  MGSEComObserver is provided as easy callback when GS related ECOM events
 *  occur.
 *
 *  @lib GSFramework.lib
 *  @since S60 5.0
 */
class MGSEComObserver
    {
public:

    /**
     * Not really used yet. To provide more extendability.
     */
    enum TEComEvent
        {
        EPluginRemoved,
        EPluginAdded
        };

public:
    virtual void HandleEComEvent( TEComEvent aEvent ) = 0;
    };


/**
 *  Utility class capsulating active object for observing ECOM events.
 *
 *  @lib GSFramework.lib
 *  @since S60 5.0
 */
NONSHARABLE_CLASS( CGSEComNotifier ) : public CActive
    {
public:

    /**
     * Two-phased constructor.
     *
     * @param aObserver Callback object.
     * @param aImplInfo Initial list of ECOM interfaces against which new list
     *        is compared when ECOM events occur.
     */
    IMPORT_C static CGSEComNotifier* NewL( MGSEComObserver* aObserver,
                                           RImplInfoPtrArray aImplInfo );

    /**
     * Destructor.
     */
    IMPORT_C virtual ~CGSEComNotifier();

protected: // from CActive

    /**
     * See base class.
     */
    void DoCancel();

    /**
     * See base class.
     */
    void RunL();

    /**
     * See base class.
     */
    TInt RunError( TInt aError );

protected:

private:

    /**
     * C++ Constructor.
     *
     * @param aObserver Callback object.
     * @param aImplInfo Initial list of ECOM interfaces against which new list
     *        is compared when ECOM events occur.
     */
    CGSEComNotifier( MGSEComObserver* aObserver,
                     RImplInfoPtrArray aImplInfo );

    /**
     * 2nd phase constructor.
     */
    void ConstructL();

    /**
     * Issues new request.
     */
    void IssueRequest();

    /**
     * @return Difference between old and new list of GS-plugins. Negative
     *         number means plugins are removed, positive means plugins are
     *         added, 0 means no change in plugin count.
     */
    TInt PluginCountDeltaL();

private: // data

    // Callback.
    MGSEComObserver* iObserver;

    // ECom handle.
    REComSession iEComSession;

    // Stored original plugin info. This will be compared against latest info
    // for any changes.
    RImplInfoPtrArray iOriginalImplInfo;

    };


#endif // GSECOMNOTIFIER_H
