#pragma config(Sensor, S1,     color,      sensorEV3_Color)
#pragma config(Sensor, S2,     uncle,      sensorEV3_Touch)
#pragma config(Sensor, S3,     sanic,      sensorEV3_Ultrasonic)
#pragma config(Sensor, S4,     dream,      sensorEV3_Touch)
#pragma config(Motor,  motorA, motorRight, tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorB, motorLeft,  tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorC, motorPaws,  tmotorEV3_Medium, PIDControl, encoder)

float speed = 50;
const float convMM = 1.666666667;
const float wheelWidth = 87;//105;
const float wheelDiameter = 68.8;
int red, green, blue; //- RGB
bool sniffing = false;
int currentBone;
int attempt = 3;
int sack[] = {0,0,0}; //^ R G B
////float actualL, actualR, ddiff, aangle;

void resetEn(){
    resetMotorEncoder(motorRight);
    resetMotorEncoder(motorLeft);
}

//# Wait senor
void waitMotor(){
    waitUntilMotorStop(motorRight);
    waitUntilMotorStop(motorLeft);
}

//# Halt
void stopMotor(){
    setMotorSpeed(motorRight, 0);
    setMotorSpeed(motorLeft, 0);
    waitMotor();
}

//# Moving

void moveMotors(float dis){
    resetEn();
    float range = dis * convMM;
    moveMotorTarget(motorRight, range, speed);
    moveMotorTarget(motorLeft, range, speed);
    waitMotor();
}

void moveMotorsBack(float dis){
    resetEn();
    float range = dis * convMM;
    moveMotorTarget(motorRight, range, -speed);
    moveMotorTarget(motorLeft, range, -speed);
}

void rotateMotors(float deg, float left){
    resetEn();
    float range = (deg*wheelWidth)/34.4;
    ////displayTextLine(1, "range: %f", range);
    if(left){
        moveMotorTarget(motorRight, range, speed);
        moveMotorTarget(motorLeft, range, -speed);
    }else{
        moveMotorTarget(motorRight, range, -speed);
        moveMotorTarget(motorLeft, range,   speed);
    }
}

void forceBackMove(){
    resetEn();
    setMotorSpeed(motorRight, -speed);
    setMotorSpeed(motorLeft, -speed);
    waitUntil(getTouchValue(uncle) && getTouchValue(dream));
    delay(50);
    stopMotor();
}

//# Moving but not weird

void drive(float dis, int speed){
    resetEn();
    float angledis = dis * convMM;

    while((angledis > getMotorEncoder(motorLeft)) && (dis >= 0)){
        int diff = getMotorEncoder(motorLeft) - getMotorEncoder(motorRight);
        setMotorSpeed(motorRight, speed + diff/2);
        setMotorSpeed(motorLeft, speed - diff/2);
        delay(20);
    }
    while((-angledis < getMotorEncoder(motorLeft)) && (dis <=0)){
        int diff = abs(getMotorEncoder(motorLeft)) - abs(getMotorEncoder(motorRight));
        setMotorSpeed(motorRight, -abs(mspeed + diff/2));
        setMotorSpeed(motorLeft, -abs(mspeed - diff/2));
        delay(20);
    }

    stopMotor();
}

