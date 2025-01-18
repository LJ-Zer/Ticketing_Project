#include <SPI.h>
#include <MFRC522.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SS_PIN 5
#define RST_PIN 0
LiquidCrystal_I2C lcd(0x27,16,2);  
MFRC522 mfrc522(SS_PIN, RST_PIN);   

String lastRfid = "";
unsigned long lastTimestamp = 0;

char ssid[] = "CvEX-U";
char pass[] = "CvEX-U_pass";

void setup() 
{
  Serial.begin(115200);    
  lcd.init();
  lcd.clear();         
  lcd.backlight();      
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    lcd.clear();
    Serial.println("No Connection!");
    lcd.setCursor(1,0);   
    lcd.print("Connecting....");
    delay (1500);
  }

  Serial.println("WiFi Connected");
  lcd.setCursor(1,0);   
  lcd.print("WiFi Connected");
  delay (1500);
  lcd.clear();

  SPI.begin();          
  mfrc522.PCD_Init();   
  lcd.setCursor(5,0);   
  lcd.print("CvEX-U");
  lcd.setCursor(3,1);   
  lcd.print("RFID Ready");

  Serial.println();
}

void loop() 
{
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  
  String rfid_code = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
    rfid_code.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    rfid_code.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  rfid_code.toUpperCase();  

  unsigned long currentTimestamp = millis();  


  if (rfid_code == lastRfid && (currentTimestamp - lastTimestamp < 30000)) {
    lcd.clear();
    lcd.setCursor(1,0);   
    lcd.print("Please proceed");
    lcd.setCursor(3,1);   
    lcd.print("to payment");
    delay (1500);
    lcd.clear();
    lcd.setCursor(5,0);   
    lcd.print("CvEX-U");
    lcd.setCursor(3,1);   
    lcd.print("RFID Ready");
    return;  
  }

  int remarks = 0;
  lcd.clear();
  lcd.setCursor(1,0);  
  lcd.print("Processing....");

  Serial.println(rfid_code);
  Serial.println(remarks);
  Sheet(rfid_code, remarks);
  delay(3000);

  lcd.clear();
  lcd.setCursor(1,0);   
  lcd.print("Please proceed");
  lcd.setCursor(3,1);   
  lcd.print("to payment");
  delay (1500);
  lcd.clear();

  lcd.setCursor(5,0);   
  lcd.print("CvEX-U");
  lcd.setCursor(3,1);   
  lcd.print("RFID Ready");

  lastRfid = rfid_code;
  lastTimestamp = currentTimestamp;
}

void Sheet(String rfid_code, int remarks) 
{
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