/*
 * x11-calc-21.c - RPN (Reverse Polish) calculator simulator.
 *
 * Copyright(C) 2018   MT
 *
 * Model specific functions
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
 * 09 Jun 14   0.1   - Initial version - MT
 * 10 Mar 14         - Changed indexes to BCD hex values - MT
 * 10 Dec 18         - Alternate  function  key now LIGHT_BLUE, allowing  it
 *                     to be a different colour to the alternate text - MT
 * 31 Aug 20         - Made  version(), about(), and error() model specific
 *                     so that the name reflects the model number - MT
 *                   - Modified version() to display a leading zero in  the
 *                     date instead of a space - MT
 * 08 Aug 21         - Tidied up spelling errors in the comments - MT
 * 10 Aug 21         - Added HP21 ROM contents - MT
 *
 * To Do :           - Modify key colour - MT
 */

#define VERSION        "0.1"
#define BUILD          "0007"
#define DATE           "10 Aug 21"
#define AUTHOR         "MT"

#define DEBUG 1        /* Enable/disable debug*/

#include <stdarg.h>    /* strlen(), etc. */
#include <stdio.h>     /* fprintf(), etc. */
#include <stdlib.h>    /* getenv(), etc. */

#include <X11/Xlib.h>  /* XOpenDisplay(), etc. */
#include <X11/Xutil.h> /* XSizeHints etc. */

#include "x11-calc-font.h"
#include "x11-calc-button.h"
#include "x11-calc-colour.h"
#include "x11-calc-cpu.h"

#include "x11-calc.h"

#include "gcc-debug.h"

oregister o_ram[DATA_REGISTERS];

