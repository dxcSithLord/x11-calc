/*
 * x11-calc-21.h - RPN (Reverse Polish) calculator simulator.
 *
 * Copyright(C) 2018    MT
 *
 * Model specific constants and function prototypes.
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
 * 30 Aug 20         - Moved functions to separate source file - MT
 * 08 Aug 21         - Tidied up spelling errors in the comments - MT
 * 30 Sep 21         - Added the additional properties for the label colour
 *                     and alternate function colour - MT
 * 12 Oct 21         - Removed Title and replaced with model number - MT
 *                   - Added init_state() - MT 
 * 
 */

#define MODEL        "21"
#define HEIGHT       385
#define WIDTH        201
#define BUTTONS      30

#define DIGITS       12

#define MEMORY_SIZE  1 /* Not used but can't be zero*/
#define ROM_SIZE     02000
#define ROM_BANKS    1

int i_rom [ROM_SIZE * ROM_BANKS];

void v_init_state();

void v_init_keypad(obutton *h_button[], oswitch *h_switch[]);
