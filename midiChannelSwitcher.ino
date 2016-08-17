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

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long debounceDelay = 50;    // the debounce time; increase if the output flickers

byte switchOccured = LOW;

byte debounce_button (uint8_t buttonPin, byte *lastButtonState, long *lastDebounceTime) {
	byte buttonState = LOW;
	int reading = digitalRead(buttonPin);

	// check to see if you just pressed the button
	// (i.e. the input went from LOW to HIGH),  and you've waited
	// long enough since the last press to ignore any noise:

	// If the switch changed, due to noise or pressing:
	if (reading != *lastButtonState) {
		// reset the debouncing timer
		*lastDebounceTime = millis();
	}

	long currentTime = millis() - *lastDebounceTime;
	if (currentTime > debounceDelay) {
		// whatever the reading is at, it's been there for longer
		// than the debounce delay, so take it as the actual current state:

		// if the button state has changed:
		if (reading != buttonState) {
			buttonState = reading;
		}
	}
	*lastButtonState = reading;
	return buttonState;
}

byte buttonStatus () {

	byte buttonState = 0;

	static long lastDebounceTime_button1;
	static long lastDebounceTime_button2;
	static long lastDebounceTime_button3;
	static long lastDebounceTime_button4;
	static long lastDebounceTime_storeButton;

	static byte lastButtonState_button1;
	static byte lastButtonState_button2;
	static byte lastButtonState_button3;
	static byte lastButtonState_button4;
	static byte lastButtonState_storeButton;

	buttonState = buttonState | (debounce_button(RELAY_BUTTON1, &lastButtonState_button1, &lastDebounceTime_button1) << RELAY_BUTTON1);
	buttonState = buttonState | (debounce_button(RELAY_BUTTON1, &lastButtonState_button2, &lastDebounceTime_button2) << RELAY_BUTTON2);
	buttonState = buttonState | (debounce_button(RELAY_BUTTON1, &lastButtonState_button3, &lastDebounceTime_button3) << RELAY_BUTTON3);
	buttonState = buttonState | (debounce_button(RELAY_BUTTON1, &lastButtonState_button4, &lastDebounceTime_button4) << RELAY_BUTTON4);
	buttonState = buttonState | (debounce_button(RELAY_BUTTON1, &lastButtonState_storeButton, &lastDebounceTime_storeButton) << STORE_BUTTON);

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

byte checkButtonStatus (byte buttonPin, byte currentButtonStatus)
{
	return (1 && (currentButtonStatus & (1 << RELAY_BUTTON1)));
}

void loop()
{
	byte currentButtonStatus = buttonStatus();

	// Read the buttons and act
	if (currentButtonStatus)
	{
		if (switchOccured == 0)
		{
			// If store button and relay button 1 is pressed together for 10 seconds the device memory is cleared

			// Rewrite condition statement to return 0x1 in the bitwise operation
			if (checkButtonStatus(RELAY_BUTTON1, currentButtonStatus))
			{
				currentRelayStatus = currentRelayStatus ^ (1 << RELAY_BUTTON1);
				digitalWrite(RELAY_BUTTON1, (currentRelayStatus && (1 << RELAY_BUTTON1)));
			}
			else if (checkButtonStatus(RELAY_BUTTON2, currentButtonStatus))
			{
				currentRelayStatus = currentRelayStatus ^ (1 << RELAY_BUTTON2);
				digitalWrite(RELAY_BUTTON1, (currentRelayStatus && (1 << RELAY_BUTTON2)));
			}
			else if (checkButtonStatus(RELAY_BUTTON3, currentButtonStatus))
			{
				currentRelayStatus = currentRelayStatus ^ (1 << RELAY_BUTTON3);
				digitalWrite(RELAY_BUTTON1, (currentRelayStatus && (1 << RELAY_BUTTON3)));
			}
			else if (checkButtonStatus(RELAY_BUTTON4, currentButtonStatus))
			{
				currentRelayStatus = currentRelayStatus ^ (1 << RELAY_BUTTON4);
				digitalWrite(RELAY_BUTTON1, (currentRelayStatus && (1 << RELAY_BUTTON4)));
			}
			else if (checkButtonStatus(STORE_BUTTON, currentButtonStatus))
			{
				EEPROM.write(presetNumber, currentRelayStatus);
			}
			switchOccured = 1;
		}
	}
	else
	{
		switchOccured = 0;
	}
}
