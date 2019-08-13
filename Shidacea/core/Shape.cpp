#include "Shape.h"

#include "ShapeCollisions.cpp"

mrb_value ruby_collider_test(mrb_state* mrb, mrb_value self) {

	mrb_value ruby_shape_1;
	mrb_value ruby_shape_2;
	mrb_value ruby_pos_1;
	mrb_value ruby_pos_2;

	//! There seem to be ghoooosts

	mrb_get_args(mrb, "oooo", &ruby_shape_1, &ruby_pos_1, &ruby_shape_2, &ruby_pos_2);

	auto type_1 = get_type_of_ruby_shape(mrb, ruby_shape_1);
	auto type_2 = get_type_of_ruby_shape(mrb, ruby_shape_2);

	auto pos_1 = MrbWrap::convert_from_object<sf::Vector2f>(mrb, ruby_pos_1);
	auto pos_2 = MrbWrap::convert_from_object<sf::Vector2f>(mrb, ruby_pos_2);

	auto result = false;
	
	//! A bit hacky, but we don't want to repeat ourselves too many times
	//! Also, this makes the code MUCH more readable and extendable

	TEST_COLLISION_CASE(Point, Point)
	else TEST_COLLISION_CASE_WITH_REVERSE(Point, Line)
	else TEST_COLLISION_CASE_WITH_REVERSE(Point, Circle)
	else TEST_COLLISION_CASE_WITH_REVERSE(Point, Box)
	else TEST_COLLISION_CASE_WITH_REVERSE(Point, Triangle)
	else TEST_COLLISION_CASE_WITH_REVERSE(Point, Quadrangle)
	else TEST_COLLISION_CASE_WITH_REVERSE(Point, Ellipse)
	else TEST_COLLISION_CASE(Line, Line)
	else TEST_COLLISION_CASE_WITH_REVERSE(Line, Circle)
	else TEST_COLLISION_CASE_WITH_REVERSE(Line, Box)
	else TEST_COLLISION_CASE_WITH_REVERSE(Line, Triangle)
	else TEST_COLLISION_CASE_WITH_REVERSE(Line, Quadrangle)
	else TEST_COLLISION_CASE_WITH_REVERSE(Line, Ellipse)
	else TEST_COLLISION_CASE(Circle, Circle)
	else TEST_COLLISION_CASE_WITH_REVERSE(Circle, Box)
	else TEST_COLLISION_CASE_WITH_REVERSE(Circle, Triangle)
	else TEST_COLLISION_CASE_WITH_REVERSE(Circle, Quadrangle)
	else TEST_COLLISION_CASE_WITH_REVERSE(Circle, Ellipse)
	else TEST_COLLISION_CASE(Box, Box)
	else TEST_COLLISION_CASE_WITH_REVERSE(Box, Triangle)
	else TEST_COLLISION_CASE_WITH_REVERSE(Box, Quadrangle)
	else TEST_COLLISION_CASE_WITH_REVERSE(Box, Ellipse)
	else TEST_COLLISION_CASE(Triangle, Triangle)
	else TEST_COLLISION_CASE_WITH_REVERSE(Triangle, Quadrangle)
	else TEST_COLLISION_CASE_WITH_REVERSE(Triangle, Ellipse)
	else TEST_COLLISION_CASE(Quadrangle, Quadrangle)
	else TEST_COLLISION_CASE_WITH_REVERSE(Quadrangle, Ellipse)
	else TEST_COLLISION_CASE(Ellipse, Ellipse)

	//! Other invalid cases will only yield false for now

	return mrb_bool_value(result);

}

