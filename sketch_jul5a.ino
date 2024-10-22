#define airPumpPin    5                    
#define airValvePin   6                    
#define airPumpON     HIGH                  
#define airPumpOFF    !airPumpON             
#define airValveON    HIGH                
#define airValveOFF   !airValveON           

#define initializeTime    2000             
unsigned long runRecordTime = 0;           
#define pulseInflateTime    250            
#define pulseDeflateTime    200            
uint8_t systemRunState = 0x00;             

#define soundSensorPin  A0  
#define threshold       600 
#define lowThreshold    300

void setup() {
  Serial.begin(9600);
  pinMode(airPumpPin, OUTPUT);             
  pinMode(airValvePin, OUTPUT);            
  pinMode(soundSensorPin, INPUT);          
  digitalWrite(airPumpPin, airPumpOFF);    
  digitalWrite(airValvePin, airValveOFF);  

  digitalWrite(airPumpPin, airPumpON);     
  delay(initializeTime);                   
  digitalWrite(airPumpPin, airPumpOFF);    
}

void loop() {
  int sensorValue = analogRead(soundSensorPin); 
  Serial.print("Sensor Value: ");
  Serial.println(sensorValue);

  static unsigned long lastTriggerTime = 0; 
  static bool pumpState = false;            

  unsigned long currentMillis = millis();
  if (sensorValue > threshold) {
    if (currentMillis - lastTriggerTime > 70) { 
      lastTriggerTime = currentMillis;

      if (pumpState) {
        digitalWrite(airPumpPin, airPumpOFF); 
        digitalWrite(airValvePin, airValveON); 
        delay(pulseDeflateTime);
        digitalWrite(airValvePin, airValveOFF); 
        pumpState = false;
      } else {
        digitalWrite(airPumpPin, airPumpON); 
        delay(pulseInflateTime); 
        digitalWrite(airPumpPin, airPumpOFF); 
        pumpState = true;
      }
    }
  } else if (sensorValue < lowThreshold) {
    digitalWrite(airPumpPin, airPumpOFF);
    digitalWrite(airValvePin, airValveOFF);
    pumpState = false;
  }
  delay(10); 
}
