/*
 * -------------------------------------
 *             MFRC522      Arduino     
 *             Reader/PCD   Uno/101     
 * Signal      Pin          Pin         
 * -------------------------------------
 * RST/Reset   RST          9           
 * SPI SS      SDA(SS)      10          
 * SPI MOSI    MOSI         11 / ICSP-4 
 * SPI MISO    MISO         12 / ICSP-1 
 * SPI SCK     SCK          13 / ICSP-3 
 */

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN 10
#define BUZZER_PIN 3
#define BUZZER_DURATION 150
#define BUZZER_TONE 800

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

void setup()
{
    Serial.begin(115200); // Initialize serial communications with the PC
    while (!Serial)
    {
        // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
    }

    SPI.begin();                       // Init SPI bus
    mfrc522.PCD_Init();                // Init MFRC522
    mfrc522.PCD_DumpVersionToSerial(); // Show details of PCD - MFRC522 Card Reader details

    pinMode(BUZZER_PIN, OUTPUT); // Init buzzer
}

void loop()
{
    // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
    if (!mfrc522.PICC_IsNewCardPresent())
    {
        return;
    }

    // Select one of the cards
    if (!mfrc522.PICC_ReadCardSerial())
    {
        return;
    }

    // Print uid
    Serial.print("UID:");
    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
        if (mfrc522.uid.uidByte[i] < 0x10)
            Serial.print(F(" 0"));
        else
            Serial.print(F(" "));
        Serial.print(mfrc522.uid.uidByte[i], HEX);
    }
    Serial.println();

    // Stop processing current picc
    mfrc522.PICC_HaltA();

    // Sound
    analogWrite(BUZZER_PIN, BUZZER_TONE); // enable buzzer
    delay(BUZZER_DURATION);
    analogWrite(BUZZER_PIN, 0); // disable buzzer
}
