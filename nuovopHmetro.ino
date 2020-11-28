//-------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------
//INCLUDE LIBRARIES
//TEMPERATURE
#include <OneWire.h>
#include <DallasTemperature.h>
//STANDARD LIBRARY
#include <stdio.h>
//-------------------------------------------------------------------------------------------------------------------------------
//OBJECTS
OneWire oneWire(5);                                                            //object that manages the communication. Pin 5
DallasTemperature temp(&oneWire);                                              //the parameter is the reference of the object
//-------------------------------------------------------------------------------------------------------------------------------
//DIRECTIVES
#define SensorPin A0                                                           //pH meter Analog output to Arduino Analog Input 0
#define Offset 0.00                                                            //deviation compensate
#define LED 13
#define samplingInterval 20
#define printInterval 1000
#define ArrayLenth  40                                                         //times of collection
//-------------------------------------------------------------------------------------------------------------------------------
//DECLARATION OF CREATEMATRIX FUNCTION
double** createMatrix(int r, int c);
//-------------------------------------------------------------------------------------------------------------------------------
//DECLARATION GLOBAL VARIABLES
int pHArray[ArrayLenth];                                                       //Store the average value of the sensor feedback
int pHArrayIndex=0;  
float tcValue;                                                                 //temperature in °C
double result;  
//-------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------
//SETUP
void setup(void){
  pinMode(LED,OUTPUT);  
  Serial.begin(9600);  
  temp.begin();
  Serial.println("pH meter experiment!");                                      //Test the serial monitor
}
//-------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------
//LOOP
void loop(void){ 
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float pHValue,voltage;
  if(millis()-samplingTime > samplingInterval) {
    //TEMPERATURE
    temp.requestTemperatures();
    tcValue=temp.getTempCByIndex(0); //temperatura in Celsius
    double coeff=tcValue/5;
    int tcApproxValue = floor(coeff)*5;
    //pH
    pHArray[pHArrayIndex++]=analogRead(SensorPin);
    if(pHArrayIndex==ArrayLenth)pHArrayIndex=0;
    voltage = avergearray(pHArray, ArrayLenth)*5.0/1024;
    pHValue = 3.5*voltage+Offset;
    int pHValueApprox = round(pHValue);
    samplingTime=millis();
    //load matrix function
    int r = 17, c = 11;
    int i, j;
    double** A = createMatrix(r, c);
    //ROW CHOICE
    switch (tcApproxValue) {
        case 5:
            i = 0;
        break;
        case 10:
            i = 1;
        break;
        case 15:
            i = 2;
        break;
        case 20:
            i = 3;
        break;
        case 25:
            i = 4;
        break;
        case 30:
            i = 5;
        break;  
        case 35:
            i = 6;
        break;
        case 40:
            i = 7;
        break;
        case 45:
            i = 8;
        break;
        case 50:
            i = 9;
        break;
        case 55:
            i = 10;
        break;
        case 60:
            i = 11;
        break;
        case 65:
            i = 12;
        break;
        case 70:
            i = 13;
        break;
        case 75:
            i = 14;
        break;
        case 80:
            i = 15;
        break;
        case 85:
            i = 16;
        break;
    }
    //COLUMN CHOICE
    switch (pHValueApprox) {
        case 2:
            j = 0;
        break;
        case 3:
            j = 1;
        break;
        case 4:
            j = 2;
        break;
        case 5:
            j = 3;
        break;
        case 6:
            j = 4;
        break;
        case 7:
            j = 5;
        break;
        case 8:
            j = 6;
        break;
        case 9:
            j = 7;
        break;
        case 10:
            j = 8;
        break;
        case 11:
            j = 9;
        break;
    }
    result=pHValueApprox+A[i][j];
  }
  if(millis() - printTime > printInterval){   //Every 1000 milliseconds, print a numerical, convert the state of the LED indicator
    Serial.print("Voltage:");
        Serial.print(voltage,2);
        Serial.print("    pH value: ");
    Serial.println(result,2);
        digitalWrite(LED,digitalRead(LED)^1);
        printTime=millis();
  }
}
//-------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------
//LOAD AVERGEARRAY FUNCTION
double avergearray(int* arr, int number){
  int i;
  int max,min;
  double avg;
  long amount=0;
  if(number<=0){
    Serial.println("Error number for the array to avraging!/n");
    return 0;
  }
  if(number<5){   //less than 5, calculated directly statistics
    for(i=0;i<number;i++){
      amount+=arr[i];
    }
    avg = amount/number;
    return avg;
  }else{
    if(arr[0]<arr[1]){
      min = arr[0];max=arr[1];
    }
    else{
      min=arr[1];max=arr[0];
    }
    for(i=2;i<number;i++){
      if(arr[i]<min){
        amount+=min;        //arr<min
        min=arr[i];
      }else {
        if(arr[i]>max){
          amount+=max;    //arr>max
          max=arr[i];
        }else{
          amount+=arr[i]; //min<=arr<=max
        }
      }//if
    }//for
    avg = (double)amount/(number-2);
  }//if
  return avg;
}
//-------------------------------------------------------------------------------------------------------------------------------
//LOAD CREATE MATRIX FUNCTION
double** createMatrix(int r, int c) {
    int i, j;
    double** A;
    A = new(double* [r]);
    for (i = 0; i < r; i++) {
        A[i] = new(double[c]);
    }
    for (i = 0; i < r; i++) {
        for (j = 0; j < c; j++) {
            A[i][j] = 0;
        }
    }

//pH          2               3              4               5              6               7           8               9              10              11             12             
/*5 °C*/      A[0][0]=0.30;   A[0][1]=0.24;  A[0][2]=0.18;   A[0][3]=0.12;  A[0][4]=0.06;   A[0][5]=0;  A[0][6]=0.06;   A[0][7]=0.12;  A[0][8]=0.18;   A[0][9]=0.24;  A[0][10]=0.30; 
/*10 °C*/     A[1][0]=0.225;  A[1][1]=0.18;  A[1][2]=0.135;  A[1][3]=0.09;  A[1][4]=0.045;  A[1][5]=0;  A[1][6]=0.045;  A[1][7]=0.09;  A[1][8]=0.135;  A[1][9]=0.18;  A[1][10]=0.225;
/*15 °C*/     A[2][0]=0.15;   A[2][1]=0.12;  A[2][2]=0.09;   A[2][3]=0.06;  A[2][4]=0.03;   A[2][5]=0;  A[2][6]=0.03;   A[2][7]=0.06;  A[2][8]=0.09;   A[2][9]=0.12;  A[2][10]=0.15; 
/*20 °C*/     A[3][0]=0.075;  A[3][1]=0.06;  A[3][2]=0.045;  A[3][3]=0.03;  A[3][4]=0.015;  A[3][5]=0;  A[3][6]=0.015;  A[3][7]=0.03;  A[3][8]=0.045;  A[3][9]=0.06;  A[3][10]=0.075;
/*25 °C*/     A[4][0]=0;      A[4][1]=0;     A[4][2]=0;      A[4][3]=0;     A[4][4]=0;      A[4][5]=0;  A[4][6]=0;      A[4][7]=0;     A[4][8]=0;      A[4][9]=0;     A[4][10]=0; 
/*30 °C*/     A[5][0]=0.075;  A[5][1]=0.06;  A[5][2]=0.045;  A[5][3]=0.03;  A[5][4]=0.015;  A[5][5]=0;  A[5][6]=0.015;  A[5][7]=0.03;  A[5][8]=0.045;  A[5][9]=0.06;  A[5][10]=0.075;
/*35 °C*/     A[6][0]=0.15;   A[6][1]=0.12;  A[6][2]=0.09;   A[6][3]=0.06;  A[6][4]=0.03;   A[6][5]=0;  A[6][6]=0.03;   A[6][7]=0.06;  A[6][8]=0.09;   A[6][9]=0.12;  A[6][10]=0.15; 
/*40 °C*/     A[7][0]=0.225;  A[7][1]=0.18;  A[7][2]=0.135;  A[7][3]=0.09;  A[7][4]=0.045;  A[7][5]=0;  A[7][6]=0.045;  A[7][7]=0.09;  A[7][8]=0.135;  A[7][9]=0.18;  A[7][10]=0.225;
/*45 °C*/     A[8][0]=0.30;   A[8][1]=0.24;  A[8][2]=0.18;   A[8][3]=0.12;  A[8][4]=0.06;   A[8][5]=0;  A[8][6]=0.06;   A[8][7]=0.12;  A[8][8]=0.18;   A[8][9]=0.24;  A[8][10]=0.30; 
/*50 °C*/     A[9][0]=0.375;  A[9][1]=0.30;  A[9][2]=0.225;  A[9][3]=0.15;  A[9][4]=0.075;  A[9][5]=0;  A[9][6]=0.075;  A[9][7]=0.15;  A[9][8]=0.225;  A[9][9]=0.30;  A[9][10]=0.375; 
/*55 °C*/     A[10][0]=0.45;  A[10][1]=0.36; A[10][2]=0.27;  A[10][3]=0.18; A[10][4]=0.09;  A[10][5]=0; A[10][6]=0.09;  A[10][7]=0.18; A[10][8]=0.27;  A[10][9]=0.36; A[10][10]=0.45; 
/*60 °C*/     A[11][0]=0.525; A[11][1]=0.42; A[11][2]=0.315; A[11][3]=0.21; A[11][4]=0.105; A[11][5]=0; A[11][6]=0.105; A[11][7]=0.21; A[11][8]=0.315; A[11][9]=0.42; A[11][10]=0.525;
/*65 °C*/     A[12][0]=0.60;  A[12][1]=0.48; A[12][2]=0.36;  A[12][3]=0.24; A[12][4]=0.12;  A[12][5]=0; A[12][6]=0.12;  A[12][7]=0.24; A[12][8]=0.36;  A[12][9]=0.48; A[12][10]=0.60;
/*70 °C*/     A[13][0]=0.675; A[13][1]=0.54; A[13][2]=0.405; A[13][3]=0.27; A[13][4]=0.135; A[13][5]=0; A[13][6]=0.135; A[13][7]=0.27; A[13][8]=0.405; A[13][9]=0.54; A[13][10]=0.675;
/*75 °C*/     A[14][0]=0.75;  A[14][1]=0.60; A[14][2]=0.45;  A[14][3]=0.3;  A[14][4]=0.15;  A[14][5]=0; A[14][6]=0.15;  A[14][7]=0.30; A[14][8]=0.45;  A[14][9]=0.60; A[14][10]=0.75;
/*80 °C*/     A[15][0]=0.825; A[15][1]=0.66; A[15][2]=0.495; A[15][3]=0.33; A[15][4]=0.165; A[15][5]=0; A[15][6]=0.165; A[15][7]=0.33; A[15][8]=0.495; A[15][9]=0.66; A[15][10]=0.825;
/*85 °C*/     A[16][0]=0.90;  A[16][1]=0.72; A[16][2]=0.54;  A[16][3]=0.36; A[16][4]=0.18;  A[16][5]=0; A[16][6]=0.18;  A[16][7]=0.36; A[16][8]=0.54;  A[16][9]=0.72; A[16][10]=0.90;

    return A;
}
