#include <SPI.h>
#include <MFRC522.h>
#include <HTTPClient.h>
#include <WiFi.h>
#define SS_PIN 5
#define RST_PIN 0

MFRC522 mfrc522(SS_PIN, RST_PIN);   

String lastRfid = "";
unsigned long lastTimestamp = 0;

char ssid[] = "HG***1*455**V5";
char pass[] = "QingtubolMF2S";

void setup() 
{
  Serial.begin(115200);   
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");

  SPI.begin();          
  mfrc522.PCD_Init();  
  Serial.println("Approximate your card to the reader...");
  Serial.println();
}

void loop() 
{
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
  
  // Show UID on serial monitor
  String rfid_code = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
    rfid_code.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    rfid_code.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  rfid_code.toUpperCase();  // Convert to uppercase

  unsigned long currentTimestamp = millis();  

  if (rfid_code == lastRfid && (currentTimestamp - lastTimestamp < 5000)) {
    Serial.println("You already tapped");
    return;  
  }

  int remarks = 1;
  Serial.print("Exit");
  Serial.println(rfid_code);
  Serial.println(remarks);
  Sheet(rfid_code, remarks);
  
  delay(5000); 

  lastRfid = rfid_code;
  lastTimestamp = currentTimestamp;
}

void Sheet(String rfid_code, int remarks) 
{
    // Remove leading and trailing whitespace
    rfid_code.trim();
    rfid_code.replace(" ", "");

    Serial.println(rfid_code);
    Serial.println(remarks);
    String SCRIPT_ID = "AKfycbxevZZkoOykOqfb0ZGqLEJntyzc_3wjS_3IBO3Lf-ThnigO4H3xH7GD_ZglYjHae60";
    String url = "https://script.google.com/macros/s/" + SCRIPT_ID + "/exec?"
                 "&rfid_code=" + rfid_code +
                 "&remarks=" + String(remarks);

    Serial.print("Generated URL: ");
    Serial.println(url);

    HTTPClient http;
    http.begin(url.c_str());
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);

    int httpCode = http.GET();
    if (httpCode > 0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpCode);
        String payload = http.getString();
        Serial.print("Payload: ");
        Serial.println(payload);
    } else {
        Serial.print("HTTP Error: ");
        Serial.println(http.errorToString(httpCode));
    }

    http.end();
}