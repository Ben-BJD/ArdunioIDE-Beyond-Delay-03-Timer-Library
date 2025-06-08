#include <Arduino.h>
#include <Timer.h>

#define LED_COUNT 3

struct BlinkLedStruct 
{
  uint8_t pin;
  int highLow;
  long blinkLimit;
  long blinkCount;
  Timer timer;
};

BlinkLedStruct leds[LED_COUNT] = 
{
  {13, LOW, 2,0,Timer(1000)},
  {12, LOW, 10,0,Timer(200)},
  {10, LOW, 20,0,Timer(100)}
};

Timer restartTimer(2500);

void blinkLedCallback(BlinkLedStruct &led) 
{
  led.highLow = (led.highLow == HIGH) ? LOW : HIGH;
  digitalWrite(led.pin, led.highLow);
  if(led.highLow == LOW) 
  {
      led.blinkCount++;
      if(led.blinkCount >= led.blinkLimit) 
      {
          led.timer.stop();
      }
  }
}

void startLedsBlinking() 
{
  for(uint8_t i=0; i<LED_COUNT; i++) 
  {
      leds[i].blinkCount = 0;
      leds[i].highLow = LOW;
      blinkLedCallback(leds[i]);
      leds[i].timer.start();
  }
}

void setup() 
{
  for(uint8_t i=0; i<LED_COUNT; i++) 
  {
      pinMode(leds[i].pin, OUTPUT);
      startLedsBlinking(); // Start blinking LEDs immediately
  }
}

void loop() 
{
  bool anyTimersRunning = false;

  for(uint8_t i=0; i<LED_COUNT; i++) 
  {
      if( leds[i].timer.update() )
      {
          blinkLedCallback(leds[i]);
      }

      if( leds[i].timer.isRunning() ) 
      {
          anyTimersRunning = true;
      }
  }

  if(!anyTimersRunning && !restartTimer.isRunning()) 
  {
    restartTimer.start();
  }

  if( restartTimer.update() )
  {
      restartTimer.stop();
      startLedsBlinking(); // Restart blinking LEDs after the restart timer expires
  }
}