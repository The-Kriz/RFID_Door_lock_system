// libraries used
#include <SPI.h>
#include <MFRC522.h>


//pins & attribute values
#define relay 8
#define SS_PIN 10
#define RST_PIN 9
#define ACCESS_DELAY 10000 // time the door should be opened for
#define DELAY 5000
MFRC522 mfrc522(SS_PIN, RST_PIN);
 
void setup() 
{
  Serial.begin(9600);  
  SPI.begin();          
  mfrc522.PCD_Init();  

  digitalWrite(relay, LOW);

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
  if (content.substring(1) == "69 C8 E2 2A") //UID of the card that need the access to the room
  {
    Serial.println("Authorized access");
    Serial.println();
    digitalWrite(relay, HIGH);
    delay(ACCESS_DELAY);
    digitalWrite(relay, LOW);
    delay(DELAY);

  }
 
 else   {
    Serial.println(" Access denied");
 

    delay(DELAY);

 
  }
}
