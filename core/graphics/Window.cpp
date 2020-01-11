#include "Window.h"

mrb_value ruby_window_init(mrb_state* mrb, mrb_value self) {

	char* title;
	mrb_int width;
	mrb_int height;
	bool fullscreen = false;

	mrb_get_args(mrb, "zii|b", &title, &width, &height, &fullscreen);

	sf::RenderWindow* window;

	if (fullscreen) {

		window = MrbWrap::convert_to_object<sf::RenderWindow>(mrb, self, sf::VideoMode(static_cast<unsigned int>(width), static_cast<unsigned int>(height)), title, sf::Style::Fullscreen);

	} else {

		window = MrbWrap::convert_to_object<sf::RenderWindow>(mrb, self, sf::VideoMode(static_cast<unsigned int>(width), static_cast<unsigned int>(height)), title);

	}

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

	//auto map = Map();
	//map.load(1000, 1000);
	//map.reload_layers(100, 100);
	//window->draw(map, sf::BlendAlpha);

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

	mrb_float scale;
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

mrb_value ruby_window_set_view(mrb_state* mrb, mrb_value self) {

	mrb_value ruby_view;

	mrb_get_args(mrb, "o", &ruby_view);

	auto window = MrbWrap::convert_from_object<sf::RenderWindow>(mrb, self);
	auto view = MrbWrap::convert_from_object<sf::View>(mrb, ruby_view);

	window->setView(*view);

	return mrb_nil_value();

}

mrb_value ruby_window_use_view(mrb_state* mrb, mrb_value self) {

	mrb_value ruby_view;
	mrb_value block = mrb_nil_value();

	mrb_get_args(mrb, "o&", &ruby_view, &block);

	auto window = MrbWrap::convert_from_object<sf::RenderWindow>(mrb, self);
	auto view = MrbWrap::convert_from_object<sf::View>(mrb, ruby_view);

	auto old_view = window->getView();
	window->setView(*view);
	mrb_yield(mrb, block, mrb_nil_value());
	window->setView(old_view);

	return mrb_nil_value();

}

mrb_value ruby_window_poll_event(mrb_state* mrb, mrb_value self) {

	auto window = MrbWrap::convert_from_object<sf::RenderWindow>(mrb, self);

	static auto event_class = mrb_class_get_under(mrb, window_ruby_module, "Event");
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

mrb_value ruby_window_draw(mrb_state* mrb, mrb_value self) {

	mrb_value ruby_draw_object;
	mrb_value ruby_render_states = mrb_nil_value();

	mrb_get_args(mrb, "o|o", &ruby_draw_object, &ruby_render_states);

	auto window = MrbWrap::convert_from_object<sf::RenderWindow>(mrb, self);

	sf::RenderStates render_states = sf::RenderStates::Default;
	if(!mrb_nil_p(ruby_render_states)) render_states = *MrbWrap::convert_from_object<sf::RenderStates>(mrb, ruby_render_states);

	draw_object(window, render_states, mrb, ruby_draw_object);

	return mrb_true_value();

}

mrb_value ruby_window_draw_translated(mrb_state* mrb, mrb_value self) {

	mrb_value ruby_draw_object;
	mrb_value ruby_coordinates;
	mrb_value ruby_render_states = mrb_nil_value();

	mrb_get_args(mrb, "oo|o", &ruby_draw_object, &ruby_coordinates, &ruby_render_states);

	auto window = MrbWrap::convert_from_object<sf::RenderWindow>(mrb, self);
	auto coordinates = MrbWrap::convert_from_object<sf::Vector2f>(mrb, ruby_coordinates);

	sf::RenderStates render_states = sf::RenderStates::Default;
	if (!mrb_nil_p(ruby_render_states)) render_states = *MrbWrap::convert_from_object<sf::RenderStates>(mrb, ruby_render_states);

	sf::Transform transform;
	transform.translate(*coordinates);
	render_states.transform *= transform;

	draw_object(window, render_states, mrb, ruby_draw_object);

	return mrb_true_value();

}

mrb_value ruby_window_has_focus(mrb_state* mrb, mrb_value self) {

	auto window = MrbWrap::convert_from_object<sf::RenderWindow>(mrb, self);

	return mrb_bool_value(window->hasFocus());

}

void draw_object(sf::RenderWindow* window, sf::RenderStates& render_states, mrb_state* mrb, mrb_value& draw_object) {

	static auto sprite_class = mrb_class_get_under(mrb, window_ruby_module, "Sprite");
	static auto map_layer_class = mrb_class_get_under(mrb, window_ruby_module, "MapLayer");
	static auto text_class = mrb_class_get_under(mrb, window_ruby_module, "Text");

	auto object_class = mrb_obj_class(mrb, draw_object);

	if (object_class == sprite_class) {

		auto sprite = MrbWrap::convert_from_object<sf::Sprite>(mrb, draw_object);
		window->draw(*sprite, render_states);

	} else if (object_class == map_layer_class) {

		auto map_layer = MrbWrap::convert_from_object<MapLayer>(mrb, draw_object);
		window->draw(*map_layer, render_states);

	} else if (object_class == text_class) {

		auto text = MrbWrap::convert_from_object<sf::Text>(mrb, draw_object);
		auto font = MrbWrap::convert_from_instance_variable<sf::Font>(mrb, draw_object, "@_font");
		text->setFont(*font);
		window->draw(*text, render_states);

	} else {

		//! TODO: Error message

	}

}

void setup_ruby_class_window(mrb_state* mrb, RClass* ruby_module) {

	window_ruby_module = ruby_module;

	auto ruby_window_class = MrbWrap::define_data_class_under(mrb, "Window", ruby_module);

	mrb_define_method(mrb, ruby_window_class, "initialize", ruby_window_init, MRB_ARGS_ARG(3, 1));

	mrb_define_method(mrb, ruby_window_class, "clear", ruby_window_clear, MRB_ARGS_NONE());
	mrb_define_method(mrb, ruby_window_class, "display", ruby_window_display, MRB_ARGS_NONE());
	mrb_define_method(mrb, ruby_window_class, "imgui_update", ruby_window_imgui_update, MRB_ARGS_NONE());

	mrb_define_method(mrb, ruby_window_class, "set_imgui_scale", ruby_set_imgui_scale, MRB_ARGS_REQ(1));

	mrb_define_method(mrb, ruby_window_class, "enable_vertical_sync", ruby_window_enable_vertical_sync, MRB_ARGS_NONE());

	mrb_define_method(mrb, ruby_window_class, "is_open?", ruby_window_is_open, MRB_ARGS_NONE());

	mrb_define_method(mrb, ruby_window_class, "close", ruby_window_close, MRB_ARGS_NONE());

	mrb_define_method(mrb, ruby_window_class, "set_view", ruby_window_set_view, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, ruby_window_class, "use_view", ruby_window_use_view, MRB_ARGS_REQ(2));

	mrb_define_method(mrb, ruby_window_class, "poll_event", ruby_window_poll_event, MRB_ARGS_NONE());

	mrb_define_method(mrb, ruby_window_class, "draw", ruby_window_draw, MRB_ARGS_ARG(1, 1));
	mrb_define_method(mrb, ruby_window_class, "draw_translated", ruby_window_draw_translated, MRB_ARGS_ARG(2, 1));

	mrb_define_method(mrb, ruby_window_class, "has_focus?", ruby_window_has_focus, MRB_ARGS_NONE());

}