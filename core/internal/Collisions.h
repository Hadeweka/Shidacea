#pragma once

#include <cmath>

//! Here resides the actual mathematical core of the collision routines
//! These are completely decoupled from any Ruby or SFML magic

//! Sadly, std::abs is not a constexpr, so this definition will provide one

template <class T> constexpr T constexpr_abs(T value) {

	return (value < 0.0f ? -value : value);

}

//! Helper function to check whether a fractional value is greater than zero without actually doing the division

template <class T> constexpr bool fraction_less_than_zero(T nominator, T denominator) {

	if (nominator == static_cast<T>(0)) return false;
	if (static_cast<bool>(nominator < static_cast<T>(0)) != static_cast<bool>(denominator < static_cast<T>(0))) return true;

	return false;

}

template <class T> constexpr bool fraction_between_zero_and_one(T nominator, T denominator) {

	if (fraction_less_than_zero(nominator, denominator)) return false;
	if (constexpr_abs(nominator) > constexpr_abs(denominator)) return false;

	return true;

}

constexpr bool collision_point_point(float x1, float y1, float x2, float y2) {

	//! Usually, this routine will yield false

	return (x1 == x2 && y1 == y2);

	return false;

}

constexpr bool collision_point_line(float x1, float y1, float x2, float y2, float dx2, float dy2) {

	//! The most useful check is to check whether the point has a normal component to the line
	//! If so, it is impossible for the point to intersect the line
	//! This case is also the most common one, so it is wise to check it first

	auto dx12 = x1 - x2;
	auto dy12 = y1 - y2;

	//! Check whether the cross product of the distance vector and the line vector is vanishing

	if (dx12 * dy2 != dy12 * dx2) return false;

	//! Otherwise, the point is on the infinite extension of the line
	//! Now, the point will be projected to the line
	//! If this projection value is smaller than 0, the point is not on the line
	//! If it is greater than the line end point projected on the line, it is also not on the line

	auto projection = dx12 * dx2 + dy12 * dy2;

	if (projection < 0.0f) return false;
	if (projection > dx2 * dx2 + dy2 * dy2) return false;

	return true;

}

constexpr bool collision_point_circle(float x1, float y1, float x2, float y2, float r2) {

	//! Simple check whether the point is inside the circle radius

	auto dx = x1 - x2;
	auto dy = y1 - y2;

	if (dx * dx + dy * dy > r2 * r2) return false;
	
	return true;

}

constexpr bool collision_point_box(float x1, float y1, float x2, float y2, float w2, float h2) {

	//! Literally the definition of an AABB

	if (x1 < x2 - w2) return false;
	if (y1 < y2 - h2) return false;
	if (x2 + w2 < x1) return false;
	if (y2 + h2 < y1) return false;
	
	return true;
}

constexpr bool collision_point_triangle(float x1, float y1, float x2, float y2, float sxa2, float sya2, float sxb2, float syb2) {

	//! Point coordinates relative to the first triangle point

	auto dx12 = x1 - x2;
	auto dy12 = y1 - y2;

	//! The next step consists of calculating the coordinates of the point formulated as linear combination of the two sides
	//! This yields the values u and v, which need to satisfy the following conditions: u >= 0, v >= 0, u + v <= 1

	auto nominator_u = dx12 * syb2 - dy12 * sxb2;
	auto denominator_u = sxa2 * syb2 - sxb2 * sya2;

	if (!fraction_between_zero_and_one(nominator_u, denominator_u)) return false;

	auto nominator_v = dx12 * sya2 - dy12 * sxa2;
	auto denominator_v = -denominator_u;

	if (!fraction_between_zero_and_one(nominator_v, denominator_v)) return false;

	//! The condition u + v <= 1 has one caveat, namely the case that the nominators of u and -v have different signs
	//! Therefore, the check for u + v >= 0 needs to be done explicitely again
	//! Furthermore, the denominator of u will be taken here as the new denominator

	auto nominator_u_v = nominator_u - nominator_v;

	if (!fraction_between_zero_and_one(nominator_u_v, denominator_u)) return false;

	//! If all values are inside the appropriate ranges, the point is inside the triangle

	return true;

}