ShapeType get_type_of_ruby_shape(mrb_state* mrb, mrb_value ruby_shape) {

	static auto ruby_class_shape_point = mrb_class_get(mrb, "ShapePoint");
	static auto ruby_class_shape_line = mrb_class_get(mrb, "ShapeLine");
	static auto ruby_class_shape_circle = mrb_class_get(mrb, "ShapeCircle");
	static auto ruby_class_shape_box = mrb_class_get(mrb, "ShapeBox");
	static auto ruby_class_shape_triangle = mrb_class_get(mrb, "ShapeTriangle");
	static auto ruby_class_shape_quadrangle = mrb_class_get(mrb, "ShapeQuadrangle");
	static auto ruby_class_shape_ellipse = mrb_class_get(mrb, "ShapeEllipse");

	if (mrb_obj_is_instance_of(mrb, ruby_shape, ruby_class_shape_point)) return ShapeType::Point;
	else if (mrb_obj_is_instance_of(mrb, ruby_shape, ruby_class_shape_line)) return ShapeType::Line;
	else if (mrb_obj_is_instance_of(mrb, ruby_shape, ruby_class_shape_circle)) return ShapeType::Circle;
	else if (mrb_obj_is_instance_of(mrb, ruby_shape, ruby_class_shape_box)) return ShapeType::Box;
	else if (mrb_obj_is_instance_of(mrb, ruby_shape, ruby_class_shape_triangle)) return ShapeType::Triangle;
	else if (mrb_obj_is_instance_of(mrb, ruby_shape, ruby_class_shape_quadrangle)) return ShapeType::Quadrangle;
	else if (mrb_obj_is_instance_of(mrb, ruby_shape, ruby_class_shape_ellipse)) return ShapeType::Ellipse;
	else return ShapeType::Unknown;

}

mrb_value ruby_shape_class_init(mrb_state* mrb, mrb_value self) {

	return self;

}

mrb_value ruby_shape_point_class_init(mrb_state* mrb, mrb_value self) {

	auto shape = MrbWrap::convert_to_object<ShapePoint>(mrb, self, "shape_point");
	return self;

}

mrb_value ruby_shape_point_class_init_copy(mrb_state* mrb, mrb_value self) {

	MrbWrap::copy_object<ShapePoint>(mrb, self, "shape_point");

	return self;

}

mrb_value ruby_shape_line_class_init(mrb_state* mrb, mrb_value self) {

	mrb_value ruby_coordinates;

	mrb_get_args(mrb, "o", &ruby_coordinates);

	auto coordinates = MrbWrap::convert_from_object<sf::Vector2f>(mrb, ruby_coordinates);

	auto shape = MrbWrap::convert_to_object<ShapeLine>(mrb, self, "shape_line");
	shape->line = *coordinates;

	return self;

}

mrb_value ruby_shape_line_class_init_copy(mrb_state* mrb, mrb_value self) {

	MrbWrap::copy_object<ShapeLine>(mrb, self, "shape_line");

	return self;

}

mrb_value ruby_shape_circle_class_init(mrb_state* mrb, mrb_value self) {

	float radius;

	mrb_get_args(mrb, "f", &radius);

	auto shape = MrbWrap::convert_to_object<ShapeCircle>(mrb, self, "shape_circle");
	shape->radius = radius;

	return self;

}

mrb_value ruby_shape_circle_class_init_copy(mrb_state* mrb, mrb_value self) {

	MrbWrap::copy_object<ShapeCircle>(mrb, self, "shape_circle");

	return self;

}

mrb_value ruby_shape_box_class_init(mrb_state* mrb, mrb_value self) {

	mrb_value ruby_coordinates;

	mrb_get_args(mrb, "o", &ruby_coordinates);

	auto coordinates = MrbWrap::convert_from_object<sf::Vector2f>(mrb, ruby_coordinates);

	auto shape = MrbWrap::convert_to_object<ShapeBox>(mrb, self, "shape_box");
	shape->diagonal = *coordinates;

	return self;

}

mrb_value ruby_shape_box_class_init_copy(mrb_state* mrb, mrb_value self) {

	MrbWrap::copy_object<ShapeBox>(mrb, self, "shape_box");

	return self;

}

mrb_value ruby_shape_triangle_class_init(mrb_state* mrb, mrb_value self) {

	//! TODO

	auto shape = MrbWrap::convert_to_object<ShapeTriangle>(mrb, self, "shape_triangle");
	return self;

}

mrb_value ruby_shape_triangle_class_init_copy(mrb_state* mrb, mrb_value self) {

	MrbWrap::copy_object<ShapeTriangle>(mrb, self, "shape_triangle");

	return self;

}

mrb_value ruby_shape_quadrangle_class_init(mrb_state* mrb, mrb_value self) {

	//! TODO

	auto shape = MrbWrap::convert_to_object<ShapeQuadrangle>(mrb, self, "shape_quadrangle");
	return self;

}

