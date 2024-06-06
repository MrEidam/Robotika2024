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
float newAngleDis;
float wait;
int red, green, blue; //- RGB
bool sniffing = false;
int currentBone;
int attempt = 5;
int sack[] = {0,0,0}; //^ R G B

void resetEn(){
    resetMotorEncoder(motorRight);
    resetMotorEncoder(motorLeft);
}

//# Wait senor
void waitMotor(){
    waitUntilMotorStop(motorRight);
    waitUntilMotorStop(motorLeft);
    waitUntilMotorStop(motorPaws);
}

//# Halt
void stopMotor(){
    setMotorSpeed(motorRight, 0);
    setMotorSpeed(motorLeft, 0);
}

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

//# Moving
void drive(float dis, int mspeed){
    resetEn();
    float angledis = fabs(dis) * convMM;

    while((angledis > getMotorEncoder(motorLeft)) && (dis >= 0)){
        int diff = getMotorEncoder(motorLeft) - getMotorEncoder(motorRight);
        setMotorSpeed(motorRight, mspeed + diff/2);
        setMotorSpeed(motorLeft, mspeed - diff/2);
        delay(20);
    }

    while((-angledis < getMotorEncoder(motorLeft)) && (dis <=0)){
        int diff = abs(getMotorEncoder(motorLeft)) - abs(getMotorEncoder(motorRight));
        setMotorSpeed(motorRight, -fabs(mspeed + diff/2));
        setMotorSpeed(motorLeft, -fabs(mspeed - diff/2));
        delay(20);
    }
}

//# ROTATO POTATO
void rotate(float deg, int mspeed){
    resetEn();
    float dis = fabs(deg) * wheelWidth / wheelDiameter;

    while((dis > getMotorEncoder(motorLeft))&&(deg>=0)){
        setMotorSpeed(motorRight, -mspeed/2);
        setMotorSpeed(motorLeft, mspeed/2);
        delay(20);
    }
    while((dis > getMotorEncoder(motorRight))&&(deg<=0)){
        setMotorSpeed(motorRight, mspeed/2);
        setMotorSpeed(motorLeft, -mspeed/2);
        delay(20);
    }
    stopMotor();
}

//# Catching cubes from above
void vDrive(int mspeed, bool collect){
    float angledis = 700 * convMM;
    newAngleDis = angledis;
    while((angledis > getMotorEncoder(motorLeft)) && (collect == true)){
        int diff = getMotorEncoder(motorLeft) - getMotorEncoder(motorRight);
        setMotorSpeed(motorRight, mspeed + diff/2);
        setMotorSpeed(motorLeft, mspeed - diff/2);
        if(currentBone != -1){
            paws(false);
            delay(250);
            stopMotor();
            waitMotor();
            newAngleDis = angledis-getMotorEncoder(motorLeft);
            vDrive(mspeed, false);
        }
        delay(20);
    }

    while((-newAngleDis < getMotorEncoder(motorLeft)) && (collect == false)){
        int diff = abs(getMotorEncoder(motorLeft)) - abs(getMotorEncoder(motorRight));
        setMotorSpeed(motorRight, -fabs(mspeed + diff/2));
        setMotorSpeed(motorLeft, -fabs(mspeed - diff/2));
        delay(20);
    }
}
/*
//# Catching cubes sidewaaaayyysz
void jDrive(int mspeed){
    resetEn();
    float angledis = 1000 * convMM;

    while(angledis > getMotorEncoder(motorLeft)){
        int diff = getMotorEncoder(motorLeft) - getMotorEncoder(motorRight);
        setMotorSpeed(motorRight, mspeed + diff/2);
        setMotorSpeed(motorLeft, mspeed - diff/2);
        if(currentBone != -1){
            delay(250);
            break;
        }
        delay(20);
    }
}*/

//# Forceback Drive
void fDrive(int mspeed, int back){
    resetEn();
    float angledis = 1000. * convMM;

    while(/*(angledis > getMotorEncoder(motorLeft)) && */(back == 0)){
        int diff = getMotorEncoder(motorLeft) - getMotorEncoder(motorRight);
        setMotorSpeed(motorRight, mspeed + diff/2);
        setMotorSpeed(motorLeft, mspeed - diff/2);
        delay(20);
    }

    while((-angledis < getMotorEncoder(motorLeft)) && (back == 1)){
        int diff = abs(getMotorEncoder(motorLeft)) - abs(getMotorEncoder(motorRight));
        setMotorSpeed(motorRight, -fabs(mspeed + diff/2));
        setMotorSpeed(motorLeft, -fabs(mspeed - diff/2));
        delay(20);
        if(wait==5||(getTouchValue(uncle) && getTouchValue(dream))){
            break;
        }
    }
}

