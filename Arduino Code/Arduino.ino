/*Automatic Water Bottle Filling  System
 * Bobin B Paul 
 * Noorul Islam Centre for Higher Education
 * Electronics and Instrumentation Engineering
 */

//------------------------------- Include the Library code:----------------------------------------
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 20, 4);


//------------------------------------Define-------------------------------------------------------------
#define CHANNEL_NUMBER 1                           //set the number of channel
#define CHANNEL_DEFAULT_VALUE 1500                 //set the default value
#define FRAME_LENGTH 4800                         //set the PPM frame length in microseconds (1ms = 1000µs)
int  PULSE_LENGTH = 1501;        //set the pulse length in microseconds (The PPM signal pulse width must range from 600us to 2.4ms. The motor speed will be zero at PPM pulse width of 1.51ms)
#define onState 1                                 //set polarity of the pulses: 1 is positive, 0 is negative
#define sigPin 7                                                     //set PPM signal output pin on the arduino
int ppm[CHANNEL_NUMBER];
int ir = 5;                                       // Digtal PIN 2 for IR Sensor
int fillrelay = 6;                                    // Digtal PIN 2 for fillrelay for Water Pump
int filltime = 6000; //                            Filling Time (ms)
int irc = 3;     //IR sensor pin for capping
int caprelay = 2;    // Analog PIN 5 for relay for Capping Motor
int servopin = 8;
Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards
int count=0;
int pos = 45;    // vari  able to store the servo position
int captime = 1800; //                            Capping Time (ms) amount of time to rotate cap
//-----------------------------------------------------------------------------------------------------

void setup() 
{
  
  lcd.begin(20, 4);
  pinMode(ir,INPUT);                              // Input of IR Sensor
  pinMode(fillrelay,OUTPUT);                          //Output of fillrelay for Water Pump
  Serial.begin(9600);
  myservo.attach(servopin);  // attaches the servo on pin 4 to the servo object
  pinMode(irc,INPUT);                              // Input of IR Sensor For Capping
  pinMode(caprelay,OUTPUT);                          //Output of Relay for Capping Motor
  Serial.begin(115200);
  digitalWrite(caprelay,HIGH);
  Serial.println("Bottle Filling System");  
  lcd.setCursor(0, 0);
  lcd.print(">--Bottle Filling--<");
  lcd.setCursor(0, 1);
  lcd.print(">------System------<");
  digitalWrite(fillrelay,HIGH);
   //initiallize default ppm values
  for(int i=0; i<CHANNEL_NUMBER; i++){
      ppm[i]= CHANNEL_DEFAULT_VALUE;
  }

  pinMode(sigPin, OUTPUT);
  digitalWrite(sigPin, !onState);  //set the PPM signal pin to the default state (off)
  
  cli();
  TCCR1A = 0; // set entire TCCR1 register to 0
  TCCR1B = 0;
  OCR1A = 100;  // compare match register, change this
  TCCR1B |= (1 << WGM12);  // turn on CTC mode
  TCCR1B |= (1 << CS11);  // 8 prescaler: 0,5 microseconds at 16mhz
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
  sei();

}

void loop() {
  
  if (digitalRead(ir) == LOW)
{
  filling();                                   //For Filling Process 
   delay(1500);
}  

else{
  
   nofilling();                                    //No Bottle 
    }
 if (digitalRead(irc) == LOW)
  {
    capping();                 // capping process
    counter();
   delay(1500);
   
  }
  else
  {
    nocapping();                // no bottle
  }    

}

ISR(TIMER1_COMPA_vect){  //leave this alone
  static boolean state = true;
  
  TCNT1 = 0;
  
  if (state) {  //start pulse
    digitalWrite(sigPin, onState);
    OCR1A = PULSE_LENGTH * 2;
    state = false;
  } else{  //end pulse and calculate when to start the next pulse
    static byte cur_chan_numb;
    static unsigned int calc_rest;
  
    digitalWrite(sigPin, !onState);
    state = true;

    if(cur_chan_numb >= CHANNEL_NUMBER){
      cur_chan_numb = 0;
      calc_rest = calc_rest + PULSE_LENGTH; 
      OCR1A = (FRAME_LENGTH - calc_rest) * 2;
      calc_rest = 0;
    }
    else{
      OCR1A = (ppm[cur_chan_numb] - PULSE_LENGTH) * 2;
      calc_rest = calc_rest + ppm[cur_chan_numb];
      cur_chan_numb++;
    }     
  }
}

void nofilling()                                  //No Bottle 
{

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(">--Bottle Filling--<");
  lcd.setCursor(0, 1);
  lcd.print(">------System------<");
  lcd.setCursor(0, 2);
  lcd.print("Filling - No Bottle");  
 Serial.println("Filling -No Bottle");
  delay(200);                                      //Refreshing
 PULSE_LENGTH = 600;
  
}
void filling()                                 //For Filling Process
{
  PULSE_LENGTH = 1510;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(">--Bottle Filling--<");
  lcd.setCursor(0, 1);
  lcd.print(">------System------<");
  lcd.setCursor(0, 2);
  Serial.println("Filling - Start");
  lcd.print("Filling Started");
  digitalWrite(fillrelay,LOW);
  delay(filltime); 
  bmove();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(">--Bottle Filling--<");
  lcd.setCursor(0, 1);
  lcd.print(">------System------<");
  lcd.setCursor(0, 2);
  Serial.println("Filling - Completed");
  lcd.print("Filling Completed");  
  digitalWrite(fillrelay,HIGH);
 

 }

 void bmove()
 {
       PULSE_LENGTH = 600;
 }
 void capping()                                 //For Capping Process
{
  
    myservo.write(95);              // tell servo to go to position in variable
        Serial.println("servo start");    
    delay(750);   
    digitalWrite(caprelay,LOW);
            Serial.println("relay start"); 
    delay(captime); 
    digitalWrite(caprelay,HIGH);
            Serial.println("relay stop");    
    delay(750); 
    myservo.write(45);              // tell servo to go to position in variable 
            Serial.println("servo back");    
    delay(500); 


}
void nocapping()                                 //For No bottle for Capping
{    
    myservo.write(45);              // tell servo to go to position in variable 
    delay(500); 
} 
 void counter()
 {
      count++;
      String StrUno = "Counter: ";
      String StrDos = StrUno + count;
      Serial.println(StrDos);    
 }
 //Bobin
 //Credit me if you like
