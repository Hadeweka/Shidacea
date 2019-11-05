class Map

	# TODO: Remove this if unneeded
	attr_accessor :map_layers

	def initialize(view_width: 20, view_height: 20)
		@view_width = view_width
		@view_height = view_height

		@map_layers = []
	end

	def load_from_file(filename)
		# TODO: Read properties from file
		@number_of_layers = 3
		@width = 100
		@height = 100
		@tile_width = 60
		@tile_height = 60

		@number_of_layers.times do |i|
			new_layer = MapLayer.new(@width, @height, @view_width, @view_height, @tile_width, @tile_height)

			# TODO: Load tiles into map layer and initialize the mesh
			new_layer.load_test_map
			new_layer.collision_active = (i == 2)

			@map_layers.push(new_layer)
		end
	end

	def update(position)
		reload(position)
	end

	def reload(position)
		@map_layers.each {|layer| layer.reload(position)}
	end

	def draw(window, offset)
		@map_layers.each do |layer|
			window.draw_translated(layer, offset)
		end
	end

end