#include "Listener.h"

mrb_value ruby_listener_listen(mrb_state* mrb, mrb_value self) {

	mrb_int port;
	mrb_value address;

	mrb_get_args(mrb, "i|o", &port, &address);

	sf::IpAddress address_to_listen_on = sf::IpAddress::Any;
	if(mrb_string_p(address)) address_to_listen_on = sf::IpAddress(mrb_str_to_cstr(mrb, address));

	//! TODO: Error message if wrong argument type is used

	auto listener = MrbWrap::convert_from_object<sf::TcpListener>(mrb, self);

	auto status_code = listener->listen(static_cast<unsigned short>(port), address_to_listen_on);

	return mrb_fixnum_value(status_code);

}

mrb_value ruby_listener_accept(mrb_state* mrb, mrb_value self) {

	mrb_value ruby_socket;

	mrb_get_args(mrb, "o", &ruby_socket);

	auto listener = MrbWrap::convert_from_object<sf::TcpListener>(mrb, self);
	auto socket = MrbWrap::convert_from_object<sf::TcpSocket>(mrb, ruby_socket);

	auto status_code = listener->accept(*socket);

	return mrb_fixnum_value(status_code);

}

void setup_ruby_class_listener(mrb_state* mrb, RClass* ruby_module) {

	auto ruby_listener_class = MrbWrap::define_data_class_under(mrb, "Listener", ruby_module);

	MrbWrap::wrap_constructor<sf::TcpListener>(mrb, ruby_listener_class);

	mrb_define_method(mrb, ruby_listener_class, "listen", ruby_listener_listen, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, ruby_listener_class, "accept", ruby_listener_accept, MRB_ARGS_REQ(1));

}