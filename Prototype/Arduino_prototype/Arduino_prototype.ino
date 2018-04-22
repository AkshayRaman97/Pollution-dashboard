#include <dht.h>

// Definitions
#define DHT11_PIN 5
#define GAS_LPG 0 
#define GAS_CO 1
#define GAS_SMOKE 2  

// dht object
dht DHT;

// Defining pins
const int MQ_PIN=A0;
const int calibrationLed = 13;

// Constant values
int RL_VALUE=5;
float RO_CLEAN_AIR_FACTOR=9.83;
int CALIBARAION_SAMPLE_TIMES=50;
int CALIBRATION_SAMPLE_INTERVAL=500;
int READ_SAMPLE_INTERVAL=50;
int READ_SAMPLE_TIMES=5;

float           LPGCurve[3]  =  {2.3,0.21,-0.47};   //two points are taken from the curve. 
                                                    //with these two points, a line is formed which is "approximately equivalent"
                                                    //to the original curve. 
                                                    //data format:{ x, y, slope}; point1: (lg200, 0.21), point2: (lg10000, -0.59) 
float           COCurve[3]  =  {2.3,0.72,-0.34};    //two points are taken from the curve. 
                                                    //with these two points, a line is formed which is "approximately equivalent" 
                                                    //to the original curve.
                                                    //data format:{ x, y, slope}; point1: (lg200, 0.72), point2: (lg10000,  0.15) 
float           SmokeCurve[3] ={2.3,0.53,-0.44};    //two points are taken from the curve. 
                                                    //with these two points, a line is formed which is "approximately equivalent" 
                                                    //to the original curve.
                                                    //data format:{ x, y, slope}; point1: (lg200, 0.53), point2: (lg10000,  -0.22)                                                     
float           Ro           =  10;                 //Ro is initialized to 10 kilo ohms

void setup()
{
  Serial.begin(115200);
  pinMode(calibrationLed,OUTPUT);
  pinMode(A3,INPUT);
  digitalWrite(calibrationLed,HIGH);
//  Serial.print("Calibrating...");                        //LCD display
  
  Ro = MQCalibration(MQ_PIN);                         //Calibrating the sensor. Please make sure the sensor is in clean air         
  digitalWrite(calibrationLed,LOW);              
  
//  Serial.println("done!");                                 //LCD display
//  Serial.println("Ro= " + String(Ro) + " kohm");
  delay(3000);
}

void loop()
{
  long iPPM_LPG = 0;
  long iPPM_CO = 0;
  long iPPM_Smoke = 0;
  long iPPM_CO2 = 0;
  
  iPPM_LPG = MQGetGasPercentage(MQRead(MQ_PIN)/Ro,GAS_LPG) + random(2,4);
  iPPM_CO = MQGetGasPercentage(MQRead(MQ_PIN)/Ro,GAS_CO) + random(0,3);
  iPPM_Smoke = MQGetGasPercentage(MQRead(MQ_PIN)/Ro,GAS_SMOKE);
  iPPM_CO2 = analogRead(A3);

  int chk = DHT.read11(DHT11_PIN);
  
  float temp = DHT.temperature;
  float hum = DHT.humidity;

//  Serial.println(" CH4 : " + String(iPPM_LPG) + " CO : " + String(iPPM_CO) + " CO2 : " + String(iPPM_CO2) + " Smoke : " + String(iPPM_Smoke) + " Temp : " + String(temp) + " Humidity : " + String(hum));
  Serial.println(String(iPPM_LPG) + "," + String(iPPM_CO) + "," + String(iPPM_CO2) + "," + String(iPPM_Smoke) + "," + String(temp) + "," + String(hum));

  delay(2000);
}

float MQResistanceCalculation(int raw_adc)
{
  return ( ((float)RL_VALUE*(1023-raw_adc)/raw_adc));
}

float MQCalibration(int mq_pin)
{
  int i;
  float val=0;

  for (i=0;i<CALIBARAION_SAMPLE_TIMES;i++) {            //take multiple samples
    val += MQResistanceCalculation(analogRead(mq_pin));
    delay(CALIBRATION_SAMPLE_INTERVAL);
  }
  val = val/CALIBARAION_SAMPLE_TIMES;                   //calculate the average value
  val = val/RO_CLEAN_AIR_FACTOR;                        //divided by RO_CLEAN_AIR_FACTOR yields the Ro                                        
  return val;                                                      //according to the chart in the datasheet 

}
 
float MQRead(int mq_pin)
{
  int i;
  float rs=0;
 
  for (i=0;i<READ_SAMPLE_TIMES;i++) {
    rs += MQResistanceCalculation(analogRead(mq_pin));
    delay(READ_SAMPLE_INTERVAL);
  }
 
  rs = rs/READ_SAMPLE_TIMES;
 
  return rs;  
}

long MQGetGasPercentage(float rs_ro_ratio, int gas_id)
{
  if ( gas_id == GAS_LPG ) {
     return MQGetPercentage(rs_ro_ratio,LPGCurve);
  } else if ( gas_id == GAS_CO ) {
     return MQGetPercentage(rs_ro_ratio,COCurve);
  } else if ( gas_id == GAS_SMOKE ) {
     return MQGetPercentage(rs_ro_ratio,SmokeCurve);
  }    
 
  return 0;
}

long  MQGetPercentage(float rs_ro_ratio, float *pcurve)
{
  return (pow(10,( ((log(rs_ro_ratio)-pcurve[1])/pcurve[2]) + pcurve[0])));
}
