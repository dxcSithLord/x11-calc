#
#  makefile - RPN (Reverse Polish) calculator simulator.
#
#  Copyright(C) 2019 - MT
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
#	Note separator (tab) at the beginning of the line CANNOT be a space..!
#
#	09 Oct 21   0.1   - Initial version - MT
#  22 Nov 21   0.2   - Added data files to backups - MT
#  25 Nov 21	0.3	- Added HP34C - MT
#

PROGRAM	=  x11-calc
FILES		=  ./src/*.c ./src/*.h ./prg/*.dat ./src/makefile
FILES		+= *.md LICENSE makefile .gitignore .gitattributes
FILES		+= ./img/x11-calc-*.png
MAKE		=  make


all: clean hp21 hp22 hp25 hp27 hp29 hp31 hp32 hp33 hp34

hp21:
	@$(MAKE) --no-print-directory -C ./src MODEL=21

hp22:
	@$(MAKE) --no-print-directory -C ./src MODEL=22

hp25:
	@$(MAKE) --no-print-directory -C ./src MODEL=25

hp27:
	@$(MAKE) --no-print-directory -C ./src MODEL=27

hp29:
	@$(MAKE) --no-print-directory -C ./src MODEL=29

hp31:
	@$(MAKE) --no-print-directory -C ./src MODEL=31

hp32:
	@$(MAKE) --no-print-directory -C ./src MODEL=32

hp33:
	@$(MAKE) --no-print-directory -C ./src MODEL=33

hp34:
	@$(MAKE) --no-print-directory -C ./src MODEL=34

hp37:
	@$(MAKE) --no-print-directory -C ./src MODEL=37

#hp11:
#	@$(MAKE) --no-print-directory -C ./src MODEL=11

clean:
	@rm  -f ./src/*.o

backup:
	@echo "$(PROGRAM)-`date +'%Y%m%d%H%M'`.tar.gz"; tar -czpf ..\/$(PROGRAM)-`date +'%Y%m%d%H%M'`.tar.gz $(FILES)

