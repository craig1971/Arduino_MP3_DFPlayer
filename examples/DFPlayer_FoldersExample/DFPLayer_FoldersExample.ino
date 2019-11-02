/***************************************************
 DFPlayer - Folders Example
 <https://www.dfrobot.com/product-1121.html>
 
 ***************************************************
 This example shows the how the DFPlayer uses files & folders on the SD card
 
 GNU Lesser General Public License.
 See <http://www.gnu.org/licenses/> for details.
 All above must be included in any redistribution
 ****************************************************/

  /* File/Folder Structure for this demo
   *  
   *  /01/001.mp3 -> "one"
   *  /01/002.mp3 -> "two"
   *  /01/003.mp3 -> "three"
   *  /02/001.mp3 -> "four"
   *  /02/002.mp3 -> "five"
   *  /02/003.mp3 -> "six"
   *  /03/001.mp3 -> "seven"
   *  /03/002.mp3 -> "eight"
   *  /03/003.mp3 -> "nine"
   *  /03/004.mp3 -> "ten"
   */
 
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

static uint8_t volume    = 10;  // volume level 0..30
uint8_t nfolders         =  2;  // number of folders on SD card 
uint8_t nfiles           =  3;  // number of mp3 files in each folder on SD card  

static uint8_t ifolder   =  1;  // number of current folder  
static uint8_t ifile     =  1;  // number of file played in current folder 


void setup() {  
   mySoftwareSerial.begin(9600);
   Serial.begin(9600);
  
   Serial.println();
   Serial.println("DFRobot DFPlayer Mini Demo");
   Serial.println("Initializing DFPlayer ... (May take 3~5 seconds)");
  
   if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
      Serial.println("Unable to begin:");
      Serial.println("1.Please recheck the connection!");
      Serial.println("2.Please insert the SD card!");
      while(true);
   }
   Serial.println("DFPlayer Mini online.");
  
   myDFPlayer.setTimeOut(500); //Set serial communictaion time out 500ms
     
   //----Set volume----
   myDFPlayer.volume(volume);
  
   //----Set different EQ----
   myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  
   //----Set device we use SD as default----
   myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
  
   //----Print Folder/File Summary----
   printFileSystemInfo(); 

   nfolders = myDFPlayer.readFolderCounts()-1;  // readFolderCounts() return 1 more than the number of folders, maybe counting root?
   nfiles   = myDFPlayer.readFileCountsInFolder(1);
  
}

void printFileSystemInfo() {   
  
   Serial.print("Player State: ");
   Serial.println(myDFPlayer.readState()); //read mp3 state
  
   Serial.print("Volume: ");
   Serial.println(myDFPlayer.readVolume()); //read current volume
  
   Serial.print("EQ Setting: ");
   Serial.println(myDFPlayer.readEQ()); //read EQ setting  
  
   int folderCount = myDFPlayer.readFolderCounts();
   Serial.print("Number of Folders: ");
   Serial.println(folderCount);

   for (int i = 1; i <= folderCount; i++) {
      int fileCount = myDFPlayer.readFileCountsInFolder(i);
      Serial.print("   ");
      Serial.print(i);
      Serial.print(" : ");
      Serial.print(fileCount);
      Serial.println(" files");
   }
  
  Serial.print("Total Files: ");
  Serial.println(myDFPlayer.readFileCounts());
}

void loop() { 
  
   static unsigned long timer = millis(); 

   if (millis() - timer > 2000) {
      timer = millis();

      //if (myDFPlayer.available()) { <= This will always return false here why???
         printNowPlayingMsg(ifolder, ifile);
      
         myDFPlayer.playFolder(ifolder, ifile); 
             
         delay(200);          // wait a little for the busy line to become LOW
      
         ifile++;  
         if (ifile > nfiles) {  
            ifile = 1;  
            ifolder++;
            if (ifolder > nfolders)
            {
               ifolder = 1;
               ifile = 1;
            }  
              /* 
               nfiles = myDFPlayer.readFileCountsInFolder((uint8_t)ifolder);  <= this is returning 255 everytime, why???
               Serial.print("  ");
               Serial.print(ifolder);
               Serial.print("  ");
               Serial.println(nfiles);    
               */
         }  
      //} 
   }
  
  //if (myDFPlayer.available()) {
  //  printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  //}
  
}

void printNowPlayingMsg(uint8_t folder, uint8_t file) {
  
   Serial.print("Now playing folder: ");
   Serial.print(folder);
   Serial.print(" file: ");  
   Serial.println(file);  
}

void printDetail(uint8_t type, int value) {
  
   switch (type) {
      case TimeOut:
         Serial.println(F("Time Out!"));
         break;
      case WrongStack:
         Serial.println(F("Stack Wrong!"));
         break;
      case DFPlayerCardInserted:
         Serial.println(F("Card Inserted!"));
         break;
      case DFPlayerCardRemoved:
         Serial.println(F("Card Removed!"));
         break;
      case DFPlayerCardOnline:
         Serial.println(F("Card Online!"));
         break;
      case DFPlayerUSBInserted:
         Serial.println("USB Inserted!");
         break;
      case DFPlayerUSBRemoved:
         Serial.println("USB Removed!");
         break;
      case DFPlayerPlayFinished:
         Serial.print(F("Number:"));
         Serial.print(value);
         Serial.println(F(" Play Finished!"));
         break;
      case DFPlayerError:
         Serial.print(F("DFPlayerError:"));
         switch (value) {
            case Busy:
               Serial.println(F("Card not found"));
               break;
            case Sleeping:
               Serial.println(F("Sleeping"));
               break;
            case SerialWrongStack:
               Serial.println(F("Get Wrong Stack"));
               break;
            case CheckSumNotMatch:
               Serial.println(F("Check Sum Not Match"));
               break;
            case FileIndexOut:
               Serial.println(F("File Index Out of Bound"));
               break;
            case FileMismatch:
               Serial.println(F("Cannot Find File"));
               break;
            case Advertise:
               Serial.println(F("In Advertise"));
               break;
            default:
               break;
         }
         break;
      default:
         break;
   }
  
}

