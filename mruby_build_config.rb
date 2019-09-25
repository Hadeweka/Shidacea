MRuby::Build.new do |conf|

  if ENV['VisualStudioVersion'] || ENV['VSINSTALLDIR']
    toolchain :visualcpp
  else
    toolchain :gcc
  end

  conf.gembox 'default'

  conf.cc.flags << '-DMRB_USE_FLOAT'

  conf.build_dir = ENV["MRUBY_BUILD_DIR"] || raise("MRUBY_BUILD_DIR undefined!")

end
