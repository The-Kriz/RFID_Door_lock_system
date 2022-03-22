// libraries used
#include <SPI.h>
#include <MFRC522.h>

//pins & attribute values
#define RELAY 8
#define SS_PIN 10
#define RST_PIN 9
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
"Akashy antony",
"Nived k",
"Harikrishnan M",
"Aryan Athul"
};

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() 
{
  pinMode(RELAY, OUTPUT);
  Serial.begin(9600);  
  SPI.begin();          
  mfrc522.PCD_Init();  
  digitalWrite(RELAY, LOW);

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
  bool foundTag = false ;
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
  
  for(int j = 0;j < members ;j++)
    {
    if (content.substring(1) == verified_rfid[j])
      {
        Serial.println("Authorized access");
        Serial.println(employ[j]);
        foundTag = true;
        Serial.println();
        digitalWrite(RELAY, HIGH);
        delay(ACCESS_DELAY);
        digitalWrite(RELAY, LOW);
        delay(DELAY);
      }}
    if (!foundTag)
        {
          Serial.println(" Access denied");
          Serial.println("Unauthorized User");
          delay(DELAY)
         ;
        }
      
}
