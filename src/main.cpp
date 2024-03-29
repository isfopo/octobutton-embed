#include <Arduino.h>
#include <EasyButton.h>

#define MIDI_CHANNEL 1
#define HOLD_TIME 500
#define CC_THRESHOLD 64

uint8_t pins[8] = {7, 6, 5, 4, 3, 2, 1, 0};

uint8_t ledPins[8] = {20, 19, 18, 17, 16, 15, 14, 13};

uint8_t ledNotes[8] = {1, 2, 3, 4, 5, 6, 7, 8};

EasyButton buttons[8] = {EasyButton(pins[0]),
                         EasyButton(pins[1]),
                         EasyButton(pins[2]),
                         EasyButton(pins[3]),
                         EasyButton(pins[4]),
                         EasyButton(pins[5]),
                         EasyButton(pins[6]),
                         EasyButton(pins[7])};

uint8_t pressedNotes[8] = {0, 1, 2, 3, 4, 5, 6, 7};

uint8_t heldNotes[8] = {8, 9, 10, 11, 12, 13, 14, 15};

bool noteIsHeld[8] = {false, false, false, false, false, false, false, false};

void sendNote(uint8_t note)
{
  usbMIDI.sendNoteOn(note, 127, MIDI_CHANNEL);
  usbMIDI.sendNoteOff(note, 0, MIDI_CHANNEL);
}

void setHeldNote(uint8_t index)
{
  noteIsHeld[index] = true;
}

void handleNoteOn(byte channel, byte note, byte velocity)
{
  if (channel == MIDI_CHANNEL)
  {
    for (int i = 0; i < 8; i++)
    {
      if (note == ledNotes[i])
      {
        digitalWrite(ledPins[i], HIGH);
      }
    }
  }
}

void handleNoteOff(byte channel, byte note, byte velocity)
{
  if (channel == MIDI_CHANNEL)
  {
    for (int i = 0; i < 8; i++)
    {
      if (note == ledNotes[i])
      {
        digitalWrite(ledPins[i], LOW);
      }
    }
  }
}

void setup()
{
  for (int i = 0; i < 8; i++)
  {
    buttons[i].begin();
  }
  Serial.begin(9600);

  usbMIDI.setHandleNoteOn(handleNoteOn);
  usbMIDI.setHandleNoteOn(handleNoteOff);

  buttons[0].onPressed([]() -> void
                       { sendNote(pressedNotes[0]); });
  buttons[1].onPressed([]() -> void
                       { sendNote(pressedNotes[1]); });
  buttons[2].onPressed([]() -> void
                       { sendNote(pressedNotes[2]); });
  buttons[3].onPressed([]() -> void
                       { sendNote(pressedNotes[3]); });
  buttons[4].onPressed([]() -> void
                       { sendNote(pressedNotes[4]); });
  buttons[5].onPressed([]() -> void
                       { sendNote(pressedNotes[5]); });
  buttons[6].onPressed([]() -> void
                       { sendNote(pressedNotes[6]); });
  buttons[7].onPressed([]() -> void
                       { sendNote(pressedNotes[7]); });

  buttons[0].onPressedFor(HOLD_TIME, []() -> void
                          { setHeldNote(0); });
  buttons[1].onPressedFor(HOLD_TIME, []() -> void
                          { setHeldNote(1); });
  buttons[2].onPressedFor(HOLD_TIME, []() -> void
                          { setHeldNote(2); });
  buttons[3].onPressedFor(HOLD_TIME, []() -> void
                          { setHeldNote(3); });
  buttons[4].onPressedFor(HOLD_TIME, []() -> void
                          { setHeldNote(4); });
  buttons[5].onPressedFor(HOLD_TIME, []() -> void
                          { setHeldNote(5); });
  buttons[6].onPressedFor(HOLD_TIME, []() -> void
                          { setHeldNote(6); });
  buttons[7].onPressedFor(HOLD_TIME, []() -> void
                          { setHeldNote(7); });

  pinMode(ledPins[0], OUTPUT);
  pinMode(ledPins[1], OUTPUT);
  pinMode(ledPins[2], OUTPUT);
  pinMode(ledPins[3], OUTPUT);
  pinMode(ledPins[4], OUTPUT);
  pinMode(ledPins[5], OUTPUT);
  pinMode(ledPins[6], OUTPUT);
  pinMode(ledPins[7], OUTPUT);
}

void loop()
{
  usbMIDI.read();

  for (int i = 0; i < 8; i++)
  {
    buttons[i].read();

    if (buttons[i].wasReleased() && noteIsHeld[i])
    {
      sendNote(heldNotes[i]);
      noteIsHeld[i] = false;
    }
  }
}
