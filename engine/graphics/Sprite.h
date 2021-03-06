//! Essentially, this code just wraps the SFML sprite class into a superclass for the later Entity class in Ruby

#pragma once

#include "MrbWrap.h"

#include <SFML/Graphics.hpp>

void setup_ruby_class_sprite(mrb_state* mrb, RClass* ruby_module);
