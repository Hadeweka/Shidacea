class TestEntity < Entity
	register_id

	add_shape(index: 0, type: ShapeCircle, radius: 25.0)
	add_box(index:0, size: Coordinates.new(25.0, 25.0))

	add_texture(index: 0, filename: "assets/graphics/test/Chishi.png", rect: IntRect.new(0, 0, 50, 50))
	add_sprite(index: 0, texture_index: 0, offset: Coordinates.new(-25.0, -25.0))

	set_hitshape(index: 7, shape_index: 0, damage: 1)
	set_hurtshape(index: 13, shape_index: 0)

	self.living = true
	self.max_hp = 20
	self.gravity_multiplier = 2.0
	self.ai_active = true

	def custom_update
		
	end

	def at_entity_collision(other_entity, hurtshape, hitshape)
		
	end

	def ai_script
		AI::forever do
			AI::wait(10)
			@position.x += 5
		end
	end

end