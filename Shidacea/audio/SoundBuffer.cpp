#include "SoundBuffer.h"

mrb_value ruby_sound_buffer_init(mrb_state* mrb, mrb_value self) {

	MrbWrap::convert_to_object<sf::SoundBuffer>(mrb, self);

	return self;

}

mrb_value ruby_sound_buffer_load_from_file(mrb_state* mrb, mrb_value self) {

	char* filename;

	mrb_get_args(mrb, "z", &filename);

	auto buffer = MrbWrap::convert_from_object<sf::SoundBuffer>(mrb, self);

	buffer->loadFromFile(filename);	//! TODO: Find out why this yields a warning

	return self;

}

void setup_ruby_class_sound_buffer(mrb_state* mrb) {

	auto ruby_sound_buffer_class = MrbWrap::define_data_class(mrb, "SoundBuffer");

	mrb_define_method(mrb, ruby_sound_buffer_class, "initialize", ruby_sound_buffer_init, MRB_ARGS_NONE());
	mrb_define_method(mrb, ruby_sound_buffer_class, "load_from_file", ruby_sound_buffer_load_from_file, MRB_ARGS_REQ(1));

}