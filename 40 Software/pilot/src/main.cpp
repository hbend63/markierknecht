#include <Arduino.h>
#include <CheapStepper.h>

// next, declare the stepper
// and connect pins 8,9,10,11 to IN1,IN2,IN3,IN4 on ULN2003 board

CheapStepper stepper (8,9,10,11); 

 // let's create a boolean variable to save the direction of our rotation

boolean moveClockwise = true;

int stopSwitch = 4;
int doorSwitch = 3;
bool door_is_open = false;
bool doorState = door_is_open;


void initPositon()
{	
	Serial.println("Finding Startposition");
	Serial.println(digitalRead(stopSwitch));
	if (digitalRead(stopSwitch)==LOW)
	     stepper.moveCW (250);
	bool found = false;	 
	while(!found)	{
		int state = digitalRead(stopSwitch);
		stepper.moveCCW (10);
		found = state==LOW;
		Serial.println(state);
		delay(5);		
	}

	door_is_open = (digitalRead(doorSwitch) == HIGH);

	if (!door_is_open)
		stepper.moveDegreesCW(50);

}

void checkDoor() {
	while (true)
	{  
	   door_is_open = (digitalRead(doorSwitch)==HIGH);	
       if (door_is_open != doorState)
	   {
		   if (door_is_open)
		   	 stepper.moveDegreesCCW(50);
		   else
		     stepper.moveDegreesCW(50);
		   doorState = door_is_open;	 	 
	   }
	   delay(1000);
	}
}

void setup() {

   pinMode(stopSwitch, INPUT); 	
   digitalWrite(stopSwitch,HIGH);
   pinMode(doorSwitch, INPUT); 	
   digitalWrite(doorSwitch,HIGH);

  // let's set a custom speed of 20rpm (the default is ~16.25rpm)
  
  stepper.setRpm(10); 
  /* Note: CheapStepper library assumes you are powering your 28BYJ-48 stepper
   * using an external 5V power supply (>100mA) for RPM calculations
   * -- don't try to power the stepper directly from the Arduino
   * 
   * accepted RPM range: 6RPM (may overheat) - 24RPM (may skip)
   * ideal range: 10RPM (safe, high torque) - 22RPM (fast, low torque)
   */

  // now let's set up a serial connection and print some stepper info to the console
  
  Serial.begin(9600); Serial.println();
  Serial.print(stepper.getRpm()); // get the RPM of the stepper
  Serial.print(" rpm = delay of ");
  Serial.print(stepper.getDelay()); // get delay between steps for set RPM
  Serial.print(" microseconds between steps");
  Serial.println();
  initPositon();
  checkDoor();
  // stepper.setTotalSteps(4076);
  /* you can uncomment the above line if you think your motor
   * is geared 63.68395:1 (measured) rather than 64:1 (advertised)
   * which would make the total steps 4076 (rather than default 4096)
   * for more info see: http://forum.arduino.cc/index.php?topic=71964.15
   */
}

void loop() {
	
    

}
