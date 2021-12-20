/*
 * x11-calc-cpu.c - RPN (Reverse Polish) calculator simulator.
 *
 * Copyright(C) 2020   MT
 *
 * Simulates the ACT processor.
 *
 * This processor simulator is based on the work of a number of individuals
 * including  Jacques LAPORTE, David HICKS, Greg SYDNEY-SMITH, Eric  SMITH,
 * Tony  NIXON and Bernhard EMESE.  Without their efforts and in some cases
 * assistance and encouragement this simulator not have been possible.
 *
 * Thank you.
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
 * 10 Sep 20   0.1   - Initial version - MT
 * 08 Aug 21         - Tidied up spelling errors in the comments and  added
 *                     a section describing the processor registers - MT
 *                   - Changed the status word into an integer - MT
 * 10 Aug 21         - Modified register names - MT
 *                   - Defined register size as a constant - MT
 * 16 Aug 21         - Added   tick()  to  execute  a  single  instruction,
 *                     currently this only decodes two  instructions ('nop'
 *                     and  'hi I'm woodstock')  neither of which  actually
 *                     does anything.  Any unrecognised opcodes are  simply
 *                     ignored - MT
 * 19 Aug 21         - Created  two data structures to hold  the  processor
 *                     properties and the register properties and added the
 *                     code to create a new processor instance - MT
 * 20 Aug 21         - Updated tick() using the new data structures - MT
 * 22 Aug 21         - Can now decode the four main instruction types - MT
 * 24 Aug 21         - Added code to decode the field modifier - MT
 * 26 Aug 21         - Re-implemented load_reg() to use the processor  data
 *                     and renamed it set_register() - MT
 *                   - Reversed order of nibbles in each register to  match
 *                     the actual numbering (when the register format  says
 *                     the left hand nibble is number 13 it means it!) - MT
 *                   - Implemented my first real op code (0 -> a[f]) - MT
 * 27 Aug 21         - Added code to exchange register contents -MT
 * 28 Aug 21         - Created a separate method to display the contents of
 *                     a  register  (replacing the existing debug  code  in
 *                     register load and exchange routines) - MT
 *                   - Implemented routines to add and the contents of  two
 *                     registers  and used this to increment a register  by
 *                     setting the carry flag and then calling add register
 *                     with a single parameter - MT
 *                   - Added an identifier to the register properties  which
 *                     allows  the register name to be shown when  printing
 *                     the value - MT
 * 29 Aug 21         - Tidied up trace output - MT
 *                   - Changed the register names - MT
 * 30 Aug 21         - Changed  carry to use the processor flags, and  made
 *                     the  first  and last values used when selecting  the
 *                     field in a register processor properties - MT
 *                   - Changed all flags and status bits to integers - MT
 *                   - Run mode set by default - MT
 *                   - Tidied up the comments (again) - MT
 *                   - Separated the 'special' instruction into four groups
 *                     to make it easier to parse the parameters - MT
 *                   - Added several more functions - MT
 *                   - Implemented code to handle subroutines - MT
 *                   - Fixed compiler warnings - MT
 *                   - Added ROM select, and register copy - MT
 * 31 Aug 21         - Fixed typo in RAM initialisation code - MT
 *                   - Using a NULL argument with the copy register routine
 *                     means it can be used to replace the function used to
 *                     zero a register - MT
 *                     using the copy routine - MT
 *                   - Added tests for equals and not equals - MT
 *                   - Implemented shift right operations - MT
 *                   - Tidied up code used to trace execution - MT
 *                   - Unimplemented operations now generate errors - MT
 *                   - Removed superfluous breaks - MT
 *  1 Sep 21         - Removed  separate routines to set and  clear  status
 *                     bits - MT
 *                   - Added  a  routine  to display the  contents  of  the
 *                     registers and processor flags - MT
 *                   - Implemented  f -> a, if s(n) = 0, p = n,  if nc goto
 *                     instructions - MT
 *  2 Sep 21         - Added  a routine to subtract two registers,  setting
 *                     carry flag beforehand and passing a single parameter
 *                     allows it to be used to decrement a register - MT
 *                   - Carry cleared at the beginning of each tick - MT
 *                   - Added a subroutine to handle go to - MT
 *  4 Sep 21         - Sorted out bug in p <> 0 test - MT
 *  5 Sep 21         - Moved trace output to the start of each  instruction
 *                     to make debugging easier - MT
 *                   - Fixed calculation of 8-bit branch addresses - MT
 *                   - Made  the handling of the carry flag consistent  (so
 *                     that  incrementing the program counter always clears
 *                     the carry flag and set the previous carry) - MT
 *                   - Implemented code used to read a key code and jump to
 *                     the correct address in memory - MT
 *                   - Added a keycode and keystate properties to store the
 *                     key  code of the key and the state of the actual key
 *                     (necessary as clearing status bit 15 when the key is
 *                     released does NOT work!) - MT
 *                   - Modified  code to allow status bit 15 to be  cleared
 *                     if a key is not pressed - MT
 *  8 Sep 21         - Stack operations now work (ENTER, X-Y, and R) - MT
 *  9 Sep 21         - Modified add and subtract to specify the destination
 *                     separately and allow NULL arguments - MT
 *                   - Logical shift clears carry flags - MT
 *                   - Don't clear status bits when tested! - MT
 *                   - Added 'y -> a', '0 - c - 1 -> c[f]', 'if a >= c[f]',
 *                     'a - c -> c[f]',  and 'if a >= b[f]' - MT
 *                   - Basic arithmetic operations now work!! - MT
 *                   - Fixed bug in clear s - MT
 * 10 Sep 21         - Restructured  decoder for group 2 type  instructions
 *                     and implemented load n - MT
 * 14 Sep 21         - Fixed  'n -> c' (registers were reversed) - MT
 *                   - Implemented '0 - c - 1 -> c[f]' - MT
 *                   - Removed  'return'  function(only two lines and  used
 *                     just once) - MT
 *                   - Modified some error messages - MT
 *                   - Tidied up extra spaces - MT
 *             0.2   - HP21 simulator works !!
 * 15 Sep 21         - Added 'clear data registers', and delayed ROM select
 *                     handling to 'jsb' and 'goto' - MT
 *                   - Moved  the processor initialisation into a  separate
 *                     routine  to allow main routine to do a reset without
 *                     having to exit and restart the program - MT
 * 16 Sep 21         - Hopefully now handles bank switching better - MT
 * 22 Sep 21         - Added 'c -> data address' - MT
 *                   - Added the line number to the unexpected opcode error
 *                     message - MT
 * 23 Sep 21         - Fixed 'go to' address calculation (hopefully) - MT
 *                   - Fixed bug in 'p - 1 -> p' - MT
 * 25 Sep 21         - Updated register debug output - MT
 *                   - Changed DATA_REGISTERS to MEMORY_SIZE - MT
 * 26 Sep 21         - Display register contents when debugging - MT
 *                   - Implemented 'c -> data' and 'data -> c' - MT
 *                   - Added  range checking to 'data register(n)-> c'  and
 *                     'c -> data address' - MT
 * 30 Sep 21         - Added routines to clear flags and status bits - MT
 *                   - Added properties for enable and select - MT
 *                   - For the HP21 and HP22 the state of the select switch
 *                     is checked when setting or clearing S 3 - MT
 *  1 Oct 21         - Converted flags to Boolean variables - MT
 *  4 Oct 21         - Added 'a -> rom address' - MT
 *  6 Oct 21         - Dumps all registers when displaying status - MT
 *  7 Oct 21         - Fixed bug in 'data register(n)-> c' - MT
 *                   - Fixed bug in 'clear registers' - MT
 *             0.3   - HP25 simulator now working..
 * 10 Oct 21         - Fixed bug in code to check the ROM size - MT
 *                   - Added 'keys -> a' and 'keys -> a' - MT
 *                   - Fixed bug in 'c -> data register(n)' - MT
 * 14 Oct 21         - Added  processor_save() and processor_restore()  and
 *                     modified  'clear data registers' to prevent it  from
 *                     clearing  the  registers  if  continuous  memory  is
 *                     enabled (bit of a 'kuldge') - MT
 * 16 Oct 21         - Checks for continuous memory when clearing registers
 *                     only when executing an instruction - MT
 *                   - Forgot to close the data file - MT
 *                   - Fixed bug with program counter (do not decrement the
 *                     program  counter  before checking for a delayed  ROM
 *                     select) - MT
 *             0.4   - HP29 simulator works..!
 *                   - Fixed bug in 'keys -> rom address' - MT
 *             0.5   - HP27 simulator works (requires testing).
 * 17 Oct 21         - Renamed init() to reset() and fixed bug in init() by
 *                     consolidating  the code used to clear the  registers
 *                     into reset() - MT
 *                   - Modified  'p = p + 1' as woodstock and spice  series
 *                     machines behave differently - MT
 *             0.6   - HP31 and HP32 simulators work (requires testing).
 *                   - Added dummy 'rom checksum' instruction - MT
 * 20 Oct 21         - Modified 'rom checksum' to set status bit and return
 *                     and ignores invalid memory addresses (prints warning
 *                     message) - MT
 * 26 Oct 21   0.7   - Added processor_load() to load saved registers  from
 *                     a file - MT
 * 17 Nov 21   0.8   - Fixed pathname on VMS - MT
 * 22 Nov 21         - Only saves the state of continuous registers - MT
 * 25 Nov 21   0.9   - Removed current rom property as this is not used and
 *                     added the rom bank property, since this is needed by
 *                     the HP34C - MT
 *                   - Implemented bank switching - MT
 *                   - Updated  the display format used to show the address
 *                     in any error or warning messages to include the bank
 *                     number - MT
 *                   - Fixed  bug in clear registers (it was  clearing  the
 *                     processor stack which it should not) - MT
 *                   - Only  loads and saves a subset of the registers when
 *                     the set of persistent registers is defined - MT
 * 27 Nov 21         - Mode the register display less verbose by creating a
 *                     separate routine to display memory - MT
 * 28 Nov 21         - Made the trace flag a processor property - MT
 *                   - Fixed bug when implementing the p register and moved
 *                     the code to modify p into a couple of functions - MT
 *                   - Fixed missing break in bank switch - MT
 *                   - Removed unnecessary data validation - MT
 *                   - Made the single step flag a processor property - MT
 *                   - Saves the last opcode executed - MT
 *                   - Incrementing the p register now works properly!
 *                     Literally the only way to get it to work properly is
 *                     to check the previous opcode if the current value is
 *                     zero and ignore it if it is the same. I would really
 *                     like to know why it works this way - MT
 * 02 Dec 21         - Modified  check above to compare previous opcode with
 *                     the current opcode (not a hard coded value) - MT
 *                   - Removed any references to TRACE - MT
 * 11 Dec 21         - Print flags as a hex value - MT
 *                   - Started to add additional instructions and processor
 *                     flags to support the HP67 - MT
 * 17 Dec 21         - Fixed bug in clear data registers - MT
 * 19 Dec 21         - Unused load register routine deleted - MT
 *                   - Bank switch sets flag - MT
 *                   - Jump to subroutine and return now saves and restores
 *                     the current bank number (it is amazing that this bug
 *                     didn't cause problems earlier) - MT
 * 20 Dec 21         - Discovered that on the HP67 when the program counter
 *                     is less then 02000 the result is an implicit  switch
 *                     to bank 0 (this may be true for other models) - MT
 *
 * To Do             - Test implicit bank switching behaviour.
 */

