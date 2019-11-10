# Structure for temporary game data
# Feel free to customize and extend this class

module SDC
	class Game

		attr_reader :dt, :meter
		attr_accessor :gravity

		def initialize
			# Distance unit in pixels
			@meter = 50.0

			# Physics time step
			@dt = 1.0 / 60.0

			# Gravity
			@gravity = Coordinates.new(0, 60.0) * @meter

			# Basic hash for global and local switches
			@switches = {}

			# Basic hash for global and local variables
			@variables = {}
		end

		# Methods for manipulating switches

		def get_switch(index)
			return @switches[index]
		end

		def set_switch(index, value: true)
			@switches[index] = value
		end

		def toggle_switch(index)
			@switches[index] = !@switches[index]
		end

		# Methods for manipulating variables

		def get_variable(index, default: nil)
			return @variables[index] ? @variables[index] : default
		end

		def set_variable(index, value)
			@variables[index] = value
		end

		# Special numerical options for variables

		def increase_variable(index, value = 1, default: 0)
			@variables[index] = @variables[index] ? @variables[index] + value : default + value
		end

		def decrease_variable(index, value = 1, default: 0)
			@variables[index] = @variables[index] ? @variables[index] - value : default - value
		end

		def multiply_variable(index, value, default: 0)
			@variables[index] = @variables[index] ? @variables[index] * value : default * value
		end

	end
end