// libraries used
#include <SPI.h>
#include <MFRC522.h>
#include <SD.h> 

//pins & attribute values
#define RELAY 8
#define SS_PIN 10 // RFID
#define RST_PIN 9 // RFID
#define CS_SD 4   // SD card
#define ACCESS_DELAY 5000 // time the door should be opened for
#define DELAY 5000
#define members 3 // enter number of people who has access

// User Tag Id and User name 
const char* verified_rfid[] = 
{
"70 82 7A 89",
"1B 6F B3 73",
"14 8F CE 33",
"54 43 C3 33"
} ;// Enter UID of the cards that need the access to the room
const char* employ[]=
{
"AKSHAY ANTONY",
"NIVED K",
"HARIKRISHNAN M",
"ARYAN ATHUL"
};

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() 
{
  pinMode(RELAY, OUTPUT);
  Serial.begin(9600);  
  SPI.begin();          
  mfrc522.PCD_Init();  
  digitalWrite(RELAY, LOW);
  Initialize_SDcard();
  Serial.println("Put your card to the reader...");
  Serial.println();
}
void loop()
{
  // Look for new card on the RFID reader 
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
    {
     return;
    }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
    {
     return;
    }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  bool foundTag = false;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
    {
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
      content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
      content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  File data = SD.open("Attendance.txt", FILE_WRITE);
  
  for(int j = 0;j < members ;j++)
    {      
      if (content.substring(1) == verified_rfid[j])
        {
          Serial.println("Authorized access");
          Serial.println(employ[j]);
          foundTag = true;
          if (data) 
          {
             data.println("");
             data.print("Authorized access"); //Stores access status on SD card
             data.print(","); //Move to next column using a ","
             data.print(verified_rfid[j]); //Store user id on SD card
             data.print(","); //Move to next column using a ","
             data.print(employ[j]); //Store date on SD card
             data.print(","); //Move to next column using a ","
             data.close();
           }
          else
           {
             Serial.println("OOPS!! SD card writing failed");
           }
          digitalWrite(RELAY, HIGH);
          delay(ACCESS_DELAY);
          digitalWrite(RELAY, LOW);
          delay(DELAY);
        }}
      if (!foundTag)
          {
            Serial.println(" Access denied");
            Serial.println("Unauthorized User");
            if (data) 
          {
             data.println("");
             data.print("Unauthorized access"); //Stores access status on SD card
             data.print(","); //Move to next column using a ","
             data.print(content.substring(1)); //Store user id on SD card
             data.print(","); //Move to next column using a ","
             data.print("Unknown User"); //Store date on SD card
             data.print(","); //Move to next column using a ","
             data.close();
           }
          else
           {
             Serial.println("OOPS!! SD card writing failed");
           }
            delay(DELAY);
          }
        
}

void Initialize_SDcard()
{
  // see if the card is present and can be initialized:
  if (!SD.begin(CS_SD)) 
  {
    Serial.println("Card failed, or not present");
    return;
  }
  File dataFile = SD.open("Attendance.txt", FILE_WRITE);
  if (dataFile) 
  {
    dataFile.println("Access,Tag_id,Name"); //Write the first row of the excel file
    Serial.println("File found");
    dataFile.close();
  }
}