#define NAME           "x11-calc"
#define VERSION        "0.9"
#define BUILD          "0117"
#define DATE           "19 Dec 21"
#define AUTHOR         "MT"

#define DEBUG  0       /* Enable/disable debug*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "x11-calc-font.h"
#include "x11-calc-label.h"
#include "x11-calc-switch.h"
#include "x11-calc-button.h"

#include "x11-calc-cpu.h"

#include "gcc-debug.h" /* print() */
#include "gcc-wait.h"  /* i_wait() */

#if defined(PERSISTENT)
static int i_persistant[] = PERSISTENT;
#endif

/* Print the contents of a register */
static void v_fprint_register(FILE *h_file, oregister *h_register) {
   const char c_name[8] = {'A', 'B', 'C', 'Y', 'Z', 'T', 'M', 'N'};
   int i_count;
   if (h_register != NULL) {
      fprintf(h_file, "reg[");
      if (h_register->id < 0)
         fprintf(h_file, "*%c", c_name[h_register->id * -1 - 1]);
      else
         fprintf(h_file, "%02d", h_register->id);
      fprintf(h_file, "] = 0x");
      for (i_count = REG_SIZE - 1; i_count >=0 ; i_count--) {
         fprintf(h_file, "%1x", h_register->nibble[i_count]);
      }
      fprintf(h_file, "  ");
   }
}

/* Display the current processor status word */
static void v_fprint_status(FILE *h_file, oprocessor *h_processor) {
   int i_count, i_temp = 0;
   for (i_count = STATUS_BITS - 1; i_count >= 0; i_count--) {
      i_temp <<= 1;
      if (h_processor->status[i_count]) i_temp |= 1;
   }
   fprintf(h_file, "0x%04X%12c", i_temp, ' ');
}

/* Display the current processor flags */
static void v_fprint_flags(FILE *h_file, oprocessor *h_processor) {
   int i_count, i_temp = 0;
   for (i_count = 0; i_count <= FLAGS; i_count++)
      i_temp += h_processor->flags[i_count] << i_count;
   fprintf(h_file, "0x%04X%12c", i_temp, ' ');

}

/* Display current register contents */
void v_fprint_registers(FILE *h_file, oprocessor *h_processor) {
   if (h_processor != NULL) {
      int i_count;
      for (i_count = 0; i_count < REGISTERS; i_count++) {
         if (i_count % 3 == 0) fprintf(h_file, "\n\t");
         v_fprint_register(h_file, h_processor->reg[i_count]);
      }
      fprintf(h_file, "\n\tflags[] = ");
      v_fprint_flags(h_file, h_processor);
      fprintf(h_file, "status  = ");
      v_fprint_status(h_file, h_processor);
      fprintf(h_file, "ptr     = %02d  ", h_processor->p);
      fprintf(h_file, "addr    = %02d\n", h_processor->addr);
   }
}

/* Display current memory contents */
void v_fprint_memory(FILE *h_file, oprocessor *h_processor) {
   if (h_processor != NULL) {
      int i_count;
      if (MEMORY_SIZE > 1) { /* Bit of a fudge but... */
         for (i_count = 0; i_count < MEMORY_SIZE; i_count++) {
            if (i_count % 3 == 0) fprintf(h_file, "\n\t");
            v_fprint_register(h_file, h_processor->mem[i_count]);
         }
         fprintf(h_file, "\n");
      }
   }
}

/* Create a new register , */
oregister *h_register_create(char c_id){
   oregister *h_register; /* Pointer to register */
   int i_count, i_temp;
   if ((h_register = malloc (sizeof(*h_register))) == NULL) {
      fprintf(stderr, "Run-time error\t: %s line : %d : %s", __FILE__, __LINE__, "Memory allocation failed!\n");
   }
   i_temp = sizeof(h_register->nibble) / sizeof(*h_register->nibble);
   h_register->id = c_id;
   for (i_count = 0; i_count < i_temp; i_count++)
      h_register->nibble[i_count] = 0;
   return (h_register);
}

/* Exchange the contents of two registers */
static void v_reg_exch(oprocessor *h_processor, oregister *h_destination, oregister *h_source) {
   int i_count, i_temp;
   for (i_count = h_processor->first; i_count <= h_processor->last; i_count++) {
      i_temp = h_destination->nibble[i_count];
      h_destination->nibble[i_count] = h_source->nibble[i_count];
      h_source->nibble[i_count] = i_temp;
   }
}

/* Copy the contents of a register */
static void v_reg_copy(oprocessor *h_processor, oregister *h_destination, oregister *h_source) {
   int i_count, i_temp;
   for (i_count = h_processor->first; i_count <= h_processor->last; i_count++) {
      if (h_source != NULL) i_temp = h_source->nibble[i_count]; else i_temp = 0;
      h_destination->nibble[i_count] = i_temp;
   }
}

/* Add the contents of two registers */
static void v_reg_add(oprocessor *h_processor, oregister *h_destination, oregister *h_source, oregister *h_argument){
   int i_count, i_temp;
   for (i_count = h_processor->first; i_count <= h_processor->last; i_count++){
      if (h_argument != NULL) i_temp = h_argument->nibble[i_count]; else i_temp = 0;
      i_temp = h_source->nibble[i_count] + i_temp;
      if (h_processor->flags[CARRY]) i_temp++;
      if (i_temp >= h_processor->base){
         i_temp -= h_processor->base;
         h_processor->flags[CARRY] = True;
      } else {
         h_processor->flags[CARRY] = False;
      }
      if (h_destination != NULL) h_destination->nibble[i_count] = i_temp; /* Destination can be null */
   }
}

