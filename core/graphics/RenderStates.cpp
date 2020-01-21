#include "RenderStates.h"

void setup_ruby_class_render_states(mrb_state* mrb, RClass* ruby_module) {

	auto ruby_render_state_class = MrbWrap::define_data_class_under(mrb, "RenderStates", ruby_module);

	MrbWrap::wrap_constructor<sf::RenderStates>(mrb, ruby_render_state_class);

}