/*
//void driveBack(float dis, int mspeed){  //todo TEST
//    resetEn();                          //- Failed
//    float angledis = dis * convMM;      //! Womp Womp
//
//    while(-angledis < getMotorEncoder(motorLeft)){
//        int diff = abs(getMotorEncoder(motorLeft)) - abs(getMotorEncoder(motorRight));
//        setMotorSpeed(motorRight, -abs(mspeed + diff/2));
//        setMotorSpeed(motorLeft, -abs(mspeed - diff/2));
//        delay(20);
//
//        ////writeDebugStreamLine("dis %f, speed %i, angledis %f, encL %i, encR %i, diff %i", dis, mspeed, angledis, getMotorEncoder(motorLeft), getMotorEncoder(motorRight), diff);
//        ////writeDebugStreamLine("Motor speedR: %f, Motor speedL: %f", -(mspeed - diff/2), -(mspeed + diff/2));
//
//        writeDebugStreamLine("encL: %f encR: %f diffrend: %f angle: %f", actualL, actualR, ddiff, aangle);
//        
//        // kde najdu výsledek: Menu > Robot > Debugger Windows > Debug Stream > v nìm Ctrl+A nebo oznaèit oblast zájmu, Ctrl+C, novy soubor > Ctrl+V
//        datalogDataGroupStart();
//        actualL = getMotorEncoder(motorLeft);
//        actualR = getMotorEncoder(motorRight);
//        ddiff = actualL * 0.99013;
//        datalogAddValue( 0, actualL );
//        datalogAddValue( 1, actualR );
//        datalogAddValue( 2, ddiff );
//        aangle = angledis;
//        datalogAddValue( 3, aangle);
//        datalogDataGroupEnd();
//
//    }
//    stopMotor();
//    ////setMotorSpeed(motorRight, 0);
//    ////setMotorSpeed(motorLeft, 0);
//}
*/

void rotate(float deg, int speed){
    resetEn();
    float dis = fabs(deg) * wheelWidth / wheelDiameter;

    while((dis > getMotorEncoder(motorLeft))&&(deg>=0)){
        setMotorSpeed(motorRight, -speed);
        setMotorSpeed(motorLeft, speed);
        delay(20);
    }
    while((dis > getMotorEncoder(motorRight))&&(deg<=0)){
        setMotorSpeed(motorRight, speed);
        setMotorSpeed(motorLeft, -speed);
        delay(20);
    }
    stopMotor();
}

//# Moving but cool

void moveUntil(float wee, int speed){
    while(getUSDistance(sanic) >= wee/10){
        setMotorSpeed(motorRight, speed);
        setMotorSpeed(motorLeft, speed);
    }
    stopMotor();
}
void moveBack(float wee){
    while(getUSDistance(sanic) <= wee){
        setMotorSpeed(motorRight, -speed);
        setMotorSpeed(motorLeft, -speed);
    }
    stopMotor();
}

//# The First steps
void mainSS(int speedM){
    drive(850,speedM);
    rotate(90,speedM);
    waitMotor();
    forceBackMove();
    drive(600,speedM);
    rotate(90,speedM);
    waitMotor();
    forceBackMove();
    drive(1000,speedM);
    rotate(-90,speedM);
    waitMotor();
    forceBackMove();
    moveUntil(200,speedM);
    rotate(-90,speedM);
    waitMotor();
    /*moveUntil(200);
    waitMotor();
    rotateMotors(45, 0);
    waitMotor();
    moveUntil(200);
    waitMotor();
    rotateMotors(45, 0);
    waitMotor();
    forceBackMove();
    moveUntil(200);
    waitMotor();
    rotateMotors(45, 1);
    waitMotor();
    forceBackMove();
    moveUntil(200);
    waitMotor();
    rotateMotors(45, 1);
    waitMotor();*/
    forceBackMove();
    stopMotor();
}

//# Paws

