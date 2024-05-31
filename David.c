#pragma config(Motor,  motorA, motorR,  tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorB, motorL,  tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorC, motorPR, tmotorEV3_Large, PIDControl, reversed, encoder)
#pragma config(Motor,  motorD, motorPL, tmotorEV3_Large, PIDControl, reversed, encoder)

#include "JoystickDriver.c"

//	F5 -> Start -> Robot -> Lego Brick -> Joystick control BASIC
int limitSpeed = 10;
int slow = 0;

void encspeedR(int rot, int tolerance, bool back){
    int encoder = getMotorEncoder(motorPR);
    int diff = abs(rot)+encoder;
    int nspeed = -diff/2;


	if((abs(diff) > tolerance) && (back == false)){
		setMotorSpeed(motorPR, -nspeed);
	}else if((abs(diff) > -tolerance) && (back == true)){
		setMotorSpeed(motorPR, -nspeed);
	}else{
		setMotorSpeed(motorPR, 0);
	}
	////displayTextLine(10, "enc: %i, diff: %i, sped: %i", encoder, diff, nspeed);
}

void encspeedL(int rot, int tolerance, bool back){
    int encoder = getMotorEncoder(motorPL);
    int diff = abs(rot)+encoder;
    int nspeed = -diff/2;


	if((abs(diff) > tolerance) && (back == false)){
		setMotorSpeed(motorPL, -nspeed);
	}else if((abs(diff) > -tolerance) && (back == true)){
		setMotorSpeed(motorPL, -nspeed);
	}else{
		setMotorSpeed(motorPL, 0);
	}
}

void popen(){
	encspeedR(100,10,true);
	delay(100);
	encspeedL(100,10,true);
	delay(50);
}

void p45(){
	encspeedR(45,10,false);
	encspeedL(45,10,false);
}

void pclose(){
	encspeedL(0,10,false);
	delay(100);
	encspeedR(0,10,false);
	delay(50);
}

void rollUp(){
	encspeedL(90,10,true);
	delay(100);
	setMotorSpeed(motorPR, -69);
	delay(50);
}

task paws(){
	resetMotorEncoder(motorPR);
	resetMotorEncoder(motorPL);
	while(true){
		if(joy1Btn(3)||joy1Btn(4)||joy1Btn(5)||joy1Btn(6)||joy1Btn(7)||joy1Btn(8)){
			if(joy1Btn(5)||joy1Btn(7))popen();
			if(joy1Btn(4))p45();
			if(joy1Btn(3))rollUp();
			if(joy1Btn(6)||joy1Btn(8))pclose();
		}else{
			setMotorSpeed(motorPR, 0);
			setMotorSpeed(motorPL, 0);
		}
		delay(50);
	}
}

task main(){
	startTask(paws);

	while(true){
		getJoystickSettings(joystick);

		displayTextLine(8, "%i", getMotorEncoder(motorPR));
		displayTextLine(9, "%i", getMotorEncoder(motorPL));

		if(joy1Btn(2)){////slow = 1;
			slow = 1;
			////setMotorSpeed(motorPR,-50);
		delay(50);
		}
		if(joy1Btn(1)){
			slow = 0;
			////setMotorSpeed(motorPR, 50);
		delay(50);
		}
		if(slow == 1){
			if(joystick.joy1_y1>limitSpeed || joystick.joy1_y1<-limitSpeed || joystick.joy1_x2>limitSpeed || joystick.joy1_x2<-limitSpeed){
				setMotorSpeed(motorR, -(joystick.joy1_y1/24)+(joystick.joy1_x2/8));
				setMotorSpeed(motorL, -(joystick.joy1_y1/24)-(joystick.joy1_x2/8));
			}else{
				setMotorSpeed(motorR, 0);
				setMotorSpeed(motorL, 0);
			}
		}else{
			if(joystick.joy1_y1>limitSpeed || joystick.joy1_y1<-limitSpeed || joystick.joy1_x2>limitSpeed || joystick.joy1_x2<-limitSpeed){
				setMotorSpeed(motorR, -(joystick.joy1_y1/4)+(joystick.joy1_x2/2));
				setMotorSpeed(motorL, -(joystick.joy1_y1/4)-(joystick.joy1_x2/2));
			}else{
				setMotorSpeed(motorR, 0);
				setMotorSpeed(motorL, 0);
			}
		}

		displayBigTextLine			(1, "My creators are:");
		displayTextLine				(4, "Program: Adam    Brichacek");
		//// displayTextLine				(5, "Stavba:  David   Fuis");
		displayTextLine				(5, "Stavba:  Jachym  Svec");/*
		displayTextLine       (7, "PR: %i", getMotorTarget(motorPR));
		displayTextLine       (8, "PL: %i", getMotorTarget(motorPL));*/
		displayTextLine       (7, "Battery: %i", nAvgBatteryLevel);
		displayBigTextLine			(12, " And I'm David");

		delay(50);

//	Honk
		if(joy1Btn(11) == 1)playTone(210, 8);
	}
}
