#include <Arduino.h>
#include <Keypad.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);
Servo myservo;
const byte ROWS = 4; 
const byte COLS = 4; 

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {6, 8, 7, 9}; 
byte colPins[COLS] = {5, 4, 3, 2}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);



char password = 'A';
char ancienPass = 'A';
char messageToSend[5];

// -------------------------------
bool isRfidOK;
bool isCodeOK;
bool openDoor;
int timer;
// -------------------------------
void setup() {
    isRfidOK = false;
    isCodeOK = false;
    myservo.attach(2);
    Serial.begin(9600);
    SPI.begin();
    mfrc522.PCD_Init();
    // Serial.println("Approximate your card to the reader...");
    // Serial.println();
    myservo.write(90); 
    delay(1500);
}

void loop() {
    Serial.println("--------------------------------------------");
    Serial.println(isRfidOK && isCodeOK);
    Serial.println();
    Serial.println(isRfidOK);
    Serial.println();
    Serial.println(isCodeOK);
    Serial.println();
    Serial.println(password);
    Serial.println();

    char customKey = customKeypad.getKey();
  
    if (customKey){
        ancienPass = password;
        // pass 65656
        password = ancienPass + customKey;
        // Serial.println("password :");
        // Serial.println(password);
    }
    // Serial.println(password);
    // if(password == '65656') {
    if(password == 'M') {
        messageToSend[5] = 'code';
        // Serial.write(messageToSend,5);
        Serial.println("code ok");
        // password = 'A';
        isCodeOK = true;
    }


        Serial.print("AAAAAAAAAAAAAAAAAAAA ");
        Serial.print(isCodeOK);
        Serial.print(" ");
        Serial.println(isRfidOK);
    if(isCodeOK){
        Serial.println("isCodeOK");
        if(isRfidOK){
            openDoor = true;
            Serial.println("open door");
            digitalWrite(2, HIGH);
            myservo.write(0);                  // sets the servo position according to the scaled value
            delay(1500);
        }
    }


    // Look for new cards
    
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
    // Serial.print("UID tag :");
    String content= "";
    byte letter;
    for (byte i = 0; i < mfrc522.uid.size; i++) 
    {
        // Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        // Serial.print(mfrc522.uid.uidByte[i], HEX);
        content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
        content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    // Serial.println();
    // Serial.print("Message : ");
    // Serial.println("content.substring(1) : ");
    // Serial.println(content[1]);
    // content.toUpperCase();
    if (content[1] == '2')
    // if (content.substring(1) == "2C DC 2B 5B")
    {
        Serial.println("Authorized access");
        timer = 60;
        isRfidOK = true;
        // Serial.println();
        messageToSend[5] = 'rfid';
        // Serial.write(messageToSend,5);
    } else   {
        Serial.println(" Access denied");
    }

    delay(1000);
}