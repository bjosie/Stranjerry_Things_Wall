#include <PS2Keyboard.h>

const int DataPin = 8;
const int IRQpin = 3;

PS2Keyboard keyboard;

const int MAX_BUFF_LENGTH = 100; // TODO
char currentBuffer[MAX_BUFF_LENGTH];
int currentIdx = 0; // current position in currentBuffer

void setup()
{
    delay(1000);
    keyboard.begin(DataPin, IRQpin);
    Serial.begin(9600);
    Serial.println("Keyboard Test:");
}

void dispatchMessage()
{
    for (int i = 0; i < currentIdx; i++)
    {
        char curr = currentBuffer[i];
        Serial.print(curr);
        Serial.println();
        // TODO dispatch `curr` to LEDs
    }
}

void loop()
{
    if (keyboard.available())
    {
        Serial.println("ligma");
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
    }
    else
    {
        //Serial.println("Keyboard not available!");
    }
    delay(50);
}

