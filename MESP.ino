#include "Wire.h"
#include <MPU6050_light.h>
#include <BleMouse.h>

const int RED = 27; // red-27 blie 26 
const int BLUE = 26;
const int YELLOW = 25;

const int mouseLButton = 5;
unsigned long lastDebounceTime = 0;

bool lastButtonState = HIGH;
bool buttonState = HIGH;

int pwm_freq = 5000; //5kHz
int pwm_res = 8; //8bit
BleMouse bleMouse("MESP","me",100);//name manufacturer, battery level (battery level indicator to be added)
MPU6050 mpu(Wire);
unsigned long timer = 0;


void setup() {
  ledcAttach(RED, pwm_freq, pwm_res);  
  ledcAttach(BLUE, pwm_freq, pwm_res);
  ledcAttach(YELLOW, pwm_freq, pwm_res);

  ledcWrite(RED, 0);  
  ledcWrite(BLUE, 0);  
  ledcWrite(YELLOW, 0);  

  pinMode(mouseLButton,INPUT_PULLUP);
  Serial.begin(115200);
  Wire.begin();
  bleMouse.begin();

  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while(status!=0){ } 
  
  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  ledcWrite(RED, 10);
  ledcWrite(BLUE, 50);delay(300);  
  ledcWrite(RED, 0);
  ledcWrite(BLUE, 0);delay(300);
  ledcWrite(RED, 10);
  ledcWrite(BLUE, 50);delay(300);
  ledcWrite(RED, 0);
  ledcWrite(BLUE, 0);delay(300);
  //mpu.upsideDownMounting = true; // uncomment this line if the MPU6050 is mounted upside-down
  mpu.calcOffsets(); 
  Serial.println("Done!\n");
}

void loop() {

  if(bleMouse.isConnected()){
    ledcWrite(BLUE, 100);
    ledcWrite(RED, 0);
    mpu.update();
    int reading = digitalRead(mouseLButton);
    if(reading != lastButtonState){
      lastDebounceTime = millis();
    }
    if((millis()-lastDebounceTime)>50){
      if(reading != buttonState){
        buttonState=reading;
        
        if(buttonState == LOW){
          bleMouse.click(MOUSE_LEFT);
          ledcWrite(YELLOW, 255);
          Serial.println("button clicked");
       }else{
          ledcWrite(YELLOW, 0);
       }
      }
    }
    lastButtonState = reading;
    
    if((millis()-timer)>10){ 
      int x=mpu.getAngleX();
      int y=mpu.getAngleY();
      Serial.print("X : ");
      Serial.print(x);
      Serial.print("\tY : ");
      Serial.print(y);
      //int cursorX = map(x,-90,90,-7,7);
      int cursorX = map(x,-90,90,15,-15);
      int cursorY = map(y,-90,90,15,-15);
      if(cursorX > -2 && cursorX < 2) cursorX=0;
      if(cursorY > -2 && cursorY < 2) cursorY=0;
      timer = millis();  
      bleMouse.move(cursorX,cursorY);
      
    }
  }else{
    ledcWrite(RED, 100);
    ledcWrite(BLUE, 0);
  }
}
