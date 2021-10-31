## x11-calc - Another RPN (Reverse Polish) calculator.

![HP21](./img/x11-calc-21.png) ![HP22](./img/x11-calc-22.png) ![HP25](./img/x11-calc-25.png)

![HP27](./img/x11-calc-27.png) ![HP29](./img/x11-calc-29.png)

![HP31](./img/x11-calc-31.png) ![HP32](./img/x11-calc-32.png) ![HP33](./img/x11-calc-33.png)

Written in C using X11.

Use  of any language extensions or non standard language features has  been
avoided in order to try to make the code as portable as possible. Currently
it will only compile without modification on Linux (Debian 10), but as with
earlier  versions of the intention is to be able to compile the code on VMS
running on either VAX or Alpha processors, and Tru64 Unix running on Alpha.

### Latest News

Added continuous memory to models that support it.  The contents of program
and  data registers are saved in a hidden file in the users' HOME directory
when the program exits or the calculator is switched off, and restored when
the application is loaded or is reset using 'Ctrl-C'.

### Status

Currently very much work in progress. The CPU simulation code is now mostly
complete but several important features required by later calculator models
have not yet be implemented.

##### HP 21 - Working

##### HP 22 - Working
The  following issues were observed when checking the behaviour against the
examples in the owners handbook..
* The examples on pages 65, 66 and 76 only work if END is selected.

##### HP 25 - Working (now with continuous memory)

##### HP 27 - Completed (not finished testing)

##### HP 29 - Working
* All registers behave as continuous memory (not correct)

##### HP 31 - Working

##### HP 32 - Completed (not finished testing)
* Hangs after self test

##### HP 33 - Working

### Compiling

To  build the simulator on Linux check that you have all the  prerequisites
installed  then  download the source code from github and unzip it  (a  new
folder  will  be created to automatically).  Then change directory  to  the
new folder run 'make all' to build all the available simulators.

e.g:

    $ wget https://github.com/mike632t/x11-calc/archive/refs/heads/master.zip
    $ unzip master.zip
    $ cd x11-calc-master
    $ make all

    $ ./bin/x11-calc-29
    x11-calc-29: Version 0.4 [Commit ID: 81c55be] 16 Oct 21 21:15:00 (Build: 0067)
    ROM Size : 4096 words

### Keyboard Shortcuts

The following keyboard shortcuts should work:

'0' - '9', '+'. '-'. '*'. '/' and 'Enter' should do what you expect them to
(when using numeric key pad you need to use numlock as usual).

'Esc' is 'Clx', 'c' is CHS, 'e' is 'EEX' and on programmable models 'Space'
corresponds to 'SST'.

'f' and where applicable 'g' and 'h' correspond to the function keys.

'Ctrl-Z'  Quits,  and  'Ctrl-C' does a reset.  For models  with  continuous
memory 'Ctrl-Z' saves the current register contents, and 'Ctrl-C'  restores
them to the original saved state.

### Loading and saving

For  models with continuous memory the contents of the registers and memory
are saved to 'hidden' data file in the user's HOME folder.

    ~/.x11-calc-nn.dat

The  current simulator state is saved when the simulator is switched off or
the program exits.

When  starting the simulator the name of the data file used to restore  the
saved state can be specified on the command line allowing previously  saved
copies of programs to be loaded automatically when the simulator starts.

If a data file is specified on the command line 'Ctrl-C' will use this file
when resetting the simulator to it's initial state, but any changes will be
saved to the data file in the user's HOME folder.

### Debugging

You  can  start the simulation in trace mode using '-t', or in single  step
mode using '-s', and set a breakpoint using '-b <octal address>'.

'Ctrl-T'  also toggles trace mode when running, 'Ctrl-S' executes the  next
instruction, 'Ctrl-Q' resumes execution, and 'Ctrl-R' displays the contents
of the CPU registers .

When in trace mode a jump to the same instruction produces no output.

### Known Issues

The simulators requires a 24 -bit colour display.

On a Raspberry Pi the display is not updated properly if either FKMS or KMS
graphics overlays are enabled.  The following entries in '/boot/config.txt'
should be commented out as shown.

    #dtoverlay=vc4-fkms-v3d
    #dtoverlay=vc4-kms-v3d

### Prerequisites

The following packages are required to build and/or run the simulator.

- Debian : gcc, make, libx11-dev, libc6-dev, xfonts-base

- Ubuntu : gcc, make, libx11-dev, libc6-dev, xfonts-base

- Fedora : gcc, make, libx11-dev, libc6-dev, xorg-x11-xfonts-base

- Gentoo : gcc, make, libx11-dev, libc6-dev, font-misc-misc

- Arch   : gcc, make, libx11-dev, libc6-dev, xorg-fonts-misc ?

- Alpine : gcc, make, libx11-dev, libc6-dev, font-misc-misc ?
