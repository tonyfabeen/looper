require 'looper'

class MyTCPServer
  include Looper::TCPServer

  def on_connection
    puts "[MyTCPServer] on connection "
  end

  def on_data(data)
    puts "[MyTCPServer] Incoming Data: #{data}"
  end

  def on_error(description)
    puts "[MyTCPServer] Error Detected: #{description}"
  end

  def on_connection_closed
    puts "[MyTCPServer] Connection Closed"
  end

end

my_tcp_server = MyTCPServer.new
my_tcp_server.start


