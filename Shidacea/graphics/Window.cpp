#include "Window.h"

mrb_value ruby_window_init(mrb_state* mrb, mrb_value self) {

	char* title;
	int width;
	int height;
	bool fullscreen = false;

	mrb_get_args(mrb, "zii|b", &title, &width, &height, &fullscreen);

	sf::RenderWindow* window;

	if (fullscreen) {

		window = MrbWrap::convert_to_object<sf::RenderWindow>(mrb, self, sf::VideoMode(width, height), title, sf::Style::Fullscreen);

	} else {

		window = MrbWrap::convert_to_object<sf::RenderWindow>(mrb, self, sf::VideoMode(width, height), title);

	}

	//sf::View view;
	//view.setViewport(sf::FloatRect(0.0, 0.0, 2.0, 8.0/6.0*2.0));
	//window->setView(view);

	MrbWrap::convert_to_instance_variable<sf::Clock>(mrb, self, "@_clock");

	ImGui::SFML::Init(*window);

	return self;

}

mrb_value ruby_window_clear(mrb_state* mrb, mrb_value self) {

	auto window = MrbWrap::convert_from_object<sf::RenderWindow>(mrb, self);
	window->clear();

	return mrb_nil_value();

}

mrb_value ruby_window_display(mrb_state* mrb, mrb_value self) {

	auto window = MrbWrap::convert_from_object<sf::RenderWindow>(mrb, self);

	ImGui::SFML::Render(*window);

	window->display();

	return mrb_nil_value();

}

mrb_value ruby_window_imgui_update(mrb_state* mrb, mrb_value self) {

	auto window = MrbWrap::convert_from_object<sf::RenderWindow>(mrb, self);
	auto clock = MrbWrap::convert_from_instance_variable<sf::Clock>(mrb, self, "@_clock");

	ImGui::SFML::Update(*window, clock->restart());

	return mrb_nil_value();

}

mrb_value ruby_set_imgui_scale(mrb_state* mrb, mrb_value self) {

	float scale;
	mrb_get_args(mrb, "f", &scale);

	ImGui::GetIO().FontGlobalScale = scale;

	return mrb_nil_value();

}

mrb_value ruby_window_enable_vertical_sync(mrb_state* mrb, mrb_value self) {

	auto window = MrbWrap::convert_from_object<sf::RenderWindow>(mrb, self);
	window->setVerticalSyncEnabled(true);

	return mrb_true_value();

}

mrb_value ruby_window_is_open(mrb_state* mrb, mrb_value self) {

	auto window = MrbWrap::convert_from_object<sf::RenderWindow>(mrb, self);

	return mrb_bool_value(window->isOpen());

}

mrb_value ruby_window_close(mrb_state* mrb, mrb_value self) {

	auto window = MrbWrap::convert_from_object<sf::RenderWindow>(mrb, self);
	window->close();

	ImGui::SFML::Shutdown();

	return mrb_nil_value();

}

mrb_value ruby_window_poll_event(mrb_state* mrb, mrb_value self) {

	auto window = MrbWrap::convert_from_object<sf::RenderWindow>(mrb, self);

	static auto event_class = mrb_class_get(mrb, "Event");
	auto new_event = mrb_obj_new(mrb, event_class, 0, NULL);

	auto event = MrbWrap::convert_from_object<sf::Event>(mrb, new_event);

	auto success = window->pollEvent(*event);

	if(success) {

		ImGui::SFML::ProcessEvent(*event);
		return new_event;

	} else {

		return mrb_nil_value();

	}

}

void setup_ruby_class_window(mrb_state* mrb) {

	auto ruby_window_class = MrbWrap::define_data_class(mrb, "Window");

	mrb_define_method(mrb, ruby_window_class, "initialize", ruby_window_init, MRB_ARGS_ARG(3, 1));

	mrb_define_method(mrb, ruby_window_class, "clear", ruby_window_clear, MRB_ARGS_NONE());
	mrb_define_method(mrb, ruby_window_class, "display", ruby_window_display, MRB_ARGS_NONE());
	mrb_define_method(mrb, ruby_window_class, "imgui_update", ruby_window_imgui_update, MRB_ARGS_NONE());

	mrb_define_method(mrb, ruby_window_class, "set_imgui_scale", ruby_set_imgui_scale, MRB_ARGS_REQ(1));

	mrb_define_method(mrb, ruby_window_class, "enable_vertical_sync", ruby_window_enable_vertical_sync, MRB_ARGS_NONE());

	mrb_define_method(mrb, ruby_window_class, "is_open?", ruby_window_is_open, MRB_ARGS_NONE());

	mrb_define_method(mrb, ruby_window_class, "close", ruby_window_close, MRB_ARGS_NONE());

	mrb_define_method(mrb, ruby_window_class, "poll_event", ruby_window_poll_event, MRB_ARGS_NONE());

}