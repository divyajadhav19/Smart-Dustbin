unsigned int A=8;
unsigned int B=9;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(A,OUTPUT);
//pinMode(B,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
int sensorvalue= analogRead(A2);
Serial.println("Moisture Present In Soil=");
Serial.println(sensorvalue);
delay(1000);

if(sensorvalue<=700)
{
  digitalWrite(A,LOW);
}
else
{
  digitalWrite(A,HIGH);
   
  }

}
