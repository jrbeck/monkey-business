require 'pp'
require 'socket'

class UdpClient
  PACKET_LENGTH = 2048
  SERVER_IP_ADDRESS = '127.0.0.1'
  SERVER_PORT = 6789

  def initialize
  end

  def test
    udp_socket.send('CONNECT', 0)
    pp receive_udp_packet
    udp_socket.send('TRAIN', 0)
    pp receive_udp_packet
    udp_socket.send('DISCONNECT', 0)
    pp receive_udp_packet
  end

  def manual_test
    quit = false
    while !quit
      input = gets.strip
      case input
      when 'quit'
        quit = true
      when 'peek'
        pp receive_udp_packet
      else
        udp_socket.send(input, 0)
        sleep(0.1)
        pp receive_udp_packet
      end
    end
  end

  def monkey_test
    send('CONNECT')
    send('TRAIN')

    5.times do
      send('TARGET')
      send('SHOOT,0.5,100.0')
      done = false
      while !done
        sleep(0.2)
        response = receive_udp_packet
        if response != :no_message
          done = true
          pp response
        end
      end
      send('RESET')
    end

    send('QUIT')
    send('DISCONNECT')
  end

  private
    def udp_socket
      return @udp_socket if @udp_socket
      @udp_socket = UDPSocket.new
      @udp_socket.connect(SERVER_IP_ADDRESS, SERVER_PORT)
      @udp_socket
    end

    def send(message, check_response = true)
      udp_socket.send(message, 0)
      return unless check_response
      sleep(0.1)
      pp receive_udp_packet
    end

    def receive_udp_packet
      udp_socket.recvfrom_nonblock(PACKET_LENGTH)
    rescue IO::WaitReadable
      :no_message
    end

    def receive_udp_packet_blocking
      udp_socket.recvfrom_nonblock(PACKET_LENGTH)
    rescue IO::WaitReadable
      IO.select([udp_socket])
      retry
    end
end


udp_client = UdpClient.new
# udp_client.test
# udp_client.manual_test
udp_client.monkey_test
