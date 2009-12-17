/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Document class required by Symbian OS architecture.
*
*/


#ifndef GSDOCUMENT_H
#define GSDOCUMENT_H

// INCLUDES
#include <AknDoc.h>
#include "GSBaseDocument.h"

// CONSTANTS

// FORWARD DECLARATIONS
class  CEikAppUi;
class  CGSModelCdma;

// CLASS DECLARATION

/**
*  CGSDocument application class.
*  @since Series60_3.1
*/
class CGSDocument : public CGSBaseDocument
    {
    public: // Constructor and destructor
        /**
        * Two-phased constructor
        * @param aApp Application.
        * @return GS document.
        */
        static CGSDocument* NewL(CEikApplication& aApp);

        ~CGSDocument();

    public: // New

        /**
        * Returns pointer to GS's CDMA model which is owned by the document.
        * @return GS CDMA model.
        */
        CGSModelCdma* ModelCdma();

    private:

        /**
        * C++ constructor
        */
        inline CGSDocument( CEikApplication& aApp ) : CGSBaseDocument( aApp ) { }

        /**
        * Symbian OS default constructor.
        *
        */
        void ConstructL();

    private: // From CEikDocument

        /**
        * Create CGSUi "App UI" object.
        * @return Eikon application UI.
        */
        CEikAppUi* CreateAppUiL();

    private: // Data

        // CDMA model
        CGSModelCdma* iModelCdma;
    };

#endif // GSDOCUMENT_H
// End of File
