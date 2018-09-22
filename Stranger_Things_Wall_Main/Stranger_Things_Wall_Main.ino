#include <PS2Keyboard.h>
#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
#include <avr/power.h>
#endif

const int KeybDIn = 8;
const int KeybCIn = 3;

PS2Keyboard keyboard;

String currentBuffer = "";

const int MAX_NUM_MSGS = 3;
String storedMessages[MAX_NUM_MSGS];
int currMsgIdx = 0;

const int LEDDIn = 5;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(50, LEDDIn, NEO_GRB + NEO_KHZ800);

void setup()
{

    delay(1000);
    keyboard.begin(KeybDIn, KeybCIn);
    Serial.begin(9600);
    Serial.println("Keyboard Test:");

#if defined(__AVR_ATtiny85__)
    if (F_CPU == 16000000)
        clock_prescale_set(clock_div_1);
#endif

    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
    randomFlash();
    dispatchMessage("ligma");
}

void dispatchMessage(String toSend)
{
    for (int i = 0; i < toSend.length(); i++)
    {
        char curr = toSend.charAt(i);
        Serial.print(curr);
        int ledIdx;
        if (curr <= 'n')
        {
            ledIdx = 49 - 2 * (curr - 'a'); // A to N is 49 to 23, 2 at a time
        }
        else
        {
            ledIdx = 2 * (curr - 'o'); // O to Z is 0 to 22
        }
        // TODO light up led at ledIdx
        flashLED(ledIdx);
    }
    Serial.println("  ");
}

void flashLED(int pixel){
  for (int idx = 0; idx < 256; idx++){
    strip.setPixelColor(pixel, 0, idx, 0);
    strip.show();
    delay(2);
  }
  for (int idx = 255; idx >= 0; idx--){
    strip.setPixelColor(pixel, 0, idx, 0);
    strip.show();
    delay(2);
  }
}

void storeMessage(String currMessage)
{
    storedMessages[currMsgIdx] = currMessage;
    currMsgIdx = (currMsgIdx + 1) % 3; // wrap back around
}

void keyboardRead()
{

    if (keyboard.available())
    {
        char typed = keyboard.read();
        Serial.println(typed);
        if (typed != -1)
        {
            // new character to be typed
            if ('a' <= typed && typed <= 'z')
            {
                currentBuffer += typed;
            }
            if ('A' <= typed && typed <= 'Z')
            {
                currentBuffer += (typed + 'a' - 'A'); // convert to lowercase
            }
            if (typed == PS2_ENTER)
            {
                dispatchMessage(currentBuffer);
                storeMessage(currentBuffer);
                currentBuffer = "";
            }
        }
    }
    else
    {
        //Serial.println("Keyboard not available!");
    }
}

bool randomFlash()
{
    if (random(10000) > 0)
    {
        int numLights = strip.numPixels();
        int numToFlash = random(numLights / 2, numLights);

        for (int i = 0; i < numToFlash; i++)
        {
            int ledIdx = random(0, numLights);
            if (random(10) > 3)
            {                                                        // flash the light with a 70% probability so it looks more random
                strip.setPixelColor(ledIdx, 0, 0, 127); // TODO choose color, currently blue
                strip.show();
                delay(random(30));
                strip.setPixelColor(ledIdx, 0, 0, 0);
            }
        }
        return true;
    }
    return false;
}

void showMessage()
{
    int msgToShowIdx = random(0, MAX_NUM_MSGS);
    String msgToShow = storedMessages[msgToShowIdx];
    dispatchMessage(msgToShow);
}

void loop()
{
//    keyboardRead();
//    if (randomFlash())
//    {
//        showMessage();
//    }
//    delay(30);
}
