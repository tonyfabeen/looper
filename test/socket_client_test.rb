require 'socket'

s = TCPSocket.new 'localhost', 8266
10.times {s.write 'Testing.. '}

while line = s.gets
  puts line.chop
end

s.close