constexpr bool collision_line_line(float x1, float y1, float dx1, float dy1, float x2, float y2, float dx2, float dy2) {

	//! This algorithm is an extension of point/line collisions
	//! First, the cross product of the two lines will be calculated
	//! If it is vanishing, the lines are both on their respective infinite extensions
	//! In that case, if the start points of one lines lies on the other line, they intersect
	//! Checking the end points is not necessary here
	
	auto cross_term = dx2 * dy1 - dy2 * dx1;

	if (cross_term == 0.0f) {

		if (collision_point_line(x1, y1, x2, y2, dx2, dy2)) return true;
		if (collision_point_line(x2, y2, x1, y1, dx1, dy1)) return true;
	
	}

	//! The lines have a normal component, so they have only up to one intersection point
	//! Now, the separating axis theorem can be applied to the situation
	//! Both lines are then projected on the other line normal
	//! If the two projections (start and end point) change their sign, they intersect the other line
	//! If the projection interval doesn't contain 0, an intersection is excluded completely

	auto y21 = y2 - y1;
	auto x21 = x2 - x1;

	auto projection_2_on_n1 = y21 * dx1 - x21 * dy1;

	if (static_cast<bool>(projection_2_on_n1 < 0.0f) == static_cast<bool>(projection_2_on_n1 < cross_term)) return false;

	auto projection_1_on_n2 = x21 * dy2 - y21 * dx2;

	if (static_cast<bool>(projection_1_on_n2 < 0.0f) == static_cast<bool>(projection_1_on_n2 < -cross_term)) return false;

	return true;
}

constexpr bool collision_line_circle(float x1, float y1, float dx1, float dy1, float x2, float y2, float r2) {

	//! This algorithm is a direct implementation of the separating axis theorem
	//! If there is axis at which the projections of both objects do not overlap, they don't intersect

	//! Calculate difference coordinates
	
	auto x21 = x2 - x1;
	auto y21 = y2 - y1;

	//! Project the circle directly on the line and check whether there is a gap or not

	auto proj_par = x21 * dx1 + y21 * dy1;
	
	if (proj_par + r2 < 0.0f) return false;
	if (proj_par - r2 > dx1 * dx1 + dy1 * dy1) return false;
	
	//! Now project the circle on the normal of the line and check for a gap

	auto proj_perp = y21 * dx1 - x21 * dy1;

	if (proj_perp - r2 > 0.0f) return false;
	if (proj_perp + r2 < 0.0f) return false;

	//! If no gaps are present, the circle and the line are intersecting

	return true;

}

constexpr bool collision_line_box(float x1, float y1, float dx1, float dy1, float x2, float y2, float w2, float h2) {

	//! First check whether any end point lies inside the box

	if (collision_point_box(x1, y1, x2, y2, w2, h2)) return true;
	if (collision_point_box(x1 + dx1, y1 + dy1, x2, y2, w2, h2)) return true;
	
	//! If this is not the case, check each axis for an intersection inside the rectangle

	//! These value can all be calculated in beforehand, since 3/4 of them will be already used in the first check
	//! This will make the code much less complicated

	//! Nominators of the line parameter

	auto nominator_x_neg = x2 - w2 - x1;
	auto nominator_x_pos = x2 + w2 - x1;
	auto nominator_y_neg = y2 - h2 - y1;
	auto nominator_y_pos = y2 + h2 - y1;

	//! These terms can be obtained by inserting the line parameter for one coordinate into the intersection equation
	//! This is only a mathematical trick to avoid divisions here

	auto nom_x_neg_dy = nominator_x_neg * dy1;
	auto nom_x_pos_dy = nominator_x_pos * dy1;
	auto nom_y_neg_dx = nominator_y_neg * dx1;
	auto nom_y_pos_dx = nominator_y_pos * dx1;

	//! Check whether the y coordinate of the intersection point with the left AABB side is actually inside the AABB
	//! The following checks will repeat this procedure for the other sides

	if ((nom_x_neg_dy >= nom_y_neg_dx) && (nom_x_neg_dy <= nom_y_pos_dx)) {

		//! The case of a vanishing dx1 should not occur, but even then, the next check will rule it out definitely
		//! Here, the line parameter of the intersection point will be checked for its sign
		//! If it is smaller than 0 or greater than 1, the line segment does not touch the AABB at this side
		//! This can again be checked by comparing the signs of the nominator and the denominator
		//! We still need to check the other sides, however

		if (fraction_between_zero_and_one(nominator_x_neg, dx1)) return true;

	}

	//! Check right side

	if ((nom_x_pos_dy >= nom_y_neg_dx) && (nom_x_pos_dy <= nom_y_pos_dx)) {

		//! The line got shifted in its coordinates, so a new line parameter check is necessary

		if (fraction_between_zero_and_one(nominator_x_pos, dx1)) return true;

	}

	//! Check bottom side

	if ((nom_y_neg_dx >= nom_x_neg_dy) && (nom_y_neg_dx <= nom_x_pos_dy)) {

		if (fraction_between_zero_and_one(nominator_y_neg, dy1)) return true;

	}

	//! Check top side

	if ((nom_y_pos_dx >= nom_x_neg_dy) && (nom_y_pos_dx <= nom_x_pos_dy)) {

		if (fraction_between_zero_and_one(nominator_y_pos, dy1)) return true;

	}

	return false;

}

