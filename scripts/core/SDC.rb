module SDC

	# Basic attributes

	@window = nil
	@scene = nil
	@next_scene = nil
	@game = nil
	@resource_manager = nil

	def self.window
		return @window
	end

	def self.window=(value)
		@window = value
	end

	def self.scene
		return @scene
	end

	def self.scene=(value)
		@scene = value
	end

	def self.next_scene
		return @next_scene
	end

	def self.next_scene=(value)
		@next_scene = value
	end

	def self.game
		return @game
	end

	def self.game=(value)
		@game = value
	end

	def self.resource_manager
		return @resource_manager
	end
	
	def self.resource_manager=(value)
		@resource_manager = value
	end

	# Script routines for easier readability, directly referencing other methods

	def self.key_pressed?(key)
		return @window.has_focus? && EventKey::is_pressed?(key)
	end

	def self.right_klick?
		return @window.has_focus? && EventMouse::is_button_pressed?(EventMouse::Right)
	end

	def self.left_klick?
		return @window.has_focus? && EventMouse::is_button_pressed?(EventMouse::Left)
	end

	def self.get_variable(index, default: nil)
		return @game.get_variable(index, default: default)
	end

	def self.set_variable(index, value)
		@game.set_variable(index, value)
	end

	def self.get_switch(index)
		return @game.get_switch(index)
	end

	def self.set_switch(index, value: true)
		@game.set_switch(index, value: value)
	end

	def self.reset_switch(index, value: false)
		@game.set_switch(index, value: value)
	end

	def self.toggle_switch(index)
		@game.toggle_switch(index)
	end

	def self.increase_variable(index, value = 1, default: 0)
		@game.increase_variable(index, value, default: default)
	end

	def self.decrease_variable(index, value = 1, default: 0)
		@game.decrease_variable(index, value, default: default)
	end

	def self.multiply_variable(index, value, default: 0)
		@game.multiply_variable(index, value, default: default)
	end

end