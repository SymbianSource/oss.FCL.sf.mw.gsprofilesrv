/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0""
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

#include "seccodeedit.h"
#include <QGraphicsSceneMouseEvent>

SecCodeEdit::SecCodeEdit(const QString &text, QGraphicsItem *parent /*= 0*/)
: HbLineEdit(text,parent)
{
}

SecCodeEdit::~SecCodeEdit()
{
}

void SecCodeEdit::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) {
        event->ignore();
        return;
    }

	if (rect().contains(event->pos())) {
        emit clicked();
        event->accept();
    }
	else {
        event->ignore();
    }
}
