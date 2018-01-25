/*
 * wiring-skeleton.cpp
 *
 *  Created on: 15.03.2017
 *      Author: niklausd
 */

#include <Arduino.h>

// PlatformIO libraries
#include <SerialCommand.h>  // pio lib install 173, lib details see https://github.com/kroimon/Arduino-SerialCommand
#include <Timer.h>          // pio lib install 1699, lib details see https://github.com/dniklaus/wiring-timer

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
#include <DetectorStrategy.h>

#ifndef BUILTIN_LED
#define BUILTIN_LED 13
#endif

SerialCommand* sCmd = 0;

class ButtonEdgeDetector : public EdgeDetector
{
private:
  DbgTrace_Port* m_trPort;

public:
  ButtonEdgeDetector()
  : m_trPort(new DbgTrace_Port("btn", DbgTrace_Level::debug))
  { }

  void onEdge(bool newState)
  {
    TR_PRINTF(m_trPort, DbgTrace_Level::debug, "Button %s", newState ? "pressed" : "released");
  }
};

void setup()
{
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, 0);

  setupProdDebugEnv();

  Button* button = new Button(new ArduinoDigitalInPinSupervisor(12), new ButtonEdgeDetector());
//  button->addDetector(new ButtonEdgeDetector());

}

void loop()
{
  if (0 != sCmd)
  {
    sCmd->readSerial();     // process serial commands
  }
  yield();                  // process Timers
}
