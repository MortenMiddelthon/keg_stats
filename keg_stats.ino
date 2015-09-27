
#include "HX711.h"

#define DOUT  3
#define CLK  2

HX711 scale(DOUT, CLK);

float calibration_factor = -10000; //-7050 worked for my 440lb max scale setup
float max_weight = 2.2;
#define ZERO_FACTOR 8331661

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
	#include <avr/power.h>
#endif

// NeoPixel DIN pins
#define KEG01	9

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS	9
// Define brightness
#define	BRIGHTNESS	100

// Number of kegs
#define	KEGS	1

Adafruit_NeoPixel kegs[] = {
	Adafruit_NeoPixel(NUMPIXELS, KEG01, NEO_GRB + NEO_KHZ800),
	};

int delayval = 50;

void setup() {
	Serial.begin(9600);
	uint8_t x = 0;
	for(x = 0; x < KEGS; x++) {
		kegs[x].begin();
		kegs[x].setBrightness(BRIGHTNESS);
		kegs[x].show();
	}
//  	scale.set_scale();
//  	scale.tare();	//Reset the scale to 0
	scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
	scale.set_offset(ZERO_FACTOR); //Zero out the scale using a previously known zero_factor
  	//long zero_factor = scale.read_average(); //Get a baseline reading
}

void loop() {
	scale.set_scale(calibration_factor); //Adjust to this calibration factor

	/*
	Serial.print("Reading: ");
	Serial.print(scale.get_units(), 1);
	Serial.print(" kg"); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person
	Serial.print(" calibration_factor: ");
	Serial.print(calibration_factor);
	Serial.println();
	*/
	displayWeight(0,scale.get_units());
	delay(50);
}

void blink(uint8_t keg, uint8_t r, uint8_t g, uint8_t b, uint8_t repeat) {
	uint8_t x = 0;
	uint8_t pixel = 0;
	for(x = 0; x < repeat; x++) {
		for(pixel = 0; pixel < NUMPIXELS; pixel++) {
			kegs[keg].setPixelColor(pixel, r, g, b);
		}
		kegs[keg].show();
		delay(200);
		for(pixel = 0; pixel < NUMPIXELS; pixel++) {
			kegs[keg].setPixelColor(pixel, 0);
		}
		kegs[keg].show();
		delay(200);
	}
}

void off(uint8_t keg) {
	uint8_t pixel = 0;
	for(pixel = 0; pixel < NUMPIXELS; pixel++) {
		kegs[keg].setPixelColor(pixel, 0);
	}
	kegs[keg].show();
}

void displayWeight(uint8_t keg, float w) {
	int weight = w * 4.5;
	uint8_t pixel = 0;
	Serial.println(weight);
	if(weight == 0) {
		for(pixel = 0; pixel < NUMPIXELS; pixel++) {
			kegs[keg].setPixelColor(pixel, BRIGHTNESS, 0, 0);
		}
	}
	else {
		for(pixel = 0; pixel < NUMPIXELS; pixel++) {
			// Set red pixels
			if(pixel < weight && pixel < 3) {
				kegs[keg].setPixelColor(pixel, BRIGHTNESS, 0, 0);
			}
			// Set yellow pixels
			else if(pixel < weight && pixel < 6) {
				kegs[keg].setPixelColor(pixel, BRIGHTNESS, BRIGHTNESS, 0);
			}
			// Set green pixels
			else if(pixel < weight && pixel < 9) {
				kegs[keg].setPixelColor(pixel, 0, BRIGHTNESS, 0);
			}
			else {
				kegs[keg].setPixelColor(pixel, 0);
			}
		}
	}
	kegs[keg].show();
}
