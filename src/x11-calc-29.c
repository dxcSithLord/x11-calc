/*
 * x11-calc-29.c - RPN (Reverse Polish) calculator simulator.
 *
 * Copyright(C) 2018   MT
 *
 * Model specific functions.
 *
 * This  program is free software: you can redistribute it and/or modify it
 * under  the terms of the GNU General Public License as published  by  the
 * Free  Software Foundation, either version 3 of the License, or (at  your
 * option) any later version.
 *
 * This  program  is distributed in the hope that it will  be  useful,  but
 * WITHOUT   ANY   WARRANTY;   without even   the   implied   warranty   of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 *
 * You  should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * 09 Mar 14   0.1   - Initial version - MT
 * 10 Mar 14         - Changed indexes to BCD hex values - MT
 * 10 Dec 18         - Alternate  function key now LIGHT_BLUE, allowing  it
 *                     to be a different colour to the alternate text - MT
 * 14 Oct 21         - Added key codes to buttons (I'd forgotten them!)- MT
 * 04 Nov 21         - Allows size of the window to be changed by modifying
 *                     the value of SCALE at compile time - MT
 *
 */

#define VERSION        "0.1"
#define BUILD          "0003"
#define DATE           "21 Sep 21"
#define AUTHOR         "MT"

#define DEBUG 0        /* Enable/disable debug*/

#include <stdarg.h>    /* strlen(), etc. */
#include <stdio.h>     /* fprintf(), etc. */
#include <stdlib.h>    /* getenv(), etc. */

#include <X11/Xlib.h>  /* XOpenDisplay(), etc. */
#include <X11/Xutil.h> /* XSizeHints etc. */

#include "x11-calc-font.h"
#include "x11-calc-button.h"
#include "x11-calc-switch.h"
#include "x11-calc-colour.h"

#include "x11-calc-cpu.h"

#include "x11-calc.h"

#include "gcc-debug.h"

