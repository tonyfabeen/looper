require 'mkmf'

find_executable('make')

CFLAGS  = '-shared'
LDFLAGS = ''

$CFLAGS  = CONFIG['CFLAGS']  = " #{CFLAGS} "
$LDFLAGS = CONFIG['LDFLAGS'] = " #{LDFLAGS} "

system("cd libuv; sh autogen.sh; sh configure; CFLAGS=#{CFLAGS} make; cd ..")

dir_config('uv', File.expand_path('../libuv/include', __FILE__),
                 File.expand_path('../libuv/.libs', __FILE__))

have_library('uv')

create_makefile('looper')
