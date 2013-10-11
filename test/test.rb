require 'looper'

class MyTCPServer
  include Looper::TCPServer
end

my_tcp_server = MyTCPServer.new