void v_init_keypad(obutton *h_button[], oswitch *h_switch[]) {

   /* Define the switches. */
   h_switch[0] = h_switch_create(00000, "OFF", "ON ", h_alternate_font, KEYBOARD_COL_A, KEYBOARD_ROW_0, ENTER_KEY_WIDTH, SWITCH_HEIGHT, True, MID_GREY, DARK_GREY);
   h_switch[1] = h_switch_create(00000, "PRGM", "RUN", h_alternate_font, KEYBOARD_COL_D, KEYBOARD_ROW_0, ENTER_KEY_WIDTH, SWITCH_HEIGHT, True, MID_GREY, DARK_GREY);

   /* Define top row of keys. */
   h_button[0] = h_button_create(00263, ' ', "SST", "FIX", "BST", h_normal_font, h_small_font, h_alternate_font, KEYBOARD_COL_A, KEYBOARD_ROW_1, SMALL_KEY_WIDTH, KEY_HEIGHT, False, BLACK, YELLOW, BLUE);
   h_button[1] = h_button_create(00262, 000, "GSB", "SCI", "RTN", h_normal_font, h_small_font, h_alternate_font, KEYBOARD_COL_B, KEYBOARD_ROW_1, SMALL_KEY_WIDTH, KEY_HEIGHT, False, BLACK, YELLOW, BLUE);
   h_button[2] = h_button_create(00261, 000, "GTO", "ENG", "LBL", h_normal_font, h_small_font, h_alternate_font, KEYBOARD_COL_C, KEYBOARD_ROW_1, SMALL_KEY_WIDTH, KEY_HEIGHT, False, BLACK, YELLOW, BLUE);
   h_button[3] = h_button_create(00260, 'f', "f", "", "", h_normal_font, h_small_font, h_alternate_font, KEYBOARD_COL_D, KEYBOARD_ROW_1, SMALL_KEY_WIDTH, KEY_HEIGHT, False, YELLOW, BACKGROUND, YELLOW);
   h_button[4] = h_button_create(00264, 'g', "g", "", "", h_normal_font, h_small_font, h_alternate_font, KEYBOARD_COL_E, KEYBOARD_ROW_1, SMALL_KEY_WIDTH, KEY_HEIGHT, False, LIGHT_BLUE, BACKGROUND, LIGHT_BLUE);

   /* Define second row of keys. */
   h_button[5] = h_button_create(00103, 000, "X-Y", "x", "%", h_normal_font, h_small_font, h_alternate_font, KEYBOARD_COL_A, KEYBOARD_ROW_2, SMALL_KEY_WIDTH, KEY_HEIGHT, False, BLACK, YELLOW, BLUE);
   h_button[6] = h_button_create(00102, 000, "R", "s", "i", h_normal_font, h_small_font, h_alternate_font, KEYBOARD_COL_B, KEYBOARD_ROW_2, SMALL_KEY_WIDTH, KEY_HEIGHT, False, BLACK, YELLOW, BLUE);
   h_button[7] = h_button_create(00101, 000, "STO", "", "DSZ", h_normal_font, h_small_font, h_alternate_font, KEYBOARD_COL_C, KEYBOARD_ROW_2, SMALL_KEY_WIDTH, KEY_HEIGHT, False, BLACK, YELLOW, BLUE);
   h_button[8] = h_button_create(00100, 000, "RCL", "", "ISZ", h_normal_font, h_small_font, h_alternate_font, KEYBOARD_COL_D, KEYBOARD_ROW_2, SMALL_KEY_WIDTH, KEY_HEIGHT, False, BLACK, YELLOW, BLUE);
   h_button[9] = h_button_create(00104, 000, "E+", "E-", "DEL", h_normal_font, h_small_font, h_alternate_font, KEYBOARD_COL_E, KEYBOARD_ROW_2, SMALL_KEY_WIDTH, KEY_HEIGHT, False, BLACK, YELLOW, BLUE);

   /* Define third row of keys. */
   h_button[10] = h_button_create(00323, 015, "ENTER", "PREFIX", "", h_normal_font, h_small_font, h_alternate_font, KEYBOARD_COL_A, KEYBOARD_ROW_3, ENTER_KEY_WIDTH, KEY_HEIGHT, False, BLACK, YELLOW, BLUE);
   h_button[11] = h_button_create(00321, 'c', "CHS", "PRGM", "DEG", h_normal_font, h_small_font, h_alternate_font, KEYBOARD_COL_C, KEYBOARD_ROW_3, SMALL_KEY_WIDTH, KEY_HEIGHT, False, BLACK, YELLOW, BLUE);
   h_button[12] = h_button_create(00320, 'e', "EEX", "REG", "RAD", h_normal_font, h_small_font, h_alternate_font, KEYBOARD_COL_D, KEYBOARD_ROW_3, SMALL_KEY_WIDTH, KEY_HEIGHT, False, BLACK, YELLOW, BLUE);
   h_button[13] = h_button_create(00324, 033, "CLX", "E", "GRD", h_normal_font, h_small_font, h_alternate_font, KEYBOARD_COL_E, KEYBOARD_ROW_3, SMALL_KEY_WIDTH, KEY_HEIGHT, False, BLACK, YELLOW, BLUE);

   /* Define fourth row of keys. */
   h_button[14] = h_button_create(00143, '-', "-", "X\x1a\x59", "X\x1b\x30", h_large_font, h_small_font, h_alternate_font, KEYBOARD_COL_1, KEYBOARD_ROW_4, SMALL_KEY_WIDTH, KEY_HEIGHT, False, BEIGE, YELLOW, BLUE);
   h_button[15] = h_button_create(00142, '7', "7", "LN", "eX", h_large_font, h_small_font, h_alternate_font, KEYBOARD_COL_2, KEYBOARD_ROW_4, NUM_KEY_WIDTH, KEY_HEIGHT, False, BEIGE, YELLOW, BLUE);
   h_button[16] = h_button_create(00141, '8', "8", "LOG", "10x", h_large_font, h_small_font, h_alternate_font, KEYBOARD_COL_3, KEYBOARD_ROW_4, NUM_KEY_WIDTH, KEY_HEIGHT, False, BEIGE, YELLOW, BLUE);
   h_button[17] = h_button_create(00140, '9', "9", "-R", "-P", h_large_font, h_small_font, h_alternate_font, KEYBOARD_COL_4, KEYBOARD_ROW_4, NUM_KEY_WIDTH, KEY_HEIGHT, False, BEIGE, YELLOW, BLUE);

   /* Define fifth row of keys. */
   h_button[18] = h_button_create(00243, '+', "+", "X>Y", "X>0", h_large_font, h_small_font, h_alternate_font, KEYBOARD_COL_1, KEYBOARD_ROW_5, SMALL_KEY_WIDTH, KEY_HEIGHT, False, BEIGE, YELLOW, BLUE);
   h_button[19] = h_button_create(00242, '4', "4", "SIN", "SIN-\xb9", h_large_font, h_small_font, h_alternate_font, KEYBOARD_COL_2, KEYBOARD_ROW_5, NUM_KEY_WIDTH, KEY_HEIGHT, False, BEIGE, YELLOW, BLUE);
   h_button[20] = h_button_create(00241, '5', "5", "COS", "COS-\xb9", h_large_font, h_small_font, h_alternate_font, KEYBOARD_COL_3, KEYBOARD_ROW_5, NUM_KEY_WIDTH, KEY_HEIGHT, False, BEIGE, YELLOW, BLUE);
   h_button[21] = h_button_create(00240, '6', "6", "TAN", "TAN-\xb9", h_large_font, h_small_font, h_alternate_font, KEYBOARD_COL_4, KEYBOARD_ROW_5, NUM_KEY_WIDTH, KEY_HEIGHT, False, BEIGE, YELLOW, BLUE);

   /* Define sixth row of keys. */
   h_button[22] = h_button_create(00163, '*', "\xd7", "X\x1d\x59", "X\x1d\x30", h_large_font, h_small_font, h_alternate_font, KEYBOARD_COL_1, KEYBOARD_ROW_6, SMALL_KEY_WIDTH, KEY_HEIGHT, False, BEIGE, YELLOW, BLUE);
   h_button[23] = h_button_create(00162, '1', "1", "INT", "FRAC", h_large_font, h_small_font, h_alternate_font, KEYBOARD_COL_2, KEYBOARD_ROW_6, NUM_KEY_WIDTH, KEY_HEIGHT, False, BEIGE, YELLOW, BLUE);
   h_button[24] = h_button_create(00161, '2', "2", "V\xaf", "x\xb2", h_large_font, h_small_font, h_alternate_font, KEYBOARD_COL_3, KEYBOARD_ROW_6, NUM_KEY_WIDTH, KEY_HEIGHT, False, BEIGE, YELLOW, BLUE);
   h_button[25] = h_button_create(00160, '3', "3", "yX", "ABS", h_large_font, h_small_font, h_alternate_font, KEYBOARD_COL_4, KEYBOARD_ROW_6, NUM_KEY_WIDTH, KEY_HEIGHT, False, BEIGE, YELLOW, BLUE);

   /* Define bottom row of keys. */
   h_button[26] = h_button_create(00223, '/', "\xf7", "X=Y", "X=0", h_large_font, h_small_font, h_alternate_font, KEYBOARD_COL_1, KEYBOARD_ROW_7, SMALL_KEY_WIDTH, KEY_HEIGHT, False, BEIGE, YELLOW, BLUE);
   h_button[27] = h_button_create(00222, '0', "0", "-H.MS", "-H", h_large_font, h_small_font, h_alternate_font, KEYBOARD_COL_2, KEYBOARD_ROW_7, NUM_KEY_WIDTH, KEY_HEIGHT, False, BEIGE, YELLOW, BLUE);
   h_button[28] = h_button_create(00221, '.', ".", "LASTx", "\x1c", h_large_font, h_small_font, h_alternate_font, KEYBOARD_COL_3, KEYBOARD_ROW_7, NUM_KEY_WIDTH, KEY_HEIGHT, False, BEIGE, YELLOW, BLUE);
   h_button[29] = h_button_create(00220, 000, "R/S", "PAUSE", "1/x", h_normal_font, h_small_font, h_alternate_font, KEYBOARD_COL_4, KEYBOARD_ROW_7, NUM_KEY_WIDTH, KEY_HEIGHT, False, BEIGE, YELLOW, BLUE);
}