constexpr bool collision_circle_circle(float x1, float y1, float r1, float x2, float y2, float r2) {

	//! Simple generalization of point/circle

	auto dx = x1 - x2;
	auto dy = y1 - y2;

	auto combined_radius = r1 + r2;

	if (dx * dx + dy * dy > combined_radius * combined_radius) return false;

	return true;

}

constexpr bool collision_box_box(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2) {

	//! Simple generalization of point/box

	if (x1 + w1 < x2 - w2) return false;
	if (y1 + h1 < y2 - h2) return false;
	if (x2 + w2 < x1 - w1) return false;
	if (y2 + h2 < y1 - h1) return false;

	return true;

}

constexpr bool collision_circle_box(float x1, float y1, float r1, float x2, float y2, float w2, float h2) {

	//! This algorithm makes use of the separating axis theorem (SAT)
	//! Essentially, the circle is projected onto both cardinal axes
	//! Then, the border points of the AABB are checked for intersection with the circle

	//! Projections of the circle onto the axes

	auto dxp = (x2 + w2 - x1);
	auto dyp = (y2 + h2 - y1);
	auto dxm = (x2 - w2 - x1);
	auto dym = (y2 - h2 - y1);

	//! Check for intersection of the circle projections with the AABB projections

	if (r1 < dxm) return false;
	if (r1 < dym) return false;
	if (dxp < - r1) return false;
	if (dyp < - r1) return false;

	//! Squared values of the points

	auto r_squared = r1 * r1;

	auto dxp2 = dxp * dxp;
	auto dxm2 = dxm * dxm;
	auto dyp2 = dyp * dyp;
	auto dym2 = dym * dym;

	//! Test collision of each border point with the circle
	//! This is effectively just another point/circle test

	if (r_squared > dxp2 + dyp2) return true;
	if (r_squared > dxm2 + dyp2) return true;
	if (r_squared > dxp2 + dym2) return true;
	if (r_squared > dxm2 + dym2) return true;

	//! The last possibility is that the circle is inside the box, which can be checked quickly

	return collision_point_box(x1, y1, x2, y2, w2, h2);

}

#ifndef SHIDACEA_IGNORE_STATIC_ASSERTIONS

//! Compile time assertions to check some test cases
//! Please submit a bug report if one of these fails
//! Also please submit a bug report if you encounter a case which fails and can be reproduced using an assertion

static_assert(true == fraction_less_than_zero(-1.0, 3.0));
static_assert(true == fraction_less_than_zero(1.0, -3.0));
static_assert(false == fraction_less_than_zero(0.0, 3.0));
static_assert(false == fraction_less_than_zero(0.0, -3.0));
static_assert(false == fraction_less_than_zero(1.0, 3.0));
static_assert(false == fraction_less_than_zero(-1.0, -3.0));

static_assert(false == fraction_between_zero_and_one(-1.0, 3.0));
static_assert(false == fraction_between_zero_and_one(1.0, -3.0));
static_assert(true == fraction_between_zero_and_one(0.0, 3.0));
static_assert(true == fraction_between_zero_and_one(0.0, -3.0));
static_assert(true == fraction_between_zero_and_one(1.0, 3.0));
static_assert(true == fraction_between_zero_and_one(-1.0, -3.0));
static_assert(false == fraction_between_zero_and_one(3.0, 1.0));
static_assert(false == fraction_between_zero_and_one(-3.0, 1.0));
static_assert(false == fraction_between_zero_and_one(-3.0, -1.0));

