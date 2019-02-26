/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 * Purpose: TODO
 *
 * 	@author Thomas Brych, Tim Klecka
 * 	@version 0.0.1 14.02.2019
 */
#include "FlightController.h"
using namespace std;

Orientation *orientation;
Barometer *barometer;
UltrasonicDistance *ultrasonic;
XMLParser *parser;
TCPServer *server;
Exit *error;
PID *pid;
PWMMotorTest *pwm;

FlightController::FlightController()
{
}

static void runUltrasonic()
{
	ultrasonic->runUltrasonic();
}

static void runBarometer()
{
	barometer->runBarometer();
}

static void runOrientation()
{
	orientation->runOrientation();
}

static void runServer()
{
	server->startUp();
}

static void runPid() {
	pid = PID::getInstance(orientation, pwm);
	pid->calcValues();
}

void FlightController::initObjects() 
{
	error = Exit::getInstance();

	int rc = wiringPiSetupGpio();
	if (rc != 0)
	{
		//The GPIO-Setup did not work
		error->sendError(0x01, true);
		return;
	}
  
	orientation = new Orientation();
	barometer = new BMP180();
	pwm = new PWMMotorTest();

	/*cout << "Hallo";
	getchar();
	pwm->CalMotor();
	cout << "Tim";
	getchar();*/

	//ultrasonic = new UltrasonicDistance();
	//parser = new XMLParser();
}

int FlightController::run()
{
	server = TCPServer::getInstance();
	thread serverThread(runServer);
	while (!server->connected) { delay(50); };
	cout << "Client connected!\n";

	initObjects();

	delay(250);

	/*pwm->ExitMotor();
	getchar();
	pwm->ArmMotor();
	pwm->SetSpeed(16, 1200);
	getchar();
	pwm->SetSpeed(16, 1500);
	getchar();
	pwm->ExitMotor();*/
	/*cout << "Hallo";
	getchar();
	pwm->ExitMotor();
	pwm->ArmMotor();
	getchar();
	pwm->ExitMotor();*/
	/*cout << "Hallo";
	getchar();
	pwm->ExitMotor();
	pwm->CalMotor();
	cout << "Tim";
	getchar();
	pwm->ExitMotor();*/

	thread pitchRollYawThread(runOrientation);
	thread barometerThread(runBarometer);
	thread pidController(runPid);
	cout << "Threads are running!\n";

	//orientation->interruptOrientation();
	//barometer->interruptBaromter();
	//pid->interruptPid();
	//cout << "Interrupting Threads! \n";

	serverThread.join();
	pitchRollYawThread.join();
	barometerThread.join();
	pidController.join();
	cout << "Stopped Threads!\n";

	return (0);
}

FlightController::~FlightController()
{
}

/*	pwm->ExitMotor();
	pwm->ArmMotor();
	cout << "0";
	pwm->SetSpeed(0, 1500);
	getchar();
	cout << "1";
	pwm->SetSpeed(0, 0);
	pwm->SetSpeed(1, 1500);
	getchar();
	cout << "2";
	pwm->SetSpeed(1, 0);
	pwm->SetSpeed(2, 1500);
	getchar();
	cout << "3";
	pwm->SetSpeed(2, 0);
	pwm->SetSpeed(3, 1500);
	getchar();
	pwm->SetSpeed(3, 0);*/
