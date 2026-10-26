# WizLite
<img width="914" height="621" alt="Screenshot 2026-10-24 173543" src="https://github.com/user-attachments/assets/d920de82-361b-4124-9a77-729ef37f9137" />
A small, 2 player capable, ch552 based, game console with a 14x9 rgb led screen. 

## Design and Inspo:
I while back, I ordered some parts from LCSC, and as a part of that order I got a discounted CH552g chip. So, I wanted to find something cool and fun to do with it. My "solution", make a 'simple' two player game controller. 

In the process of building this, I made a number of decisions that made what was supposed to be a simple game controller into... an overengineered, ambitious project. For example, a simple OLED screen would have worked but did I do that, no. I wanted to make a small matrix of LEDS, which then became a matrix of rgb LEDS, which then finally became a tiny (807 size) 14x9 LED matrix. Additionally, the form factor of this whole project is VERY small (only around 75x50mm), making it even more interesting ;) 

## PCB:
For this project I made a two-layer PCB on Kicad, I limited myself to two layers in order to reduce the cost of the pcb manufacturing and to make it simpler. The pcb is lightly bigger than 75x50mm, and is designed with the LED matrix screen in the middle, the buttons on the top and bottom, Ch552 + passives to the right, and USB + passives to left. 

On the pcb, I added some text to demarcate the player one and player two sides, and I also added some pcb art to the front and back for *aesthetics*. 

### Schematic: 
<img width="1301" height="869" alt="Screenshot 2026-10-25 215401" src="https://github.com/user-attachments/assets/29a12721-f082-4965-a5d5-464b1799fe13" />

### PCB Layout:
<img width="634" height="678" alt="Screenshot 2026-10-25 215342" src="https://github.com/user-attachments/assets/a98c6055-4368-4b4d-9a02-b8e70f02e924" />

### Assembly/Ordering: 
The LED pads used on the pcb 

## Firmware:
This project has the main firmware written in C, for the Arduino IDE, using the ch55xdunio library. Using C was new for me, but it was very similar to C++, and I was able to get a hang of it decently quickly. The basic idea is that the firmware drives the 9x14 RGB LED matrix, reads the user buttons, and executes game instructions. The firmware was (in my opinion) the hardest part of this project, but it also what I am most proud of. Anyway, here is the full breakdown of the code and other details about it.

#### Games (only one for now):
- Snake (single player)
- More TBD

Hold Player 1 LEFT + RIGHT for 2 seconds to leave a game and return to the menu.

#### How it works (like very high level) :
- **Menu:** A carousel of game cover pictures. LEFT/RIGHT pans, UP starts the selected game.
- **Screen:** screenUpdate(x, y, r, g, b) sets a pixel with (0,0) at the top left. It handles the split display, the zigzag LED wiring and a brightness limit so USB power limits aren't excided, and stores the result directly in the LED buffer. screenShow() sends that to the LEDs to acutally turn it on.
- **Pictures:** Each picture (like on the game cover carousel) has its own list of colors plus 14 rows of 9 characters. Use PictureMaker.html to draw one or import it from a Figma screenshot, and it writes the code for you. **NOTE:** This part (and only this part) of the code was made fully using AI, it looks vide coded, and that is because it is. Please understand if there are small bugs in it.
- **Text:** I added a 3x5 font (A-Z, 0-9) that can be called separately or with micro:bit style scrolling for longer texts.

#### Files:
| File | Purpose |
|---|---|
| `WizGameLite.ino` | Game menu and game list |
| `WizBase.c / .h` | Screen, buttons, text, shared pictures |
| `Snake.c / .h` | Snake game and its cover picture |
| `PictureMaker.html` | Tool for making pictures (opens in a browser, make sure javascript is enabled) |

#### PictureMaker:
<img width="1686" height="897" alt="Screenshot 2026-10-22 011710" src="https://github.com/user-attachments/assets/8add4a06-d2b9-41f1-a8e4-2f2c4fdc680f" />

#### Adding a game:
See the "HOW TO ADD A GAME" comment in `WizGameLite.ino`.

#### AI Usage: 
The code for the actual controller was fully coded by me (with only limited debug and learning help from AI). However, the online picture maker was fully vibe code, and it was also the first time I ever have vibe coded :)

## BOM: 
| Part | Designators | Quantity | Retailer | Total Price |
|------|-------------|----------|----------|-------------|
| XL-0807RGBC-2812B-S (WS2812B-style addressable RGB LED, 2.0x1.8mm) `C41413181` | U2–U127 | 126x5 (640 ordered) | LCSC | $13.38 |
| 47uF ±10% 16V Ceramic Capacitor, 1210 (CS3225X7R476K160NRL) `C5440143` | C3, C4 | 2x5 | LCSC | $2.23 |
| 10kΩ ±1% 0805 Resistor (0805W8F1002T5E) `C17414` | R1–R4, R7–R11 | 9x5 (100 MOQ) | LCSC | $0.34 |
| 100n Capacitor, 0805 | C1, C2 | 2x5 | Already have | $0.00 |
| 330Ω Resistor, 0805 | R12, R13 | 2x5 | Already have | $0.00 |
| 5.1kΩ Resistor, 0805 | R5, R6 | 2x5 | Already have | $0.00 |
| CH552G Microcontroller (SOIC-16) | U1 | 1x5 | Already have | $0.00 |
| USB-C Receptacle (HCTL HC-TYPE-C-16P-01A) | J1 | 1x5 | Already have | $0.00 |
| SMD Push Button Switch (TS-1088-xR020) | SW1–SW9 | 9x5 | Already have | $0.00 |
| Test Point Pad (D1.0mm) | TP1–TP4 | 4x5 | Already have | $0.00 |
| PCB (5x boards, Black, 1.6mm, OSP) | – | 1x5 | JLCPCB | $4.00 |
| SMT Stencil (75x100mm, no frame, top) | – | 1 | JLCPCB | $3.10 |

**Parts Subtotal:** LCSC ($15.95, after $1.15 discount) + JLCPCB ($7.10) = $23.05

**Shipping:** LCSC ($9.77) + JLCPCB ($11.83) = $21.60

**Total Cost For 5 PCBs: $44.65**



