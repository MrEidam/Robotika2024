#pragma config(Motor,  motorA, motorR, tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorB, motorK, tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorC, motorL, tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorD, motorP, tmotorEV3_Large, PIDControl, encoder)
#include "JoystickDriver.c"

int Sub0 = 2;
int Sub1 = 4;
int FastM = true;

//	F5 -> Start -> Robot -> Lego Brick -> Joystick control BASIC
task main(){
	while(true){
		getJoystickSettings(joystick);

		if(joy1Btn(4) == 1)FastM = false;
		if(joy1Btn(2) == 1)FastM = true;

		if(FastM == true){// FastM
			if(joystick.joy1_y1>0 || joystick.joy1_y1<0 || joystick.joy1_x2>0 || joystick.joy1_x2<0){
				setMotorSpeed(motorR, (joystick.joy1_y1/Sub0)-(joystick.joy1_x2/Sub1));
				setMotorSpeed(motorL, (joystick.joy1_y1/Sub0)+(joystick.joy1_x2/Sub1));
			}else{
				setMotorSpeed(motorR, 0);
				setMotorSpeed(motorL, 0);
			}

		}else{// SlowM
			if(joystick.joy1_y1>0 || joystick.joy1_y1<0 || joystick.joy1_x2>0 || joystick.joy1_x2<0){
				setMotorSpeed(motorR, (joystick.joy1_y1/(Sub0*3)-(joystick.joy1_x2/(Sub1*3))));
				setMotorSpeed(motorL, (joystick.joy1_y1/(Sub0*3)+(joystick.joy1_x2/(Sub1*3))));
			}else{
				setMotorSpeed(motorR, 0);
				setMotorSpeed(motorL, 0);
			}
		}

		if(joy1Btn(5) == 1 || joy1Btn(7) == 1 || joy1Btn(6) == 1 || joy1Btn(8) == 1 || joy1Pov(Pov2) == 1 || joy1Pov(Pov6) == 1){
			if(joy1Btn(6) == 1 || joy1Btn(8) == 1){
				setMotorSpeed(motorK, -99.69420);
				setLEDColor(ledRedFlash);
			}

			if(joy1Btn(5) == 1){
				setMotorSpeed(motorP, 80);
				setLEDColor(ledOrangeFlash);
			}

			if(joy1Btn(7) == 1){
				setMotorSpeed(motorP, -80);
				setLEDColor(ledOrangeFlash);
			}

			if(joy1Pov(Pov2) == 1){
				setMotorSpeed(motorP, 25);
				setLEDColor(ledOrangeFlash);
			}

			if(joy1Pov(Pov6) == 1){
				setMotorSpeed(motorP, -25);
				setLEDColor(ledOrangeFlash);
			}
		}else{
			setMotorSpeed(motorK, 0);
			setMotorSpeed(motorP, 0);
			setLEDColor(ledGreen);
		}

		displayBigTextLine			(1, "My masters are:");
		displayTextLine				(4, "Program: Adam    Brichacek");
		displayTextLine				(5, "Stavba:  David   Fuis");
		displayTextLine				(6, "Stavba:  Jachym  Svec");
		displayBigTextLine			(12, " And I'm Gus");

		//displayBigTextLine(7, "Y - : %i", joystick.joy1_y1);
		//displayBigTextLine(9, "X - : %i", joystick.joy1_x2);

		delay(100);

//  Blinking LEDs RED & GREEN
		if(joy1Pov(Pov6) == 1 && joy1Btn(9) == 1){
		  for(int i = 0;i<50.69;i++){
				setLEDColor(ledRed);
	 			delay(100);

	 			setLEDColor(ledGreen);
			  delay(100);
			}
		}

//	Honk
		if(joy1Btn(11) == 1)playTone(210, 8);
	}
}
