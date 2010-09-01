/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef GSPLUGINWRAPPER_H
#define GSPLUGINWRAPPER_H

// System includes
#include <e32base.h>

// Classes referenced
class CGSBaseDocument;


/**
 *  Utility class for separating ECOM plugin destruction from view-framework
 *  view cleanup.
 *
 *  Owned by CGSBaseDocument which has the longest lifetime during GS
 *  application run. This will ensure that ECOM resources are not deleted
 *  before view cleanup.
 *
 *  Problem/rationale:
 *  Before the functionality provided by this class, GS would occasionally
 *  crash in exit. This was caused by the dual nature of CGSPluginInterface
 *  class: It is both ECOM-plugin having individual dll and view-framework
 *  dependant CAknView owned by CCoeAppUi. When exiting GS, it was possible
 *  that a  plugin still had local views (CAknViews) which were owned
 *  by view-framework.
 *
 *  When the view framework destroys the registered views, it would cause
 *  the ECOM plugin dll to be unloaded. Should this happen before all the views
 *  owned and implemented by the plugin have been destroyed, an access
 *  violation would occur because the code needed to call the view's destructor
 *  was no longer loaded into memory within the GS process.
 *
 *  Solution:
 *  Purpose of CGSPluginWrapper is to move ECOM-related memory cleanup outside
 *  the scope of CGSPluginInterface destructor. This way view-framework can
 *  call destructor in an arbitrary order on all it's owned views even though
 *  plugins which had local views were already deleted.
 *
 */
NONSHARABLE_CLASS( CGSPluginWrapper ) : public CBase
    {
    public:
        CGSPluginWrapper( CGSBaseDocument& aDocument );
        IMPORT_C ~CGSPluginWrapper();

    public:
        IMPORT_C void SetDetails( TUid aDestructorIdKey );
        inline CGSBaseDocument& Document() { return iDocument; }

    private:
        TUid iDestructorIdKey;
        CGSBaseDocument& iDocument;
    };




#endif // GSPLUGINWRAPPER_H
// End of File
