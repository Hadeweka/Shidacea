#include "Music.h"

void setup_ruby_class_music(mrb_state* mrb, RClass* ruby_module) {

	auto ruby_music_class = MrbWrap::define_data_class_under(mrb, "Music", ruby_module);

	MrbWrap::define_constructor_with_no_args<sf::Music>(mrb, ruby_music_class);

	MrbWrap::define_function_with_no_args<sf::Music, sf::SoundStream, &sf::Music::play>(mrb, ruby_music_class, "play");
	MrbWrap::define_function_with_no_args<sf::Music, sf::SoundStream, &sf::Music::stop>(mrb, ruby_music_class, "stop");
	MrbWrap::define_function_with_no_args<sf::Music, sf::SoundStream, &sf::Music::pause>(mrb, ruby_music_class, "pause");

	MrbWrap::define_getter<sf::Music, mrb_bool, decltype(&sf::Music::getLoop), &sf::Music::getLoop>(mrb, ruby_music_class, "looping?");
	MrbWrap::define_setter<sf::Music, sf::SoundStream, mrb_bool, bool, &sf::Music::setLoop>(mrb, ruby_music_class, "looping=");

	MrbWrap::define_mruby_function(mrb, ruby_music_class, "open_from_file", MRUBY_FUNC {

		char* filename;

		mrb_get_args(mrb, "z", &filename);

		auto music = MrbWrap::convert_from_object<sf::Music>(mrb, self);

		auto return_value = music->openFromFile(std::string(filename));

		//! TODO: Convert this to a ruby function
		music->setLoopPoints(sf::Music::Span(sf::milliseconds(2 * 1000 * 4), sf::milliseconds(2 * 1000 * 4)));

		return mrb_bool_value(return_value);

	}, MRB_ARGS_REQ(1));

}