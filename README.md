# **PS2 Plus**

A PS2 controller emulator for PIC16F microcontroller. Thank you to mcranford13/Matthew for the initial code base as this is a fork of his project with continuation. I've debugged the PS2 protocol and increased the compatibility across PS2 games. More testing will be done to see if there are any remaining bugs. 

## **Build**

You will need a pickit3, MPLAB (_free_), and XC8 compiler to flash this. To build from source, open the firmware project in MPLAB and click 'run'. This will compile the project and then initiate the transfer to the mcu.

## **Analog Stick Setup**

> 1. Press Select, L1, and L2 to enter configuration mode. The PS2 should say that there is no controller connected (if you are in a game).
> 2. Move the analog sticks slowly in a circle.
> 3. Press Select, L1, and R1 to exit. The PS2 should now recognize the controller.
> 4. Reboot the controller (rebooting the ps2 works too).
> Analog sticks should now be working.

To set the analog sticks back to the default values:
> 1. Press Select, L1, and L2 to enter configuration mode.
> 2. Press L2, R2, Select, Start.
> 3. Reboot controller (or ps2).

## **Hardware**

A new pcb with new pin mapping will be uploaded soon. 

## **Acknowledgements**

This project has only moved forward due to these individuals and sites (in no particular order):

1. [Curious Inventor](http://store.curiousinventor.com/guides/PS2) - documentation of the PS2 controller Protocol
2. [Scanlime](https://gist.github.com/scanlime/5042071) - documentation of the PS2 controller protocol
3. [Aurelio Manara](https://github.com/Aurelio92/GCPlus) - for answering countless questions and advice.
4. [Gman](https://gmanmodz.com/) - for initial interest in this project and countless contributions to it.
5. [PS2Dev Team](https://github.com/ps2dev) - for all the hard and amazing work on homebrew for the PS2.

