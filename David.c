#pragma config(Motor,  motorA, motorR, tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorD, motorPR, tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorC, motorL, tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorB, motorPL, tmotorEV3_Large, PIDControl, encoder)
#include "JoystickDriver.c"

//	F5 -> Start -> Robot -> Lego Brick -> Joystick control BASIC
int limitSpeed = 50;
// int maxRotate = 90;
// int pawStatus = 0;
// int pstop = 0;

void encspeedR(int rot, int tolerance){
    int encoder = getMotorEncoder(motorPR);
    int diff = rot-encoder;
    int nspeed = diff/2;
    
    if(abs(diff) > tolerance){
        setMotorSpeed(motorPR, nspeed);
    }else{
        setMotorSpeed(motorPR, 0);
    }
	displayTextLine(8, "diff: %i, sped: %i", diff, nspeed);
}

void encspeedL(int rot, int tolerance){
    int encoder = getMotorEncoder(motorPL);
    int diff = rot-encoder;
    int nspeed = diff/2;
    
    if(abs(diff) > tolerance){
        setMotorSpeed(motorPL, nspeed);
    }else{
        setMotorSpeed(motorPL, 0);
    }
}

void popen(){
	encspeedR(90,10);
	delay(100);
	encspeedL(90,10);
	delay(50);
}

void p45(){
	encspeedR(55,10);
	encspeedL(35,10);
}

void pclose(){
	encspeedL(0,10);
	delay(100);
	encspeedR(0,10);
	delay(50);
}

task main(){
	resetMotorEncoder(motorPR);
	resetMotorEncoder(motorPL);
	while(true){
		getJoystickSettings(joystick);


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

		if(joy1Btn(4)||joy1Btn(5)||joy1Btn(6)||joy1Btn(7)||joy1Btn(8)){
			if(joy1Btn(5)||joy1Btn(7))popen();
			if(joy1Btn(4))p45();
			if(joy1Btn(6)||joy1Btn(8))pclose();
		}else{
			setMotorSpeed(motorPR, 0);
			setMotorSpeed(motorPL, 0);
		}

		
		delay(100);

//	Honk
		if(joy1Btn(11) == 1)playTone(210, 8);
	}
}
