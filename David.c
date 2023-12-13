#pragma config(Motor,  motorA, motorR, tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorD, motorPR, tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorC, motorL, tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorB, motorPL, tmotorEV3_Large, PIDControl, encoder)
#include "JoystickDriver.c"

//	F5 -> Start -> Robot -> Lego Brick -> Joystick control BASIC

int limitSpeed = 20;
int maxRotate = 90;
int paws = false; // true = open | false = close

void popen(){
	paws = true;
	moveMotorTarget(motorPR, maxRotate, 100);
	delay(250);
	moveMotorTarget(motorPL, maxRotate, 100);
	delay(100);
}

void pclose(){
	paws = false;
	moveMotorTarget(motorPL, -maxRotate, -100);
	delay(250);
	moveMotorTarget(motorPR, -maxRotate, -100);
	delay(100);
}

task main(){
	while(true){
		getJoystickSettings(joystick);

		resetMotorEncoder(motorPR);
		resetMotorEncoder(motorPL);

		if(joystick.joy1_y1>limitSpeed || joystick.joy1_y1<-limitSpeed || joystick.joy1_x2>limitSpeed || joystick.joy1_x2<-limitSpeed){
			setMotorSpeed(motorR, -(joystick.joy1_y1/2)+(joystick.joy1_x2/2));
			setMotorSpeed(motorL, -(joystick.joy1_y1/2)-(joystick.joy1_x2/2));
		}else{
			setMotorSpeed(motorR, 0);
			setMotorSpeed(motorL, 0);
		}

		displayBigTextLine			(1, "My masters are:");
		displayTextLine				(4, "Program: Adam    Brichacek");
		displayTextLine				(5, "Stavba:  David   Fuis");
		displayTextLine				(6, "Stavba:  Jachym  Svec");/*
		displayTextLine       (7, "PR: %i", getMotorTarget(motorPR));
		displayTextLine       (8, "PL: %i", getMotorTarget(motorPL));*/
		displayTextLine       (7, "Battery: %i", nAvgBatteryLevel);
		displayBigTextLine			(12, " And I'm David");

		if(joy1Btn(5)||joy1Btn(6)||joy1Btn(7)||joy1Btn(8)){
			if((joy1Btn(5)||joy1Btn(7)) && (paws == false))popen();
			if((joy1Btn(6)||joy1Btn(8)) && (paws == true))pclose();
		}else{
			setMotorSpeed(motorPR, 0);
			setMotorSpeed(motorPL, 0);
		}




		delay(100);

//	Honk
		if(joy1Btn(11) == 1)playTone(210, 8);
	}
}
