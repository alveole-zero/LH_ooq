#include <NewPing.h>
#include <Adafruit_NeoPixel.h>


#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 50// Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define PIN 9 // DO NOT ATTACHED THE STRAND TO THE TIMER PIN
// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 30

// option debug
//#define DEBUG_MODE
#define DEBUG_BRIGHTNESS true
#define DEBUG_BLINKING false

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

unsigned int pingSpeed = 60; // How frequently are we going to send out a ping (in milliseconds). 50ms would be 20 times a second.
unsigned long pingTimer;     // Holds the next ping time.
int distance = 0; // the measured distance by Sonar
unsigned int brightness = 0;    // how bright the LED is
int tolerance = 10;
unsigned long startMillis = 0;  
const int EXPONENT = 3;
const int LAPSE = 2; // delay ms

// detection variables
const int MAX_LAPSE_WITHOUT_DETECTION = 500; //ms 
const int CNT_WITHOUT_DETECTION = MAX_LAPSE_WITHOUT_DETECTION  / LAPSE;
int cnt = 0;
bool updated = false;

// blinking variable
unsigned long cntBeforeBlinking = 0;
const int MAX_LAPSE_BEFORE_BLINKING = 5; // SECONDES
const int MIN_LAPSE_BEFORE_BLINKING = 1; // SECONDES
unsigned long cntBlink = 0;
const unsigned int MIN_DELAY_BLINK = 2; //ms 
const unsigned int MAX_DELAY_BLINK = 10 ; //ms 
const unsigned int MAX_BLINKING_INTENSITY = 50; // from 0 up to 255
bool blinkLed = false;

void setup() {
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
  pixels.begin(); // This initializes the NeoPixel library.
  updateLED(0,0,0); //OFF on start
  pingTimer = millis(); // Start now.
}

void loop() {
  // Notice how there's no delays in this sketch to allow you to do other processing in-line while doing distance pings.
  if (millis() >= pingTimer) {   // pingSpeed milliseconds since last ping, do another ping.
    pingTimer += pingSpeed;      // Set the next ping time.   
    sonar.ping_timer(echoCheck); // Send out the ping, calls "echoCheck" function every 24uS where you can check the ping status.
  }

  if(!updated){
    cnt++;
    if (cnt > CNT_WITHOUT_DETECTION ){
      updateLED(0,0,0);
      blinkLed =true ;
      cnt=0;
    }
  }


  if(blinkLed){
#ifdef DEBUG_MODE && DEBUG_BLINKING
   Serial.println(cntBlink);
#endif
    if( cntBlink > cntBeforeBlinking ){
       blinkLedStrip(random(MIN_DELAY_BLINK, MAX_DELAY_BLINK));
    }else{
       cntBlink ++;
    }
  } else {
    cntBlink = 0;
    cntBeforeBlinking = random(MIN_LAPSE_BEFORE_BLINKING, MAX_LAPSE_BEFORE_BLINKING)*1000/LAPSE;
#ifdef DEBUG_MODE && DEBUG_BLINKING
    Serial.print("cntBeforeBlinking : ");
    Serial.println(cntBeforeBlinking);
#endif
  }

  // delay to help calculating (a little bit) more accurately the time for no detection or blinking
  if(cnt + cntBlink > 0){
    delayWithTimer(LAPSE);
  }  
}

void updateLED(int red,int green,int blue) {
    for(int i=0;i<NUMPIXELS;i++){
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(red,green,blue)); 
    }   
    pixels.show(); 
}

void blinkLedStrip(int delayBlink){
#ifdef DEBUG_MODE && DEBUG_BLINKING
  Serial.println("blinking...");
#endif
  updateLED(random(0, MAX_BLINKING_INTENSITY),random(0, MAX_BLINKING_INTENSITY),random(0, MAX_BLINKING_INTENSITY));
  delayWithTimer(delayBlink);
  updateLED(0,0,0);
  delayWithTimer(delayBlink);
}

void delayWithTimer(int valdelay){ // valdelay in ms
  startMillis = millis();
  while(millis() - startMillis < valdelay){
    // do nothing      
  }
  return;
}

void echoCheck() { // Timer2 interrupt calls this function every 24uS where you can check the ping status.
  // Don't do anything here!  
  if (updated = sonar.check_timer()) { // This is how you check to see if the ping was received.
    // Here's where you can add code.
    distance = sonar.ping_result / US_ROUNDTRIP_CM;
    if (distance <= 1 || distance >= MAX_DISTANCE - tolerance ){
      brightness = 0;
      blinkLed = true;
    }else{
      float factor = (float) map(distance, 1, MAX_DISTANCE, 1000,0)/1000.0;
      brightness = pow(factor, EXPONENT)*255;
      blinkLed =false;
    }
#ifdef DEBUG_MODE && DEBUG_BRIGHTNESS
    Serial.print("brigthness : ");
    Serial.println(brightness);
#endif
    cnt = 0;
    updateLED(brightness,0,0);      
  } 
  // Don't do anything here!
}
