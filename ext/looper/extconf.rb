require 'mkmf'

GEM_PATH = `gem env gemdir`.chop
VERSION  = File.read('../../VERSION').chop

find_executable('make')
system('cd ../.. && make build_libuv')
system("echo 'export LD_LIBRARY_PATH=#{GEM_PATH}/gems/looper-#{VERSION}/ext/deps/libuv/.libs:$LD_LIBRARY_PATH' >> ~/.bashrc")

dir_config('uv', File.expand_path('../deps/libuv/include'),
                 File.expand_path('../deps/libuv/.libs'))

have_library('uv')

create_makefile('looper')
