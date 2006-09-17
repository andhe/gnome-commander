/*
    LibGViewer - GTK+ File Viewer library 
    Copyright (C) 2006 Assaf Gordon
    
    Part of
        GNOME Commander - A GNOME based file manager
        Copyright (C) 2001-2006 Marcus Bjurman

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
*/

#include <config.h>

/*
 note: this file is "#include-d" in "inputmodes.c".
 It is not compiled by itself.
*/

/* see "http://en.wikipedia.org/wiki/CP437" */
static unsigned int ascii_cp437_to_unicode[256] = {
0x2E, /* NULL will be shown as a dot */
0x263A,
0x263B,
0x2665,
0x2666,
0x2663,
0x2660,
0x2022,
0x25D8,
0x25CB,
0x25D9,
0x2642,
0x2640,
0x266A,
0x266B,
0x263C,
0x25BA,
0x25C4,
0x2195,
0x203C,
0xB6,
0xA7,
0x25AC,
0x21A8,
0x2191,
0x2193,
0x2192,
0x2190,
0x221F,
0x2194,
0x25B2,
0x25BC,
0x20,
0x21,
0x22,
0x23,
0x24,
0x25,
0x26,
0x27,
0x28,
0x29,
0x2A,
0x2B,
0x2C,
0x2D,
0x2E,
0x2F,
0x30,
0x31,
0x32,
0x33,
0x34,
0x35,
0x36,
0x37,
0x38,
0x39,
0x3A,
0x3B,
0x3C,
0x3D,
0x3E,
0x3F,
0x40,
0x41,
0x42,
0x43,
0x44,
0x45,
0x46,
0x47,
0x48,
0x49,
0x4A,
0x4B,
0x4C,
0x4D,
0x4E,
0x4F,
0x50,
0x51,
0x52,
0x53,
0x54,
0x55,
0x56,
0x57,
0x58,
0x59,
0x5A,
0x5B,
0x5C,
0x5D,
0x5E,
0x5F,
0x60,
0x61,
0x62,
0x63,
0x64,
0x65,
0x66,
0x67,
0x68,
0x69,
0x6A,
0x6B,
0x6C,
0x6D,
0x6E,
0x6F,
0x70,
0x71,
0x72,
0x73,
0x74,
0x75,
0x76,
0x77,
0x78,
0x79,
0x7A,
0x7B,
0x7C,
0x7D,
0x7E,
0x2302,
0xC7,
0xFC,
0xE9,
0xE2,
0xE4,
0xE0,
0xE5,
0xE7,
0xEA,
0xEB,
0xE8,
0xEF,
0xEE,
0xEC,
0xC4,
0xC5,
0xC9,
0xE6,
0xC6,
0xF4,
0xF6,
0xF2,
0xFB,
0xF9,
0xFF,
0xD6,
0xDC,
0xA2,
0xA3,
0xA5,
0x20A7,
0x192,
0xE1,
0xED,
0xF3,
0xFA,
0xF1,
0xD1,
0xAA,
0xBA,
0xBF,
0x2310,
0xAC,
0xBD,
0xBC,
0xA1,
0xAB,
0xBB,
0x2591,
0x2592,
0x2593,
0x2502,
0x2524,
0x2561,
0x2562,
0x2556,
0x2555,
0x2563,
0x2551,
0x2557,
0x255D,
0x255C,
0x255B,
0x2510,
0x2514,
0x2534,
0x252C,
0x251C,
0x2500,
0x253C,
0x255E,
0x255F,
0x255A,
0x2554,
0x2569,
0x2566,
0x2560,
0x2550,
0x256C,
0x2567,
0x2568,
0x2564,
0x2565,
0x2559,
0x2558,
0x2552,
0x2553,
0x256B,
0x256A,
0x2518,
0x250C,
0x2588,
0x2584,
0x258C,
0x2590,
0x2580,
0x3B1,
0xDF,
0x393,
0x3C0,
0x3A3,
0x3C3,
0xB5,
0x3C4,
0x3A6,
0x398,
0x3A9,
0x3B4,
0x221E,
0x3C6,
0x3B5,
0x2229,
0x2261,
0xB1,
0x2265,
0x2264,
0x2320,
0x2321,
0xF7,
0x2248,
0xB0,
0x2219,
0xB7,
0x221A,
0x207F,
0xB2,
0x25A0,
0xA0
};

static int unicode2utf8(unsigned int unicode, unsigned char*out)
{
	int bytes_needed = 0 ;
	if (unicode<0x80) {
		bytes_needed = 1 ;
		out[0] = (unsigned char)(unicode&0xFF) ;
	}
	else
	if (unicode<0x0800) {
		bytes_needed = 2 ;
		out[0] = (unsigned char)(unicode>>6 | 0xC0) ;
		out[1] = (unsigned char)((unicode&0x3F)| 0x80) ;
	}
	else
	if (unicode<0x10000) {
		bytes_needed = 3 ;
		out[0] = (unsigned char)((unicode>>12) | 0xE0) ;
		out[1] = (unsigned char)(((unicode>>6) & 0x3F) | 0x80) ;
		out[2] = (unsigned char)((unicode & 0x3F) | 0x80) ;
	}
	else {
		bytes_needed = 4 ;
		out[0] = (unsigned char)((unicode>>18) | 0xE0) ;
		out[1] = (unsigned char)(((unicode>>12) & 0x3F) | 0x80) ;
		out[2] = (unsigned char)(((unicode>>6) & 0x3F) | 0x80) ;
		out[3] = (unsigned char)((unicode & 0x3F) | 0x80) ;
	}

	return bytes_needed ;
}
