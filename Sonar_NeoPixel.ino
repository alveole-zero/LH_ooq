
// ---------------------------------------------------------------------------
// Example NewPing library sketch that does a ping about 20 times per second.
// ---------------------------------------------------------------------------

#include <NewPing.h>
#include <Adafruit_NeoPixel.h>


#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200  // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
// How many NeoPixels are attached to the Arduino?
#define PIN 9
#define NUMPIXELS 30

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

unsigned int pingSpeed = 60; // How frequently are we going to send out a ping (in milliseconds). 50ms would be 20 times a second.
unsigned long pingTimer;     // Holds the next ping time.
int distance = 0; // the measured distance by Sonar
int brightness = 0;    // how bright the LED is
unsigned long startMillis = 0;  

int mode = 1;

int delayval = 2; // delay
int tolerance = 10;
int countto = 0;
int countrl = 0;

void setup() {
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
  //pinMode(PIN, OUTPUT);

  pixels.begin(); // This initializes the NeoPixel library.
  pingTimer = millis(); // Start now.
}

void loop() {
  // Notice how there's no delays in this sketch to allow you to do other processing in-line while doing distance pings.
  if (millis() >= pingTimer) {   // pingSpeed milliseconds since last ping, do another ping.
    pingTimer += pingSpeed;      // Set the next ping time.
    sonar.ping_timer(echoCheck); // Send out the ping, calls "echoCheck" function every 24uS where you can check the ping status.
  // Do other stuff here, really. Think of it as multi-tasking.
  }
  
// This sends the updated pixel color to the hardware.
//  startMillis = millis();
//  while(millis() - startMillis < delayval ){
//      // do nothing
//  }
//  delay(delayval); // Delay for a period of time (in milliseconds).
  
}

void updateLED(int red,int green,int blue) {
    for(int i=0;i<NUMPIXELS;i++){
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(red,green,blue)); 
    }   
    pixels.show(); 
}

void echoCheck() { // Timer2 interrupt calls this function every 24uS where you can check the ping status.
  // Don't do anything here!
  
  if (sonar.check_timer()) { // This is how you check to see if the ping was received.
    // Here's where you can add code.
//     Serial.print("Ping: ");
//     Serial.print(sonar.ping_result / US_ROUNDTRIP_CM); // Ping returned, uS result in ping_result, convert to cm with US_ROUNDTRIP_CM.
//    Serial.println("cm");
    distance = sonar.ping_result / US_ROUNDTRIP_CM;
    if (distance <= 1 || distance >= MAX_DISTANCE-tolerance ){
      brightness = 0;
    }else{
      float factor = (float) map(distance, 1, MAX_DISTANCE, 1000,0)/1000.0;
      brightness = factor*factor*factor*255;
    }
    updateLED(brightness,0,0);
    countto=0;
    countrl=0;
    mode=1;
  } else {
      countto++;
      countrl++;
      if(countto>2000 && mode==1){
        brightness=0;
        countto=0;
        mode=2;
      }
      if(countrl>random(250,5000) && mode == 2){
        updateLED(random(0, 150),random(0, 150),random(0, 150));
        countrl=0;
      }      
      if(countrl==2 && mode == 2){
        updateLED(0,0,0);
      }      
   //   Serial.println(countto);
  }
  // Don't do anything here!
}
