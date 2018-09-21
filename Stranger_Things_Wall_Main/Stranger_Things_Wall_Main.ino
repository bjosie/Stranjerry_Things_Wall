#include <PS2Keyboard.h>

#include <unordered_map>
using namespace std;

const int KeybDIn = 8;
const int KeybCIn = 3;

PS2Keyboard keyboard;

String currentBuffer = "";

const int MAX_NUM_MSGS = 3;
String storedMessages[MAX_NUM_MSGS];
int currMsgIdx = 0;

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

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
}

void dispatchMessage()
{
    for (int i = 0; i < currentBuffer.length(); i++)
    {
        char curr = currentBuffer.charAt(i);
        Serial.print(curr);
        int ledIdx = curr <= 'n' ? 49 - (curr);
        if (curr <= 'n')
        {
            ledIdx = 49 - 2 * (curr - 'a'); // A to N is 49 to 23, 2 at a time
        }
        else
        {
            ledIdx = 2 * (curr - 'o'); // O to Z is 0 to 22
        }
        // TODO light up led at ledIdx
    }
    Serial.println("  ");
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
                dispatchMessage();
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
    if (random(10000) > 9999)
    {
        int numLights = strip.numPixels();
        int numToFlash = random(numLights / 2, numLights);

        for (int i = 0; i < numToFlash; i++)
        {
            int ledIdx = random(0, numLights);
            if (random(10) > 3)
            {                                                        // flash the light with a 70% probability so it looks more random
                strip.setPixelColor(ledIdx, strip.Color(0, 0, 127)); // TODO choose color, currently blue
            }
        }
        return true;
    }
    return false;
}

void showMessage();

void loop()
{
    keyboardRead();
    if (randomFlash())
    {
        showMessage();
    }
    delay(30);
}