mrb_value ruby_shape_quadrangle_class_init_copy(mrb_state* mrb, mrb_value self) {

	MrbWrap::copy_object<ShapeQuadrangle>(mrb, self, "shape_quadrangle");

	return self;

}

mrb_value ruby_shape_ellipse_class_init(mrb_state* mrb, mrb_value self) {

	mrb_value ruby_coordinates;

	mrb_get_args(mrb, "o", &ruby_coordinates);

	auto coordinates = MrbWrap::convert_from_object<sf::Vector2f>(mrb, ruby_coordinates);

	auto shape = MrbWrap::convert_to_object<ShapeEllipse>(mrb, self, "shape_ellipse");
	shape->semiaxes = *coordinates;

	return self;

}

mrb_value ruby_shape_ellipse_class_init_copy(mrb_state* mrb, mrb_value self) {

	MrbWrap::copy_object<ShapeEllipse>(mrb, self, "shape_ellipse");

	return self;

}

void setup_ruby_collider(mrb_state* mrb) {

	auto module_collider = mrb_define_module(mrb, "Collider");
	auto ruby_shape_class = mrb_define_class(mrb, "Shape", mrb->object_class);

	auto ruby_shape_point_class = MrbWrap::define_data_class(mrb, "ShapePoint", ruby_shape_class);
	auto ruby_shape_line_class = MrbWrap::define_data_class(mrb, "ShapeLine", ruby_shape_class);
	auto ruby_shape_circle_class = MrbWrap::define_data_class(mrb, "ShapeCircle", ruby_shape_class);
	auto ruby_shape_box_class = MrbWrap::define_data_class(mrb, "ShapeBox", ruby_shape_class);
	auto ruby_shape_triangle_class = MrbWrap::define_data_class(mrb, "ShapeTriangle", ruby_shape_class);
	auto ruby_shape_quadrangle_class = MrbWrap::define_data_class(mrb, "ShapeQuadrangle", ruby_shape_class);
	auto ruby_shape_ellipse_class = MrbWrap::define_data_class(mrb, "ShapeEllipse", ruby_shape_class);

	mrb_define_method(mrb, ruby_shape_class, "initialize", ruby_shape_class_init, MRB_ARGS_NONE());

	mrb_define_method(mrb, ruby_shape_point_class, "initialize", ruby_shape_point_class_init, MRB_ARGS_NONE());
	mrb_define_method(mrb, ruby_shape_point_class, "initialize_copy", ruby_shape_point_class_init_copy, MRB_ARGS_REQ(1));

	mrb_define_method(mrb, ruby_shape_line_class, "initialize", ruby_shape_line_class_init, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, ruby_shape_line_class, "initialize_copy", ruby_shape_line_class_init_copy, MRB_ARGS_REQ(1));

	mrb_define_method(mrb, ruby_shape_circle_class, "initialize", ruby_shape_circle_class_init, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, ruby_shape_circle_class, "initialize_copy", ruby_shape_circle_class_init_copy, MRB_ARGS_REQ(1));

	mrb_define_method(mrb, ruby_shape_box_class, "initialize", ruby_shape_box_class_init, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, ruby_shape_box_class, "initialize_copy", ruby_shape_box_class_init_copy, MRB_ARGS_REQ(1));

	mrb_define_method(mrb, ruby_shape_triangle_class, "initialize", ruby_shape_triangle_class_init, MRB_ARGS_NONE());	//! TODO
	mrb_define_method(mrb, ruby_shape_triangle_class, "initialize_copy", ruby_shape_triangle_class_init_copy, MRB_ARGS_REQ(1));

	mrb_define_method(mrb, ruby_shape_quadrangle_class, "initialize", ruby_shape_quadrangle_class_init, MRB_ARGS_NONE());	//! TODO
	mrb_define_method(mrb, ruby_shape_quadrangle_class, "initialize_copy", ruby_shape_quadrangle_class_init_copy, MRB_ARGS_REQ(1));

	mrb_define_method(mrb, ruby_shape_ellipse_class, "initialize", ruby_shape_ellipse_class_init, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, ruby_shape_ellipse_class, "initialize_copy", ruby_shape_ellipse_class_init_copy, MRB_ARGS_REQ(1));

	mrb_define_module_function(mrb, module_collider, "test", ruby_collider_test, MRB_ARGS_REQ(4));

}
