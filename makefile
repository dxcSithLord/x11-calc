#
#  makefile - RPN (Reverse Polish) calculator simulator.
#
#  Copyright(C) 2019 - MT
#
#  makefile.
#
#  This  program is free software: you can redistribute it and/or modify it
#  under  the terms of the GNU General Public License as published  by  the
#  Free  Software Foundation, either version 3 of the License, or (at  your
#  option) any later version.
#
#  This  program  is distributed in the hope that it will  be  useful,  but
#  WITHOUT   ANY   WARRANTY;   without even   the   implied   warranty   of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
#  Public License for more details.
#
#  You  should have received a copy of the GNU General Public License along
#  with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#	Note seperator (tab) at the beginning of the line CANNOT be a space..!
#
#	01 May 13   0.1	- Initial version - MT
#  02 Jul 13         - Seperated code and headers for button and colour
#                      manipulation into separate files - MT
#  09 Mar 14   0.2   - Seperated  code for different calculator models into
#                      a  seperate files (instead of using a single  source
#                      file with conditional compilation - MT
#  27 Sep 20   0.3   - Added make clean option - MT
#  08 Aug 21   0.4   - Removed x11-calc-format - MT
#  11 Aug 21	0.5	- Backup created in parent folder - MT
#	12 Sep 21	0.6	- Added '-no-pie' option so that the resulting program
#							  can be executed by double clicking on it - MT
#	23 Sep 21	0.7	- Model number can now be selected just by setting the
#							  MODEL number in this file - MT
#  26 Sep 21	0.8	- Added images to list of files (to backup) - MT
#  30 Sep 21	0.9	- Builds label and switch 'classes' - MT
#   3 Oct 21	0.10	- Fixed issue with backup file name - MT
#

MODEL		= 21
PROGRAM	= x11-calc
SOURCES 	= x11-calc.c x11-calc-cpu.c x11-calc-display.c x11-calc-segment.c x11-calc-button.c x11-calc-switch.c x11-calc-label.c x11-calc-colour.c x11-keyboard.c gcc-wait.c
FILES		= *.c *.h LICENSE README.md makefile x11-calc-instruction-set.md .gitignore .gitattributes
FILES		+= images/x11-calc-*.png
OBJECTS	= $(SOURCES:.c=.o)
OUTPUT	= $(PROGRAM).out
LIBS		= -lX11 -lm
INCLUDE	= .
FLAGS		= -Wall -pedantic -ansi
FLAGS		+= -Wno-comment -Wno-deprecated-declarations
FLAGS		+= -no-pie
#FLAGS		+= -g # For debugging
CC			= gcc

all: clean $(PROGRAM)-$(MODEL) $(OBJECTS)
FLAGS		+= -D HP$(MODEL)
SOURCES 	+= x11-calc-$(MODEL).c

$(PROGRAM)-$(MODEL): $(OBJECTS)
	@$(CC) $(FLAGS) $(OBJECTS) -o $@ $(LIBS)

$(OBJECTS) : $(SOURCES)
	@echo $(PROGRAM)-$(MODEL)
#	@echo $(CC) $(FLAGS) -c $(SOURCES)
	@$(CC) $(FLAGS) -c $(SOURCES) -I $(INCLUDE)

clean:
	@rm -f *.o
	@rm -f $(OBJECTS)
	@rm -f $(PROGRAM)

backup:
	@echo "$(PROGRAM)-`date +'%Y%m%d%H%M'`.tar.gz"; tar -czpf ..\/$(PROGRAM)-`date +'%Y%m%d%H%M'`.tar.gz $(FILES)

