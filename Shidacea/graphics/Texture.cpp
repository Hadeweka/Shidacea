#include "Texture.h"

mrb_value ruby_texture_init(mrb_state* mrb, mrb_value self) {

	MrbWrap::convert_to_instance_variable<sf::Texture>(mrb, self, "@_texture", "texture");

	return self;

}

mrb_value ruby_texture_load_from_file(mrb_state* mrb, mrb_value self) {

	char* filename;
	mrb_value intrect = mrb_nil_value();

	mrb_get_args(mrb, "z!o", &filename, &intrect);

	auto texture = MrbWrap::convert_from_instance_variable<sf::Texture>(mrb, self, "@_texture");

	if (mrb_nil_p(intrect)) {

		texture->loadFromFile(filename);	//! TODO: Find out why this yields a warning
	
	} else {

		auto intrect_value = MrbWrap::convert_from_instance_variable<sf::IntRect>(mrb, intrect, "@_intrect");
		texture->loadFromFile(filename, *intrect_value);

	}

	return self;

}

void setup_ruby_class_texture(mrb_state* mrb) {

	auto ruby_texture_class = mrb_define_class(mrb, "Texture", mrb->object_class);

	mrb_define_method(mrb, ruby_texture_class, "initialize", ruby_texture_init, MRB_ARGS_NONE());
	mrb_define_method(mrb, ruby_texture_class, "load_from_file", ruby_texture_load_from_file, MRB_ARGS_ARG(1, 1));

}