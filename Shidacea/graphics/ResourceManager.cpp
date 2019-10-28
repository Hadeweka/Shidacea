#include "ResourceManager.h"

mrb_value ruby_resource_manager_init(mrb_state* mrb, mrb_value self) {

	MrbWrap::convert_to_object<ResourceManager>(mrb, self);

	return self;

}

void setup_ruby_class_resource_manager(mrb_state* mrb) {

	auto ruby_resource_manager_class = MrbWrap::define_data_class(mrb, "ResourceManager");

	mrb_define_method(mrb, ruby_resource_manager_class, "initialize", ruby_resource_manager_init, MRB_ARGS_NONE());

}