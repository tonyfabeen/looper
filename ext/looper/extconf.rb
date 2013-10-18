require 'mkmf'

#$CFLAGS = CONFIG['CFLAGS'] = '-shared -fPIC'
find_executable('make')
system('cd ../.. && make build_libuv')

dir_config('uv', File.expand_path('../deps/libuv/include'),
                 File.expand_path('../deps/libuv/.libs'))

have_library('uv')

create_makefile('looper')