//# Schod* DRIVE
void rDrive(float dis, float mspeed){
    waitMotor();

    drive(5,mspeed/4);
    drive(5,mspeed/2);
    drive(dis-20,mspeed);
    drive(5,mspeed/2);
    drive(5,mspeed/4);

    stopMotor();
    waitMotor();
}

void forceBackMove(){
    resetEn();

    wait = 0;
    fDrive(speed, 1);

    while(!(getTouchValue(uncle) && getTouchValue(dream))){
        delay(1000);
        wait++;
        if(wait==5||(getTouchValue(uncle) && getTouchValue(dream))){
            stopMotor();
            delay(100);
            break;
        }
    }

////    waitUntil(getTouchValue(uncle) && getTouchValue(dream));
    delay(100);
    stopMotor();
}

//# Moving but with eyes

void moveUntil(float wee, int mspeed){
    while(getUSDistance(sanic) >= wee/10){
        setMotorSpeed(motorRight, mspeed);
        setMotorSpeed(motorLeft, mspeed);
    }
    stopMotor();
}
/*
void moveBack(float wee, int mspeed){
    while(getUSDistance(sanic) <= wee/10){
        setMotorSpeed(motorRight, -mspeed);
        setMotorSpeed(motorLeft, -mspeed);
    }
    stopMotor();
}
*/
//# The First steps
void mainSS(int speedM){
    rDrive(720,speedM);
    waitMotor();
    rotate(90,speedM);
    rDrive(450,speedM);////600
    waitMotor();
    rotate(90,speedM);
    rDrive(600,speedM);////1000
    waitMotor();
    rotate(-90,speedM);
    waitMotor();
    forceBackMove();
    moveUntil(200,speedM);
    rotate(-90,speedM);
    waitMotor();
    forceBackMove();
    stopMotor();
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
	if(r>40||g>30||b>45){           //* zahájení američana.c
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

task gimmeColor(){
    while(sniffing){
        getColorRawRGB(color, red, green, blue);
        displayBigTextLine(10, "RGB: %i %i %i", red, green, blue);
		currentBone = getColor(red, green, blue);
		delay(50);
    }
}

void goingToBones(){
    rDrive(1400,45);
    waitMotor();
    rotate(-90, 50);
    forceBackMove();
    rDrive(150,40);
    waitMotor();
    rotate(90, 50);
    rDrive(600,45);
    waitMotor();
    rotate(90,60);
    waitMotor();
    rotate(-180,60);
    waitMotor();
    forceBackMove();
    sniffing = true;
    startTask(gimmeColor);
}

void sortBones(int mspeed, int i, int j);

void collectBonesFailed(int mspeed, int i, int j){
    rDrive(150,mspeed);
        waitMotor();
        rDrive(1000-(1000*i)/attempt, mspeed);
        waitMotor();
        rotate(-90, mspeed);
        paws(true);

        resetEn();
        vDrive(mspeed, true);

        stopMotor();
        waitMotor();
        rotate(90, mspeed);
        waitMotor();
        forceBackMove();
        waitMotor();
        sortBones(speed, i,-1);

    /*rDrive(150, mspeed);
    waitMotor();
    rotate(-90, mspeed);
    waitMotor();
    rDrive(500-(500*i)/attempt, mspeed);
    waitMotor();
    rotate(90, mspeed);
    waitMotor();
    forceBackMove();
    paws(true);
    waitMotor();

    jDrive(mspeed);
    paws(false);

    stopMotor();
    waitMotor();
    forceBackMove();
    rDrive(175, mspeed);
    waitMotor();
    rotate(90, mspeed);
    waitMotor();
    rDrive(500-(500*i)/attempt, mspeed);
    waitMotor();
    rotate(-90, mspeed);
    waitMotor();
    forceBackMove();
    waitMotor();*/
    sortBones(speed, i, j);
}

void sortBones(int mspeed, int i, int j){
    rDrive(100,mspeed);
    waitMotor();
    if(currentBone == 0){       //! RED/Comunnis
        sack[0]++;
        rDrive(900, mspeed);  ////233
        waitMotor();
        rotate(180, mspeed/2);
        waitMotor();
        forceBackMove();
        waitMotor();
        rDrive(100, mspeed);
        waitMotor();
        rotate(-90,mspeed/2);
        waitMotor();
        paws(true);
        waitMotor();
        rDrive(375, mspeed);
        waitMotor();
        rDrive(-375, mspeed);
        waitMotor();
        paws(false);
        waitMotor();
        rotate(90, mspeed);
        waitMotor();
        forceBackMove();
        waitMotor();
        rDrive(900, mspeed);
        waitMotor();
        rotate(180, mspeed/2);
        waitMotor();
        forceBackMove();
    }else if(currentBone == 1){ //^ Grün
        sack[1]++;
        rDrive(475, mspeed);
        waitMotor();
        rotate(90, mspeed/2);
        waitMotor();
        paws(true);
        waitMotor();
        rDrive(375, mspeed);
        waitMotor();
        rDrive(-375, mspeed);
        waitMotor();
        rotate(-90, mspeed/2);
        paws(false);
        waitMotor();
        forceBackMove();
    }else if(currentBone == 2){ //? CAPITALISTIC PIG
        sack[2]++;
        waitMotor();
        rotate(90, mspeed/2);
        waitMotor();
        paws(true);
        waitMotor();
        rDrive(375, mspeed);
        waitMotor();
        rDrive(-375, mspeed);
        waitMotor();
        rotate(-90, mspeed/2);
        paws(false);
        waitMotor();
        forceBackMove();
    }else{                      //- Error forceback & repeat
        if(j==-1){
            forceBackMove();
            collectBonesFailed(mspeed, i, i);
        }else{
            forceBackMove();
        }
    }
}

void collectBones(int attemps, int mspeed){
    for(int i = 0; i < attemps; i++){
        rDrive(150,mspeed);
        waitMotor();
        rDrive(1000-(1000*i)/attemps, mspeed);
        waitMotor();
        rotate(-90, mspeed);
        paws(true);

        resetEn();
        vDrive(mspeed, true);

        stopMotor();
        waitMotor();
        rotate(90, mspeed);
        waitMotor();
        forceBackMove();
        waitMotor();
        sortBones(speed, i,-1);

        /*
        rDrive(150, mspeed);
        waitMotor();
        rotate(-90, mspeed);
        waitMotor();
        rDrive(500-(500*i)/attemps, mspeed);
        waitMotor();
        rotate(90, mspeed);
        waitMotor();
        forceBackMove();
        paws(true);
        waitMotor();

        jDrive(mspeed);
        paws(false);

        stopMotor();
        waitMotor();
        forceBackMove();
        rDrive(175, mspeed);
        waitMotor();
        rotate(90, mspeed);
        waitMotor();
        rDrive(500-(500*i)/attemps, mspeed);
        waitMotor();
        rotate(-90, mspeed);
        waitMotor();
        forceBackMove();
        waitMotor();
        sortBones(speed, i,-1);
        */
    }
    stopMotor();
}

//# Wake up! We need to BackUp
void backToDen(int mspeed){
    rDrive(150,mspeed);
    waitMotor();
    rotate(-90,mspeed/2);
    waitMotor();
    rDrive(1450,mspeed);
    waitMotor();
    moveUntil(500,mspeed);
    waitMotor();
    rotate(90,mspeed/2);
    waitMotor();
    forceBackMove();
    waitMotor();
    rDrive(600,mspeed);
    waitMotor();
    rotate(90,mspeed/2);
    waitMotor();
    rDrive(500, mspeed);
    waitMotor();
    rotate(-90,mspeed/2);
    waitMotor();
    rDrive(350,mspeed);
    waitMotor();
    rotate(-90,mspeed/2);
    waitMotor();
    forceBackMove();
    waitMotor();
    moveUntil(100,mspeed*2);
}

//# OKAAAAY, LET'S GO
task main(){
    resetMotorEncoder(motorPaws);

    paws(false);
    waitMotor();
    forceBackMove();

    waitMotor();
    mainSS(50);
    waitMotor();
    goingToBones();
    waitMotor();
    collectBones(attempt,30);
    waitMotor();
    backToDen(45);
    waitMotor();
    /*/  
    //!drive(200,60);
    //!rotate(90, 60);
    //!drive(-200,60);
    //waitMotor();
    paws(false); //# Closing paws(20,10);
    waitMotor();
    forceBackMove();
    paws(true); //# OPENNING paws(-1,10);
    waitMotor();
    sniffing = true;
    startTask(gimmeColor);/*/
    playTone(210, 8);
    delay(20);
    while(true){
        displayBigTextLine(7, "Battery: %i", nAvgBatteryLevel);
        delay(20);
    }
}
