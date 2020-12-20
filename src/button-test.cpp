/*
 * wiring-skeleton.cpp
 *
 *  Created on: 15.03.2017
 *      Author: niklausd
 */


#define WIRINGTIMER_SUPPRESS_WARNINGS
#include <Timer.h>          // pio lib install 1699, lib details see https://github.com/dniklaus/wiring-timer
#include <SerialCommand.h>  // pio lib install 173, lib details see https://github.com/kroimon/Arduino-SerialCommand

// private libraries
#include <DbgCliNode.h>
#include <DbgCliTopic.h>
#include <DbgCliCommand.h>
#include <DbgTracePort.h>
#include <DbgTraceContext.h>
#include <DbgTraceOut.h>
#include <DbgPrintConsole.h>
#include <DbgTraceLevel.h>
#include <AppDebug.h>
#include <ProductDebug.h>
#include <RamUtils.h>
#include <ArduinoDigitalInPinSupervisor.h>
#include <Button.h>
#include <EdgeDetector.h>
#include <DetectorStrategy.h>

#include <Arduino.h>

#define BUTTON_PIN 11

SerialCommand* sCmd = 0;

//-----------------------------------------------------------------------------

class ButtonEdgeDetector : public EdgeDetector
{
private:
  DbgTrace_Port* m_trPort;
  unsigned long int m_count;

public:
  ButtonEdgeDetector()
  : m_trPort(new DbgTrace_Port("btn", DbgTrace_Level::info))
  , m_count(0)
  { }

  void onEdge(bool newState)
  {
    if (newState)
    {
      m_count++;
      TR_PRINTF(m_trPort, DbgTrace_Level::info, "Count: %d", m_count);
    }
    TR_PRINTF(m_trPort, DbgTrace_Level::debug, "Button %s", newState ? "pressed" : "released");
    if (0 != button())
    {
      if (0 != button()->adapter())
      {
        button()->adapter()->notifyStatusChanged(newState);
      }
    }
  }
};

//-----------------------------------------------------------------------------

class MyButtonAdapter : public ButtonAdapter
{
public:
  void notifyStatusChanged(bool isActive)
  {
    Serial.print("Button ");
    Serial.println(isActive ? "pressed" : "released");
  }
};

//-----------------------------------------------------------------------------

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 0);

  setupProdDebugEnv();

  new Button(new ArduinoDigitalInPinSupervisor(BUTTON_PIN), new ButtonEdgeDetector(), new MyButtonAdapter());
}

void loop()
{
  if (0 != sCmd)
  {
    sCmd->readSerial();     // process serial commands
  }
  scheduleTimers();         // process Timers
}
