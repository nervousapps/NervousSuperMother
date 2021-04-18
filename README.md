# NervousSuperMother

Current version : 0.2.0

This work is inspired (a lot) by https://github.com/ghostintranslation work on his motherboards.

Thanks for that great inspiring work !

Platform built around Teensy 3.5/3.6 and adapted for 4.1 with DaddyBoard addon PCB.

It is mainly a development platform using almost all ports of Teensy.

# ModularGrid : https://www.modulargrid.net/e/other-unknown-nervoussupersynth

![Image of NervousSuperSynth bare pcb](https://github.com/nervousapps/NervousSuperSynth/raw/proto_1/images/bare_pcb.png)

![Image of NervousSuperSynth monster vs proto_1 1](https://github.com/nervousapps/NervousSuperSynth/blob/proto_1/images/monstervsproto1_1.png)

![Image of NervousSuperSynth monster vs proto_1 2](https://github.com/nervousapps/NervousSuperSynth/blob/proto_1/images/monstervsproto1_2.png)

![Image of NervousSuperSynth monster vs proto_1 3](https://github.com/nervousapps/NervousSuperSynth/blob/proto_1/images/monstervsproto1_3.png)

![Image of NervousSuperSynth monster vs proto_1 4](https://github.com/nervousapps/NervousSuperSynth/blob/proto_1/images/monstervsproto1_4.png)

# I/O
- 2 trigger/gate input
- 4 CV input
- MIDI IN
- SGTL5000 input
- SGTL5000 output with analog volume control
- PT8211 DAC stereo output
- LED screen 20x2 with contrast control

# Analog controls
- 10 sliders
- 5 potentiometers
- 3 switches

# Digital controls
- 2 encoders with pushbutton

# Hardware controls
- RESET
- PROGRAM

# Driver
https://github.com/nervousapps/NervousSuperMother/tree/withdaddy

# Firmware
There are several firmwares for this motherboard:
- NervousSuperSynth (https://github.com/nervousapps/NervousSuperSynth/tree/common_base)
- MicroDexed (https://codeberg.org/nervousapps/MicroDexed/src/branch/nervoussupermother)

# Compilation
PlatformIO or Teensyduino

# SCHEMATICS & PCB
NervousSuperMother : https://oshwlab.com/xtreemechille/supersynth-copy
![Image of NervousSuperSynth monster vs proto_1 3](https://image.easyeda.com/histories/3226f8f64cd142eb9272e89193aaf473.png)

SuperDaddyBoard : https://oshwlab.com/xtreemechille/supersynth-copy_copy
![Image of NervousSuperSynth monster vs proto_1 3](https://image.easyeda.com/histories/089716ea3f2345f78fa68dfbae1b574d.png)

SMD and TH parts, one PCB.

I have some PCBs in stock, assembled, kit or bare, contact me at nervousrvoltage@icloud.com if you are interested.

# HARDWARE FIX
- One slider is not connected

# BOM
https://docs.google.com/spreadsheets/d/1-4hl6AcAoJYJrRvISyrwIaEGTYHwAB_IcKs__rtEGYY/edit?usp=sharing

# PANEL
https://cad.onshape.com/documents/76309e780cb99c0f2066b519/w/a28c1d39745cdb4e42b1148c/e/933dfecdfdc7ae9463372842

I made the panel here : https://www.john-steel.com/

# TODO
- clean code
- separate switch and potentiometers from sliders
