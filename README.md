# Arduino_MP3_DFPlayer
How to use the DF Robot DF Player Mini to play MP3 files with an Arduino

The DFPlayer Mini MP3 Player with built in amplification on a breakout board 
The module can be used as a stand alone module with attached battery, speaker and push buttons or used in combination with microcontroller with RX/TX capabilities.


These modules can be purchased on [Amazon](https://www.amazon.com/gp/product/B01MQD5IIA)

DFPlayer Mini MP3 Player [Overview and Specs](https://wiki.dfrobot.com/DFPlayer_Mini_SKU_DFR0299)

<img src="./images/DFR0299.jpg" width=150>


## Hardware & Setup

<img src="./images/playerMini.png" width=400>

<img src="./images/miniplayer_pin_map.png" width=250> 
<img src="./images/pin_map_desc_en.png" width=400> 

## Copying your mp3 into you micro SD card
The DFPlayer mini has three modes in which mp3 files can be played:
### mode #1:
The files are stored in the root directory of the SD card. The file names do not matter. Files are accessed based on the number corresponding to the order in which they were written to the card. This sounds weird - I agree. I would only use this if I had a small number of files, and the files are not likely to change.
### mode #2:
Files are stored in folders, numbered 01-99, with filenames 001-255.mp3 or 001-255.wav, so you can store up to 99x255=25245 files. The files are accessed based on folder and file number.
### mode #3:
Up to 2999 files are stored in the directory "mp3" with four-digit filenames 0001.mp3-2999.mp3

## Vendory Arduino Library 
Available via Libray Manager in Arduino IDE
[DFRobotDFPlayerMini.h](https://github.com/DFRobot/DFRobotDFPlayerMini/blob/master/DFRobotDFPlayerMini.h)

## Other References and Examples: 
https://reprage.com/post/dfplayer-mini-cheat-sheet

https://www.dfrobot.com/blog-277.html

http://educ8s.tv/arduino-mp3-player/

https://www.youtube.com/watch?v=UodfePdNfg8
