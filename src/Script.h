#pragma once

#include "MrbWrap.h"
#include "MrbLoad.h"
#include "Version.h"

void setup_ruby_script_module(mrb_state* mrb, RClass* ruby_module);