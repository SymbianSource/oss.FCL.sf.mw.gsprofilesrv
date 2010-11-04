#
# Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
#
# Initial Contributors:
# Nokia Corporation - initial contribution.
#
# Contributors:
#
# Description:
#

do_nothing :
	@rem do_nothing

MAKMAKE : do_nothing

BLD : do_nothing

CLEAN : do_nothing
	del \epoc32\RELEASE\winscw\udeb\Z\sys\bin\MT_GSFramework.dll	/f
	del \epoc32\RELEASE\winscw\udeb\Z\sys\bin\T_GSBackgroundImage.dll /f
	del \epoc32\RELEASE\winscw\udeb\Z\sys\bin\T_GSListBox.dll /f
	del \epoc32\RELEASE\winscw\udeb\Z\sys\bin\MT_CGSLauncher.dll	/f


LIB : do_nothing

CLEANLIB : do_nothing

RESOURCE : do_nothing

FREEZE : do_nothing

SAVESPACE : do_nothing

RELEASABLES : do_nothing

FINAL :
	echo COPYING
	copy \epoc32\RELEASE\winscw\udeb\MT_GSFramework.dll \epoc32\RELEASE\winscw\udeb\Z\sys\bin /y
	copy \epoc32\RELEASE\winscw\udeb\T_GSBackgroundImage.dll \epoc32\RELEASE\winscw\udeb\Z\sys\bin /y
	copy \epoc32\RELEASE\winscw\udeb\T_GSListBox.dll \epoc32\RELEASE\winscw\udeb\Z\sys\bin /y
	copy \epoc32\RELEASE\winscw\udeb\MT_CGSLauncher.dll \epoc32\RELEASE\winscw\udeb\Z\sys\bin	/y
