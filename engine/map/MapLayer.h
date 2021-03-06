#pragma once

#include <cmath>

#include "MrbWrap.h"

#include <SFML/Graphics.hpp>

#include "Tileset.h"

class MapLayer : public sf::Drawable, public sf::Transformable {

public:

	MapLayer(unsigned int width, unsigned int height, unsigned int view_width, unsigned int view_height, unsigned int tile_width, unsigned int tile_height);

	void reload(float cam_x, float cam_y);
	void load_test_map();

	void set_tile(unsigned int x, unsigned int y, unsigned int tile_id);

	unsigned int get_tile(unsigned int x, unsigned int y);

	void set_collision_active(bool value = true);
	bool is_collision_active();

	void link_tileset(Tileset* tileset);

private:

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	unsigned int width = 0;
	unsigned int height = 0;

	unsigned int view_width = 0;
	unsigned int view_height = 0;

	unsigned int tile_width = 0;
	unsigned int tile_height = 0;

	unsigned int background_tile = 0;

	std::vector<std::vector<unsigned int>> tiles;
	sf::VertexArray vertices;

	Tileset* tileset = nullptr;

	unsigned int frame_counter = 0;

	bool collision_active = true;

};

void setup_ruby_class_map_layer(mrb_state* mrb, RClass* ruby_module);
