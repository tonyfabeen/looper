require 'mkmf'

#$CFLAGS = CONFIG['CFLAGS'] = '-shared -fPIC'
find_executable('make')
system('cd ../.. && make build_libuv')
system('export LD_LIBRARY_PATH=/var/lib/gems/1.9.1/gems/looper-0.0.1/ext/deps/libuv/.libs')

dir_config('uv', File.expand_path('../deps/libuv/include'),
                 File.expand_path('../deps/libuv/.libs'))

have_library('uv')

create_makefile('looper')
