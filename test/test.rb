require 'looper'

class MyTCPServer
  include Looper::TCPServer

  def on_connection
    puts "[MyTCPServer] on connection "
  end

  def on_data(data)
    puts "Aqui tem Data: #{data}"
  end
end

my_tcp_server = MyTCPServer.new
my_tcp_server.start
