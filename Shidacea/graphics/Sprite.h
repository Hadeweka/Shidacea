//! Essentially, this code just wraps the SFML sprite class into a superclass for the later Entity class in Ruby

#pragma once

#include "Helper.h"
#include "ResourceManager.h"

#include <SFML/Graphics.hpp>

mrb_value ruby_sprite_init(mrb_state* mrb, mrb_value self);
mrb_value ruby_sprite_link_texture(mrb_state* mrb, mrb_value self);

mrb_value ruby_sprite_position(mrb_state* mrb, mrb_value self);
mrb_value ruby_sprite_position_equals(mrb_state* mrb, mrb_value self);

mrb_value ruby_sprite_draw(mrb_state* mrb, mrb_value self);

static sf::Sprite* get_sprite(mrb_state* mrb, mrb_value self);

void setup_ruby_class_sprite(mrb_state* mrb);