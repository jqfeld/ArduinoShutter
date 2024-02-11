#include <Servo.h> 
#include <CmdBuffer.hpp>
#include <CmdCallback.hpp>
#include <CmdParser.hpp>

CmdCallback<3> cmdCallback;
char strSet[] = "SET";

Servo servo; 


void functSet(CmdParser *myParser)
{
  int pos = atoi(myParser->getCmdParam(1));
  Serial.println(pos);
  servo.write(pos);
}


void setup()
{
  Serial.begin(9600);
  cmdCallback.addCmd(strSet, &functSet);

  servo.attach(9); // connect yellow wire of servo with pin d9
}

 

void loop()
{ 
  CmdBuffer<32> myBuffer;
  CmdParser myParser;

  // Serial.println("To set servo position use SET <pos>.");
  // Serial.println("<pos> can be an integer between 0 and 180.");
  
  cmdCallback.loopCmdProcessing(&myParser, &myBuffer, &Serial);
}
