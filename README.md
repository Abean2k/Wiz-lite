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