/* Subtract the contents of two registers */
static void v_reg_sub(oprocessor *h_processor, oregister *h_destination, oregister *h_source, oregister *h_argument){
   int i_count, i_temp;
   for (i_count = h_processor->first; i_count <= h_processor->last; i_count++){
      if (h_argument != NULL) i_temp = h_argument->nibble[i_count]; else i_temp = 0;
      if (h_source != NULL) i_temp = (h_source->nibble[i_count] - i_temp); else i_temp = (0 - i_temp);
      if (h_processor->flags[CARRY]) i_temp--;
      if (i_temp < 0) {
         i_temp += h_processor->base;
         h_processor->flags[CARRY] = True;
      }
      else
         h_processor->flags[CARRY] = False;
      /* if (i_temp < 0) v_error("Invalid data (%02x) at %1o-%04o in %s line : %d\n", i_temp, h_processor->bank, h_processor->pc, __FILE__, __LINE__); */
      if (h_destination != NULL) h_destination->nibble[i_count] = i_temp; /* Destination can be null */
   }
}

/* Test if registers are equal */
static void v_reg_test_eq(oprocessor *h_processor, oregister *h_destination, oregister *h_source) {
  int i_count, i_temp;
  h_processor->flags[CARRY] = False; /* Clear carry - Do If True */
   for (i_count = h_processor->first; i_count <= h_processor->last; i_count++){
      if (h_source != NULL) i_temp = h_source->nibble[i_count]; else i_temp = 0;
      if (h_destination->nibble[i_count] != i_temp) {
         h_processor->flags[CARRY] = True; /* Set carry */
         break;
      }
   }
}

/* Test if registers are not equal */
static void v_reg_test_ne(oprocessor *h_processor, oregister *h_destination, oregister *h_source) {
  int i_count, i_temp;
  h_processor->flags[CARRY] = True;
   for (i_count = h_processor->first; i_count <= h_processor->last; i_count++){
      if (h_source != NULL) i_temp = h_source->nibble[i_count]; else i_temp = 0;
      if (h_destination->nibble[i_count] != i_temp) {
         h_processor->flags[CARRY] = False; /* Clear carry - Do If True */
         break;
      }
   }
}

/* Increment the contents of a register */
static void v_reg_inc(oprocessor *h_processor, oregister *h_register){
   h_processor->flags[CARRY] = True; /* Set carry */
   v_reg_add (h_processor, h_register, h_register, NULL); /* Add carry to register */
}

/* Logical shift right a register */
static void v_reg_shr(oprocessor *h_processor, oregister *h_register){
   int i_count;
   h_processor->flags[CARRY] = False; /* Clear carry */
   for (i_count = h_processor->first; i_count <= h_processor->last; i_count++)
      if (i_count == h_processor->last)
         h_register->nibble[i_count] = 0;
      else
         h_register->nibble[i_count] = h_register->nibble[i_count + 1];
}

 /* Logical shift left a register */
static void v_reg_shl(oprocessor *h_processor, oregister *h_register){
   int i_count;
   for (i_count = h_processor->last; i_count >= h_processor->first; i_count--)
      if (i_count == h_processor->first)
         h_register->nibble[i_count] = 0;
      else
         h_register->nibble[i_count] = h_register->nibble[i_count - 1];
   h_processor->flags[PREV_CARRY] = h_processor->flags[CARRY] = False;
}

/* Increment program counter */
static void v_op_inc_pc(oprocessor *h_processor) {
   if (h_processor->pc++ >= ((ROM_SIZE * ROM_BANKS) - 1)) h_processor->pc = 0;
   h_processor->flags[PREV_CARRY] = h_processor->flags[CARRY];
   h_processor->flags[CARRY] = False;
}

