//Farhaan Nazirkhan (nfarhaan.com) & Sarwin Rajiah's C++ Code

#include <SoftwareSerial.h>;
SoftwareSerial mySerial(2, 3); //SIM800L Tx & Rx is connected to Arduino #3 & #2

void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}

// SIM RELATED STUFF
// SMS RELATED STUFF
void SendMessage(String message)
{
  Serial.print("sending message:");
  Serial.println(message);

  mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();

  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  mySerial.println("AT+CMGS=\"+23057714573\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerial();
  mySerial.print(message); //text content
  updateSerial();
  mySerial.write(26);

  Serial.println("Message sent");

}

#define LIQUID_SWITCH_VALUE 1


// WATER RELATED STUFF ( moisture ) --------------------------

#define WATER_FLOAT_SENSOR 4
int is_wtank_empty;

#define WATER_MOTOR 11
int moisture = 0;
int threshold_moisture = 100;
long last_water_pump_active = 0; // the last time the pump was active
long moisture_duration = 5000;   // time in miliseconds for which the water pump will be active
long interval_moisture = 8000;   // the delay between when the pump can be activated again (so as to ensure proper distribution of already supplied liquid)
int flag_water = 0;

void checkWaterLevel()
{
  // water level read 0 = LOW = empty, 1 = HIGH = full
  is_wtank_empty = digitalRead(WATER_FLOAT_SENSOR);
  //Serial.println(is_wtank_empty);

  if (is_wtank_empty == LIQUID_SWITCH_VALUE && flag_water == 0)
  {
    // Serial.println("water tank empty");
    SendMessage("Water tank is empty");
    flag_water = 1;
  }
  if (flag_water == 1 && is_wtank_empty != LIQUID_SWITCH_VALUE)
  {
    flag_water = 0;
  }
}

// checks and regulates the moisture level
void moisture_check_reg()
{
  // moisture read
  moisture = analogRead(A3);
  //Serial.println(moisture);
  if (moisture > threshold_moisture && flag_water == 0)
  { // need to activate water pump
    if (last_water_pump_active + interval_moisture <= millis() || last_water_pump_active == 0)
    {
      last_water_pump_active = millis();

      Serial.println("water_watering");

      digitalWrite(WATER_MOTOR, LOW);
      delay(moisture_duration);

      digitalWrite(WATER_MOTOR, HIGH);

      Serial.println("water_done_watering");
    }
    Serial.println("water_waiting (pump recently activated)");
  }
  else
  {
    digitalWrite(WATER_MOTOR, HIGH);
  }
}
// PH VALUE TO BE CHANGE HERE
#define ph 7


// NPH RELATED STUFF --------------------------

#define NPH_FLOAT_SENSOR 5
int is_nph_tank_empty;

#define NPH_MOTOR 13
int threshold_nph = 8;
long last_nph_pump_active = 0; // the last time the pump was active
long nph_duration = 5000;      // time in miliseconds for which the nph pump will be active
long interval_nph = 8000;      // the delay between when the pump can be activated again (so as to ensure proper distribution of already supplied liquid)
int flag_nph = 0;

void checkNPHLevel()
{
  // water level read 0 = LOW = empty, 1 = HIGH = full
  is_nph_tank_empty = digitalRead(NPH_FLOAT_SENSOR);

  if (is_nph_tank_empty == LIQUID_SWITCH_VALUE && flag_nph == 0)
  {
    // Serial.println("nph tank empty");

    SendMessage("NPH tank is empty");
    flag_nph = 1;
  }
  if (flag_nph == 1 && is_nph_tank_empty != LIQUID_SWITCH_VALUE)
  {
    flag_nph = 0;
  }
}

// checks and regulates the nph level
void nph_check_reg()
{

  // ph read
  // ph = analogRead(A2);

  if (ph > threshold_nph && flag_nph == 0)
  { // need to activate npm pump to reduce ph
    if (last_nph_pump_active + interval_nph <= millis() || last_nph_pump_active == 0)
    {
      last_nph_pump_active = millis();

      Serial.println("nph_watering");

      digitalWrite(NPH_MOTOR, LOW);
      delay(nph_duration);
      digitalWrite(NPH_MOTOR, HIGH);

      Serial.println("nph_done_watering");
    }
    Serial.println("nph_waiting (pump recently activated)");
  }
  else{
      digitalWrite(NPH_MOTOR, HIGH);
  }
}

// PPH RELATED STUFF --------------------------

#define PPH_FLOAT_SENSOR 6
int is_pph_tank_empty;

#define PPH_MOTOR 12
int threshold_pph = 6;
long last_pph_pump_active = 0; // the last time the pump was active
long pph_duration = 5000;      // time in miliseconds for which the nph pump will be active
long interval_pph = 8000;      // the delay between when the pump can be activated again (so as to ensure proper distribution of already supplied liquid)
int flag_pph = 0;

void checkPPHLevel()
{
  // water level read 0 = LOW = empty, 1 = HIGH = full
  is_pph_tank_empty = digitalRead(PPH_FLOAT_SENSOR);

  if (is_pph_tank_empty == LIQUID_SWITCH_VALUE && flag_pph == 0)
  {
    // Serial.println("pph tank empty");
    SendMessage("PPH tank is empty");
    flag_pph = 1;
  }
  if (flag_pph == 1 && is_pph_tank_empty != LIQUID_SWITCH_VALUE)
  {
    flag_pph = 0;
  }
}

// checks and regulates the pph level
void pph_check_reg()
{
  // ph read
  // ph = analogRead(A2);

  if (ph < threshold_pph && flag_pph == 0)
  { // need to activate npm pump to reduce ph
    if (last_pph_pump_active + interval_pph <= millis() || last_pph_pump_active == 0)
    {
      last_pph_pump_active = millis();
      Serial.println("pph_watering");

      digitalWrite(PPH_MOTOR, LOW);
      delay(pph_duration);
      digitalWrite(PPH_MOTOR, HIGH);

      Serial.println("pph_done_watering");
    }
    Serial.println("pph_waiting (pump recently activated)");
  }
  else{
      digitalWrite(PPH_MOTOR, HIGH);
  }
}

// TEMPERATURE RELATED STUFF --------------------------
#define FAN 10
int temperature = 0;
int T_temp = 20;
// checks and regulates the temperature
void temp_check_reg()
{
  // temperature read
  temperature = analogRead(A0);
  temperature = ((temperature * 500) / 1023) - 5;
  
  Serial.println(temperature);

  if (temperature >= T_temp)
  {
    //Serial.print("fan is on");
    digitalWrite(FAN, HIGH);
    // temperature = (-40 + 0.488155 * (analogRead(A0) +20))
  }
  else
  {
    digitalWrite(FAN, LOW);
  }
}

// LUMINOSITY RELATED STUFF --------------------------
#define LIGHT_RELAY 9
int T_lux = 200;
int luminosity = 0;
// checks and regulates the luminosity
void lux_check_reg()
{
    // luminosity read
    luminosity = analogRead(A1);
    // Serial.println("L:"+luminosity);
    if (luminosity <= T_lux)
    {
        digitalWrite(LIGHT_RELAY, HIGH);
    }
    else
    {
        digitalWrite(LIGHT_RELAY, LOW);
    }
}

void setup()
{
  Serial.begin(9600);


  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  
  //Begin serial communication with Arduino and SIM800L
  mySerial.begin(9600);

  Serial.println("Initializing..."); 



  // Water Check
  pinMode(WATER_FLOAT_SENSOR, INPUT_PULLUP);
  flag_water = 0;

  // PPH Check
  pinMode(PPH_FLOAT_SENSOR, INPUT_PULLUP);
  flag_pph = 0;

  // NPH Check
  pinMode(NPH_FLOAT_SENSOR, INPUT_PULLUP);
  flag_nph = 0;

  // Moisture sensor
  pinMode(A3, INPUT);
  pinMode(WATER_MOTOR, OUTPUT);

  // NPH sensor
  pinMode(A2, INPUT);
  pinMode(NPH_MOTOR, OUTPUT);

  // PPH sensor
  pinMode(A2, INPUT);
  pinMode(PPH_MOTOR, OUTPUT);

  // temperature sensor
  pinMode(10, OUTPUT);
  pinMode(A0, INPUT);

  // Luminosity sensor
  pinMode(A1, INPUT);
  pinMode(9, OUTPUT);
}

void loop()
{
  checkWaterLevel();
  checkPPHLevel(); 
  checkNPHLevel();

  moisture_check_reg();
  nph_check_reg();

  pph_check_reg();
  lux_check_reg();
  temp_check_reg();

}
