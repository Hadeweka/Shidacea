# Launshi module with helper functions and singleton instance variables

module SDC
	module Launshi

		PROJECT_FILE_NAME = "project.json"

		class Config

			attr_reader :path, :json
			
			def initialize(path, config_json)
				@path = path
				@json = config_json
			end

		end

		def self.reset_configs
			@configs = []
			@final_config = nil
		end

		def self.load_configs(path)
			Dir.foreach(path) do |entry|
				if !entry.start_with?(".") && File.directory?(path + "/" + entry) then
					config_file_name = path + "/" + entry + "/" + PROJECT_FILE_NAME

					if File.exist?(config_file_name) then
						puts "Found project folder #{entry}"
						new_config = self.load_config_file(path + "/" + entry, PROJECT_FILE_NAME)

						@configs.push(new_config)

						self.load_thumbnail(new_config)
					end
				end
			end
		end

		def self.set_final_config(index)
			@final_config = @configs[index]
		end

		def self.get_final_config
			return @final_config
		end

		def self.get_configs
			return @configs
		end

		def self.load_config_file(path, filename)
			full_name = path + "/" + filename
		
			content = ""
			file = File.open(full_name, "r")
			while !file.eof?
				content += file.readline
			end

			file.close

			return Config.new(path, JSON.parse(content))
		end

		def self.load_scripts(config)
			config.json["scripts"]&.each do |script|
				if script.end_with?("/") then
					SDC::Script.load_recursively(script.chop)
				elsif script.end_with?(".rb") then
					SDC::Script.load(script)
				else
					raise("Invalid file to load: #{script}")
				end
			end
		end

		def self.load_thumbnail(config)
			old_path = SDC::Script.path
			SDC::Script.path = config.path
			SDC::Data.preload_texture(config.path.to_sym, config.json["thumbnail"])
			SDC::Script.path = old_path
		end

	end
end