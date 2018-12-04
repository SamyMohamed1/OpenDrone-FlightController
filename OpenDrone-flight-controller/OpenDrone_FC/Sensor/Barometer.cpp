#include "Barometer.h"
#include <bmp180.h>
#include <wiringPi.h>

Barometer::Barometer()
{
	bmp180Setup(200);
}

float *Barometer::getBarometerValues()
{
	static float ar[2];
	ar[0] = analogRead(200 + 0); //Temperatur
	ar[1] = analogRead(200 + 1); //Pressure - hPa
	return ar;
}

Barometer::~Barometer()
{
}