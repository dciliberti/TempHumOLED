/************************************************************************
   Display temperature and humidity. Use a DHT22 sensor to acquire data 
   about room relative humidity and temperature. Display the values on a 
   0.96" OLED screen and through serial communication. Uses a toggle
   switch to show temperature or humidity data on the OLED display,
   including actual value, min and max values, elapsed time.

    Copyright (C) 2023  Danilo Ciliberti 
    danilo.ciliberti@smartup-engineering.com
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program. If not, see <https://www.gnu.org/licenses/>
*************************************************************************/

// Libraries
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

// Initialize DHT sensor for normal 16mhz Arduino
DHT dht(2, DHT22);

// Variables
double temp, hum, time; // temperature, relative humidity, time

// Variables initialization
double minTemp = 99;  // minimum temperature
double minHum = 99;   // minimum humidity
double maxTemp = -99; // maximum temperature
double maxHum = 0;    // maximum humidity
int togglePin = 5;    // toggle switch digital pin

void setup() {
  pinMode(togglePin, INPUT_PULLUP); //configure pin as an input and enable the internal pull-up resistor
  
  Serial.begin(9600);
  Serial.println("Time\t Hum\t Temp");
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done

  // Show image buffer on the display hardware
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen
  display.display();
  delay(2000);

  // Clear the buffer
  display.clearDisplay();

  // Initialize the temperature sensor
  dht.begin();

}

void loop() {

  // Display time passed since the program started
  time = millis();
  time = time/1000;
  Serial.print(time,1);
  Serial.print("s\t");

  // Read and display humidity
  hum = dht.readHumidity();
  Serial.print(hum,1);
  Serial.print("%\t");

  // Read and display temperature
  temp = dht.readTemperature();
  Serial.print(temp,1);
  Serial.println("°C");

  // Store minimum and maximum values
  if (temp < minTemp) {
    minTemp = temp;
  }
  if (temp > maxTemp) {
    maxTemp = temp;
  }
  if (hum < minHum) {
    minHum = hum;
  }
  if (hum > maxHum) {
    maxHum = hum;
  }

  // Display values on screen
  display.clearDisplay();       // avoid "noise" on the display due to serial print commands (unknown cause)
  display.setTextSize(1);
  display.setTextColor(WHITE);

  // Display humidity or temperature according to toggle switch status
  int toggleStatus = digitalRead(togglePin);
  
  if (toggleStatus == 1){
   display.setCursor(0, 0);
   display.print("Temp.(C)    Min / Max");

   display.setTextSize(3);
   display.setCursor(0, 18);
   display.print(temp,1);

   display.setTextSize(2);
   display.setCursor(80, 15);
   display.print(minTemp,1);
   display.setCursor(80, 30);
   display.print(maxTemp,1); 
  } else {
   display.setCursor(0, 0);
   display.print("Humidity(%) Min / Max");

   display.setTextSize(3);
   display.setCursor(0, 18);
   display.print(hum,1);

   display.setTextSize(2);
   display.setCursor(80, 15);
   display.print(minHum,1);
   display.setCursor(80, 30);
   display.print(maxHum,1);
  }

  // Display elapsed time anyway below
  display.setCursor(0, 50);
  display.print("Time:");
  display.print(time,1);
  display.print("s");
  
  display.display();
  delay(1000);
  display.clearDisplay();

}
