//  
 //  --- example code for using the DFPlayer module ---  
 //  
 // Notice:  
 // All of the information/code here is largely taken from a posting  
 // by user Yerke in the forum at:  
 //  http://forum.banggood.com/forum-topic-59997.html  
 //  
 // This is example code for using DFPlayer in mode #2 (microSD card   
 // with directories 01-99, with filenames 001.mp3-255.mp3)  
 // In this example we assume three files (001.mp3, 002.mp3, 003.mp3  
 // in directory "01" on the SD card.  
 //  
 // This example shows how to  
 // - init DFPlayer  
 // - set the volume  
 // - check the "busy" pin of the DFPlayer  
 // - repeatedly play three MP3 sound files with names 001.mp3, 002.mp3, 003.mp3  
 //  which are stored in directory "01" on the SDcard.  
 //  
 // **************************************************  
 // Very important:  
 // In many setups, the DFPlayer produces noise.
 // To fix this, one should do two things   
 // - The DFPlayer prefers voltages below 5V.  
 //   For me it worked, by connecting it in series with a 1N4001 diode to 5V.  
 // - One should also use serial 1K resistors to connect its TX/RX pins to   
 //   the RX/TX pins of the Arduino  
 //   
 // Connect:  
 // - DFPlayer TX (pin 3) to Arduino pin 10 via a 1k resistor  
 // - DFPlayer RX (pin 2) to Arduino pin 11 via a 1k resistor  
 // - DFPlayer Busy (pin 16) to Arduino pin 12  
 // - DFPlayer speaker output (pins 6,8) to a speaker  
 // - DFPlayer VCC (pin 1) to 5V through a 1N4001 diode  
 // - DFPlayer GND (pin 7 or 10) to GND of the power supply and to Arduino GND  
 // **************************************************  
 //  
 // Wait at least 5 seconds before powering down/up the DFPlayer  
 //  

 const byte pinDfpRX   = 10;  
 const byte pinDfpTX   = 11;  
 const byte pinDfpBusy = 12;  
 
 const byte VOLUME = 10;  
 
 #include "SoftwareSerial.h"  
 SoftwareSerial mySerial(pinDfpRX, pinDfpTX); // RX, TX  

void setup () {  
   Serial.begin(9600);       // for printout  
   mySerial.begin(9600);      // for communication with DFPlayer   
   pinMode(pinDfpBusy, INPUT);   // init Busy pin from DFPlayer (lo: file is playing / hi: no file playing) 

   Serial.println("Initializing DFP");
   dfpInit(pinDfpBusy, VOLUME);
   Serial.println("Done");
}  

void loop () {  
   const  byte nfiles   = 3;   // number of mp3 files on SD card  
   static byte ifile    = 1;   // number of file played next  
   const  byte nfolders = 2;  
   static byte ifolder  = 1;      
  
   if (dfpIsReady()) {    // if no mp3 is playing -> play next file  
      delay(600);  
      printNowPlayingMsg(ifolder, ifile);
      
      dfpExecute(0x0F,ifolder,ifile); // play (0x0F) file [ifile] from folder [folder] 
      delay(200);          // wait a little for the busy line to become LOW
      
      ifile++;  
      if (ifile > nfiles) {  
         ifile = 1;  
         ifolder++;
         if (ifolder>nfolders) 
         {
            ifolder = 1;
            delay(4000);        // add a little delay before repeating the loop
         }      
      }  
    }  
 }  

void printNowPlayingMsg(uint8_t folder, uint8_t file) {
  
   Serial.print("Now playing folder: ");
   Serial.print(folder);
   Serial.print(" file: ");  
   Serial.println(file);  
}


boolean dfpIsReady() 
{
   return digitalRead(pinDfpBusy) == HIGH;
}


void dfpInit(int BUSY_PIN, int volume)
{
   byte dfpVolume = (byte) volume; // set volume of DFPLayer - in range: 0x00-0x30 - default=0x30 
   
   dfpExecute(0x3F, 0x00, 0x00);  // Send request for initialization parameters  
   while (mySerial.available()<10) // Wait until initialization parameters are received (10 bytes)  
   delay(30);            // have >20ms delays between commands  
   dfpExecute(0x06,0x00,dfpVolume); // set volume DL=0x00-0x30, default=0x30  
   delay(30);            // have >20ms delays between commands 
}
 
 
 // --- Excecute the DFPlayer command with two parameters (folder and file numbers)
 void dfpExecute(byte CMD, byte Par1, byte Par2)  
 {   
  # define Start_Byte   0x7E  
  # define Version_Byte  0xFF  
  # define Command_Length 0x06  
  # define Acknowledge  0x00   
  # define End_Byte    0xEF  
  // Calculate the checksum (2 bytes)  
  uint16_t checksum = -(Version_Byte + Command_Length + CMD + Acknowledge + Par1 + Par2);  
  // Build the command line  
  uint8_t Command_line[10] = { Start_Byte, Version_Byte, Command_Length, CMD, Acknowledge,  
         Par1, Par2, highByte(checksum), lowByte(checksum), End_Byte};  
  // Send the command line to DFPlayer  
  for (byte i=0; i<10; i++) mySerial.write( Command_line[i]);  
 } 
