#pragma config(Sensor, S1,     color,      sensorEV3_Color)
#pragma config(Sensor, S2,     uncle,      sensorEV3_Touch)
#pragma config(Sensor, S3,     sanic,      sensorEV3_Ultrasonic)
#pragma config(Sensor, S4,     dream,      sensorEV3_Touch)
#pragma config(Motor,  motorA, motorRight, tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorB, motorLeft,  tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorC, motorPaws,  tmotorEV3_Medium, PIDControl, encoder)

float speed = 50;
int red = 0, green = 0, blue = 0; //- RGB
bool sniffing = false;
int currentBone;
int cube = 0;

//# Paws

void paws(bool open){
    if(open){
        setMotorSpeed(motorPaws, -60);
        delay(250);
        setMotorSpeed(motorPaws, 0);
    }else{
        setMotorSpeed(motorPaws, 60);
        delay(250);
        setMotorSpeed(motorPaws, 0);
    }
}

//# Colors
//! 0 = red
//^ 1 = green
//? 2 = blue

int max(int a, int b, int c){       // vybere nejvyssi hodnotu
		if( a>b && a>c ) return a;
		if( b>a && b>c ) return b;
		if( c>a && c>b ) return c;
	return -1;
}

int getColor(int r, int g, int b){  //? returnuje barvu kostky
	if(r>30||g>30||b>50){           //* zahájení američana.c
		if(max(r,g,b)==r){          //! communist
			return 0;
		}else if(max(r,g,b)==g){    //^ detekce zelených zmrdů
			return 1;
		}else{                      //? capitalistické svin�?
			return 2;
		}
	}else{                          //- Error
		return -1;
	}
}

//# OKAAAAY, LET'S GO
task main(){
    resetMotorEncoder(motorPaws);

    playTone(210, 8);
    delay(20);
    while(true){
        getColorRawRGB(color, red, green, blue);
        if(getButtonPress(1)){
            paws(false);
        }
        if(getButtonPress(3)){
            paws(true);
        }
        if(getButtonPress(2)&&(cube==0)){
            displayBigTextLine(2, "Red: %i %i %i", red, green, blue);
            delay(150);
        }else if(getButtonPress(2)&&(cube==1)){
            displayBigTextLine(4, "Green: %i %i %i", red, green, blue);
            delay(150);
        }else if(getButtonPress(2)&&(cube==2)){
            displayBigTextLine(6, "Blue: %i %i %i", red, green, blue);
            delay(150);
        }
        if(getButtonPress(4)&&(cube<2)){
            cube++;
            delay(250);
        }else if(getButtonPress(5)&&(cube>0)){
            cube--;
            delay(250);
        }

        displayTextLine(15, "Cube: %i", cube);
        delay(20);
    }
}
/*
    1 up
    2 mid
    3 down
    4 right
    5 left
*/