#include <Servo.h>   //servo library
#include<SoftwareSerial.h>
SoftwareSerial GSM(5, 6);
Servo servo;     
int trigPin = 5;    
int echoPin = 6;   
int servoPin = 7;

long duration, dist, average;   
long aver[3];   //array for average
int buzzerPin = 8;                          //Pin number the buzzer is connected
int echoPin2 = 11;                           // Echo Pin of the ultrasonic sensor
int trigPin2 = 12;                           // Trigger Pin of the ultrasonic sensor
unsigned long previousMillis = 0;           //This will be used to store the millisecnd value after each time the buzzer is triggered. This is used because delay function is not used for the buzzer timing.
int maxBeepInterval = 1000;                 //Max beep interval
int minBeebInterval = 300;                   //Min beep interval
int frequency = 1000;                       //Buzzer frequency in Hz
long distance;  //Distance of the object from sensorlong echoTime;                              //Time taken for the echo
float echoTime;
int triggerAlarmDistance = 10;             //Buzzer will be triggered if the distance is less than this value. (cm)

void setup() 
{   
    Serial.begin(9600);
    servo.attach(servoPin);  
    pinMode(trigPin, OUTPUT);  
    pinMode(echoPin, INPUT);  
    servo.write(0);         //close cap on power on
    delay(100);
    servo.detach(); 
    pinMode(buzzerPin, OUTPUT);               //Set pin modes 
    pinMode(echoPin, INPUT);  
    pinMode(trigPin, OUTPUT); 
    GSM.begin(9600);               // the GPRS baud rate 
    Serial.begin(9600);             // the Serial port of Arduino baud rate. 
    delay(500);
} 

void measure() {  
digitalWrite(10,HIGH);
digitalWrite(trigPin, LOW);
delayMicroseconds(5);
digitalWrite(trigPin, HIGH);
delayMicroseconds(15);
digitalWrite(trigPin, LOW);
pinMode(echoPin, INPUT);
duration = pulseIn(echoPin, HIGH);
dist = (duration/2) / 29.1;    //obtain distance
}
void loop() { 
  for (int i=0;i<=2;i++) {   //average distance
    measure();               
   aver[i]=dist;            
    delay(10);              //delay between measurements
  }
 dist=(aver[0]+aver[1]+aver[2])/3;    

if ( dist<50 ) {
//Change distance as per your need
 servo.attach(servoPin);
  delay(1);
 servo.write(0);  
 delay(3000);       
 servo.write(150);    
 delay(1000);
 servo.detach();      
}
Serial.print(dist);

if(distance < triggerAlarmDistance) 
{   
unsigned long currentMillis = millis();    
int val = map(distance, 2, triggerAlarmDistance, minBeebInterval, maxBeepInterval);     
sendSMS();  
Serial.println("Sent SMS!");   
Serial.println( "dustbin is reached max level");  
delay(10);
if (currentMillis - previousMillis >= val) 
{     
tone(buzzerPin, frequency, val * 0.75);  //Trigger the buzzer (Buzzer pin, frequency, duration). Here durationn is set to 75% of the time before it will be triggered next.         
previousMillis = currentMillis;          //Save current millisend value when the buzzer is triggered.     
} 
}    
delay(6000);                                  //Delay 60ms before next reading.
}

void sendSMS() 
{ 
GSM.println("AT+CMGF=1");    // Set modem to text mode 
delay(2000);
GSM.write("AT+CMGS=");      // Start composing message  
GSM.write(0x22);            // hex equivalent of double-quote '"' 
GSM.write("+918378935180"); // the number to send the SMS to  
GSM.write(0x22);  GSM.write(0x0D);            // hex equivalent of Carraige return  
GSM.write(0x0A);            // hex equivalent of newline  
delay(2000); 
GSM.print("Alert! Dustbin is Full");  // Send the text message to the GSM module  
GSM.write(0x0D);            // hex equivalent of Carraige return  
GSM.write(0x0A);            // hex equivalent of newline  
delay(1000); 
GSM.write(26);        // equivalent of CTRL-Z  delay(3000);
}
