//! Essentially, this code just wraps the SFML sprite class into a superclass for the later Entity class in Ruby

#pragma once

#include "Helper.h"
#include "ResourceManager.h"

#include <SFML/Graphics.hpp>

sf::Sprite* get_sprite(mrb_state* mrb, mrb_value self);

void setup_ruby_class_sprite(mrb_state* mrb);