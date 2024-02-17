/***************************************************
  This is an example sketch for our optical Fingerprint sensor

  Designed specifically to work with the Adafruit Fingerprint sensor
  ----> http://www.adafruit.com/products/751

  These displays use TTL Serial to communicate, 2 pins are required to
  interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Adafruit_Fingerprint.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
int up_clicked = 0;
int down_clicked = 0;

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define mySerial Serial2



Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nDelete Finger");

  // set the data rate for the sensor serial port
  finger.begin(57600);

  pinMode(19, INPUT_PULLUP);
  pinMode(18, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);


  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1)
      ;
  }


  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text
  display.println("STARTING...");
  display.display();
}


uint8_t readnumber(void) {
  uint8_t id = 0;
  int x = 0;
  Serial.println("Please type in the ID # (from 1 to 127) you want to delete...");
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println("ENTER ID TO BE DELETED");
  display.display();
  delay(2000);
  while (x == 0) {

    if ((digitalRead(5) == LOW) && up_clicked == 0) {
      id = id + 1;
      up_clicked = 1;
    }
    if ((digitalRead(18) == LOW) && down_clicked == 0) {
      id = id - 1;
      down_clicked = 1;
    }
    if ((digitalRead(5) == HIGH) && up_clicked == 1) {

      up_clicked = 0;
    }
    if ((digitalRead(18) == HIGH) && down_clicked == 1) {

      down_clicked = 0;
    }
    if (digitalRead(19) == LOW) {
      x = 1;
    }
    Serial.println(id);
    display.clearDisplay();
    display.setTextSize(3);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.println(id);
    display.display();
  }
  delay(50);
  return id;
}
void loop()  // run over and over again
{
  
  uint8_t id = readnumber();
  if (id == 0) {  // ID #0 not allowed, try again!
    return;
  }



  deleteFingerprint(id);
  delay(2000);
}

uint8_t deleteFingerprint(uint8_t id) {
  uint8_t p = -1;

  p = finger.deleteModel(id);

  if (p == FINGERPRINT_OK) {
    Serial.println("Deleted!");
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.println("ID DELETED");
    display.display();
    Serial.println(id);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.println("COMMUNICATION ERROR");
    display.display();
    Serial.println(id);
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not delete in that location");
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.println("CANT DELETE THAT");
    display.display();
    Serial.println(id);
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.println("ERROR WRITING TO FLASH");
    display.display();
    Serial.println(id);
  } else {
    Serial.print("Unknown error: 0x");
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.println("UNKNOWN ERROR");
    display.display();
    Serial.println(id);
    Serial.println(p, HEX);
  }
  delay(2000);
  return p;
}
