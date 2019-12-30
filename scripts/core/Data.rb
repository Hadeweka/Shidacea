module SDC
	module Data

		extend SDCMeta::DataStorage

		self.define_new_data_type(:entity, plural: :entities, as_hash: true)
		self.define_new_data_type(:text)
		self.define_new_data_type(:tileset, as_hash: true)
		self.define_new_data_type(:texture, as_hash: true)
		self.define_new_data_type(:sound_buffer)
		self.define_new_data_type(:music_track, as_hash: true)
		self.define_new_data_type(:map_config, as_hash: true)
		self.define_new_data_type(:filename, as_hash: true)

		# TODO: Generate meta-methods for these

		def self.load_texture(file_index, filename: nil)
			filename = self.filenames[file_index] if !filename

			if !self.textures[file_index] then
				texture = SDC::Texture.new
				texture.load_from_file(filename)
				self.add_texture(texture, index: file_index)
			end

			return self.textures[file_index]
		end

		def self.preload_texture(file_index, filename)
			self.add_filename(filename, index: file_index)
			self.load_texture(file_index)
		end

		def self.load_music(file_index, filename: nil)
			filename = self.filenames[file_index] if !filename

			if !self.music_tracks[file_index] then
				music = SDC::Music.new
				music.open_from_file(filename)
				self.add_music_track(music, index: file_index)
			end

			return self.music_tracks[file_index]
		end

		def self.preload_music(file_index, filename)
			self.add_filename(filename, index: file_index)
			self.load_music(file_index)
		end

	end
end