int i_rom[ROM_SIZE * ROM_BANKS] = {
   00664, 00013, 01460, 00031, 01360, 01020, 01566, 00011,
   00432, 01410, 00774, 01352, 00022, 01156, 00752, 01152,
   00117, 00756, 01020, 00552, 00137, 00432, 00157, 00406,
   01146, 00412, 01304, 01274, 01020, 00032, 00774, 00642,
   01366, 00401, 00625, 00243, 01210, 01116, 01116, 00255,
   00664, 00003, 01010, 00551, 00072, 00126, 00351, 00535,
   00774, 00072, 00156, 00446, 00337, 00756, 00642, 00226,
   00272, 01020, 00616, 01174, 01062, 00377, 01222, 00032,
   00423, 01474, 00377, 00472, 01142, 00417, 00244, 00115,
   00720, 01672, 00423, 00756, 01672, 01020, 00132, 00405,
   00510, 00537, 00410, 00332, 00405, 00510, 00556, 01522,
   00113, 01020, 00610, 00710, 01020, 00272, 00257, 01366,
   00401, 00633, 01366, 00401, 01010, 00551, 00641, 00303,
   00072, 00366, 01062, 00667, 01222, 01056, 00016, 00022,
   00426, 00774, 01472, 00170, 01156, 00732, 00743, 00742,
   01032, 00737, 00472, 00620, 00354, 00205, 00676, 00356,
   00416, 01017, 01136, 01472, 00202, 00732, 01154, 00170,
   00032, 00232, 00222, 00356, 01363, 00032, 00662, 01216,
   01672, 00703, 01010, 01143, 00232, 01534, 00230, 01262,
   01151, 00303, 00774, 00072, 00652, 00652, 00752, 00752,
   01416, 00243, 00232, 00226, 01366, 00247, 00232, 00366,
   01416, 00257, 01732, 00656, 01332, 00257, 01243, 01152,
   01152, 00016, 01622, 01522, 00267, 00472, 00537, 01466,
   00273, 01262, 00132, 01032, 00774, 01506, 00305, 00416,
   01020, 00420, 00662, 01410, 01156, 01502, 00300, 00706,
   01407, 00434, 00314, 01262, 01361, 00303, 01562, 00401,
   00551, 00032, 00226, 01525, 01463, 00172, 00372, 00572,
   00572, 00632, 00372, 00436, 00272, 00556, 01603, 01166,
   00556, 00616, 01474, 01542, 00346, 01732, 01772, 00216,
   00432, 00132, 01274, 00530, 01772, 01723, 00742, 01632,
   01673, 00532, 00732, 00620, 01746, 01354, 00357, 00402,
   00232, 00372, 01020, 00471, 00301, 00243, 00132, 01010,
   00537, 00664, 00007, 00432, 01172, 00422, 00244, 00424,
   01444, 01317, 01544, 01327, 00444, 01335, 00144, 01341,
   01474, 00730, 00274, 01020, 00674, 00630, 00630, 01030,
   00630, 00530, 00230, 00430, 01130, 00130, 00130, 00630,
   00774, 01020, 00432, 00774, 00430, 00530, 00203, 00032,
   00064, 00551, 00226, 00372, 01675, 00372, 01004, 00655,
   00372, 01211, 00372, 00605, 01254, 00461, 01274, 00730,
   00222, 00372, 00373, 00172, 01162, 00453, 01646, 00232,
   00736, 00232, 01122, 00357, 00132, 00642, 00064, 01445,
   00777, 01646, 01122, 00447, 00022, 00472, 00642, 00357,
   01646, 00642, 00373, 00232, 01675, 00372, 00432, 00232,
   00064, 00551, 00064, 00351, 00426, 00257, 00420, 00762,
   01410, 01032, 00573, 00472, 00732, 01776, 00372, 00620,
   01020, 00756, 01672, 01352, 00614, 01522, 00551, 01216,
   01352, 00602, 00426, 00032, 00556, 01023, 00406, 01562,
   00576, 01172, 00412, 01304, 00424, 00577, 00422, 00664,
   00003, 01672, 01156, 01007, 00416, 01362, 00612, 00132,
   01032, 01256, 00436, 01020, 00232, 00706, 00706, 00706,
   00232, 01117, 00756, 01032, 01113, 00472, 01166, 01147,
   01027, 00232, 00716, 00232, 00732, 01034, 00623, 01552,
   00566, 01117, 00432, 00244, 00741, 01172, 00430, 00772,
   00422, 01772, 01544, 00724, 00444, 00666, 00144, 00700,
   01644, 00710, 00644, 00715, 00720, 01020, 00274, 01505,
   00030, 01030, 00530, 00330, 00130, 00730, 00674, 01020,
   01674, 01505, 00330, 00030, 01030, 00430, 01574, 01020,
   01174, 01505, 01505, 00374, 01020, 01074, 01505, 00274,
   01020, 00330, 00330, 01020, 01574, 00330, 00130, 00030,
   00130, 00730, 01130, 01030, 00030, 00430, 00330, 00474,
   01020, 00474, 00630, 01130, 00330, 00130, 00430, 00730,
   00130, 01030, 00030, 00530, 00630, 00774, 01020, 00432,
   00774, 00230, 00330, 00030, 00230, 00530, 01030, 00530,
   00030, 01130, 00230, 01130, 01130, 00430, 00774, 01020,
   00774, 01372, 01127, 01562, 01147, 00064, 00551, 01356,
   01205, 00756, 00032, 01626, 01356, 01226, 01646, 00162,
   01274, 00620, 01122, 00107, 00122, 00436, 00153, 01672,
   00742, 00122, 00172, 00420, 00522, 00410, 00222, 00732,
   01672, 01162, 00203, 01410, 00410, 00472, 00732, 01122,
   00137, 00262, 01122, 00522, 00303, 00132, 00732, 00353,
   00544, 01055, 00762, 00620, 00132, 00122, 00732, 00147,
   01162, 00720, 00372, 00164, 01211, 01672, 00372, 00407,
   00472, 01142, 00403, 01562, 01070, 00244, 01232, 00432,
   01431, 00064, 01361, 00022, 00734, 01117, 01262, 01224,
   00376, 00624, 01277, 00064, 00301, 00664, 00003, 01234,
   00401, 01010, 01526, 01140, 01310, 00232, 00164, 00007,
   01522, 01134, 00232, 00064, 00551, 00432, 00527, 01234,
   00401, 01010, 00172, 01512, 01134, 00656, 01116, 00736,
   01526, 01201, 01516, 01174, 00222, 00262, 00562, 00562,
   00622, 01362, 01175, 00404, 00422, 00372, 01310, 00372,
   00037, 01516, 01156, 00132, 00563, 00272, 01102, 01526,
   01213, 00432, 00477, 00064, 01361, 00416, 00064, 00641,
   01556, 01223, 01162, 00222, 00416, 00073, 00704, 00064,
   01361, 01123, 01356, 01111, 01172, 00372, 00066, 00164,
   01675, 00232, 01072, 01312, 01246, 01072, 00232, 00372,
   01352, 01253, 01272, 00206, 00620, 00732, 00554, 01254,
   00774, 01502, 01274, 00726, 00232, 00222, 00064, 00421,
   00064, 00535, 00426, 00477, 00656, 00620, 01323, 00372,
   00164, 01675, 00372, 00064, 00701, 00517, 00720, 01156,
   00254, 01306, 01020, 00630, 01344, 00421, 01457, 00374,
   01455, 01474, 00530, 00174, 01030, 00474, 01020, 01374,
   01455, 01474, 01130, 00674, 01020, 00174, 01455, 01574,
   01020, 00574, 01455, 00374, 01020, 00756, 00756, 00064,
   00701, 00410, 00164, 00211, 00064, 01757, 00232, 00164,
   00211, 00756, 00756, 00064, 00641, 00410, 00364, 01671,
   01663, 01210, 00610, 00710, 01366, 01500, 01362, 01403,
   00704, 01204, 00422, 00064, 00641, 00064, 00045, 01314,
   01154, 01504, 01572, 01420, 01010, 00710, 01310, 00432,
   00032, 00072, 00266, 01362, 01435, 00404, 01534, 01435,
   01234, 01435, 01214, 00704, 00414, 00774, 01352, 01626,
   00531, 00634, 01637, 01651, 00432, 00172, 00372, 01672,
   00642, 01206, 00333, 00132, 00232, 00064, 01361, 00410,
   00232, 00064, 00475, 01156, 00064, 01525, 00372, 00132,
   00710, 00232, 00064, 00651, 01552, 01637, 00132, 01163,
   00752, 00222, 01532, 01405, 00132, 01010, 00372, 01310,
   00372, 00064, 00511, 00564, 00305, 00132, 00232, 00710,
   00064, 00271, 01010, 01310, 00510, 00117, 01524, 00132,
   01020, 00172, 01332, 01546, 01102, 01532, 01631, 00232,
   00634, 01545, 01615, 00432, 00633, 01671, 00232, 00531,
   00432, 00264, 01435, 00372, 01671, 00572, 01772, 00372,
   01234, 01574, 01655, 00372, 00232, 01072, 00232, 00372,
   00432, 00064, 01361, 00022, 00734, 01600, 01655, 00472,
   00422, 01424, 01614, 00756, 00756, 00064, 00701, 01624,
   01614, 00172, 01732, 01032, 00064, 01445, 01524, 01125,
   01010, 01310, 00022, 00232, 00664, 00003, 01356, 01531,
   00132, 00624, 00401, 00531, 00064, 01065, 01615, 00774,
   00410, 00510, 00436, 00756, 01356, 01654, 00762, 00620,
   00654, 01643, 00510, 00657, 00410, 00432, 00762, 01772,
   01367, 00232, 00410, 00742, 00262, 00410, 01732, 01732,
   01122, 01333, 00022, 00472, 00232, 00172, 01632, 01307,
   00410, 00762, 00410, 00132, 00732, 00620, 00654, 01675,
   00372, 00510, 00064, 00701, 00022, 01503, 00756, 01646,
   01556, 01716, 00410, 00416, 00274, 00372, 00164, 00015,
   00372, 01557, 00472, 01142, 01553, 01672, 00402, 01366,
   01551, 00720, 01527, 01224, 01747, 01204, 01020, 01214,
   01020, 01672, 00756, 01556, 01751, 01020, 00774, 00432,
   00730, 01030, 00530, 00330, 01130, 01030, 00130, 00630,
   00330, 00330, 01130, 00730, 00530, 00774, 01020, 01653,
   00311, 01311, 00064, 00261, 00664, 00003, 01604, 00373,
   00764, 01025, 00220, 00311, 01262, 00725, 00321, 00113,
   00311, 01311, 00064, 00575, 00023, 00311, 01156, 00430,
   00530, 00330, 00530, 01130, 00230, 00330, 00730, 01020,
   00311, 00064, 00261, 00321, 00725, 00023, 01204, 00634,
   00125, 00734, 00174, 00714, 01743, 01262, 00741, 01217,
   00125, 00113, 00610, 00710, 00232, 00432, 00774, 00130,
   01030, 00774, 01020, 01404, 01053, 01614, 01414, 01053,
   00604, 01504, 00364, 00103, 00364, 01671, 01617, 00771,
   00032, 00226, 00372, 00364, 01671, 00372, 00264, 01627,
   01204, 00403, 01234, 00046, 01214, 00424, 00172, 00404,
   01743, 00710, 00410, 00234, 00136, 01310, 00510, 00023,
   00407, 00064, 00261, 01217, 00420, 00424, 00157, 00234,
   00152, 01310, 00204, 00404, 00036, 01126, 00432, 01020,
   00604, 00272, 01417, 00125, 00013, 00330, 00230, 00756,
   00064, 01143, 00414, 01743, 00704, 01743, 00064, 00553,
   01204, 00703, 00764, 01051, 00272, 00072, 00210, 01714,
   01724, 00207, 00664, 00067, 00014, 01053, 00000, 00000,
   00707, 01656, 01656, 00232, 01160, 00070, 01144, 00132,
   00232, 00244, 00247, 01444, 00056, 01544, 00055, 00444,
   00141, 00064, 00575, 00064, 00031, 01324, 01412, 01360,
   00510, 00023, 00364, 01671, 00572, 00572, 01772, 00766,
   00416, 00553, 00230, 00530, 00430, 00756, 01020, 00720,
   01020, 00771, 00264, 01673, 00000, 00764, 01025, 01504,
   00604, 01204, 01010, 00610, 00232, 00032, 00072, 00226,
   01362, 00317, 00704, 01224, 00316, 00404, 00422, 00372,
   01424, 00104, 01634, 00330, 00232, 00272, 01772, 01632,
   00164, 00211, 00372, 01156, 01552, 00342, 01156, 01613,
   00756, 01672, 00372, 00410, 00510, 00572, 00572, 00572,
   01772, 00372, 01552, 00373, 00164, 00655, 00432, 00372,
   00510, 00572, 00732, 01434, 00367, 01672, 01772, 00372,
   01032, 00513, 00472, 00372, 00510, 00372, 01434, 00504,
   01556, 00503, 00732, 00423, 00774, 00510, 01224, 00717,
   01534, 00542, 00372, 00710, 00564, 01661, 01310, 00710,
   00132, 00064, 00271, 00434, 00426, 01262, 00664, 00003,
   00620, 00644, 00404, 00756, 00143, 00432, 00372, 00164,
   00015, 00372, 00164, 00605, 00654, 00437, 00372, 00410,
   01672, 01672, 00432, 00774, 00130, 00410, 01274, 00630,
   00677, 00072, 00356, 00774, 00332, 00556, 01603, 00332,
   00364, 01645, 00642, 00373, 00720, 00353, 00064, 00535,
   00072, 00064, 01527, 00756, 01552, 00513, 01032, 01707,
   00472, 00064, 01361, 00022, 01424, 00530, 00372, 00510,
   00372, 00064, 00701, 00410, 00364, 01671, 00064, 00471,
   01156, 00410, 00510, 00756, 00157, 00732, 00167, 00432,
   00032, 00642, 00064, 01445, 00133, 01646, 01646, 01122,
   00627, 00022, 00410, 00232, 01072, 00472, 00410, 00172,
   00262, 01142, 00637, 00232, 00726, 00232, 01366, 00575,
   01162, 00022, 01672, 00677, 00756, 01162, 00763, 00422,
   00410, 00232, 01132, 00510, 01224, 00610, 01216, 00132,
   01332, 00704, 00064, 00701, 00634, 00542, 00172, 01074,
   00442, 01147, 00732, 00676, 01153, 00662, 01672, 00172,
   00756, 00732, 00236, 00064, 00511, 00305, 01534, 00671,
   00372, 00132, 00710, 00232, 00064, 00651, 00232, 00410,
   00616, 01310, 00510, 00232, 01651, 01010, 01310, 00710,
   00216, 00016, 01672, 00410, 00064, 00471, 00064, 00301,
   01577, 00372, 00132, 00510, 00232, 01056, 00016, 01672,
   00064, 00701, 00422, 00613, 00432, 00624, 00713, 01172,
   00412, 00422, 00133, 01534, 00537, 00426, 01567, 00624,
   00725, 00132, 00064, 01065, 00613, 01534, 00537, 00372,
   00132, 00710, 00064, 00271, 01661, 01310, 00710, 00117,
   00332, 01156, 01627, 00332, 00353, 00620, 01354, 00741,
   00332, 00403, 00064, 00301, 00734, 00757, 01262, 00064,
   00033, 00132, 01032, 00364, 01615, 00447, 01674, 00232,
   01242, 00232, 00664, 01323, 00064, 01471, 00133, 00010,
   00063, 01047, 01474, 00430, 00232, 01710, 00432, 00610,
   00010, 01260, 00214, 00067, 00204, 00764, 01025, 00310,
   00064, 00031, 00410, 00510, 00165, 00024, 01222, 01552,
   01045, 01156, 01063, 00416, 01127, 00032, 01610, 00774,
   00422, 00620, 01116, 00207, 01020, 00016, 00266, 01672,
   00756, 00237, 01137, 01024, 01057, 00762, 00620, 01552,
   01066, 00756, 01352, 01067, 00574, 01113, 00756, 01676,
   01207, 00520, 00456, 01522, 00157, 01116, 00736, 00357,
   01124, 01262, 00464, 00621, 01174, 00642, 00453, 00706,
   00720, 00427, 01102, 00744, 01135, 01144, 01122, 00324,
   01122, 00662, 00016, 00172, 00620, 01326, 01132, 01410,
   00345, 01676, 00262, 00232, 00132, 00420, 00620, 00006,
   01106, 00410, 00510, 00072, 00372, 00222, 00262, 00774,
   01322, 01155, 00430, 01274, 00620, 01162, 00663, 00742,
   00731, 00731, 00731, 00742, 00742, 00677, 00254, 00267,
   01516, 01174, 01674, 00630, 00372, 00016, 01410, 00210,
   01714, 01724, 01200, 01734, 01203, 00754, 01445, 00510,
   00067, 01474, 00764, 00053, 00344, 01217, 00620, 00762,
   00147, 00165, 01004, 00510, 00422, 00032, 00266, 00072,
   00146, 00472, 00006, 01532, 01237, 01566, 01222, 01522,
   01053, 00262, 01352, 01246, 01216, 01152, 00420, 01106,
   01034, 01142, 00256, 01674, 00706, 00706, 00706, 00613,
   00056, 01407, 00174, 00706, 00172, 01674, 01646, 01646,
   01646, 00116, 01074, 01312, 01301, 01006, 01303, 00146,
   00016, 00345, 00216, 00056, 00132, 00064, 00031, 01144,
   01014, 00607, 01460, 00010, 00772, 00410, 00432, 00410,
   00610, 00610, 01110, 01110, 01110, 01110, 00372, 00132,
   01710, 01710, 01132, 01532, 01402, 00774, 00742, 01160,
   01360, 00720, 00756, 00354, 01337, 01156, 01160, 00232,
   00070, 00232, 01632, 01532, 01402, 01156, 01633, 00774,
   00064, 00011, 01775, 01451, 01775, 01174, 00230, 00754,
   01366, 00272, 00572, 00572, 00232, 01260, 00763, 00742,
   00534, 01044, 00416, 00410, 01674, 00053, 01051, 00604,
   01717, 00774, 00432, 00420, 01172, 00072, 00720, 00752,
   00554, 01416, 00374, 01630, 01230, 01230, 01430, 01230,
   00232, 00732, 00732, 00732, 01717, 01124, 01141, 01366,
   01665, 01674, 01306, 01673, 01633, 00310, 00020, 01025,
   01004, 00164, 00517, 00624, 01447, 01025, 00164, 00237,
   00663, 00257, 01747, 01007, 00634, 00774, 01051, 00353,
   01051, 00474, 01204, 01717, 00624, 00777, 00432, 00753,
   01143, 01417, 00507, 00624, 00271, 00724, 01755, 00064,
   00611, 01273, 00624, 00131, 01051, 01274, 00704, 01717,
   00652, 00624, 00010, 00724, 00221, 01234, 01100, 01656,
   01656, 01710, 00004, 01444, 01015, 00464, 01063, 00652,
   00652, 00652, 01467, 00624, 01712, 00734, 01551, 00244,
   01762, 00610, 00710, 01263, 00624, 01574, 01025, 00773,
   00363, 01303, 01563, 00624, 00202, 00734, 01571, 00244,
   01313, 01310, 00664, 00053, 01025, 00604, 01004, 00264,
   00003, 00624, 01470, 00064, 00167, 00314, 00414, 01014,
   01114, 01514, 00614, 00714, 01214, 01314, 00774, 01020,
   00033, 00453, 01647, 01727, 00624, 00075, 01161, 01273,
   00624, 00163, 00064, 00223, 01010, 01310, 00232, 01020,
   00652, 00652, 00577, 00624, 00275, 00734, 01651, 00244,
   01761, 00610, 00710, 01262, 00064, 01111, 00664, 00003,
   00624, 01011, 01051, 00424, 01435, 00464, 00621, 00072,
   00130, 00272, 01106, 01104, 01674, 00106, 01526, 01141,
   00106, 01114, 01717, 00624, 00013, 01051, 00304, 00434,
   01523, 01717, 01025, 00264, 01747, 00724, 01411, 00652,
   00652, 00652, 00503, 00624, 00107, 00734, 01731, 00244,
   01760, 00064, 00251, 01273, 00624, 01006, 01051, 01124,
   00766, 01366, 01763, 01262, 00434, 01000, 01116, 00664,
   00607, 00624, 00006, 01051, 00473, 00254, 01507, 00620,
   00620, 00620, 00620, 00664, 00767, 00624, 00073, 01110,
   01273, 00624, 00252, 01025, 01204, 01161, 00767, 00547
};

