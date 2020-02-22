/**
 *******************************************************************************
 * @file       symbol.c
 * @version    V0.7.0
 * @date       2020.02.22
 * @brief      Symbol font table file.
 *******************************************************************************
 */ 
#include "cogui.h"

const co_uint16_t tm_symbol16x16[] = {
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, // Symbol = [ ], mirror to Ascii=[ ]
0x0000,0xC006,0x600C,0x3018,0x1830,0x0C60,0x06C0,0x0380,0x0380,0x06C0,0x0C60,0x1830,0x3018,0x600C,0xC006,0x0000, // Symbol = [X], mirror to Ascii=[!]
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xFFFF,0xFFFF,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, // Symbol = [-], mirror to Ascii=["]
};

cogui_font_t tm_symbol_16x16 = {
    "tm",
	16,
	16,
	tm_symbol16x16
};