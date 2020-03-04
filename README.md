# Morse-Code-NXP-KL46Z
Implements a Morse Code application using NXP's FRDM KL46Z board by flashing the message using the Green on-board LED.

Dots are 250 ms, Dashes are 750 ms. Led is turned off between any two symbols (dot or dash) for 250 ms.
Delay between two letters in a message is 750 ms. Delay between two words is 1 sec.

After successful flashing of the specified message, the Red on-board LED turns on to indicate completion of one iteration.
