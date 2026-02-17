#include <Adafruit_NeoPixel.h>
#include <Servo.h>

Servo esc;

// Wheel Control Pins
const int wpin = 9;

//Neopixel Pins
const int lpin1 = 3;
const int numpixels = 21;

Adafruit_NeoPixel pixels(numpixels, lpin1, NEO_GRBW + NEO_KHZ800);

// Reed Switch
const int rpin = 2;

// Speed Variables
const int go = 2000;
const int stop = 1500;

// Random Color List
uint32_t colors[] = {

  pixels.Color(255, 150, 0,30), // Grey   - COMMON
  pixels.Color(255, 150, 0,30),
  pixels.Color(255, 150, 0,30), // 3
  pixels.Color(255, 150, 0,30),
  pixels.Color(255, 150, 0,30),
  pixels.Color(255, 150, 0,30), // 6
  pixels.Color(255, 150, 0,30),
  pixels.Color(255, 150, 0,30),
  pixels.Color(255, 150, 0,30), // 9
  pixels.Color(255, 150, 0,30), // 10

  pixels.Color(100, 250, 0, 5), // Green  - UNCOMMON
  pixels.Color(100, 250, 0, 5),
  pixels.Color(100, 250, 0, 5), // 3
  pixels.Color(100, 250, 0, 5),
  pixels.Color(100, 250, 0, 5), // 5

  pixels.Color(0, 255, 200, 5), // Aqua   - RARE
  pixels.Color(0, 255, 200, 5),
  pixels.Color(0, 255, 200, 5), // 3

  pixels.Color(191, 0, 200, 0), // Purple - EPIC
  pixels.Color(191, 0, 200, 0), // 2

  pixels.Color(255, 70, 0, 0)   // Gold   - LEGENDARY
};
int numberOfColors = sizeof(colors) / sizeof(colors[0]);


void spin();
void setup(){
    Serial.begin(9600);

    //Wheel pin setup
    esc.attach(wpin);   // Signal wire on pin 9
    esc.writeMicroseconds(1000);  // Minimum throttle
    delay(3000); 


    // Reed pin setup
    pinMode(rpin, INPUT_PULLUP);

    // NeoPixel setup
    pixels.begin();
    pixels.setBrightness(5);
    
    //for (int i = 7; i<21; i++){
      //              pixels.setPixelColor(i, pixels.Color(0, 0, 0,0)); //Change to idle
        //}
}

void spin(int ms){
    // Spin the wheel
    esc.writeMicroseconds(go);
    delay(ms);
    esc.writeMicroseconds(stop);

}

void light_cycle(){
    int delayChangeValue = 1;
    for(int i = 0; i<40; i++){
        int newColor = random(0, numberOfColors);
        //Set random color here at start of change. select rand from list of predefined colors.
        for (int j = 0; j<numpixels; j++){
            pixels.setPixelColor(j, colors[newColor]); // Change color to rand int
        }
        pixels.show();
        delay(delayChangeValue); 
        delayChangeValue += 14;

    }

}

void loop(){
    pixels.clear();
    Serial.print("cycle\n");
    //set logic so if box is open it doesnt reset till box closes again
    if (digitalRead(rpin) == LOW){
        Serial.println("Box Open...\nLoop Entered...");
        Serial.println("Spin Should Start");
        spin(1500);
        light_cycle();
        Serial.println("Spin Should be Stopped");
        delay(500);
        while(digitalRead(rpin) == LOW){
            Serial.println("Box Open...\n Waiting...");
            delay(1000);
        }
    } else {

        for (int i = 0; i<numpixels; i++){
            pixels.setPixelColor(i, pixels.Color(255, 150, 0,30)); //Change to idle
        }
        pixels.show();
        while(digitalRead(rpin) == HIGH){
            Serial.println("Box Closed...\n Waiting...");
            esc.writeMicroseconds(stop);
            delay(1000);

        }
    }
}