/* Load saved processor state */
void v_processor_load(oprocessor *h_processor, char *s_pathname) {
#if defined(CONTINIOUS)
   FILE *h_datafile;
   int i_count, i_counter;

   if ((h_processor != NULL) && (s_pathname != NULL)) { /* Check processor and pathname are defined */
      h_datafile = fopen(s_pathname, "r");
      if (h_datafile !=NULL) { /* If file exists and can be opened restore state */
         debug(fprintf(stderr,"Loading %s \n", s_pathname));
#if defined(PERSISTENT)
         for (i_count = 0; i_count < (sizeof(i_persistant) / sizeof(i_persistant[0])); i_count++) {
            for (i_counter = REG_SIZE - 1; i_counter >= 0 ; i_counter--) {
               fscanf(h_datafile, "%x,", &h_processor->mem[i_persistant[i_count]]->nibble[i_counter]);
            }
#else
         for (i_count = 0; i_count < MEMORY_SIZE; i_count++) {
            for (i_counter = REG_SIZE - 1; i_counter >= 0 ; i_counter--) {
               fscanf(h_datafile, "%x,", &h_processor->mem[i_count]->nibble[i_counter]);
            }
#endif
         }
         fclose(h_datafile);
      }
      else
         v_warning("Unable to open %s\n", s_pathname); /* Can't open data file */
   }
#endif
}

/* Save processor state */
void v_processor_save(oprocessor *h_processor) {
#if defined(CONTINIOUS)
   FILE *h_datafile;
   char *s_dir = getenv("HOME");
   char s_filename[] = FILENAME;
   char s_filetype[] = ".dat";
   char *s_pathname;
   int i_count, i_counter;

   if (h_processor != NULL) { /* Check processor defined and continuous memory enabled */
      if (s_dir == NULL) s_dir = ""; /* Use current folder if HOME not defined */
#if defined(unix)
      s_pathname = malloc((strlen(s_dir) + strlen(s_filename) +
         strlen(s_filetype) + 2) * sizeof(char*));
      strcpy(s_pathname, s_dir);
      strcat(s_pathname, "/.");
#else
      s_pathname = malloc((strlen(s_dir) + strlen(s_filename) +
         strlen(s_filetype)) * sizeof(char*));
      strcpy(s_pathname, s_dir);
#endif
      strcat(s_pathname, s_filename);
      strcat(s_pathname, s_filetype);
      h_datafile = fopen(s_pathname, "w");
      if (h_datafile !=NULL) { /* If file exists and can be opened save state */
         debug(fprintf(stderr,"Saving %s \n", s_pathname));
#if defined(PERSISTENT)
         for (i_count = 0; i_count < (sizeof(i_persistant) / sizeof(i_persistant[0])); i_count++) {
            for (i_counter = REG_SIZE - 1; i_counter >= 0 ; i_counter--) {
               fprintf(h_datafile, "%02x,", h_processor->mem[i_persistant[i_count]]->nibble[i_counter]);
            }
#else
         for (i_count = 0; i_count < MEMORY_SIZE; i_count++) {
            for (i_counter = REG_SIZE - 1; i_counter >= 0 ; i_counter--) {
               fprintf(h_datafile, "%02x,", h_processor->mem[i_count]->nibble[i_counter]);
            }
#endif
            fprintf(h_datafile,"\n");
         }
         fclose(h_datafile);
      }
      else
         v_warning("Unable to open %s\n", s_pathname); /* Can't open data file */
   }
#endif
}

/* Restore saved processor state */
void v_processor_restore(oprocessor *h_processor) {
#if defined(CONTINIOUS)
   char *s_dir = getenv("HOME");
   char s_filename[] = FILENAME;
   char s_filetype[] = ".dat";
   char *s_pathname;

   if (h_processor != NULL) { /* Check processor defined */
      if (s_dir == NULL) s_dir = ""; /* Use current folder if HOME not defined */
#if defined(unix)
      s_pathname = malloc((strlen(s_dir) + strlen(s_filename) +
         strlen(s_filetype) + 2) * sizeof(char*));
      strcpy(s_pathname, s_dir);
      strcat(s_pathname, "/.");
#else
      s_pathname = malloc((strlen(s_dir) + strlen(s_filename) +
         strlen(s_filetype)) * sizeof(char*));
      strcpy(s_pathname, s_dir);
#endif
      strcat(s_pathname, s_filename);
      strcat(s_pathname, s_filetype);
      v_processor_load(h_processor, s_pathname); /* Load settings */
   }
#endif
}

/* Reset processor */
void v_processor_reset(oprocessor *h_processor) {
   int i_count;
   h_processor->first = 0; h_processor->last = REG_SIZE - 1;
   for (i_count = 0; i_count < REGISTERS - 2; i_count++) /*Clear the CPU registers and stack */
      v_reg_copy(h_processor, h_processor->reg[i_count], NULL); /* Copying nothing to a register clears it */
   for (i_count = 0; i_count < STACK_SIZE; i_count++) /* Clear the processor stack */
      h_processor->stack[i_count] = 0;
   for (i_count = 0; i_count < MEMORY_SIZE; i_count++) /*Clear memory */
      v_reg_copy(h_processor, h_processor->mem[i_count], NULL); /* Copying nothing to a register clears it */
   for (i_count = 0; i_count < STATUS_BITS; i_count++) /* Clear the processor status word */
      h_processor->status[i_count] = False;
   for (i_count = 0; i_count < FLAGS; i_count++) /* Clear the processor flags */
      h_processor->flags[i_count] = False;
   h_processor->rom_number = 0;
   h_processor->opcode = 0;
   h_processor->bank = 0;
   h_processor->pc = 0;
   h_processor->sp = 0;
   h_processor->f = 0;
   h_processor->p = 0;
   h_processor->addr = 0;
   h_processor->base = 10;
   h_processor->code = 0;
   h_processor->keypressed = False;
   h_processor->enabled = True;
   h_processor->trace = False;
   h_processor->step = False;

   h_processor->status[5] = True; /* TO DO - Check which flags should be set by default */
}

/* Create a new processor , */
oprocessor *h_processor_create(int *h_rom){
   oprocessor *h_processor;
   int i_count;
   if ((h_processor = malloc(sizeof(*h_processor)))==NULL)
      v_error("Memory allocation failed!"); /* Attempt to allocate memory to hold the processor structure */
   for (i_count = 0; i_count < REGISTERS; i_count++)
      h_processor->reg[i_count] = h_register_create((i_count + 1) * -1); /* Allocate storage for the registers */
   for (i_count = 0; i_count < MEMORY_SIZE; i_count++)
      h_processor->mem[i_count] = h_register_create(i_count); /* Allocate storage for the RAM */
   h_processor->rom = h_rom ; /* Address of ROM */
   h_processor->select = False;
   v_processor_reset(h_processor);
   return(h_processor);
}

/* Delayed ROM select */
static void v_delayed_rom(oprocessor *h_processor) { /* Delayed ROM select */
   if (h_processor->flags[DELAYED_ROM]) {
      h_processor->pc = (h_processor->rom_number << 8 | (h_processor->pc & 0xff)); /* Note - Uses an eight bit address */
      h_processor->flags[DELAYED_ROM] = False; /* Clear flag */
   }
}

/* Increment ptr register */
static void v_op_inc_p(oprocessor *h_processor) {
#if defined(SPICE)
   int i_opcode;
   i_opcode = h_processor->rom[h_processor->bank * ROM_SIZE + h_processor->pc];
   if (h_processor->p == REG_SIZE - 1)
      h_processor->p = 0;
   else {
      if (h_processor->p > 0)
         h_processor->p++;
      else {
         /* Literally the only way is to work out if 'P' should be
          * incremented when it is zero is to check the previous opcode !! */
         if (h_processor->opcode != i_opcode)
            h_processor->p++;
      }
   }
#else
   if (h_processor->p == REG_SIZE) h_processor->p = 0; else h_processor->p++;
#endif
}

/* Decrement ptr register */
static void v_op_dec_p(oprocessor *h_processor) {
   if (h_processor->p == 0) h_processor->p = REG_SIZE - 1; else h_processor->p--;
}

/* Jump to subroutine */
void op_jsb(oprocessor *h_processor, int i_count){
   h_processor->stack[h_processor->sp] = h_processor->bank * ROM_SIZE + h_processor->pc; /* Push current address on the stack */
   h_processor->sp = (h_processor->sp + 1) & (STACK_SIZE - 1); /* Update stack pointer */
   h_processor->pc = ((h_processor->pc & 0xff00) | i_count); /* Note - Uses an eight bit address */
   v_delayed_rom(h_processor);
   h_processor->pc--; /* Program counter will be auto incremented before next fetch */
}

/* Conditional go to */
void v_op_goto(oprocessor *h_processor){
   if (h_processor->trace) fprintf(stdout, "\n%1o-%04o %04o    then go to %05o",
      h_processor->bank, h_processor->pc, h_processor->rom[h_processor->bank * ROM_SIZE + h_processor->pc], h_processor->rom[h_processor->bank * ROM_SIZE + h_processor->pc]);
   if (!h_processor->flags[PREV_CARRY]) { /* Do if True */
      h_processor->pc = ((h_processor->pc & 0xfc00) | h_processor->rom[h_processor->bank * ROM_SIZE + h_processor->pc]); /* Note - Uses a _ten_ bit address */
      /* v_delayed_rom(h_processor); Not - applicable ?*/
      h_processor->pc--; /* Program counter will be auto incremented before next fetch */
   }
}

/* Decode and execute a single instruction */
void v_processor_tick(oprocessor *h_processor) {

   static const int i_set_p[16] = { 14,  4,  7,  8, 11,  2, 10, 12,  1,  3, 13,  6,  0,  9,  5, 14 };
   static const int i_tst_p[16] = { 4 ,  8, 12,  2,  9,  1,  6,  3,  1, 13,  5,  0, 11, 10,  7,  4 };

   unsigned int i_opcode;
   unsigned int i_field; /* Field modifier */
   const char *s_field; /* Holds pointer to field name */

   if (h_processor->enabled) {
      if (h_processor->trace)
         fprintf(stdout, "%1o-%04o %04o  ", h_processor->bank, h_processor->pc, h_processor->rom[h_processor->bank * ROM_SIZE + h_processor->pc]);
      if (h_processor->keypressed) h_processor->status[15] = True; /* Set status bit if key pressed */
#if defined(HAWKEYE) /* Seems to use a flag rather then the status word for the switch position */
      h_processor->flags[MODE] = h_processor->select; /* Set the program mode flag based on switch position */
#else
      h_processor->status[3] = h_processor->select; /* Set status bit based on switch position */
#endif
#if defined(SPICE) /* Setting S(5) breaks the self test on Spice machines */
      h_processor->status[5] = False; /* Self Test */
#else
      h_processor->status[5] = True; /* Low Power */
#endif
      i_opcode = h_processor->rom[h_processor->bank * ROM_SIZE + h_processor->pc]; /* Get next instruction */
      switch (i_opcode & 03) {
      case 00: /* Special operations */
         switch ((i_opcode >> 2) & 03) {
         case 00: /* Group 0 */
            switch ((i_opcode >> 4) & 03) {
            case 00:
               switch (i_opcode){
               case 00000: /* nop */
                  if (h_processor->trace) fprintf(stdout, "nop");
                  break;
#if defined(HAWKEYE)
               /*
                * 00100   Test ready
                * 00300   Test W/PGM switch
                * 00400   Set key pressed
                * 00500   Test if a key was pressed
                * 01000   Set default function keys
                * 01100   Test if default function keys set
                * 01200   Set merge flag
                * 01300   Test merge flag
                * 01400   Set waiting for card side 2 flag
                * 01500   Test waiting for card side 2 flag
                * 01700   Read/Write data to/from card via RAM $99 and $9B
                */
               case 00100: /* test motor on */
                  /** if (!h_processor->trace)
                     debug(fprintf(stdout, "\n%1o-%04o %04o  ", h_processor->bank, h_processor->pc, h_processor->rom[h_processor->bank * ROM_SIZE + h_processor->pc]);
                        h_processor->step = h_processor->trace = True); */
                  if (h_processor->trace) fprintf(stdout, "test motor on");
                  h_processor->status[3] = True; /* device always ready */
                  h_processor->crc[CARD] = False;
                  break;
               case 00300: /* test mode flag */
                  if (h_processor->trace) fprintf(stdout, "test mode flag (%d)", !h_processor->flags[MODE] );
                  h_processor->status[3] = !h_processor->flags[MODE]; /* Test the PRGM/RUN switch */
                  break;
               case 00400: /* set key pressed flag */
                  if (h_processor->trace) fprintf(stdout, "set key pressed flag");
                  h_processor->crc[ANYKEY] = True; /* Sets the any key pressed flag */
                  break;
               case 00500: /* test key pressed flag */ /* f -x- */
                  if (h_processor->trace) fprintf(stdout, "test key pressed flag");
                  h_processor->status[3] = h_processor->crc[ANYKEY];
                  if (h_processor->crc[ANYKEY]) h_processor->crc[ANYKEY] = False;
                  break;
               case 01000: /* set default function flag */
                  if (h_processor->trace) fprintf(stdout, "set flag 4");
                  h_processor->crc[FUNCTION] = True;
                  break;
               case 01100: /* test default function key flag */
                  if (h_processor->trace) fprintf(stdout, "test flag 4");
                  h_processor->status[3] = h_processor->crc[FUNCTION];
                  if (h_processor->crc[FUNCTION]) h_processor->crc[FUNCTION] = False;
                  break;
               case 01200: /* set merge flag */
                  if (h_processor->trace) fprintf(stdout, "set merge flag");
                  h_processor->crc[MERGE] = True;
                  break;
               case 01300: /* test merge flag */
                  if (h_processor->trace) fprintf(stdout, "clear flag 0");
                  h_processor->status[3] = h_processor->crc[MERGE];
                  if (h_processor->crc[MERGE]) h_processor->crc[MERGE] = False;
                  break;
               case 01400: /* set waiting flag */
                  if (h_processor->trace) fprintf(stdout, "clear waiting flag");
                  h_processor->crc[PAUSE] = True;
                  break;
               case 01500: /* test pause flag ? */
                  if (h_processor->trace) fprintf(stdout, "clear flag 1");
                  h_processor->status[3] = h_processor->crc[PAUSE];
                  if (h_processor->crc[PAUSE]) h_processor->crc[PAUSE] = False;
                  break;
               case 01700: /* read from or write to card */
                  if (h_processor->trace) fprintf(stdout, "card read write");
                  h_processor->status[3] = h_processor->crc[PAUSE];
                  if (h_processor->crc[PAUSE]) h_processor->crc[PAUSE] = False;
                  break;
#endif
               default:
                  v_error("Unexpected opcode (%04o) at %1o-%04o in %s line : %d\n", i_opcode, h_processor->bank, h_processor->pc, __FILE__, __LINE__);
               }
               break;
            case 01:
               switch (i_opcode){
               case 00020: /* keys -> rom address */
                  if (h_processor->trace) fprintf(stdout, "keys -> rom address");
                  h_processor->pc++;
                  h_processor->pc &= 0x0f00;
                  v_delayed_rom(h_processor);
                  h_processor->pc += h_processor->code;
                  h_processor->pc--; /* Program counter will be auto incremented before next fetch */
                  break;
               case 00120: /* keys -> a */
                  if (h_processor->trace) fprintf(stdout, "keys -> a");
                  h_processor->reg[A_REG]->nibble[2] = (h_processor->code >> 4);
                  h_processor->reg[A_REG]->nibble[1] = (h_processor->code & 0x0f);
                  break;
               case 00220: /* a -> rom address */
                  {
                     int i_addr;
                     if (h_processor->trace) fprintf(stdout, "a -> rom address");
                     h_processor->pc &= 0x0ff00;
                     i_addr = h_processor->pc + (h_processor->reg[A_REG]->nibble[2] << 4) + h_processor->reg[A_REG]->nibble[1];
                     if (i_addr < (ROM_SIZE * ROM_BANKS))
                         h_processor->pc = i_addr;
                     else
                        v_error("Address (%02o) out of range at %1o-%04o in %s line : %d\n", i_addr, h_processor->bank, h_processor->pc, __FILE__, __LINE__);
                     v_delayed_rom(h_processor);
                     h_processor->pc--; /* Program counter will be auto incremented before next fetch */
                  }
                  break;
               case 00320: /* reset twf */
                  if (h_processor->trace) fprintf(stdout, "reset twf");
                  break;
               case 00420: /* binary */
                  if (h_processor->trace) fprintf(stdout, "binary");
                  h_processor->base = 16;
                  break;
               case 00520: /* rotate left a */
                  if (h_processor->trace) fprintf(stdout, "rotate left a");
                  {
                     int c_temp, i_count;
                     c_temp = h_processor->reg[A_REG]->nibble[REG_SIZE - 1];
                     for (i_count = REG_SIZE - 1; i_count > 0; i_count--)
                        h_processor->reg[A_REG]->nibble[i_count] = h_processor->reg[A_REG]->nibble[i_count - 1];
                     h_processor->reg[A_REG]->nibble[0] = c_temp;
                     h_processor->flags[PREV_CARRY] = h_processor->flags[CARRY] = False;
                  }
                  break;
               case 00620: /* p - 1 -> p */
                  if (h_processor->trace) fprintf(stdout, "p - 1 -> p");
                  v_op_dec_p(h_processor);
                  break;
               case 00720: /* p + 1 -> p */
                  if (h_processor->trace) fprintf(stdout, "p + 1 -> p");
                  v_op_inc_p(h_processor);
                  break;
               case 01020: /* return */
                  if (h_processor->trace) fprintf(stdout, "return");
                  h_processor->sp = (h_processor->sp - 1) & (STACK_SIZE - 1); /* Update stack pointer */
                  h_processor->pc = h_processor->stack[h_processor->sp] & (ROM_SIZE - 1); /* Pop program counter from the stack */
                  h_processor->bank = h_processor->stack[h_processor->sp] / ROM_SIZE; /* Get new bank number */
                  break;
               default:
                  v_error("Unexpected opcode (%04o) at %1o-%04o in %s line : %d\n", i_opcode, h_processor->bank, h_processor->pc, __FILE__, __LINE__);
               }
               break;
            case 02: /* select rom */
               if (h_processor->trace) fprintf(stdout, "select rom %02o", i_opcode >> 6);
               h_processor->pc = (i_opcode >> 6) * 256 + (h_processor->pc % 256);
               break;
            case 03:
               switch (i_opcode) {
#if defined(HAWKEYE)
               /*
                * 00060   Set display digits
                * 00160   Test display digits
                * 00260   Motor on
                * 00360   Motor off
                * 00560   Test if card inserted
                * 00660   Set card write mode
                * 00760   Set card read mode
                */
               case 00060: /* set display digits */
                  if (h_processor->trace) fprintf(stdout, "set display digits");
                  h_processor->crc[DISPLAY] = True;
                  break;
               case 00160: /* test display digits */
                  if (h_processor->trace) fprintf(stdout, "test display digits");
                  h_processor->status[3] = h_processor->crc[DISPLAY];
                  if (h_processor->crc[DISPLAY]) h_processor->crc[DISPLAY] = False;
                  break;
               case 00260: /* card reader motor on */
                  if (h_processor->trace) fprintf(stdout, "motor on");
                  break;
               case 00360: /* card reader motor off */
                  if (h_processor->trace) fprintf(stdout, "motor off");
                  break;
               case 00560: /* test card inserted */
                  if (h_processor->trace) fprintf(stdout, "test card inserted");
                  h_processor->status[3] = h_processor->crc[CARD]; /* Test if card is inserted */
                  break;
               case 00660: /* card reader set write mode */
                  if (h_processor->trace) fprintf(stdout, "set write mode");
                  break;
               case 00760: /* card reader set read mode */
                  if (h_processor->trace) fprintf(stdout, "set read mode");
                  break;
#endif
               case 01060: /* bank switch */
                  if (h_processor->trace) fprintf(stdout, "bank switch");
                  h_processor->flags[BANK_SWITCH] = (!h_processor->flags[BANK_SWITCH]);
                  h_processor->bank ^=  1; /* Toggle bank number */
                  break;
               case 01160: /* c -> addr */
                  {
                     int i_addr;
                     if (h_processor->trace) fprintf(stdout, "c -> addr ");
                     i_addr = (h_processor->reg[C_REG]->nibble[1] << 4) + h_processor->reg[C_REG]->nibble[0];
                     h_processor->addr = i_addr;
                     if (i_addr < MEMORY_SIZE)
                        h_processor->addr = i_addr;
                     else {
                        h_processor->addr = MEMORY_SIZE - 1;
                        /** v_warning("Address (%02o) out of range at %1o-%04o in %s line : %d\n", i_addr, h_processor->bank, h_processor->pc, __FILE__, __LINE__); /* Print warning */
                     }
                  }
                  break;
               case 01260: /* clear data registers */
                  {
                     int i_count;
                     if (h_processor->trace) fprintf(stdout, "clear data registers");
                     h_processor->first = 0; h_processor->last = REG_SIZE - 1;
                     for (i_count = h_processor->addr & ~0x0f; i_count < (h_processor->addr & ~0x0f) + 16; i_count++)
                        if (i_count < MEMORY_SIZE) /* Check memory size */
                           v_reg_copy(h_processor, h_processor->mem[i_count], NULL); /* Copying nothing to a register clears it */
                  }
                  break;
               case 01360: /* c -> data */
                  if (h_processor->trace) fprintf(stdout, "c -> data");
                  h_processor->first = 0; h_processor->last = REG_SIZE - 1;
                  v_reg_copy(h_processor, h_processor->mem[h_processor->addr], h_processor->reg[C_REG]);
                  break;
               case 01460: /* rom checksum */
                  if (h_processor->trace) fprintf(stdout, "rom checksum");
                  h_processor->status[5] = False;
                  h_processor->sp = (h_processor->sp - 1) & (STACK_SIZE - 1); /* Update stack pointer */
                  h_processor->pc = h_processor->stack[h_processor->sp]; /* Pop program counter on the stack */
                  break;
               case 01760: /* hi I'm woodstock */
                  if (h_processor->trace) fprintf(stdout, "hi I'm woodstock");
                  break;
               default:
                  v_error("Unexpected opcode (%04o) at %1o-%04o in %s line : %d\n", i_opcode, h_processor->bank, h_processor->pc, __FILE__, __LINE__);
               }
            }
            break;
         case 01: /* Group 1 */
            switch ((i_opcode >> 4) & 03) {
            case 00: /* 1 -> s(n) */
               if (h_processor->trace) fprintf(stdout, "1 -> s(%d)", i_opcode >> 6);
               h_processor->status[i_opcode >> 6] = True;
               break;
            case 01: /* if 1 = s(n) */
               if (h_processor->trace) fprintf(stdout, "if 1 = s(%d)", i_opcode >> 6);
               h_processor->flags[CARRY] = !h_processor->status[i_opcode >> 6];
               v_op_inc_pc(h_processor); /* Increment program counter */
               v_op_goto(h_processor);
               break;
            case 02: /* if p = n */
               if (h_processor->trace) fprintf(stdout, "if p = %d", i_tst_p[i_opcode >> 6]);
               h_processor->flags[CARRY] = !(h_processor->p == i_tst_p[i_opcode >> 6]);
               v_op_inc_pc(h_processor); /* Increment program counter */
               v_op_goto(h_processor);
               break;
            case 03: /* delayed select rom n */
               if (h_processor->trace) fprintf(stdout, "delayed select rom %02o", i_opcode >> 6);
               h_processor->rom_number = i_opcode >> 6;
               h_processor->flags[DELAYED_ROM] = True;
            }
            break;
         case 02: /* Group 2 */
            switch ((i_opcode >> 4) & 03) {
            case 00: /* Sub group 0 */
               switch (i_opcode) {
               case 00010: /* clear registers */
                  if (h_processor->trace) fprintf(stdout, "clear registers");
                  {
                     int i_count;
                     h_processor->first = 0; h_processor->last = REG_SIZE - 1;
                     for (i_count = 0; i_count < REGISTERS - 2; i_count++) /* Don't clear M or N */
                        v_reg_copy(h_processor, h_processor->reg[i_count], NULL); /* Copying nothing to a register clears it */
                     /** for (i_count = 0; i_count < STACK_SIZE; i_count++) /* Clear the processor stack */
                     /**    h_processor->stack[i_count] = 0; */
                  }
                  break;
               case 00110: /* clear s */
                  if (h_processor->trace) fprintf(stdout, "clear s");
                  {
                     int i_count;
                     for (i_count = 0; i_count < sizeof(h_processor->status) / sizeof(*h_processor->status); i_count++)
                        switch (i_count) {
                           case 1:  /* Scientific notation */
                           case 2:  /* Auto Enter (if set entering digit will push 'X') */
                           case 5:  /* Low power (Woodstock) / Self test (Spice) */
                           case 15: /* Set if any key is pressed */
                              break;
                           default:
                              h_processor->status[i_count] = False; /* Clear all bits except bits 1, 2, 5, 15 */
                        }
                  }
                  break;
               case 00210: /* display toggle */
                  if (h_processor->trace) fprintf(stdout, "display toggle");
                  h_processor->flags[DISPLAY_ENABLE] = (!h_processor->flags[DISPLAY_ENABLE]);
                  break;
               case 00310: /* display off */
                  if (h_processor->trace) fprintf(stdout, "display off");
                  h_processor->flags[DISPLAY_ENABLE] = False;
                  break;
               case 00410: /* m exch c */
                  if (h_processor->trace) fprintf(stdout, "m exch c");
                  h_processor->first = 0; h_processor->last = REG_SIZE - 1;
                  v_reg_exch(h_processor, h_processor->reg[M_REG], h_processor->reg[C_REG]);
                  break;
               case 00510: /* m -> c */
                  if (h_processor->trace) fprintf(stdout, "m -> c");
                  h_processor->first = 0; h_processor->last = REG_SIZE - 1;
                  v_reg_copy(h_processor, h_processor->reg[C_REG], h_processor->reg[M_REG]);
                  break;
               case 00610: /* n exch c */
                  if (h_processor->trace) fprintf(stdout, "n exch c");
                  h_processor->first = 0; h_processor->last = REG_SIZE - 1;
                  v_reg_exch(h_processor, h_processor->reg[N_REG], h_processor->reg[C_REG]);
                  break;
               case 00710: /* n -> c */
                  if (h_processor->trace) fprintf(stdout, "n -> c");
                  h_processor->first = 0; h_processor->last = REG_SIZE - 1;
                  v_reg_copy(h_processor, h_processor->reg[C_REG], h_processor->reg[N_REG]);
                  break;
               case 01010: /* stack -> a */
                  if (h_processor->trace) fprintf(stdout, "stack -> a");
                     h_processor->first = 0; h_processor->last = REG_SIZE - 1;
                     v_reg_copy(h_processor, h_processor->reg[A_REG], h_processor->reg[Y_REG]); /* T = Z */
                     v_reg_copy(h_processor, h_processor->reg[Y_REG], h_processor->reg[Z_REG]); /* T = Z */
                     v_reg_copy(h_processor, h_processor->reg[Z_REG], h_processor->reg[T_REG]); /* T = Z */
                  break;
               case 01110: /* down rotate */
                  if (h_processor->trace) fprintf(stdout, "stack -> a");
                     h_processor->first = 0; h_processor->last = REG_SIZE - 1;
                     v_reg_exch(h_processor, h_processor->reg[T_REG], h_processor->reg[C_REG]); /* T = Z */
                     v_reg_exch(h_processor, h_processor->reg[C_REG], h_processor->reg[Y_REG]); /* T = Z */
                     v_reg_exch(h_processor, h_processor->reg[Y_REG], h_processor->reg[Z_REG]); /* T = Z */
                  break;
               case 01210: /* y -> a */
                  if (h_processor->trace) fprintf(stdout, "y -> a");
                  h_processor->first = 0; h_processor->last = REG_SIZE - 1;
                  v_reg_copy(h_processor, h_processor->reg[A_REG], h_processor->reg[Y_REG]);
                  break;
               case 01310: /* c -> stack */
                  if (h_processor->trace) fprintf(stdout, "stack -> a");
                     h_processor->first = 0; h_processor->last = REG_SIZE - 1;
                     v_reg_copy(h_processor, h_processor->reg[T_REG], h_processor->reg[Z_REG]); /* T = Z */
                     v_reg_copy(h_processor, h_processor->reg[Z_REG], h_processor->reg[Y_REG]); /* T = Z */
                     v_reg_copy(h_processor, h_processor->reg[Y_REG], h_processor->reg[C_REG]); /* T = Z */
                  break;
               case 01410: /* decimal */
                  if (h_processor->trace) fprintf(stdout, "decimal");
                  h_processor->base = 10;
                  break;
               case 01610: /* f -> a */
                  h_processor->reg[A_REG]->nibble[0] = h_processor->f;
                  if (h_processor->trace) fprintf(stdout, "f -> a");
                  break;
               case 01710: /* f exch a */
                  if (h_processor->trace) fprintf(stdout, "f exch a");
                  {
                     int i_temp;
                     i_temp = h_processor->reg[A_REG]->nibble[0];
                     h_processor->reg[A_REG]->nibble[0] = h_processor->f;
                     h_processor->f = i_temp;
                  }
                  break;
               default:
                  v_error("Unexpected opcode (%04o) at %1o-%04o in %s line : %d\n", i_opcode, h_processor->bank, h_processor->pc, __FILE__, __LINE__);
               }
               break;
            case 01: /* load n */
               if (h_processor->trace) fprintf(stdout, "load (%d)", i_opcode >> 6);
               h_processor->reg[C_REG]->nibble[h_processor->p] = i_opcode >> 6;
               v_op_dec_p(h_processor);
               break;
            case 02: /* c -> data register(n) */
               h_processor->addr &= 0xfff0;
               h_processor->addr += (i_opcode >> 6);
               if (h_processor->trace) fprintf(stdout, "c -> data register(%d)", h_processor->addr);
               if ((h_processor->addr) < MEMORY_SIZE) {
                  h_processor->first = 0; h_processor->last = REG_SIZE - 1;
                  v_reg_copy(h_processor, h_processor->mem[h_processor->addr], h_processor->reg[C_REG]); /* C -> reg(n) */
               }
               else {
                  v_error("Address (%02o) out of range at %1o-%04o in %s line : %d\n", i_opcode >> 6, h_processor->bank, h_processor->pc, __FILE__, __LINE__);
               }
               break;
            case 03: /* data -> c or data register(n)-> c */
               if ((i_opcode >> 6) < MEMORY_SIZE) {
                  h_processor->first = 0; h_processor->last = REG_SIZE - 1;
                  if ((i_opcode >> 6) == 0) {
                     if (h_processor->trace) fprintf(stdout, "data -> c");
                     v_reg_copy(h_processor, h_processor->reg[C_REG], h_processor->mem[h_processor->addr]);
                  }
                  else {
                     h_processor->addr &= 0xfff0;
                     h_processor->addr += (i_opcode >> 6);
                     if (h_processor->trace) fprintf(stdout, "data register(%d) -> c", h_processor->addr);
                     if ((h_processor->addr) < MEMORY_SIZE)
                        v_reg_copy(h_processor, h_processor->reg[C_REG], h_processor->mem[h_processor->addr]);
                     else {
                        v_error("Address (%02o) out of range at %1o-%04o in %s line : %d\n", h_processor->addr, h_processor->bank, h_processor->pc, __FILE__, __LINE__);
                     }
                  }
               }
               else {
                  v_error("Invalid register (REG[%d]) at %1o-%04o in %s line : %d\n", i_opcode >> 6, h_processor->bank, h_processor->pc, __FILE__, __LINE__);
               }
               break;
            default:
               v_error("Unexpected error in %s line : %d\n", __FILE__, __LINE__);
            }
            break;

         case 03: /* Group 3 */
            switch ((i_opcode >> 4) & 03) {
            case 00: /* 0 -> s(n) */
               if (h_processor->trace) fprintf(stdout, "0 -> s(%d)", i_opcode >> 6);
               h_processor->status[i_opcode >> 6] = False;
               break;
            case 01: /* if 0 = s(n) */
               if (h_processor->trace) fprintf(stdout, "if 0 = s(%d) ", i_opcode >> 6);
               h_processor->flags[CARRY] = h_processor->status[i_opcode >> 6];
               v_op_inc_pc(h_processor); /* Increment program counter */
               v_op_goto(h_processor);
               break;
            case 02: /* if p <> n */
               /* 01354 if p <>  0  00554 if p <>  1  00354 if p <>  2
                * 00754 if p <>  3  00054 if p <>  4  01254 if p <>  5
                * 00654 if p <>  6  01654 if p <>  7  00154 if p <>  8
                * 00454 if p <>  9  01554 if p <> 10  01454 if p <> 11
                * 00254 if p <> 12  01154 if p <> 13  N/A   if p <> 14
                * N/A   if p <> 15
               */
               if (h_processor->trace) fprintf(stdout, "if p # %d", i_tst_p[i_opcode >> 6]);
               h_processor->flags[CARRY] = (h_processor->p == i_tst_p[i_opcode >> 6]);
               v_op_inc_pc(h_processor); /* Increment program counter */
               v_op_goto(h_processor);
               break;
            case 03: /* p = n */
               /* 01474  0 -> p  01074  1 -> p  00574  2 -> p
                * 01174  3 -> p  00174  4 -> p  01674  5 -> p
                * 01374  6 -> p  00274  7 -> p  00374  8 -> p
                * 01574  9 -> p  00674 10 -> p  00474 11 -> p
                * 00774 12 -> p  01274 13 -> p  N/A   14 -> p
                * N/A   15 -> p
                * */
               if (h_processor->trace) fprintf(stdout, "p = %d", i_set_p[i_opcode >> 6]);
               h_processor->p = i_set_p[i_opcode >> 6];
               break;
            }
            break;
         }
         break;
      case 01: /* jsb */
         if (h_processor->trace) fprintf(stdout, "jsb %05o", ((h_processor->pc & 0xff00) | i_opcode >> 2)); /* Note - uses and eight bit address */
         op_jsb(h_processor, (i_opcode >> 2));
         break;
      case 02: /* Arithmetic operations */
         i_field = (i_opcode >> 2) & 7;

         switch (i_field) {
         case 0: /* 000   P  : determined by P register             ([P])       */
            h_processor->first = h_processor->p; h_processor->last = h_processor->p;
            s_field = "p";
            if (h_processor->p >= REG_SIZE) {
               v_fprint_registers(stderr, h_processor);
               v_error("Unexpected error at %1o-%04o in %s line : %d\n", h_processor->bank, h_processor->pc, __FILE__, __LINE__);
               h_processor->last = 0;
            }
            break;
         case 1: /* 100  WP  : word up to and including P register  ([0 .. P])  */
            h_processor->first =  0; h_processor->last =  h_processor->p; /* break; bug in orig??? */
            s_field = "wp";
            if (h_processor->p >= REG_SIZE) {
               v_error("Unexpected error in %s line : %d\n", __FILE__, __LINE__);
               h_processor->last = REG_SIZE - 1;
            }
            break;
         case 2: /* 110  XS  : exponent and sign                    ([0 .. 2])  */
            h_processor->first = EXP_SIZE - 1; h_processor->last = EXP_SIZE - 1;
            s_field = "xs";
            break;
         case 3: /* 010   X  : exponent                             ([0 .. 1])  */
            h_processor->first = 0; h_processor->last = EXP_SIZE - 1;
            s_field = "x";
            break;
         case 4: /* 111   S  : sign                                 ([13])      */
            h_processor->first = REG_SIZE - 1; h_processor->last = REG_SIZE - 1;
            s_field = "s";
            break;
         case 5: /* 001   M  : mantissa                             ([3 .. 12]) */
            h_processor->first = EXP_SIZE; h_processor->last = REG_SIZE - 2;
            s_field = "m";
            break;
         case 6: /* 011   W  : word                                 ([0 .. 13]) */
            h_processor->first = 0; h_processor->last = REG_SIZE - 1;
            s_field = "w";
            break;
         case 7: /* 101  MS  : mantissa and sign                    ([3 .. 13]) */
            h_processor->first = EXP_SIZE; h_processor->last = REG_SIZE - 1;
            s_field = "ms";
            break;
         }

         switch (i_opcode >> 5)
         {
         case 0000: /* 0 -> a[f] */
            if (h_processor->trace) fprintf(stdout, "0 -> a[%s]", s_field);
            v_reg_copy(h_processor, h_processor->reg[A_REG], NULL);
            break;
         case 0001: /* 0 -> b[f] */
            if (h_processor->trace)fprintf(stdout, "0 -> b[%s]", s_field);
            v_reg_copy(h_processor, h_processor->reg[B_REG], NULL);
            break;
         case 0002: /* a exch b[f] */
            if (h_processor->trace) fprintf(stdout, "a exch b[%s]", s_field);
            v_reg_exch(h_processor, h_processor->reg[A_REG], h_processor->reg[B_REG]);
            break;
         case 0003: /* a -> b[f] */
            if (h_processor->trace) fprintf(stdout, "a -> b[%s]", s_field);
            v_reg_copy(h_processor, h_processor->reg[B_REG], h_processor->reg[A_REG]);
            break;
         case 0004: /* a exch c[f] */
            if (h_processor->trace) fprintf(stdout, "a exch c[%s]", s_field);
            v_reg_exch(h_processor, h_processor->reg[A_REG], h_processor->reg[C_REG]);
            break;
         case 0005: /* c -> a[f] */
            if (h_processor->trace) fprintf(stdout, "c -> a[%s]", s_field);
            v_reg_copy(h_processor, h_processor->reg[A_REG], h_processor->reg[C_REG]);
            break;
         case 0006: /* b -> c[f] */
            if (h_processor->trace) fprintf(stdout, "b -> c[%s]", s_field);
            v_reg_copy(h_processor, h_processor->reg[C_REG], h_processor->reg[B_REG]);
            break;
         case 0007: /* b exchange c[f] */
            if (h_processor->trace) fprintf(stdout, "b exch c[%s]", s_field);
            v_reg_exch(h_processor, h_processor->reg[B_REG], h_processor->reg[C_REG]);
            break;
         case 0010: /* 0 -> c[f] */
            if (h_processor->trace) fprintf(stdout, "0 -> c[%s]", s_field);
            v_reg_copy(h_processor, h_processor->reg[C_REG], NULL);
            break;
         case 0011: /* a + b -> a[f] */
            if (h_processor->trace) fprintf(stdout, "a + b -> a[%s]", s_field);
            v_reg_add(h_processor, h_processor->reg[A_REG], h_processor->reg[A_REG], h_processor->reg[B_REG]);
            break;
         case 0012: /* a + c -> a[f] */
            v_reg_add(h_processor, h_processor->reg[A_REG], h_processor->reg[A_REG], h_processor->reg[C_REG]);
            if (h_processor->trace) fprintf(stdout, "a + c -> a[%s]", s_field);
            break;
         case 0013: /* c + c -> c[f] */
            if (h_processor->trace) fprintf(stdout, "c + c -> c[%s]", s_field);
            v_reg_add(h_processor, h_processor->reg[C_REG], h_processor->reg[C_REG], h_processor->reg[C_REG]);
            break;
         case 0014: /* a + c -> c[f] */
            if (h_processor->trace) fprintf(stdout, "a + c -> c[%s]", s_field);
            v_reg_add(h_processor, h_processor->reg[C_REG], h_processor->reg[C_REG], h_processor->reg[A_REG]);
            break;
         case 0015: /* a + 1 -> a[f] */
            if (h_processor->trace) fprintf(stdout, "a + 1 -> a[%s]", s_field);
            v_reg_inc(h_processor, h_processor->reg[A_REG]);
            break;
         case 0016: /* shift left a[f] */
            if (h_processor->trace) fprintf(stdout, "shift left a[%s]", s_field);
            fflush(stdout);
            v_reg_shl(h_processor, h_processor->reg[A_REG]);
            break;
         case 0017: /* c + 1 -> c[f] */
            if (h_processor->trace) fprintf(stdout, "c + 1 -> c[%s]\t", s_field);
            v_reg_inc(h_processor, h_processor->reg[C_REG]);
            break;
         case 0020: /* a - b -> a[f] */
            if (h_processor->trace) fprintf(stdout, "a - b -> a[%s]", s_field);
            v_reg_sub(h_processor, h_processor->reg[A_REG], h_processor->reg[A_REG], h_processor->reg[B_REG]);
            break;
         case 0021: /* a - c -> c[f] */
            if (h_processor->trace) fprintf(stdout, "a - c -> c[%s]", s_field);
            v_reg_sub(h_processor, h_processor->reg[C_REG], h_processor->reg[A_REG], h_processor->reg[C_REG]);
            break;
         case 0022: /* a - 1 -> a[f] */
            if (h_processor->trace) fprintf(stdout, "a - 1 -> a[%s]", s_field);
            h_processor->flags[CARRY] = True; /* Set carry */
            v_reg_sub(h_processor, h_processor->reg[A_REG], h_processor->reg[A_REG], NULL);
            break;
         case 0023: /* c - 1 -> c[f] */
            if (h_processor->trace) fprintf(stdout, "c - 1 -> c[%s]", s_field);
            h_processor->flags[CARRY] = True; /* Set carry */
            v_reg_sub(h_processor, h_processor->reg[C_REG], h_processor->reg[C_REG], NULL);
            break;
         case 0024: /* 0 - c -> c[f] */
            if (h_processor->trace) fprintf(stdout, "0 - c -> c[%s]", s_field);
            v_reg_sub(h_processor, h_processor->reg[C_REG], NULL, h_processor->reg[C_REG]);
            break;
         case 0025: /* 0 - c - 1 -> c[f] */
            if (h_processor->trace) fprintf(stdout, "0 - c - 1 -> c[%s]", s_field);
            h_processor->flags[CARRY] = True; /* Set carry */
            v_reg_sub(h_processor, h_processor->reg[C_REG], NULL, h_processor->reg[C_REG]);
            break;
         case 0026: /* if b[f] = 0 */
            if (h_processor->trace) fprintf(stdout, "if b[%s] = 0", s_field);
            v_reg_test_eq(h_processor, h_processor->reg[B_REG], NULL);
            v_op_inc_pc(h_processor); /* Increment program counter */
            v_op_goto(h_processor);
            break;
         case 0027: /* if c[f] = 0 */
            if (h_processor->trace) fprintf(stdout, "if c[%s] = 0", s_field);
            v_reg_test_eq(h_processor, h_processor->reg[C_REG], NULL);
            v_op_inc_pc(h_processor); /* Increment program counter */
            v_op_goto(h_processor);
            break;
         case 0030: /* if a >= c[f] */
            if (h_processor->trace) fprintf(stdout, "if a >= c[%s]", s_field);
            v_reg_sub(h_processor, NULL, h_processor->reg[A_REG], h_processor->reg[C_REG]);
            v_op_inc_pc(h_processor); /* Increment program counter */
            v_op_goto(h_processor);
            break;
         case 0031: /* if a >= b[f] */
            if (h_processor->trace) fprintf(stdout, "if a >= b[%s]", s_field);
            v_reg_sub(h_processor, NULL, h_processor->reg[A_REG], h_processor->reg[B_REG]);
            v_op_inc_pc(h_processor); /* Increment program counter */
            v_op_goto(h_processor);
            break;
         case 0032: /* if a[f] <> 0 */
            if (h_processor->trace) fprintf(stdout, "if a[%s] <> 0", s_field);
            v_reg_test_ne(h_processor, h_processor->reg[A_REG], NULL);
            v_op_inc_pc(h_processor); /* Increment program counter */
            v_op_goto(h_processor);
            break;
         case 0033: /* if c[f] <> 0 */
            if (h_processor->trace) fprintf(stdout, "if c[%s] <> 0", s_field);
            v_reg_test_ne(h_processor, h_processor->reg[C_REG], NULL);
            v_op_inc_pc(h_processor); /* Increment program counter */
            v_op_goto(h_processor);
            break;
         case 0034: /* a - c -> a[f] */
            if (h_processor->trace) fprintf(stdout, "a - c -> a[%s]", s_field);
            v_reg_sub(h_processor, h_processor->reg[A_REG], h_processor->reg[A_REG], h_processor->reg[C_REG]);
            break;
         case 0035: /* shift right a[f] */
            if (h_processor->trace) fprintf(stdout, "shift right a[%s]", s_field);
            v_reg_shr(h_processor, h_processor->reg[A_REG]);
            break;
         case 0036: /* shift right b[f] */
            v_reg_shr(h_processor, h_processor->reg[B_REG]);
            if (h_processor->trace) fprintf(stdout, "shift right b[%s]", s_field);
            break;
         case 0037: /* shift right c[f] */
            if (h_processor->trace) fprintf(stdout, "shift right c[%s]", s_field);
            v_reg_shr(h_processor, h_processor->reg[C_REG]);
            break;
         default:
            v_error("Unexpected error in %s line : %d\n", __FILE__, __LINE__);
         }
         break;

      case 03:/* Subroutine calls and long conditional jumps */
         switch (i_opcode & 03) {
         case 00:
            if (h_processor->trace) fprintf(stdout, "call\t%05o", i_opcode >> 2);
            v_error("Unexpected opcode (%04o) at %1o-%04o in %s line : %d\n", i_opcode, h_processor->bank, h_processor->pc, __FILE__, __LINE__);
            break;
         case 01:
            if (h_processor->trace) fprintf(stdout, "call\t%05o", i_opcode >> 2);
            v_error("Unexpected opcode (%04o) at %1o-%04o in %s line : %d\n", i_opcode, h_processor->bank, h_processor->pc, __FILE__, __LINE__);
            break;
         case 02:
            if (h_processor->trace) fprintf(stdout, "jump\t%05o", i_opcode >> 2);
            v_error("Unexpected opcode (%04o) at %1o-%04o in %s line : %d\n", i_opcode, h_processor->bank, h_processor->pc, __FILE__, __LINE__);
            break;
         case 03: /* if nc go to */
            if (h_processor->trace) fprintf(stdout, "if nc go to %05o", (h_processor->pc & 0xff00) | i_opcode >> 2); /* Note - Uses an eight bit address */
            if (!h_processor->flags[PREV_CARRY]) {
               h_processor->pc = (h_processor->pc & 0xff00) | i_opcode >> 2;
               v_delayed_rom(h_processor);
               h_processor->pc--; /* Program counter will be auto incremented before next fetch */
            }
            break;
         default:
            v_error("Unexpected error in %s line : %d\n", __FILE__, __LINE__);
         }
         break;
      default:
         v_error("Unexpected error in %s line : %d\n", __FILE__, __LINE__);
      }
      if (h_processor->trace) {
         fprintf(stdout, "\n");
      }
      h_processor->opcode = i_opcode;
      if (h_processor->trace) debug(v_fprint_registers(stdout, h_processor));
#if defined(HAWKEYE)
      if ((h_processor->pc < 02000) && (h_processor->bank > 0)) h_processor->bank = 0; /* Implicit bank switch !! */
#endif
      v_op_inc_pc(h_processor); /* Increment program counter (also updates CARRY flag) */
   }
}
