#include <PS2Keyboard.h>

#include <unordered_map>

const int KeybDIn = 8;
const int KeybCIn = 3;

PS2Keyboard keyboard;

const int MAX_BUFF_LENGTH = 20; // TODO
char currentBuffer[MAX_BUFF_LENGTH];
int currentIdx = 0; // current position in currentBuffer



#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

const int LEDDIn = 5;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(50, LEDDIn, NEO_GRB + NEO_KHZ800);



void setup() {
  
    delay(1000);
    keyboard.begin(KeybDIn, KeybCIn);
    Serial.begin(9600);
    Serial.println("Keyboard Test:");


    #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
    #endif

    
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'

}

void dispatchMessage()
{
    for (int i = 0; i < currentIdx; i++)
    {
        char curr = currentBuffer[i];
        Serial.print(curr);
        // TODO dispatch `curr` to LEDs
    }
    Serial.println("  ");
}

void loop() {
   if (keyboard.available())
    {
        //Serial.println("yeet");
        char typed = keyboard.read();
        Serial.println(typed);
        if (typed != -1)
        {
            // new character to be typed
            if ('a' <= typed && typed <= 'z')
            {
                currentBuffer[currentIdx] = typed;
                currentIdx += 1;
            }
            if ('A' <= typed && typed <= 'Z')
            {
                currentBuffer[currentIdx] = typed + 'a' - 'A'; // convert to lowercase
                currentIdx += 1;
            }
            if (currentIdx == MAX_BUFF_LENGTH || typed == PS2_ENTER)
            {
                dispatchMessage();
                currentIdx = 0;
            }
        }
    } else {
        //Serial.println("Keyboard not available!");
    }
    delay(30);
}
