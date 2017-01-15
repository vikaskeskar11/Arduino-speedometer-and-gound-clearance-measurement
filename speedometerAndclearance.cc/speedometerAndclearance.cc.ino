// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
// read the hall effect sensor on pin 2

#include <NewPing.h>
#define  TRIGGER_PIN  11
#define  ECHO_PIN     10
#define MAX_DISTANCE 500 // Maximum distance we want to ping for (in centimeters).


LiquidCrystal lcd(9, 8, 7, 6, 5, 4);

const int hallPin=2;
const unsigned long sampleTime=1000;
const int maxRPM = 10200; 

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
int DistanceIn;
int DistanceCm;
int velocity=0;

void setup() 
{
  pinMode(hallPin,INPUT);
  Serial.begin(9600);
  // set up the LCD's number of rows and columns: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("initializing");
  delay(1000);
  lcd.clear();


}

void loop() 
{
  delay(100);
  int rpm=getRpm();
  lcd.clear();
  displayRPM(rpm);
  displayOnLcd(rpm);

  delay(100);// Wait 100ms between pings (about 10 pings/sec). 29ms should be the shortest delay between pings.
  lcd.setCursor(0, 1);
  delay(100);// Wait 100ms between pings (about 10 pings/sec). 29ms should be the shortest delay between pings.
  DistanceCm = sonar.ping_cm();
  lcd.print("Ping: ");
  lcd.print(DistanceCm); // Convert ping time to distance and print result 
                            // (0 = outside set distance range, no ping echo)
  lcd.print(" cm     ");
  velocity=0.1*2*3.14*rpm/60;
  lcd.setCursor(9, 0);
 lcd.print(velocity);
   lcd.setCursor(12, 0);
 lcd.print("m/s");

}
 
int getRpm()
{
  // sample for sampleTime in millisecs
  int kount=0;
  boolean cflag=LOW;
  unsigned long CurrentTime=0;
  unsigned long startTime=millis();
  while (CurrentTime<=sampleTime)
  {
    if (digitalRead(hallPin)==HIGH)
    {
      cflag=HIGH;
    }
    if (digitalRead(hallPin)==LOW && cflag==HIGH)
    {
      kount++;
      cflag=LOW;
    }
    CurrentTime=millis()-startTime;
  }
  int counttorpm = int(60000./float(sampleTime))*kount;
  return counttorpm;
}
    
void displayRPM(int rpm) 
{
  lcd.clear();
  // set the cursor to column 0, line 1
  lcd.setCursor(0, 0); 
  // print the number of seconds since reset:
  lcd.print(rpm);
  lcd.setCursor(3,0);
  lcd.print("RPM");

}

void displayOnLcd(int rpm)
{
  int RpmOnBar=map(rpm,0,maxRPM,0,15);
  lcd.setCursor(0,1);
  if (rpm!=0)
  {
  for (int i=0; i<=RpmOnBar; i++)
   {
        lcd.setCursor(i,1);
        lcd.write(1023);
      }
  }
} 

