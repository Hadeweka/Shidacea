#pragma once

#include "MrbWrap.h"

#include <SFML/Network.hpp>

void setup_ruby_class_socket(mrb_state* mrb, RClass* ruby_module);