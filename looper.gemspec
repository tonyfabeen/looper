Gem::Specification.new do |s|
  s.name        = "looper"
  s.version     = "0.0.1"
  s.platform    = Gem::Platform::RUBY
  s.authors     = ["Tony Fabeen"]
  s.email       = ["tony.fabeen@gmail.com"]
  s.homepage    = "http://github.com/tonyfabeen/looper"
  s.summary     = "Event I/O Library for Network Ruby Applications"
  s.description = "Event I/O Library for High Performance Network Ruby Applications"

  s.required_rubygems_version = ">= 1.3.6"
  s.rubyforge_project         = "looper"

  s.files        = Dir.glob("{ext}/**/*")

  s.extensions << 'ext/looper/extconf.rb'
end
