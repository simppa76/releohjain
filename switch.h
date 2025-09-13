#pragma once
#include "esphome.h"

namespace esphome {
namespace bin_switch {

static uint32_t output_state = 0;  // 32-bit for 32 relays

const int LATCH_PIN = 4;
const int CLOCK_PIN = 12;
const int MOSI_PIN  = 14;

class binSwitch : public Component, public switch_::Switch {
 public:
  explicit binSwitch(uint8_t pin) : pin_num_(pin) {}

  void setup() override {
    pinMode(LATCH_PIN, OUTPUT);
    pinMode(CLOCK_PIN, OUTPUT);
    pinMode(MOSI_PIN, OUTPUT);
    digitalWrite(LATCH_PIN, HIGH);
    this->publish_state(false);
    set_outputs();
  }

  void write_state(bool state) override {
    if (state) {
      bitSet(output_state, pin_num_);
    } else {
      bitClear(output_state, pin_num_);
    }
    set_outputs();
    this->publish_state(state);
  }

 protected:
  uint8_t pin_num_;

  void set_outputs() {
    digitalWrite(LATCH_PIN, LOW);
    shiftOut(MOSI_PIN, CLOCK_PIN, LSBFIRST, output_state);
    shiftOut(MOSI_PIN, CLOCK_PIN, LSBFIRST, output_state >> 8);
    shiftOut(MOSI_PIN, CLOCK_PIN, LSBFIRST, output_state >> 16);
    shiftOut(MOSI_PIN, CLOCK_PIN, LSBFIRST, output_state >> 24);
    digitalWrite(LATCH_PIN, HIGH);
  }
};

}  // namespace bin_switch
}  // namespace esphome
