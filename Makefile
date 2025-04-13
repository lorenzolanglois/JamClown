# SPDX-License-Identifier: CC0-1.0
#
# SPDX-FileContributor: Antonio Niño Díaz, 2024

BLOCKSDS	?= /opt/blocksds/core

# User config

NAME		:= jamclown
GAME_TITLE	:= JamClown
GAME_SUBTITLE	:=
GAME_AUTHOR	:= Lorenzo Langlois
GAME_ICON	:= icon.bmp

# Source code paths

BINDIRS		:= data
GFXDIRS		:= graphics
INCLUDEDIRS	:= includes

include $(BLOCKSDS)/sys/default_makefiles/rom_arm9/Makefile
