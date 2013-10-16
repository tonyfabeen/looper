require 'mkmf'

#$CFLAGS = CONFIG['CFLAGS'] = '-shared -fPIC'

`cd ../deps/libuv; sh autogen.sh; sh configure; make; cd ../..`

dir_config('uv', File.expand_path('../deps/libuv/include'),
                   File.expand_path('../deps/libuv/.libs'))

have_library('uv')

create_makefile('looper')