void v_init_keypad(obutton* h_button[]){

   /* Define top row of keys. */ 
   h_button[0] = h_button_create(00064, "/\xaf", "FIX", "", h_normal_font, h_small_font, h_alternate_font, 12, 85, 33, 30, False, BLACK);
   h_button[1] = h_button_create(00063, "1/x", "SCI", "", h_normal_font, h_small_font, h_alternate_font, 48, 85, 33, 30, False, BLACK);
   h_button[2] = h_button_create(00062, "yX", "\x1c", "", h_normal_font, h_small_font, h_alternate_font, 84, 85, 33, 30, False, BLACK);
   h_button[3] = h_button_create(00061, "eX", "10x", "", h_normal_font, h_small_font, h_alternate_font, 120, 85, 33, 30, False, BLACK);
   h_button[4] = h_button_create(00060, "LN", "LOG", "", h_normal_font, h_small_font, h_alternate_font, 156, 85, 33, 30, False, BLACK);
   
   /* Define second row of keys. */ 
   h_button[5] = h_button_create(00224, "X-Y", "DEG", "", h_normal_font, h_small_font, h_alternate_font, 12, 128, 33, 30, False, BLACK);
   h_button[6] = h_button_create(00223, "R", "RAD", "", h_normal_font, h_small_font, h_alternate_font, 48, 128, 33, 30, False, BLACK);
   h_button[7] = h_button_create(00222, "STO", "GRD", "", h_normal_font, h_small_font, h_alternate_font, 84, 128, 33, 30, False, BLACK);
   h_button[8] = h_button_create(00221, "RCL", "LASTx", "", h_normal_font, h_small_font, h_alternate_font, 120, 128, 33, 30, False, BLACK);
   h_button[9] = h_button_create(00220, "f", "", "", h_normal_font, h_small_font, h_alternate_font, 156, 128, 33, 30, False, YELLOW); /* Shift key. */

   /* Define third row of keys. */
   h_button[10] = h_button_create(00163, "ENTER", "MANT PREFIX", "", h_normal_font, h_small_font, h_alternate_font, 12, 171, 69, 30, False, BLACK);
   h_button[11] = h_button_create(00162, "CHS", "ALL", "", h_normal_font, h_small_font, h_alternate_font, 84, 171, 33, 30, False, BLACK);
   h_button[12] = h_button_create(00161, "EEX", "REG", "", h_normal_font, h_small_font, h_alternate_font, 120, 171, 33, 30, False, BLACK);
   h_button[13] = h_button_create(00160, "CLX", "STK", "", h_normal_font, h_small_font, h_alternate_font, 156, 171, 33, 30, False, BLACK);

   /* Define fourth row of keys. */
   h_button[14] = h_button_create(00243, "-", "-R", "", h_large_font, h_small_font, h_alternate_font, 12, 214, 33, 30, False, LIGHT_GRAY);
   h_button[15] = h_button_create(00242, "7", "SIN", "", h_large_font, h_small_font, h_alternate_font, 52, 214, 41, 30, False, LIGHT_GRAY);
   h_button[16] = h_button_create(00241, "8", "COS", "", h_large_font, h_small_font, h_alternate_font, 100, 214, 41, 30, False, LIGHT_GRAY);
   h_button[17] = h_button_create(00240, "9", "TAN", "", h_large_font, h_small_font, h_alternate_font, 148, 214, 41, 30, False, LIGHT_GRAY);

   /* Define fifth row of keys. */
   h_button[18] = h_button_create(00143, "+", "-P", "", h_large_font, h_small_font, h_alternate_font, 12, 257, 33, 30, False, LIGHT_GRAY);
   h_button[19] = h_button_create(00142, "4", "SIN-\xb9", "", h_large_font, h_small_font, h_alternate_font, 52, 257, 41, 30, False, LIGHT_GRAY);
   h_button[20] = h_button_create(00141, "5", "COS-\xb9", "", h_large_font, h_small_font, h_alternate_font, 100, 257, 41, 30, False, LIGHT_GRAY);
   h_button[21] = h_button_create(00140, "6", "TAN-\xb9", "", h_large_font, h_small_font, h_alternate_font, 148, 257, 41, 30, False, LIGHT_GRAY);

   /* Define sixth row of keys. */
   h_button[22] = h_button_create(00323, "\xd7", "-DEG", "", h_large_font, h_small_font, h_alternate_font, 12, 300, 33, 30, False, LIGHT_GRAY);
   h_button[23] = h_button_create(00322, "1", "-in", "", h_large_font, h_small_font, h_alternate_font, 52, 300, 41, 30, False, LIGHT_GRAY);
   h_button[24] = h_button_create(00321, "2", "-\x07\x46", "", h_large_font, h_small_font, h_alternate_font, 100, 300, 41, 30, False, LIGHT_GRAY);
   h_button[25] = h_button_create(00320, "3", "-lbm", "", h_large_font, h_small_font, h_alternate_font, 148, 300, 41, 30, False, LIGHT_GRAY);

   /* Define bottom row of keys. */
   h_button[26] = h_button_create(00103, "\xf7", "-RAD", "", h_large_font, h_small_font, h_alternate_font, 12, 343, 33, 30, False, LIGHT_GRAY);
   h_button[27] = h_button_create(00102, "0", "-mm", "", h_large_font, h_small_font, h_alternate_font, 52, 343, 41, 30, False, LIGHT_GRAY);
   h_button[28] = h_button_create(00101, ".", "-\x07\x43", "", h_large_font, h_small_font, h_alternate_font, 100, 343, 41, 30, False, LIGHT_GRAY);
   h_button[29] = h_button_create(00100, "%", "-kg", "", h_normal_font, h_small_font, h_alternate_font, 148, 343, 41, 30, False, LIGHT_GRAY);

}