static_assert(false == collision_point_point(1.0f, 2.0f,     3.0f, 4.0f));
static_assert(true == collision_point_point(1.0f, 9.0f,     1.0f, 9.0f));

static_assert(true == collision_point_line(0.2f, 0.2f,     0.0f, 0.0f, 1.0f, 1.0f));
static_assert(false == collision_point_line(0.2f, 0.3f,     0.0f, 0.0f, 1.0f, 1.0f));
static_assert(true == collision_point_line(1.0f, 0.0f,     0.0f, 0.0f, 1.0f, 0.0f));
static_assert(true == collision_point_line(1.0f, 0.0f,      1.0f, 0.0f, 1.0f, 0.0f));
static_assert(false == collision_point_line(1.0f, 0.0f,     1.1f, 0.0f, 1.0f, 0.0f));

static_assert(true == collision_point_circle(2.0f, 3.0f,     4.0f, 5.0f, 3.0f));

static_assert(true == collision_point_box(-3.0f, -5.0f,     3.0f, 2.0f, 10.0f, 9.0f));

static_assert(true == collision_point_triangle(0.0f, 0.0f,     0.0f, 0.2f, 3.0f, -1.0f, -3.0f, -1.0f));
static_assert(false == collision_point_triangle(0.0f, 0.0f,     0.0f, 0.2f, 3.0f, 1.0f, -3.0f, 1.0f));

static_assert(true == collision_line_line(0.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f, 1.0f, -1.0f));
static_assert(false == collision_line_line(0.0f, 0.0f, 1.0f, 0.0f,     1.1f, -1.0f, 0.0f, 2.0f));
static_assert(true == collision_line_line(0.0f, 0.0f, 1.0f, 0.0f,     0.9f, -1.0f, 0.0f, 2.0f));
static_assert(false == collision_line_line(0.0f, 0.0f, 1.0f, 1.0f,     0.0f, 0.1f, 1.0f, 1.0f));

static_assert(true == collision_line_line(0.0f, 0.0f, 1.0f, 0.0f,     1.0f, 0.0f, 1.0f, 0.0f));
static_assert(false == collision_line_line(0.0f, 0.0f, 1.0f, 0.0f,     1.1f, 0.0f, 1.0f, 0.0f));
static_assert(false == collision_line_line(1.1f, 0.0f, 1.0f, 0.0f,     0.0f, 0.0f, 1.0f, 0.0f));

static_assert(true == collision_line_circle(1.0f, 1.0f, 8.0f, 8.0f,     -3.0f, -3.0f, 100.0f));
static_assert(true == collision_line_circle(1.0f, 1.0f, 8.0f, 8.0f,      4.0f, 4.0f, 0.1f));
static_assert(false == collision_line_circle(1.0f, 1.0f, 8.0f, 8.0f,      10.0f, 10.0f, 0.9f));

static_assert(true == collision_line_box(3.0f, 2.0f, 8.0f, 11.0f,     5.0f, 6.0f, 5.0f, 5.0f));
static_assert(false == collision_line_box(11.0f, 0.0f, 11.0f, 13.0f,     5.0f, 6.0f, 5.0f, 5.0f));
static_assert(true == collision_line_box(1.0f, 1.0f, 7.0f, 7.0f,     4.0f, 4.0f, 2.0f, 2.0f));

static_assert(true == collision_box_box(1.0f, 2.0f, 3.0f, 4.0f,     4.5f, 7.5f, 2.0f, 2.0f));
static_assert(false == collision_box_box(1.0f, 2.0f, 3.0f, 4.0f,     4.5f, 7.5f, 1.4f, 1.4f));

static_assert(true == collision_circle_box(1.0f, -3.0f, 4.0f,     0.0f, 0.0f, 5.0f, 4.0f));
static_assert(false == collision_circle_box(1.0f, -3.0f, 4.0f,     0.0f, 0.0f, 5.0f, 2.0f));

#endif