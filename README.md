# Hyper C

Hyper C is a program development system published by WSM Group Inc. and David B. McClain around 1985-1986 for 6502 and 65c02 Apple II computers.  The source files reflect the text and directory structure originally provided.  

The technical reference manual I obtained and scanned can be retrieved from https://mirrors.apple2.org.za/Apple%20II%20Documentation%20Project/Software/Languages/C/WSM%20Group%20HyperC/ as well as a few other archives.  The binaries have been archived at various ftp sites in the past and can be found by searching https://mirrors.apple2.org.za .  Various utilities and libraries may also be found there.  Apple IIgs users can use HC.SYSTEM by Gary Desrochers to run Hyper C in 6502 emulation mode on those machines.

As originally implemented Hyper C was developed for a proprietary disk operating system called CDOS for 5 1/4" floppy disks similar to but not compatible with Apple II DOS 3.3.  A later version was released for Apple II ProDOS and subsequently enhanced to utilize the Apple II SANE floating point math package.

As documented, the Hyper C compiler generates pseudo-code for a hypothetical 16 bit processor similar to but not the same as Steve Wozniak's Sweet 16 or the 6502 FIG Forth by W. F. Ragsdale based on Charles H. Moore's Forth.  The pseudo-code is then processed by an interpreter referred to as the C Engine.  The C Engine is entered by using the software interrupt BRK opcode of the 6502.  The source code for the C engine may be examined by reading OPSYS/INT65.S.  The structure and behavior of the hypothetical 16 bit processor is documented in the manual referenced above.
