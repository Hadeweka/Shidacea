# This is where you want to do the most work
# Each relevant scene can be defined here

class SceneTest < Scene 

	def handle_event(event)
		if event.type == EventType::KeyPressed then
			puts "Key Code = " + event.key_code.to_s
		elsif event.type == EventType::Closed
			$next_scene = nil
		end
	end

	def update
		if EventKey::is_pressed?(EventKey::A) then
			EventMouse::set_position([300, 200], $window)
		elsif EventKey::is_pressed?(EventKey::B) then
			puts EventMouse::get_position($window)
		elsif EventKey::is_pressed?(EventKey::C) then
			@test_entity.position += Coordinates.new((rand*10 - 5).to_i, (rand*10 - 5).to_i)
		end
	end

	def at_init
		@test_entity = TestEntity.new(@sprites)
		@test_entity.position = Coordinates.new(400, 400)
	end

	def draw
		@test_entity.draw($window)
	end

end