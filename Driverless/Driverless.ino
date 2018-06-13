/*

                -----------------------------
                |                            |
                |                            |
                |                            |
                |                            |
                |                            |
                |        Arduino mini pro    |
                |                            |
         ------>|BatteryCur(A2)              |
         ------>|BatteryVol(A1)       Pow(3) |------>
         ------>|APPS (A0)            Dir(2) |------>
                |                            |
                -----------------------------
                
*/
#include <Timer.h>
Timer T0;


/*----------------------*/
/*-----MOTOR Driver-----*/
/*----------------------*/
#define DirPin 2
#define PowPin 3
/*----------------------*/


/*----------------------*/
/*-------Battery--------*/
/*----------------------*/
#define BatteryCurPin A2
#define BatteryVolPin A1
#define EachBatterySatVol 3.6
#define EachBatteryStdVol 3.2
#define EachBatteryCutOffVol 2.5
#define EachBatteryAh  10
#define BatteryCell 8
static const double totalEnergy = (EachBatterySatVol - EachBatteryCutOffVol)* EachBatteryAh * BatteryCell;
double g_BatteryPercent = 0;
double g_currentEnergy = 0;
/*----------------------*/


/*----------------------*/
/*---------APPS---------*/
/*----------------------*/
#define APPSPin  A0
#define V0 2.3
#define VMax 5
#define VMin 0
#define VTH 2.6
#define VTL 2.0
/*----------------------*/

double getPowVal(double APPSVal){
  if(APPSVal > VMax) return 100;
  if(APPSVal < VMin) return 100;
	if(VTL <= APPSVal && APPSVal <= VTH) return 0;
	if(APPSVal > VTH){
		return (APPSVal - VTH)/(VMax - VTH) * 100.0;
	}else{
		return (VTL - APPSVal)/(VTL - VMin) * 100.0;		
	}
}


double getBatteryPow(double APPSVal){
//	(EachBatterSatVol - EachBatteryCutOffVol)
}

void chkBattery(){
  
}

void setup() {
  // put your setup code here, to run once:
	pinMode(DirPin,OUTPUT);
	pinMode(PowPin,OUTPUT);
	pinMode(BatteryCurPin, INPUT_PULLUP);
	pinMode(BatteryVolPin, INPUT_PULLUP);
	pinMode(APPSPin , INPUT);
	//T0.every(100,chkBattery);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
	double APPSVal = analogRead(APPSPin)/1023.0 * 5;// analog Value(0~1023) to Voltage(0~5V)
	static int steering = 0;
	int powerVal = getPowVal(APPSVal)/100*255;
	Serial.print(powerVal);
	Serial.print("  ");
	Serial.println(APPSVal);
	
	if(APPSVal > VTH){
    if(steering == 1){
      analogWrite(PowPin, 0);  
      delay(500);
    } 
    steering = 2;
  	analogWrite(PowPin, powerVal);  
    digitalWrite(DirPin, HIGH);
	}else if(APPSVal < VTL){
		if(steering == 2){
		  analogWrite(PowPin, 0);  
      delay(500);
    } 
    steering = 1;
    analogWrite(PowPin, powerVal);	
		digitalWrite(DirPin, LOW);
	}else{
    if(steering != 0) delay(500);
    steering = 0;
		analogWrite(PowPin, 0);
	}
 
}
