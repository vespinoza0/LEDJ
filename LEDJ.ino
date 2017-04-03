#include <MP3.h>
#include <SoftwareSerial.h>
#include <IRremote.h>
#include <LedControl.h>
#include <Servo.h>
///////////////////////////////////////////////////////////////////////////
/////LedControl//////
int DIN = 24;  //DIN connected to MAX7219
int CS =  26;  //CS connected to MAX7219
int CLK = 28;  //CLK connected to MAX7219
//////Patterns for LED Matrices to print/////////
//NOTE THAT MY SECOND MATRIX WAS PRINTED UPSIDE DOWN//
byte smile[8]= {0x3C,0x42,0x95,0xA1,0xA1,0x95,0x42,0x3C};
byte smile2[8]= {0x3C,0x42,0xA9,0x85,0x85,0xA9,0x42,0x3C};
byte frown[8]= {0x3C,0x42,0xA5,0x91,0x91,0xA5,0x42,0x3C};
byte frown2[8]= {0x3C,0x42,0xA5,0x89,0x89,0xA5,0x42,0x3C};
byte neutral[8]= {0x3C,0x42,0xA5,0xA1,0xA1,0xA5,0x42,0x3C};
byte neutral2[8]= {0x3C,0x42,0xA5,0x85,0x85,0xA5,0x42,0x3C};
byte first[8]= {0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00};
byte second[8]= {0x00,0x00,0x3C,0x3C,0x3C,0x3C,0x00,0x00};
byte third[8]= {0x00,0x7E,0x7E,0x7E,0x7E,0x7E,0x7E,0x00};
byte fourth[8]= {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
byte right[8]= {0x18,0x18,0x18,0x18,0xFF,0x7E,0x3C,0x18};
byte left[8]= {0x18,0x3C,0x7E,0xFF,0x18,0x18,0x18,0x18}; 
byte up[8]= {0x08,0x0C,0x0E,0xFF,0xFF,0x0E,0x0C,0x08};
byte down[8]= {0x10,0x30,0x70,0xFF,0xFF,0x70,0x30,0x10};
byte A[8]= {0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,};
byte B[8]= {0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0};
byte C[8]= {0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0};
byte D[8]= {0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0};
byte E[8]= {0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8};
byte F[8]= {0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC};
byte G[8]= {0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE};
byte AA[8]= {0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01};
byte BB[8]= {0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03};
byte CC[8]= {0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07};
byte DD[8]= {0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F};
byte EE[8]= {0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F};
byte FF[8]= {0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F};
byte GG[8]= {0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F};

LedControl lc=LedControl(DIN,CLK,CS,2);
//////MP3//////
MP3 mp3; //define mp3 class
int volume = 15;  //default volume
//////IR module//////
int  irRemotePin =  32;  //IR pin = 32
IRrecv irrecv(irRemotePin);
decode_results results;  //decodes IR signal received 
int power = 22;    //power supply for IR receiver created in case you need an extra power pin. I plugged it in the 5V on top of the MP3 shield
//////servo////// 
int pos = 0;
Servo myservo;  // create servo object to control a servo
/////LEDJ eyes//////
int eyes = 40;  //eyes pin = 40
int pauseCount = 0; // int created to count the number of times pause/play is pressed (pause and play are controlled by the same button) 
int flag=0;  //int created to turn OFF/ON device and to swtich to different patterns that are controlled by speaker voltage
///////////////////////////////////////////////////////////////////////////
// the setup routine runs once when you press reset:
void setup() {    
///////////////////////////////////////////////////////////////////////////
//////LedControl set-up//////
  lc.shutdown(0,false);    //The MAX7219 is in power-saving mode on startup
  lc.shutdown(1,false);
  lc.setIntensity(0,13);    // Set the brightness to maximum value (1-15)
  lc.setIntensity(1,13);
  lc.clearDisplay(0);         // and clear the display
  lc.clearDisplay(1); 
///////////////////////////////////////////////////////////////////////////
////MP3 set-up
  mp3.volume(volume);    //set volume to the default//
  mp3.set_mode(MP3::RANDOM);    //set MP3 Shield Random mode */
///////////////////////////////////////////////////////////////////////////
////IR set-up 
  pinMode(power, OUTPUT);    // initialize the digital pin as an output
  digitalWrite(power, HIGH);    //set pin output to HIGH
  pinMode(irRemotePin, INPUT);    // initialize the digital pin as an input
  irrecv.enableIRIn();    //Start the receiver  
///////////////////////////////////////////////////////////////////////////  
//////servo//////
  myservo.attach(41);    //servo pin = 41 
////LEDJ eyes
  pinMode(eyes, OUTPUT);    // initialize the digital pin as an output
  digitalWrite(eyes, LOW);    //set pin output to LOW
}
//////the loop routine runs over and over again forever:
void loop() {
  if (irrecv.decode(&results)) {
    irrecv.resume(); // Receive the next value
    switch(results.value)
    {
      case 16712445: if(flag==1 || flag ==2){ mp3.pause(); printPause(); break;} //OK Button Pressed if device has been turned on 
      case 16761405: if(flag==1 || flag ==2){ delay(100); printNext(); mp3.next(); break;}   //"Next button pressed" if device has been turned on 
      case 16720605: if(flag==1 || flag ==2){ delay(100); printPrevious(); mp3.previous();break;}   //"Previous button pressed" if device has been turned on 
      case 16718055: if(flag==1 || flag ==2){ mp3.volume(23); mp3.play_sd(0x0005);break;}   //"5 button pressed" This case statement is not needed. I originally set track005 in SD card to Beethoven's 5th symphony 
      case 16736925: if(flag==1 || flag ==2){ increaseVolume(); mp3.volume(volume); printIncrease();break;}  ///"Volume Up button pressed" if device has been turned on 
      case 16754775: if(flag==1 || flag ==2){ decreaseVolume(); mp3.volume(volume); printDecrease(); break;}   ///"Volume Down button pressed" if device has been turned on 
      case 16738455: if(flag==0){ printSmile(); servoUp(); delay(200); eyesON();delay(200); mp3.begin(MP3_SOFTWARE_SERIAL); mp3.volume(24); mp3.play_sd(0x0001); flag=1; break;}   ///"1 button pressed" turning on the device
      case 16730805: if(flag==1 || flag==2){ printFrown(); eyesOFF(); mp3.stop(); delay(200); servoDown(); printClear(); flag=0; break;}    ///"0 button pressed" turning OFF the device
      case 16726215: if(flag==1){ printByte(fourth); printByte2(fourth); flag=2; break;} //"8 button pressed" if device has been turned on, sets flag to 2 
      case 16728765: if(flag==1 || flag ==2){ mp3.set_mode(MP3::CYCLE); break;}  ///"Star button pressed" if device has been turned on, sets mode to CYCLE songs 
      case 16732845: if(flag==1 || flag ==2){ mp3.set_mode(MP3::RANDOM); break;}   //"Sharp button pressed" if device has been turned on, sets mode to RANDOM songs
      case 16734885: if(flag ==2){ printByte(fourth); printByte2(fourth); flag =1; break;}  //"9 button pressed " if device has been turned on, sets flag to 1
    }
  }
  if(flag==2){ //flag ==2 will print out a vertical pattern on the LED Matrices based on the voltage coming out of the speaker
  int voltage = analogRead(A8);
   if (voltage >= 0 && voltage < 50) {
   lc.clearDisplay(0);  
   lc.clearDisplay(1);
   delay (10);
 }else if (voltage >= 50 && voltage < 170) {
  printByte(A);
  printByte2(AA);
   delay(10);
 }else if (voltage >= 170 && voltage <290) {
  printByte(B);
  printByte2(BB);
   delay(10);
 }else if (voltage >= 290 && voltage < 410) {
   printByte(C);
   printByte2(CC);
   delay(10);
 }else if (voltage >=410 && voltage <= 530) {
    printByte(D); 
    printByte2(DD);
    delay(10);}
    else if (voltage >= 530 && voltage <650) {
  printByte(E);
  printByte2(EE);
   delay(10);
 }else if (voltage >= 650 && voltage < 770) {
   printByte(F);
   printByte2(FF);
   delay(10);
 }else if (voltage >=770 && voltage <= 890) {
    printByte(G); 
    printByte2(GG);
    delay(10);}
  else if (voltage >=890) {
    printByte(fourth); 
    printByte2(fourth);
    delay(10);}
}
//////////////////////////////////////////////////////////////////////
 if(flag==1){//flag ==1 will print out a flashing pattern on the LED Matrices based on the voltage coming out of the speaker
  int voltage = analogRead(A8);
   if (voltage >= 0 && voltage < 50) {
   lc.clearDisplay(0);  
   lc.clearDisplay(1);
   delay (10);
 }else if (voltage >= 94 && voltage < 274) {
  printByte(first);
  printByte2(first);
   delay(10);
 }else if (voltage >= 274 && voltage <524) {
  printByte(second);
  printByte2(second);
   delay(10);
 }else if (voltage >= 524 && voltage < 750) {
   printByte(third);
   printByte2(third);
   delay(10);
 }else if (voltage >=750 && voltage <= 1023) {
    printByte(fourth); 
    printByte2(fourth);
    delay(10);}
}
}
///////////////////////////////////////////////////////////////////////////  
//////////////////////////****FUNCTIONS****////////////////////////// 

void increaseVolume(){    //Increase volume function
  if(volume<31)
  {volume++;}
}
void decreaseVolume(){    //Decrease volume function
  if(volume>=1)
  { volume--;}
}
void printByte(byte character []){    //PrintByte function for first LED Matrix
  int i = 0;
  for(i=0;i<8;i++)
  {lc.setRow(0,i,character[i]);}
}
void printByte2(byte character []){    //PrintByte function for second LED Matrix
  int i = 0;
  for(i=0;i<8;i++)
  {lc.setRow(1,i,character[i]);}
}
void printNext(){      //Function that prints the "right" pattern on both LED matrices to indicate "next song"
   printByte(right); printByte2(left);
   pauseCount=0;
   delay(600);
}
void printPrevious(){      //Function that prints the "left" pattern on both LED matrices to indicate "previous song"
   printByte(left); printByte2(right);
   pauseCount=0;
   delay(600);
}
void printIncrease(){      //Function that prints the "Increase" pattern on both LED matrices ()
   printByte(up); printByte2(down);
   delay(400);
}
void printDecrease(){      //Function that prints the "Decrease" pattern on both LED matrices
   printByte(down); printByte2(up);
   delay(400);
}
  void printFrown(){      //Function that prints the "frown" pattern on both LED matrices
   printByte(frown); printByte2(frown2);
   delay(3000);
}
/////////////////////////////////////////////////////////////////////////////
void printPause(){   //Function that prints the "frown" pattern on both LED matrices if system is paused,
   if(pauseCount%2 == 1){    //or prints the smile pattern if play is pressed. This is determined by the int pauseCount
   printByte(smile); printByte2(smile2);    //considering that play and pause is controlled by the same single button on the control
   delay(1000);
   }else if(pauseCount%2 == 0){    
    printByte(neutral); printByte2(neutral2);
   delay(1000);
   }
   pauseCount++;
}
/////////////////////////////////////////////////////////////////////////////
void printSmile(){      //Function that prints the "smile" pattern on both LED matrices
   printByte(smile); printByte2(smile2);
   delay(2000);
}
void printNeutral(){      //Function that prints the "neutral" pattern on both LED matrices
  printByte(neutral); printByte2(neutral2);
 delay(2000);
}
void printClear(){      //Function that clears both LED matrices
 lc.clearDisplay(0);  
 lc.clearDisplay(1);
}
void eyesON(){      //Function that turns LEDJ eyes ON
  digitalWrite(eyes, HIGH);
}

void eyesOFF(){      //Function that turns LEDJ eyes OFF
  digitalWrite(eyes, LOW);
}
void servoUp(){      //Servo function that turns servo up, summoning LEDJ 
  for(pos = 0; pos < 170; pos += 1){ // in steps of 1 degree 
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(20);                       // waits 20ms for the servo to reach the position 
  }   
} 
void servoDown(){      //Servo function that turns servo down, hiding LEDJ in box 
for(pos = 170; pos>=1; pos-=1){                                
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(20);                       // waits 20ms for the servo to reach the position 
  }
}

  