void paws(bool open){
    if(open){
        setMotorSpeed(motorPaws, -100);
        delay(500);
        setMotorSpeed(motorPaws, 0);
    }else{
        setMotorSpeed(motorPaws, 100);
        delay(500);
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
	if(r>30||g>25||b>30){           //* zahájení američana.c
		if(max(r,g,b)==r){          //! communist
			return 0;
		}else if(max(r,g,b)==g){    //^ detekce zelených zmrdů
			return 1;
		}else{                      //? capitalistické svin�?
			return 2;
		}
	}else{
		return -1;
	}
}

task gimmeColor(){
    while(sniffing){
        getColorRawRGB(color, red, green, blue);
        ////displayBigTextLine(10, "Hex: %i %i %i", red, green, blue);
		currentBone = getColor(red, green, blue);
		delay(50);
    }
}

void goingToBones(){
    drive(2000,40);
    rotate(90,69);
    waitMotor();
    rotate(-180,69);
    waitMotor();
    forceBackMove();
    sniffing = true;
    startTask(gimmeColor);
}

void sortBones(int mspeed){
    drive(100,mspeed);
    waitMotor();
    if(currentBone == 0){       //! RED/Comunnis
        sack[0]++;
        drive(1016.666666667, mspeed);  ////233
        rotate(90, mspeed);
        paws(true);
        drive(395, mspeed);
        drive(-395, mspeed);
        rotate(-90, mspeed);
        forceBackMove();
    }else if(currentBone == 1){ //^ Grün
        sack[1]++;
        drive(550, mspeed);
        rotate(90, mspeed);
        paws(true);
        drive(395, mspeed);
        drive(-395, mspeed);
        rotate(-90, mspeed);
        forceBackMove();
    }else if(currentBone == 2){ //? CAPITALISTIC PIG
        sack[2]++;
        drive(83.333333, mspeed);
        rotate(90, mspeed);
        paws(true);
        drive(395, mspeed);
        drive(-395, mspeed);
        rotate(-90, mspeed);
        forceBackMove();
    }else{                      //- Error forceback
        forceBackMove();
    }
}

void collectBones(int attemps, int mspeed){
    for(int i = 0; i < attemps; i++){
        drive(100, mspeed);
        rotate(-90, mspeed);
        waitMotor();
        drive(500-(500*i)/attemps, mspeed);
        rotate(90, mspeed);
        waitMotor();
        forceBackMove();
        paws(true);
        drive(1105, mspeed);
        paws(false);
        forceBackMove();
        drive(100, mspeed);
        rotate(90, mspeed);
        waitMotor();
        drive(500-(500*i)/attemps, mspeed);
        rotate(-90, mspeed);
        waitMotor();
        forceBackMove();
        sortBones(speed);
    }
    stopMotor();
}

//# Wake up! We need to BackUp
void backToDen(int mspeed){
    drive(100,mspeed);
    rotate(-90,mspeed);
    waitMotor();
    drive(1450,mspeed);
    moveUntil(500,mspeed);
    waitMotor();
    rotate(90,mspeed);
    waitMotor();
    forceBackMove();
    moveUntil(100,mspeed);
    rotate(90,mspeed);
    waitMotor();
    forceBackMove();
    drive(800, mspeed);
    rotate(-90,mspeed);
    waitMotor();
    forceBackMove();
    drive(500,mspeed);
    rotate(-90,mspeed);
    waitMotor();
    forceBackMove();
    moveUntil(100,mspeed);
    /*
    moveMotors(50);
    waitMotor();
    rotateMotors(45, 1);
    waitMotor();
    moveUntil(200);
    waitMotor();
    rotateMotors(45, 0);
    waitMotor();
    forceBackMove();
    moveUntil(200);
    waitMotor();
    rotateMotors(45, 0);
    waitMotor();
    forceBackMove();
    moveUntil(200);
    waitMotor();
    rotateMotors(45, 1);
    waitMotor();
    forceBackMove();
    moveUntil(200);
    waitMotor();
    rotateMotors(45, 1);
    waitMotor();
    forceBackMove();
    moveUntil(100);*/
}

//# OKAAAAY, LET'S GO
task main(){/*
    forceBackMove();
    mainSS(50);
    goingToBones();
    collectBones(attempt,45);
    backToDen(50);
    ////drive(150,50);
    ////rotate(90, 50);
    ////waitMotor();
    /////!drive(-200,50);
    /////*moveMotorsBack(200);*/
    drive(-200,20);
    waitMotor();
    playTone(210, 8);
    delay(20);
    while(true){
        displayBigTextLine(7, "Battery: %i", nAvgBatteryLevel);
        delay(20);
    }
}