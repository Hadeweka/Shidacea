#pragma once

#include "MrbWrap.h"
#include "Shape.h"

#include <array>

#include <SFML/Graphics.hpp>

//! NOTE: Point is not working yet and might never work at all.
//! NOTE: Just use a circle.

class PointShape : public sf::Shape {

public:

	explicit PointShape(const sf::Vector2f& point = sf::Vector2f()) {

		m_point = point;

		update();

	}

	void setPoint(const sf::Vector2f& value) {

		m_point = value;

		update();

	}

	virtual std::size_t getPointCount() const {

		return 1;

	}

	virtual sf::Vector2f getPoint(std::size_t index) const {

		return m_point;

	}

private:

	sf::Vector2f m_point;

};

//! The LineShape essentially behaves like a rectangle with overlapping points
//! This way, an actual line can be drawn at all

class LineShape : public sf::Shape {

public:

	explicit LineShape(const sf::Vector2f& start_point = sf::Vector2f(),
		const sf::Vector2f& end_point = sf::Vector2f()) {

		m_points[0] = start_point;
		m_points[1] = end_point;

		//! This allows for always visible lines
		setOutlineThickness(0.5);

		update();

	}

	void setStartPoint(const sf::Vector2f& value) {

		m_points[0] = value;

		update();

	}

	void setEndPoint(const sf::Vector2f& value) {

		m_points[1] = value;

		update();

	}

	sf::Vector2f getEndPoint() {

		return m_points[1];

	}

	virtual std::size_t getPointCount() const {

		return 4;

	}

	virtual sf::Vector2f getPoint(std::size_t index) const {

		return m_points[(index > 1 ? 3 - index : index)];

	}

private:

	std::array<sf::Vector2f, 2> m_points;

};

class TriangleShape : public sf::Shape {

public:
	
	explicit TriangleShape(const sf::Vector2f& point_0 = sf::Vector2f(), 
		const sf::Vector2f& point_1 = sf::Vector2f(), 
		const sf::Vector2f& point_2 = sf::Vector2f()) {

		m_edge_points[0] = point_0;
		m_edge_points[1] = point_1;
		m_edge_points[2] = point_2;

		update();

	}

	void setPoint(std::size_t index, const sf::Vector2f& value) {

		m_edge_points[index] = value;

		update();

	}

	void setSide1(const sf::Vector2f& value) {

		setPoint(1, value);

	}

	void setSide2(const sf::Vector2f& value) {

		setPoint(2, value);

	}

	sf::Vector2f getSide1(){

		return m_edge_points[1];

	}

	sf::Vector2f getSide2() {

		return m_edge_points[2];

	}

	virtual std::size_t getPointCount() const {

		return 3;

	}

	virtual sf::Vector2f getPoint(std::size_t index) const {

		return m_edge_points[index];

	}

private:

	std::array<sf::Vector2f, 3> m_edge_points;


};

void setup_ruby_class_draw_shape(mrb_state* mrb, RClass* ruby_module);