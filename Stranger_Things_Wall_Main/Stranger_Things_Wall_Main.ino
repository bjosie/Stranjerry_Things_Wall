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

void flashLED(int pixel, int maxGreen, int maxRed, int maxBlue)
{
    for (int idx = 0; idx < 256; idx++)
    {
        strip.setPixelColor(pixel, 0, idx, 0);
        strip.show();
        delay(2);
    }
    for (int idx = 255; idx >= 0; idx--)
    {
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
        for (int s = 0; i < 6) //  6ish seconds
        {
            bool pixelsToLight[strip.numPixels()]; // whether a pixel will be lit up this time
            for (int i = 0; i < strip.numPixels(); i++)
            {
                pixelsToLight[i] = random(10) < 6 // 60% chance
            }
            for (int i = 0; i < 256; i++)
            {
                for (int j = 0; j < strip.numPixels(); j++)
                {
                    if (pixelsToLight[j])
                    {
                        strip.setColorPixel(j, i, i, i);
                    }
                }
                strip.show();
                delay(2)
            }
            for (int i = 256; i >= 0; i++)
            {
                for (int j = 0; j < strip.numPixels(); j++)
                {
                    if (pixelsToLight[j])
                    {
                        strip.setColorPixel(j, i, i, i);
                    }
                }
                strip.show();
                delay(2)
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

void randomEffect()
{
    int choice = random(9);

    if (choice == 0)
    {
        colorWipe(strip.Color(255, 0, 0), 50);
    }
    if (choice == 1)
    {
        colorWipe(strip.Color(0, 255, 0), 50);
    }
    if (choice == 2)
    {
        colorWipe(strip.Color(0, 0, 255), 50);
    }
    if (choice == 3)
    {
        theaterChase(strip.Color(127, 127, 127), 50);
    }
    if (choice == 4)
    {
        theaterChase(strip.Color(127, 0, 0), 50);
    }
    if (choice == 5)
    {
        theaterChase(strip.Color(0, 0, 127), 50);
    }
    if (choice == 6)
    {
        rainbow(20);
    }
    if (choice == 7)
    {
        rainbowCycle(20);
    }
    if (choice == 8)
    {
        theaterChaseRainbow(50);
    }
}

void loop()
{
    keyboardRead();
    if (randomFlash())
    {
        showMessage();
    }
    else
    {
        randomEffect();
    }
    delay(30);
}

void colorWipe(uint32_t c, uint8_t wait)
{
    for (uint16_t i = 0; i < strip.numPixels(); i++)
    {
        strip.setPixelColor(i, c);
        strip.show();
        delay(wait);
    }
}

void rainbow(uint8_t wait)
{
    uint16_t i, j;

    for (j = 0; j < 256; j++)
    {
        for (i = 0; i < strip.numPixels(); i++)
        {
            strip.setPixelColor(i, Wheel((i + j) & 255));
        }
        strip.show();
        delay(wait);
    }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait)
{
    uint16_t i, j;

    for (j = 0; j < 256 * 5; j++)
    { // 5 cycles of all colors on wheel
        for (i = 0; i < strip.numPixels(); i++)
        {
            strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
        }
        strip.show();
        delay(wait);
    }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait)
{
    for (int j = 0; j < 10; j++)
    { //do 10 cycles of chasing
        for (int q = 0; q < 3; q++)
        {
            for (uint16_t i = 0; i < strip.numPixels(); i = i + 3)
            {
                strip.setPixelColor(i + q, c); //turn every third pixel on
            }
            strip.show();

            delay(wait);

            for (uint16_t i = 0; i < strip.numPixels(); i = i + 3)
            {
                strip.setPixelColor(i + q, 0); //turn every third pixel off
            }
        }
    }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait)
{
    for (int j = 0; j < 256; j++)
    { // cycle all 256 colors in the wheel
        for (int q = 0; q < 3; q++)
        {
            for (uint16_t i = 0; i < strip.numPixels(); i = i + 3)
            {
                strip.setPixelColor(i + q, Wheel((i + j) % 255)); //turn every third pixel on
            }
            strip.show();

            delay(wait);

            for (uint16_t i = 0; i < strip.numPixels(); i = i + 3)
            {
                strip.setPixelColor(i + q, 0); //turn every third pixel off
            }
        }
    }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos)
{
    WheelPos = 255 - WheelPos;
    if (WheelPos < 85)
    {
        return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    if (WheelPos < 170)
    {
        WheelPos -= 85;
        return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    WheelPos -= 170;
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
