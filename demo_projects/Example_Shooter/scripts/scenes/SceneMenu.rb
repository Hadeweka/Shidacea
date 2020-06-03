module ShooterTest
	class SceneMenu < SDC::Scene

		def handle_event(event)
			if event.has_type?(:Closed) then
				SDC.next_scene = nil
			elsif event.has_type?(:KeyReleased) then
				if event.key_pressed?(:Q) then
					@player_ship.rotate_drive(-1)
				end

				if event.key_pressed?(:E) then
					@player_ship.rotate_drive(1)
				end
			end
		end

		def at_init
			@player_ship = PlayerShip.new
			@player_ship.position = SDC.xy(300, 200)
			@particles = []

			@space = Space.new(width: 100000, height: 100000, star_count: 1000)

			@player_ship.add_drive(:ImpulseDrive)
			@player_ship.add_drive(:HyperDrive)
			@player_ship.add_drive(:DarkMatterDrive)
			@player_ship.add_drive(:QuantumDrive)
			@player_ship.add_drive(:IonDrive)
			@player_ship.add_drive(:DisplacementDrive)
			@player_ship.add_drive(:WarpDrive)
		end

		def draw
			view_player = SDC::View.new(SDC::FloatRect.new(@player_ship.position.x - SDC.draw_width * 0.5, @player_ship.position.y - SDC.draw_height * 0.5, SDC.draw_width, SDC.draw_height))

			SDC.window.use_view(view_player) do
				@space.draw SDC.window
				@player_ship.draw SDC.window
				@particles.each {|particle| particle.draw(SDC.window)}
			end

			minimap_shape = SDC::DrawShapeRectangle.new
			minimap_shape.size = SDC.xy(SDC.draw_width * 0.2, SDC.draw_height * 0.2)
			minimap_shape.fill_color = SDC::Color.new(128, 128, 128, 128)
			SDC.window.draw_translated(minimap_shape, Z_MINIMAP, SDC.xy(SDC.draw_width * 0.8, 0.0))

			view_minimap = SDC::View.new(SDC::FloatRect.new(0, 0, @space.width, @space.height))
			view_minimap.set_viewport(SDC::FloatRect.new(0.8, 0.0, 0.2, 0.2))

			SDC.window.use_view(view_minimap) do
				player_indicator = SDC::DrawShapeRectangle.new
				player_indicator.size = SDC.xy(@space.width * 0.01, @space.height * 0.02)
				player_indicator.fill_color = SDC::Color.new(255, 0, 0, 255)
				SDC.window.draw_translated(player_indicator, Z_MINIMAP, @player_ship.position)
			end
		end

		def in_cam(pos, tolerance: 0)
			return false if !pos.x.between?(@player_ship.position.x - SDC.draw_width * 0.5 - tolerance, @player_ship.position.x + SDC.draw_width * 0.5 + tolerance)
			return false if !pos.y.between?(@player_ship.position.y - SDC.draw_height * 0.5 - tolerance, @player_ship.position.y + SDC.draw_height * 0.5 + tolerance)
			return true
		end

		def add_particle(particle)
			@particles.push(particle)
		end

		def update
			if SDC.key_pressed?(:A) then
				@player_ship.rotate(-5.0)
			end

			if SDC.key_pressed?(:D) then
				@player_ship.rotate(5.0)
			end

			if SDC.key_pressed?(:W) then
				@player_ship.boost
			end

			if SDC.key_pressed?(:S) then
				@player_ship.brake
			end

			@player_ship.update
			@particles.reject! do |particle|
				particle.update
				particle.gone
			end
		end

	end
end