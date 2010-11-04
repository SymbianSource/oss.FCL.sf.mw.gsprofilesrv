#
# Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
# Description:   Icons makefiles for project MediaFileList
#

ifeq (WINS,$(findstring WINS, $(PLATFORM)))
ZDIR=/epoc32/release/$(PLATFORM)/$(CFG)/Z
else
ZDIR=/epoc32/data/z
endif

# ----------------------------------------------------------------------------
# 
# ----------------------------------------------------------------------------

TARGETDIR=$(ZDIR)/resource/apps
HEADERDIR=/epoc32/include
ICONTARGETFILENAME=$(TARGETDIR)/mediafilelist.mif
HEADERFILENAME=$(HEADERDIR)/mediafilelist.mbg

do_nothing :
	@rem do_nothing

MAKMAKE : do_nothing

BLD : do_nothing

CLEAN : do_nothing

LIB : do_nothing

CLEANLIB : do_nothing

# ----------------------------------------------------------------------------
#
# NOTE 1: DO NOT DEFINE MASK FILE NAMES! They are included automatically by
# MifConv if the mask detph is defined.
#
# NOTE 2: Usually, source paths should not be included in the bitmap
# definitions. MifConv searches for the icons in all icon directories in a
# predefined order, which is currently \s60\icons, \s60\bitmaps2, \s60\bitmaps.
# The directory \s60\icons is included in the search only if the feature flag
# __SCALABLE_ICONS is defined.
# ----------------------------------------------------------------------------

RESOURCE :
	mifconv $(ICONTARGETFILENAME) /h$(HEADERFILENAME) \
	/c8,8 qgn_indi_ai_nt_music.svg \
	/c8,8 qgn_prop_mup_audio.svg \
	/c8,8 qgn_prop_fmgr_file_video.svg \
	/c8,8 qgn_prop_fmgr_file_voicerec.svg \
	/c8,8 qgn_prop_mup_artist.svg \
	/c8,8 qgn_prop_mup_album.svg \
	/c8,8 qgn_prop_mup_genre.svg \
	/c8,8 qgn_prop_mup_composer.svg \
	/c8,8 qgn_prop_link_embd_small.svg \
	/c8,8 qgn_prop_folder_small.svg \
	/c8,8 qgn_prop_sml_sync_off.svg \
	/c8,8 qgn_indi_fmgr_ms_add.svg \
	/c8,8 qgn_indi_mmc_add.svg \
	/c8,8 qgn_prop_mce_unknown_read \


FREEZE : do_nothing

SAVESPACE : do_nothing

RELEASABLES :
	@echo $(HEADERFILENAME)&& \
	@echo $(ICONTARGETFILENAME)

FINAL : do_nothing