int i_rom[ROM_SIZE * ROM_BANKS] = {
   00000, 00000, 01464, 01417, 00264, 00557, 00256, 01160,
   00070, 00232, 00520, 00520, 01152, 00053, 01020, 00664,
   00033, 00610, 00710, 01020, 00104, 00710, 01356, 00034,
   00000, 00314, 00324, 00035, 00075, 00264, 00735, 01163,
   00224, 00053, 00124, 00061, 00204, 01414, 00710, 01556,
   00052, 00075, 00167, 00214, 00075, 01104, 01515, 01414,
   00467, 01104, 01414, 01515, 00463, 01114, 00357, 00372,
   01104, 00564, 00171, 00372, 01334, 00101, 01465, 00214,
   00114, 01414, 01515, 00272, 01750, 01074, 00130, 01160,
   00332, 01750, 00172, 00332, 01465, 00420, 00264, 00735,
   00332, 00224, 00223, 00114, 01314, 00422, 00410, 00332,
   00314, 00334, 00232, 01434, 00140, 00764, 01701, 00613,
   00464, 00001, 00264, 01745, 01760, 00310, 00210, 01714,
   01724, 00147, 00114, 00314, 00334, 00163, 01334, 00165,
   01314, 00372, 00313, 01334, 00232, 00514, 00524, 00173,
   01545, 01545, 00000, 01734, 00152, 00310, 00372, 00120,
   01514, 00420, 00416, 00756, 01112, 01017, 01156, 00012,
   01656, 00516, 00556, 00556, 00516, 00716, 01074, 00510,
   00416, 00164, 00220, 01424, 00306, 01714, 01734, 00306,
   00214, 00273, 00372, 01304, 00710, 00464, 01715, 00510,
   00236, 00410, 00710, 01556, 00251, 00510, 00236, 00432,
   01263, 00031, 00364, 01241, 00474, 00230, 00372, 00310,
   00210, 01714, 01724, 00261, 00310, 00314, 00334, 00144,
   00372, 00710, 00031, 00124, 00311, 00224, 00311, 00267,
   01314, 00075, 01324, 01301, 01364, 01027, 00710, 00031,
   00210, 00232, 00410, 01464, 00107, 00224, 00321, 00134,
   00322, 00077, 01020, 01074, 00230, 01160, 01770, 00372,
   01020, 01474, 01502, 00361, 01102, 01725, 01142, 00372,
   01074, 00230, 01160, 01770, 00102, 00620, 01714, 01724,
   00175, 00054, 00345, 01474, 00642, 01627, 01474, 00102,
   01020, 00002, 01725, 00742, 01577, 01274, 00620, 01302,
   00366, 00332, 01020, 00000, 00000, 00000, 00000, 00000,
   00264, 00737, 00434, 00407, 00130, 01230, 00657, 00734,
   00404, 00634, 00575, 01715, 00604, 01474, 01230, 01073,
   00756, 00756, 01504, 00317, 01407, 01333, 01463, 00647,
   00434, 00651, 00634, 01312, 01267, 00000, 00756, 00756,
   00103, 00257, 00756, 00756, 00756, 01553, 00264, 00043,
   00530, 01430, 00713, 00264, 00003, 00613, 00013, 01557,
   00264, 00063, 00107, 00756, 00213, 00264, 00023, 01227,
   01137, 01047, 01013, 00424, 00477, 00724, 00662, 00001,
   01237, 00523, 00473, 00000, 00457, 00434, 00535, 00624,
   00533, 00430, 00547, 00424, 00114, 00603, 00434, 00537,
   00624, 01154, 00630, 00547, 00434, 00536, 00624, 00534,
   00530, 01730, 00713, 00756, 00756, 00756, 00756, 00756,
   00756, 00713, 00434, 00562, 00624, 00450, 00630, 01330,
   00713, 00430, 01230, 01074, 00434, 00562, 00624, 00561,
   00742, 00742, 00001, 01524, 00124, 00272, 00410, 00001,
   00314, 00334, 00300, 01464, 00107, 01034, 00404, 01715,
   01204, 01073, 00434, 00024, 00634, 01301, 00430, 00001,
   01357, 00424, 00620, 00730, 00001, 00704, 01504, 00717,
   00624, 00625, 00630, 01630, 00713, 00530, 01037, 00424,
   00635, 01030, 00001, 01004, 01067, 00624, 00643, 01730,
   00001, 01204, 01073, 00630, 01037, 00001, 00604, 00404,
   01073, 00634, 00655, 01224, 00660, 00430, 01630, 00657,
   00030, 00547, 00001, 00704, 00410, 01237, 00424, 00675,
   01230, 00001, 00704, 00604, 01073, 00624, 00670, 00630,
   00247, 00434, 00710, 00624, 00710, 00630, 01530, 00713,
   01130, 00001, 01204, 01353, 00724, 00724, 00434, 00727,
   00624, 00727, 01130, 00410, 00410, 01756, 00713, 00430,
   01330, 00657, 00756, 00424, 00745, 00724, 00747, 00624,
   00756, 01024, 01045, 01224, 00747, 00742, 00657, 00206,
   00410, 00606, 00410, 00206, 00510, 00713, 01034, 00747,
   01524, 00745, 01637, 00410, 00714, 01004, 01214, 01020,
   00742, 00742, 00742, 00742, 01004, 01073, 00000, 00000,
   00125, 00544, 00773, 00113, 00125, 00544, 00772, 00107,
   00125, 00544, 00771, 00103, 00125, 00544, 00770, 00742,
   00742, 00742, 00742, 00164, 00657, 00434, 01033, 00130,
   01230, 01474, 01020, 00634, 01027, 00734, 01027, 01224,
   01027, 01024, 01027, 00410, 01020, 00216, 00410, 01474,
   00202, 01552, 01306, 00216, 00716, 00216, 00164, 01073,
   00372, 00420, 00432, 01274, 00230, 01130, 00372, 01474,
   00357, 01726, 01102, 01502, 01071, 01074, 00642, 00473,
   00774, 01302, 01110, 00642, 00433, 01020, 01102, 01020,
   00642, 00457, 00642, 01102, 00620, 00407, 01102, 00174,
   00706, 00372, 00230, 00372, 00403, 00064, 00307, 00432,
   00610, 00710, 01020, 01074, 00130, 01630, 01074, 00605,
   00527, 00232, 00420, 00432, 01106, 00232, 01160, 00232,
   01360, 01106, 00627, 01020, 00314, 00324, 00114, 00432,
   01074, 00230, 01630, 01474, 01751, 00535, 00527, 00414,
   00014, 01614, 00614, 00714, 01014, 01214, 01514, 01020,
   00574, 00630, 00230, 01530, 00272, 00710, 01556, 01213,
   01074, 00230, 01133, 00752, 01412, 01226, 00412, 01474,
   00742, 01402, 01226, 00432, 01324, 01210, 00610, 00710,
   01020, 00032, 00432, 00114, 00214, 01274, 01630, 01230,
   01230, 01430, 01230, 00420, 01146, 00232, 00372, 00310,
   00210, 00574, 00430, 01156, 01257, 00210, 01146, 01273,
   00210, 01714, 01724, 01261, 00314, 00324, 01271, 01734,
   01264, 01724, 01276, 00314, 00324, 01271, 00210, 00064,
   00313, 01304, 01001, 01304, 00064, 00167, 00412, 01014,
   00464, 01533, 00314, 00324, 00114, 01364, 01313, 00000,
   00000, 00000, 00000, 00000, 00000, 00000, 00000, 00000,
   00000, 00000, 00000, 00000, 00000, 00000, 00000, 00000,
   00000, 00000, 00000, 00000, 00000, 00000, 00000, 00000,
   00000, 00000, 00000, 00000, 00000, 00000, 00000, 00000,
   00000, 00000, 00000, 00000, 00000, 00000, 00000, 00000,
   00527, 00303, 00607, 01413, 01147, 00000, 00000, 00000,
   00000, 00000, 00000, 00000, 00000, 00000, 00000, 00000,
   00000, 00000, 01413, 01142, 01142, 01142, 00717, 00000,
   00633, 00766, 00766, 00766, 01177, 01152, 01233, 01137,
   01133, 00503, 00517, 00607, 00343, 00353, 00323, 00253,
   01147, 01172, 01172, 01163, 01172, 01172, 01167, 01172,
   01172, 00717, 01142, 01362, 01462, 01142, 01174, 00730,
   00130, 01374, 01132, 00437, 01455, 01362, 01615, 00403,
   00752, 00642, 01562, 01477, 00772, 00772, 00417, 00642,
   00642, 00730, 01172, 01074, 00202, 01242, 00202, 00732,
   00220, 00000, 01172, 00717, 00772, 00577, 01407, 00463,
   01362, 01612, 00752, 01362, 01611, 00623, 01174, 00130,
   00430, 01020, 01172, 00717, 00730, 01413, 01407, 00772,
   00677, 01362, 01610, 01455, 00230, 00417, 01374, 00130,
   00530, 01137, 00453, 00563, 01417, 01413, 00067, 00742,
   01174, 00130, 00530, 00422, 01362, 01571, 00745, 00330,
   01020, 00232, 00732, 01656, 01112, 00732, 00732, 01474,
   01102, 00510, 00474, 00206, 00232, 00432, 01174, 01020,
   01152, 01152, 01152, 01374, 00535, 01074, 00030, 01230,
   01474, 01362, 01624, 01602, 00202, 00723, 01152, 01152,
   01067, 00730, 00230, 00717, 00742, 00742, 00717, 00766,
   00766, 01374, 00230, 00330, 00574, 00130, 00766, 01067,
   00432, 01172, 01474, 01230, 00531, 00430, 01402, 01661,
   00422, 00230, 01074, 00012, 00652, 00202, 01242, 00202,
   00220, 00730, 01455, 01413, 01152, 00067, 01174, 00230,
   00430, 00772, 00772, 00772, 00717, 00766, 00067, 01174,
   00230, 00330, 00717, 01174, 01730, 01730, 01020, 00000,
   00000, 00000, 00000, 00000, 00000, 00000, 00000, 00000,
   00000, 00000, 00000, 00000, 00000, 00000, 00000, 00000,
   00000, 00000, 00000, 00000, 00000, 00000, 00000, 00000,
   00000, 00000, 00000, 00000, 00000, 00000, 00000, 01347,
   01172, 01172, 01172, 01353, 01172, 01413, 01152, 01363,
   01427, 01437, 01166, 01166, 01166, 01443, 01373, 00000,
   01165, 00232, 01025, 01656, 01656, 01710, 00162, 01410,
   01122, 00213, 00272, 01512, 00031, 01074, 00130, 00030,
   01416, 00037, 00356, 00016, 01610, 00516, 01456, 00045,
   00237, 00016, 01610, 00656, 00516, 00177, 00237, 00422,
   00762, 00362, 00016, 01610, 00237, 00116, 01116, 01670,
   01055, 01322, 00177, 00620, 01105, 01362, 00071, 01322,
   00202, 00756, 01352, 00070, 01152, 01352, 00173, 00752,
   01776, 01135, 01322, 00150, 00122, 01122, 01522, 00103,
   01512, 00130, 00557, 00022, 00356, 00416, 00574, 00130,
   01156, 00516, 00416, 00330, 01616, 00463, 00516, 01756,
   01616, 00503, 00516, 00316, 01116, 00573, 01352, 00132,
   01216, 01152, 00256, 01174, 00706, 00713, 01156, 00662,
   00523, 01376, 00037, 01672, 00756, 01556, 00143, 00677,
   01512, 00141, 00667, 00662, 01116, 01516, 00153, 00420,
   00016, 01116, 00520, 01710, 01672, 01610, 01670, 00262,
   00372, 00332, 01020, 01670, 00272, 00022, 00557, 01552,
   00054, 00257, 00756, 00620, 00343, 01074, 00130, 01160,
   00332, 01650, 01020, 00422, 01274, 00620, 01116, 01067,
   01020, 00032, 00246, 00536, 00006, 00236, 01020, 00276,
   00420, 01106, 00256, 01410, 01020, 01074, 00230, 01160,
   01670, 01020, 01614, 00410, 01165, 01342, 00255, 01156,
   01342, 00254, 01604, 01267, 00004, 00510, 00774, 01020,
   00372, 00564, 00325, 01270, 00372, 01326, 01034, 01570,
   00232, 01270, 00564, 00065, 01524, 01032, 01504, 01010,
   01310, 01370, 01343, 00756, 01352, 00311, 00416, 01474,
   00230, 01156, 01020, 01415, 01516, 00317, 00656, 01116,
   01616, 01523, 01116, 01116, 00656, 00216, 01410, 01356,
   00360, 00412, 00436, 01415, 00756, 01256, 00216, 00036,
   00716, 00732, 00732, 01174, 00730, 01643, 00420, 00656,
   01410, 01636, 01633, 00536, 00716, 00232, 00230, 01772,
   00272, 01464, 00747, 01410, 00436, 00032, 01356, 00411,
   01274, 00130, 00030, 00630, 00236, 00474, 00730, 01474,
   01636, 01142, 00003, 00474, 00130, 01636, 01152, 00027,
   00736, 00474, 00420, 01106, 01020, 01366, 01034, 00664,
   00513, 00664, 01317, 00232, 01534, 00427, 01262, 00664,
   00757, 00171, 01360, 01020, 00664, 00177, 01566, 00441,
   00432, 01352, 00451, 01410, 01156, 00752, 01152, 00253,
   00756, 01020, 00774, 00552, 00277, 00432, 00317, 00406,
   01146, 00412, 01304, 01274, 01020, 01074, 00030, 01160,
   01020, 01504, 00410, 00325, 01370, 00232, 00510, 00121,
   00145, 00510, 00272, 00161, 00232, 01470, 00115, 00145,
   01570, 01210, 00115, 00145, 01210, 00232, 00272, 00161,
   00232, 01670, 00115, 00145, 01210, 00510, 00161, 00232,
   01770, 00115, 00145, 00432, 00774, 00742, 00232, 01270,
   00115, 00145, 01314, 00064, 00327, 00410, 00325, 00432,
   00774, 00742, 00232, 01270, 00131, 00372, 00332, 00410,
   01326, 01034, 01322, 01034, 01570, 00272, 00161, 01270,
   00065, 01470, 01524, 00575, 01670, 00131, 00510, 00065,
   00422, 00105, 01524, 01032, 01504, 00171, 01010, 01310,
   01370, 00727, 00432, 00774, 00742, 00232, 00115, 00145,
   01352, 00623, 00432, 01564, 00633, 01605, 00174, 01542,
   00632, 01451, 00072, 00432, 00214, 01274, 01630, 01230,
   01230, 01430, 01230, 00420, 01146, 00232, 00310, 00210,
   00574, 00430, 01156, 01253, 00210, 00132, 01102, 01561,
   01102, 01277, 00132, 00210, 00004, 00314, 00324, 00674,
   00014, 01320, 00334, 00664, 00014, 00000, 00314, 01320,
   00324, 00706, 00004, 00000, 00324, 00674, 00210, 01561,
   00064, 00313, 00432, 00420, 00474, 01172, 01630, 01730,
   01330, 01630, 01730, 01030, 00530, 00130, 00430, 00330,
   00130, 00430, 00364, 00013, 00432, 01172, 01160, 01770,
   01020, 00464, 01165, 00014, 00032, 01714, 01724, 00745,
   00004, 00174, 01734, 00751, 00120, 00716, 00206, 00552,
   00552, 01723, 00130, 01747, 01174, 00130, 00552, 01747,
   01166, 00216, 01650, 00464, 01165, 01174, 00014, 01020,
   00032, 00774, 00642, 01366, 01034, 00513, 00710, 01152,
   00123, 00574, 00630, 01474, 01142, 00123, 01074, 01342,
   01022, 01304, 00230, 01530, 00064, 00107, 01210, 01116,
   01116, 00175, 00064, 00343, 00264, 01763, 01010, 00072,
   00126, 00265, 00451, 00774, 00072, 00156, 00446, 00253,
   00756, 00642, 00226, 00272, 01020, 00616, 01174, 01062,
   00313, 01222, 00032, 00337, 01474, 00313, 00472, 01142,
   00333, 00244, 01100, 00720, 01672, 00337, 00756, 01672,
   01020, 00132, 00321, 00510, 00453, 00410, 00332, 00321,
   00510, 00556, 01522, 01076, 01020, 00272, 00177, 01366,
   01034, 01010, 00521, 00217, 00072, 00366, 01062, 00547,
   01222, 01056, 00016, 00022, 00426, 00774, 00603, 00742,
   01032, 00577, 00472, 00620, 00354, 01155, 00676, 00356,
   00416, 00657, 01136, 01472, 01152, 00732, 01154, 01140,
   00032, 00232, 00222, 00356, 01177, 00032, 00662, 01216,
   01672, 00563, 01010, 00765, 00217, 00774, 00072, 00652,
   00652, 00752, 00752, 01416, 01206, 00232, 00226, 01366,
   01212, 00232, 00366, 01416, 01222, 01732, 00656, 01332,
   01222, 01057, 01152, 01152, 00016, 01622, 01522, 01232,
   00472, 00453, 01466, 01236, 01262, 00132, 01032, 00774,
   01506, 01250, 00416, 01020, 00420, 00662, 01410, 01156,
   01502, 01243, 00706, 01223, 00434, 01257, 01262, 01175,
   00217, 01562, 01034, 00032, 00226, 01335, 01277, 00172,
   00372, 00572, 00572, 00632, 00372, 00436, 00272, 00556,
   01413, 01166, 00556, 00616, 01474, 01542, 01310, 01732,
   01772, 00216, 00432, 00132, 01274, 00530, 01772, 01533,
   00742, 01632, 01503, 00532, 00732, 00620, 01746, 01354,
   01321, 00402, 00232, 00372, 01020, 00432, 01160, 01570,
   01010, 01310, 01370, 00153, 00372, 01755, 00153, 01474,
   00202, 01552, 01360, 00216, 00716, 00216, 00164, 01073,
   00412, 01014, 00464, 01533, 00000, 00000, 00506, 01746,
   01546, 01366, 01020, 01326, 01406, 00774, 00316, 00756,
   00756, 01552, 01451, 00620, 01354, 01410, 00332, 01020,
   01156, 00017, 00432, 00326, 01034, 01433, 00720, 01154,
   01423, 00271, 00133, 00720, 00271, 00620, 00620, 00271,
   00272, 00332, 00237, 00032, 00261, 00720, 01144, 01441,
   00720, 00261, 00732, 00532, 00372, 00664, 00451, 00664,
   01277, 01312, 01406, 00053, 01746, 00606, 00246, 01746,
   00546, 00546, 01046, 01034, 01366, 00032, 00256, 00632,
   00416, 01020, 00032, 00226, 00372, 01064, 00065, 00372,
   01004, 00751, 00372, 01305, 00372, 00701, 01254, 01503,
   01274, 00730, 00222, 00372, 00503, 00172, 01162, 00563,
   01646, 00232, 00736, 00232, 01122, 00467, 00132, 00642,
   00664, 01261, 01073, 01646, 01122, 00557, 00022, 00472,
   00642, 00467, 01646, 00642, 00503, 00232, 01064, 00065,
   00372, 00432, 00232, 00664, 00265, 00426, 00363, 00762,
   01032, 00677, 00472, 00732, 01776, 00372, 00620, 01020,
   00756, 01672, 01352, 01633, 01522, 01570, 01216, 01352,
   01621, 00426, 00032, 00556, 01117, 00406, 01562, 01615,
   01172, 00412, 01304, 00424, 01616, 00422, 00064, 00343,
   01672, 01156, 01103, 00416, 01362, 01631, 00132, 01032,
   01256, 00436, 01020, 00232, 00706, 00706, 00706, 00232,
   01213, 00756, 01032, 01207, 00472, 01166, 01243, 01123,
   00232, 00716, 00232, 00732, 01034, 01642, 01552, 01605,
   01213, 00432, 00244, 01777, 01172, 00430, 00772, 00422,
   01772, 01544, 01743, 00444, 01705, 00144, 01717, 01644,
   01727, 00644, 01734, 00720, 01020, 00274, 01601, 00030,
   01030, 00530, 00330, 00130, 00730, 00674, 01020, 01674,
   01601, 00330, 00030, 01030, 00430, 01574, 01020, 01174,
   01601, 01601, 00374, 01020, 01074, 01601, 00274, 01020,
   00330, 00330, 01020, 01574, 00330, 00130, 00030, 00130,
   00730, 01130, 01030, 00030, 00430, 00330, 00474, 01020,
   00232, 01074, 00130, 01160, 01670, 01410, 01274, 00562,
   01757, 01130, 01763, 00030, 00232, 00420, 01020, 00474,
   00630, 01130, 00330, 00130, 00430, 00730, 00130, 01030,
   00030, 00530, 00630, 00774, 01020, 00432, 00774, 00230,
   00330, 00030, 00230, 00530, 01030, 00530, 00030, 01130,
   00230, 01130, 01130, 00430, 00774, 01020, 00774, 01372,
   00163, 01562, 00201, 01356, 00237, 00756, 00032, 01626,
   01356, 00255, 01646, 00162, 01274, 00620, 01122, 00267,
   00122, 00436, 00333, 01672, 00742, 00122, 00172, 00420,
   00522, 00410, 00222, 00732, 01672, 01162, 00363, 01410,
   00410, 00472, 00732, 01122, 00317, 00262, 01122, 00522,
   00463, 00132, 00732, 00533, 00544, 00111, 00762, 00620,
   00132, 00122, 00732, 00327, 01162, 00720, 00372, 00764,
   01305, 01672, 00372, 00567, 00472, 01142, 00563, 01562,
   00124, 00244, 00261, 00432, 01555, 00664, 01175, 00022,
   00734, 00153, 01262, 01224, 00734, 00624, 00325, 00664,
   00215, 00064, 00343, 01234, 00172, 01010, 01526, 00174,
   01310, 00232, 00264, 01763, 01522, 00170, 00232, 00432,
   00707, 01234, 00172, 01010, 00172, 01512, 00170, 00656,
   01116, 00736, 01526, 00233, 01516, 00226, 00222, 00262,
   00562, 00562, 00622, 01362, 00227, 00404, 00422, 00372,
   01310, 00372, 00217, 01516, 00210, 00132, 00743, 00272,
   01102, 01526, 00245, 00432, 00657, 00664, 01175, 00664,
   00521, 00522, 01122, 00416, 00253, 00704, 00664, 01175,
   01257, 01356, 00145, 01172, 00372, 00066, 00065, 00232,
   01072, 01312, 00274, 01072, 00232, 00372, 01352, 00301,
   01272, 00206, 00620, 00732, 00554, 00302, 00774, 01502,
   00322, 00726, 00232, 00222, 00664, 00335, 00664, 00451,
   00426, 00657, 00656, 00620, 01453, 00372, 00065, 00372,
   00664, 00561, 00677, 00720, 01156, 00254, 00333, 01020,
   01074, 00130, 01160, 00510, 01750, 00464, 01267, 00000,
   01210, 00464, 01211, 01366, 00462, 01362, 00362, 00704,
   01204, 00422, 00664, 00521, 00022, 00564, 00205, 01314,
   01154, 00466, 01572, 00400, 01010, 01770, 01310, 00432,
   00032, 00464, 01211, 00072, 00266, 01362, 00417, 00404,
   01534, 00417, 01234, 00417, 01214, 00704, 00414, 00774,
   01352, 00607, 00634, 00620, 01541, 00432, 00172, 00372,
   01672, 00642, 01206, 00237, 00132, 00232, 00664, 01175,
   00410, 00232, 00664, 00411, 01156, 00664, 01335, 00372,
   00132, 01770, 00232, 00664, 00531, 00022, 01552, 00620,
   00132, 01063, 00752, 00222, 01532, 00365, 00132, 01010,
   00372, 01310, 00372, 00664, 00425, 01364, 00555, 00132,
   00232, 01770, 00664, 00205, 01010, 01310, 00510, 00027,
   00172, 01332, 00526, 01102, 01532, 00612, 00132, 00634,
   00524, 01505, 00432, 00533, 01264, 00211, 00232, 00432,
   01064, 01561, 00372, 01264, 00211, 00572, 01772, 00372,
   01234, 00554, 01545, 00372, 00232, 01072, 00232, 00372,
   00432, 00664, 01175, 00022, 00734, 00560, 01545, 00472,
   00422, 00024, 00575, 00756, 00756, 00664, 00561, 00022,
   01624, 00575, 00172, 01732, 01032, 00664, 01261, 01524,
   00161, 01010, 01310, 00022, 00232, 00064, 00343, 01356,
   00510, 00132, 00624, 00172, 00664, 00725, 00022, 01505,
   00774, 00410, 00510, 00436, 00756, 01356, 00635, 00762,
   00620, 00654, 00624, 00510, 00553, 00410, 00432, 00762,
   01772, 01273, 00232, 00410, 00742, 00262, 00410, 01732,
   01732, 01122, 01237, 00022, 00472, 00232, 00172, 01632,
   01213, 00410, 00762, 00410, 00132, 00732, 00620, 00654,
   00656, 00372, 00664, 00561, 01373, 01646, 01122, 01367,
   00022, 00416, 00410, 00274, 00372, 01601, 00372, 01447,
   00472, 01142, 01443, 01672, 00402, 01366, 00530, 00720,
   01423, 01224, 00725, 01204, 01020, 01214, 01020, 01672,
   00756, 01556, 00727, 01020, 00132, 01010, 00764, 00653,
   00432, 01172, 00422, 00244, 00766, 01444, 01007, 01544,
   01017, 00444, 01025, 00144, 01031, 01474, 00730, 00274,
   01020, 00000, 00000, 00000, 00000, 00000, 00674, 00630,
   00630, 01030, 00630, 00530, 00230, 00430, 01130, 00130,
   00130, 00630, 00307, 00630, 01344, 00756, 00017, 00374,
   00015, 01474, 00530, 00174, 01030, 00474, 01020, 01374,
   00015, 01474, 01130, 00674, 01020, 00174, 00015, 01574,
   01020, 00574, 00015, 00374, 01020, 00432, 00774, 00430,
   00530, 00307, 00774, 00432, 00730, 01030, 00530, 00330,
   01130, 01030, 00130, 00630, 00330, 00330, 01130, 00730,
   00530, 00774, 01020, 01504, 00604, 01204, 01010, 00347,
   00272, 00464, 01211, 01064, 01601, 00232, 00032, 00072,
   00226, 01362, 01110, 00704, 01224, 01107, 00404, 00422,
   00372, 00024, 01216, 01634, 01121, 00232, 00272, 01772,
   01632, 00165, 00372, 01156, 01552, 01132, 01156, 00553,
   00756, 01672, 00372, 00410, 00510, 00572, 00572, 00572,
   01772, 00372, 01552, 01163, 00764, 00751, 00432, 00372,
   00510, 00572, 00732, 00034, 01157, 01672, 01772, 00372,
   01032, 00763, 00472, 00372, 00510, 00372, 00034, 01226,
   01556, 01225, 00732, 01133, 01234, 01205, 01214, 00424,
   01203, 00404, 00703, 00414, 00703, 01204, 00634, 01177,
   00734, 01214, 00714, 00703, 00704, 00703, 00211, 00557,
   00132, 01032, 01164, 01505, 01157, 00756, 01552, 01235,
   01032, 01103, 00472, 00664, 01175, 00022, 00024, 01252,
   00372, 00510, 00372, 00664, 00561, 00022, 00410, 00211,
   00664, 00405, 01156, 00410, 00510, 00756, 01473, 00732,
   01503, 00432, 00032, 00642, 00664, 01261, 01447, 00774,
   00510, 01224, 01510, 01534, 01264, 00372, 01770, 01364,
   00745, 01310, 01770, 00132, 00664, 00205, 00434, 01311,
   01262, 00064, 00343, 00620, 00644, 01267, 00756, 01457,
   00432, 00372, 01164, 01601, 00372, 00764, 00701, 00654,
   01322, 00372, 00410, 01672, 01672, 00432, 00774, 00130,
   00410, 01274, 00630, 01673, 01646, 01646, 01122, 01623,
   00022, 00410, 00232, 01072, 00472, 00410, 00172, 00262,
   01142, 01633, 00232, 00726, 00232, 01366, 01374, 01162,
   00022, 01672, 01673, 00756, 01162, 01757, 00422, 00410,
   00232, 01132, 00510, 01224, 01407, 01216, 00132, 01332,
   01473, 00664, 00561, 00022, 00634, 01264, 00232, 01650,
   00232, 00132, 00664, 00425, 00555, 00372, 00132, 01534,
   01460, 01770, 00232, 00664, 00531, 00232, 00410, 00616,
   01310, 00510, 00232, 00735, 01010, 01310, 01770, 00222,
   01670, 00232, 00410, 00664, 00405, 00664, 00215, 00547,
   00510, 00232, 01056, 00232, 01670, 00232, 00664, 00561,
   00422, 01264, 01323, 00432, 00624, 01504, 01172, 00412,
   01304, 00422, 00064, 00343, 01534, 01261, 00432, 00537,
   00624, 01517, 00132, 00664, 00725, 01264, 01323, 01534,
   01261, 00372, 00132, 01770, 00664, 00205, 00745, 01310,
   01770, 01264, 01433, 00072, 00356, 00774, 00332, 00556,
   00667, 00332, 01164, 01535, 00642, 00643, 00720, 00623,
   00664, 00451, 00072, 00664, 01337, 00332, 01156, 00713,
   00332, 00623, 00620, 01354, 01556, 00332, 00653, 00664,
   00215, 00734, 01574, 01262, 00564, 00173, 00520, 00520,
   00520, 00520, 00520, 00520, 01020, 01074, 01160, 00372,
   00070, 00376, 00316, 00752, 00752, 00752, 01410, 01252,
   00420, 01133, 00720, 00720, 00706, 00706, 01152, 01113,
   00070, 00232, 00520, 00520, 01646, 01646, 00242, 00620,
   00242, 00332, 01253, 00272, 01160, 00070, 00474, 00206,
   00520, 00520, 00232, 01474, 01360, 00232, 01142, 01217,
   00264, 01743, 00372, 00410, 00710, 01356, 01771, 01074,
   00620, 00620, 01152, 01343, 00272, 01160, 00070, 00232,
   00706, 00706, 01074, 00246, 01507, 01160, 01074, 00346,
   00070, 00206, 00771, 00771, 01160, 00306, 00232, 01360,
   00332, 01474, 01146, 01427, 00232, 01160, 01772, 01772,
   01360, 00510, 00372, 01314, 00264, 01757, 00464, 01165,
   01672, 00330, 01474, 01142, 01112, 01617, 01360, 01747,
   01434, 01755, 00764, 01701, 01677, 00464, 00001, 00264,
   01745, 00310, 00210, 00432, 00574, 00530, 01172, 01757,
   00310, 00064, 00307, 01734, 01766, 00432, 01733, 00000,
   00163, 00143, 00163, 00163, 00652, 00652, 00153, 00203,
   00207, 01237, 01217, 01123, 01167, 01177, 01103, 00064,
   00307, 00510, 00256, 00012, 00332, 00774, 01314, 00220,
   01664, 00717, 01664, 00207, 01410, 00716, 01564, 00220,
   01004, 01074, 00232, 01730, 01074, 00032, 00246, 01522,
   00057, 00732, 00732, 00246, 00732, 00732, 00233, 00710,
   01556, 00064, 00064, 00075, 01074, 00620, 00620, 01152,
   00327, 01160, 00372, 00070, 00372, 00132, 01442, 00153,
   00720, 00720, 01354, 00076, 00607, 01160, 00232, 00070,
   00232, 01532, 00114, 00607, 01074, 01442, 00153, 01174,
   01442, 00153, 01674, 01442, 00153, 00274, 01442, 00153,
   01574, 01442, 00153, 00474, 01442, 00153, 01274, 01442,
   00153, 01474, 01142, 00427, 00624, 00664, 01530, 00604,
   00427, 00720, 00403, 00620, 01002, 01502, 00151, 00752,
   00620, 00620, 00254, 00157, 00752, 00752, 00752, 01410,
   01252, 00272, 00710, 01034, 00214, 00232, 00474, 00706,
   00706, 00706, 00232, 00224, 00214, 00204, 00124, 00214,
   01714, 01724, 00210, 00432, 00216, 00610, 00064, 00267,
   01321, 00564, 00065, 01143, 01321, 01262, 00664, 00755,
   00564, 00171, 01324, 00664, 01223, 01321, 01133, 01321,
   00664, 00175, 01143, 01321, 01360, 00064, 00307, 01321,
   01134, 00253, 01310, 00132, 01074, 00130, 01160, 01770,
   01651, 01750, 00064, 00337, 01324, 00271, 00510, 01074,
   00402, 01160, 00070, 00132, 00232, 01410, 01314, 01020,
   00432, 00610, 00077, 01760, 00432, 00410, 00432, 00610,
   00110, 00420, 00464, 01165, 00272, 01701, 01626, 01526,
   00334, 01474, 00402, 01651, 01650, 00416, 01074, 00130,
   01160, 01260, 00064, 00307, 00432, 01074, 00330, 01074,
   00264, 01751, 00432, 01701, 00574, 00230, 01651, 01650,
   00264, 01763, 00232, 01074, 00230, 01160, 00232, 01020,
   00774, 00330, 00430, 01130, 00330, 00430, 00230, 01030,
   00430, 01020, 00000, 00000, 00000, 00000, 00000, 00000,
   01403, 01437, 00137, 00147, 01677, 01427, 00533, 01004,
   01204, 01113, 01113, 01113, 01113, 01113, 01113, 00664,
   01567, 00664, 00465, 00064, 00343, 00422, 00117, 01764,
   01457, 01764, 01233, 00064, 00407, 00264, 01767, 00000,
   01413, 01037, 00407, 00453, 00517, 00513, 00523, 00477,
   00473, 00443, 01463, 00667, 00703, 00623, 00577, 00167,
   01417, 01057, 00107, 00127, 01017, 01013, 01023, 00423,
   00777, 00433, 01543, 00733, 00737, 00633, 00607, 00364,
   00003, 00664, 01305, 00117, 00764, 00353, 01064, 01643,
   01264, 00317, 01010, 01310, 00232, 01204, 00604, 01004,
   01064, 00173, 01204, 00604, 01264, 00343, 00432, 01160,
   01250, 01350, 01450, 01550, 01650, 01750, 00657, 01210,
   01072, 01372, 00553, 00643, 01210, 01072, 01572, 00553,
   00064, 00075, 01314, 00064, 00307, 01210, 01504, 00713,
   01210, 00232, 01262, 00664, 00755, 00737, 01262, 01566,
   00574, 01524, 00553, 00643, 01362, 00553, 00643, 01004,
   00764, 00627, 01204, 00604, 01504, 01164, 00003, 01605,
   00406, 00216, 00117, 01605, 00016, 00664, 01175, 01156,
   00226, 00117, 00432, 00420, 01160, 00070, 00774, 00620,
   01766, 00054, 00627, 01352, 00636, 00432, 01224, 00767,
   01275, 00266, 00330, 00030, 01426, 00664, 01247, 00420,
   00756, 01410, 01126, 01237, 01304, 01464, 00220, 01356,
   00666, 01156, 01356, 00667, 00264, 01763, 01766, 01020,
   01766, 01311, 01772, 01772, 01152, 01772, 01464, 00223,
   00064, 00203, 01004, 00664, 01623, 00064, 00023, 00332,
   01310, 01114, 00064, 00313, 01521, 01074, 00130, 01160,
   01770, 00372, 01464, 01263, 01134, 00727, 01310, 01020,
   01521, 01264, 00211, 00572, 00572, 01772, 00766, 00416,
   01507, 00272, 01352, 00746, 00756, 01020, 00756, 01356,
   00756, 01156, 00726, 00620, 01526, 00747, 01020, 01434,
   00762, 01604, 00464, 01165, 00432, 01750, 01447, 01362,
   00670, 01275, 01772, 01772, 01772, 00372, 00710, 00464,
   01715, 00520, 00520, 00012, 00372, 01410, 00464, 01457,
   00064, 00407, 00064, 00337, 00216, 00272, 01474, 01230,
   01410, 01416, 01025, 00332, 01020, 00732, 00332, 00220,
   00564, 00353, 00343, 00623, 00627, 00503, 00527, 00364,
   00007, 00061, 00464, 01165, 01274, 00330, 01162, 01112,
   00233, 00716, 00716, 00212, 01650, 00372, 01564, 01507,
   01010, 01310, 00232, 00273, 00464, 01303, 00564, 00627,
   01764, 01137, 00372, 00710, 00124, 01100, 00234, 00300,
   00422, 01772, 01772, 01772, 01572, 01755, 00064, 00257,
   01364, 01643, 00303, 00733, 00707, 00613, 00143, 00677,
   00664, 00171, 00643, 00664, 00133, 01410, 00332, 00664,
   00475, 00643, 00323, 00333, 00443, 00000, 00603, 00677,
   00564, 00347, 00264, 01773, 01262, 01010, 00664, 00755,
   00064, 00343, 00517, 00743, 01504, 00757, 00353, 01364,
   01573, 00264, 01737, 00061, 01764, 00003, 01110, 00273,
   00664, 00001, 00643, 00432, 01160, 00070, 00232, 00564,
   01053, 00372, 00420, 00432, 01274, 00230, 00330, 00372,
   01474, 01063, 01726, 01102, 01502, 01212, 01074, 00642,
   01223, 00774, 01706, 00620, 01302, 01223, 01646, 00232,
   00730, 00232, 00774, 01302, 01236, 01020, 00642, 01207,
   00642, 01102, 00620, 01157, 01102, 00174, 00706, 00372,
   00230, 00372, 01107, 01474, 01502, 01304, 01102, 01441,
   01142, 00372, 01074, 00230, 01160, 01770, 00102, 00620,
   00314, 01320, 00324, 01303, 00054, 01267, 01474, 00642,
   01337, 01474, 00102, 01020, 00002, 01441, 00742, 01307,
   01274, 00620, 01302, 01311, 00332, 01020, 00574, 00310,
   00720, 00314, 01220, 00324, 01346, 00762, 01507, 00354,
   01320, 01220, 00324, 01346, 01120, 00324, 01340, 01503,
   01034, 01344, 01234, 01346, 00564, 01153, 01274, 00314,
   01730, 01020, 01410, 01074, 01252, 01152, 00252, 01342,
   01762, 01142, 01342, 01367, 00542, 01767, 00130, 00574,
   01030, 00420, 00512, 01764, 01713, 00130, 01757, 00000,
   01424, 01417, 01134, 01405, 01310, 01104, 00432, 00420,
   01172, 00422, 01710, 00016, 01710, 01404, 00113, 00611,
   00424, 01510, 01074, 00742, 00247, 00732, 00720, 01154,
   01423, 00620, 00206, 00620, 00272, 01146, 00216, 01526,
   01465, 00236, 00372, 00332, 01015, 00332, 01635, 00064,
   00317, 01142, 00754, 01457, 00704, 00416, 00157, 00724,
   01431, 01710, 00656, 01710, 00147, 00335, 00207, 00116,
   01410, 00016, 01610, 00616, 00774, 01542, 01505, 01156,
   00620, 00424, 01475, 00726, 00367, 00116, 00420, 01020,
   01474, 00202, 00272, 00574, 00706, 01174, 01646, 00336,
   00232, 00256, 01410, 01352, 01527, 00412, 01216, 00335,
   00236, 00564, 00171, 01154, 01442, 01414, 01604, 00372,
   00332, 00233, 01074, 00420, 00130, 01160, 01670, 00202,
   01710, 00202, 01274, 01362, 01601, 01162, 01562, 01562,
   00704, 01007, 01162, 01562, 01567, 00404, 00703, 01410,
   00762, 00767, 00404, 00704, 01003, 00762, 01362, 01573,
   01130, 00420, 01020, 00434, 01607, 01174, 00706, 01610,
   00420, 00734, 01617, 00662, 00434, 01617, 00662, 01074,
   00130, 01160, 00232, 01650, 00232, 00262, 01020, 01424,
   01635, 00704, 01474, 00002, 00013, 00611, 00704, 00232,
   00332, 00434, 01444, 01174, 01646, 00223, 01424, 01671,
   01134, 01653, 01310, 01104, 00432, 01404, 00420, 00774,
   00130, 00272, 01106, 00016, 00704, 00404, 01710, 01610,
   00213, 00611, 00232, 00332, 00424, 01446, 01366, 01653,
   01610, 01474, 01030, 01474, 01402, 01444, 01174, 00006,
   00404, 00704, 00223, 01434, 01732, 00611, 00232, 00332,
   01410, 00424, 01741, 01366, 01446, 01262, 00362, 00322,
   00262, 00223, 01366, 01737, 01262, 01564, 01507, 00064,
   00307, 01174, 01646, 00212, 01252, 00212, 00503, 01074,
   00230, 01160, 00332, 01750, 01020, 00610, 00124, 01106,
   00064, 00307, 01656, 01656, 01074, 00242, 00716, 00416,
   00314, 01514, 00420, 00164, 00220, 00000, 00000, 00000
};
