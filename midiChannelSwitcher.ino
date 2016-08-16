#include <MIDI.h>
#include <EEPROM.h>

MIDI_CREATE_DEFAULT_INSTANCE();

#define RELAY_BUTTON1 2
#define RELAY_BUTTON2 3
#define RELAY_BUTTON3 4
#define RELAY_BUTTON4 5
#define STORE_BUTTON 6
#define RELAY_OUT1 7
#define RELAY_OUT2 8
#define RELAY_OUT3 9
#define RELAY_OUT4 10
#define LED 13

byte presetNumber = 0;
byte currentRelayStatus = 0;

byte buttonStatus () {
	byte buttonState = 0;
	buttonState = buttonState | (digitalRead(RELAY_BUTTON1) << RELAY_BUTTON1);
	buttonState = buttonState | (digitalRead(RELAY_BUTTON2) << RELAY_BUTTON2);
	buttonState = buttonState | (digitalRead(RELAY_BUTTON3) << RELAY_BUTTON3);
	buttonState = buttonState | (digitalRead(RELAY_BUTTON4) << RELAY_BUTTON4);
	buttonState = buttonState | (digitalRead(STORE_BUTTON) << STORE_BUTTON);

	return buttonState;
}

void setRelays (byte status)
{
	// Store the relay status
	currentRelayStatus = status;
	// Rewrite condition statement to return 0x1 in the bitwise operation
	digitalWrite(RELAY_BUTTON1, (status & (1 << RELAY_BUTTON1)));
	digitalWrite(RELAY_BUTTON1, (status & (1 << RELAY_BUTTON1)));
	digitalWrite(RELAY_BUTTON1, (status & (1 << RELAY_BUTTON1)));
	digitalWrite(RELAY_BUTTON1, (status & (1 << RELAY_BUTTON1)));
}

void programChangeCallback (byte channel, byte number)
{
	setRelays(EEPROM.read(number));
	presetNumber = number;
}

void setup()
{
    pinMode(RELAY_BUTTON1, INPUT);
    pinMode(RELAY_BUTTON2, INPUT);
    pinMode(RELAY_BUTTON3, INPUT);
    pinMode(RELAY_BUTTON4, INPUT);
    pinMode(STORE_BUTTON, INPUT);
    pinMode(RELAY_OUT1, OUTPUT);
    pinMode(RELAY_OUT2, OUTPUT);
    pinMode(RELAY_OUT3, OUTPUT);
    pinMode(RELAY_OUT4, OUTPUT);
    pinMode(LED, OUTPUT);
    MIDI.setHandleProgramChange(programChangeCallback);
	presetNumber = 0;
	setRelays(EEPROM.read(presetNumber));
    MIDI.begin(MIDI_CHANNEL_OMNI);          // Launch MIDI and listen to all channels
}

void loop()
{

	byte currentButtonStatus = buttonStatus();

	// Read the buttons and act
	if (currentButtonStatus)
	{
		// If store button and relay button 1 is pressed together for 10 seconds the device memory is cleared

		// Rewrite condition statement to return 0x1 in the bitwise operation
		if (currentButtonStatus & (1 << RELAY_BUTTON1))
		{
			currentRelayStatus = currentRelayStatus ^ (1 << RELAY_BUTTON1);
			digitalWrite(RELAY_BUTTON1, (currentRelayStatus && (1 << RELAY_BUTTON1)));
		}
		else if (currentButtonStatus & (1 << RELAY_BUTTON2))
		{
			currentRelayStatus = currentRelayStatus ^ (1 << RELAY_BUTTON2);
			digitalWrite(RELAY_BUTTON1, (currentRelayStatus && (1 << RELAY_BUTTON2)));
		}
		else if (currentButtonStatus & (1 << RELAY_BUTTON3))
		{
			currentRelayStatus = currentRelayStatus ^ (1 << RELAY_BUTTON3);
			digitalWrite(RELAY_BUTTON1, (currentRelayStatus && (1 << RELAY_BUTTON3)));
		}
		else if (currentButtonStatus & (1 << RELAY_BUTTON4))
		{
			currentRelayStatus = currentRelayStatus ^ (1 << RELAY_BUTTON4);
			digitalWrite(RELAY_BUTTON1, (currentRelayStatus && (1 << RELAY_BUTTON4)));
		}
		else if (currentButtonStatus & (1 << STORE_BUTTON))
		{
			EEPROM.write(presetNumber, currentRelayStatus);
		}
	}
}
