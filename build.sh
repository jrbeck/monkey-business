#!/bin/sh

clear

# TCP
# g++ \
#   source/TcpClient.cpp \
#   -ansi -pedantic -Wall -Wextra -O3 -std=c++11 -o client
#
# g++ \
#   source/TcpServer.cpp \
#   -ansi -pedantic -Wall -Wextra -O3 -std=c++11 -o server

# UDP
# g++ \
#   source/Udp/UdpClient.cpp \
#   source/Udp/UdpListener.cpp \
#   source/Udp/UdpMessage.cpp \
#   -lSDL2 -ansi -pedantic -Wall -Wextra -O3 -std=c++11 -o client
#
# g++ \
#   source/Udp/UdpServer.cpp \
#   source/Udp/UdpListener.cpp \
#   source/Udp/UdpMessage.cpp \
#   -lSDL2 -ansi -pedantic -Wall -Wextra -O3 -std=c++11 -o server

# MONKEY
g++ \
  source/main.cpp \
  source/MonkeyGame.cpp \
  source/MonkeyState.cpp \
  source/MonkeyServer.cpp \
  source/ClientState.cpp \
  source/Tokenizer.cpp \
  source/TokenParser.cpp \
  source/EngineModes/Lobby/Controller.cpp \
  source/EngineModes/Lobby/State.cpp \
  source/EngineModes/Lobby/Renderer.cpp \
  source/EngineModes/BananaToss/Controller.cpp \
  source/EngineModes/BananaToss/State.cpp \
  source/EngineModes/BananaToss/Renderer.cpp \
  source/engine/SdlApp.cpp \
  source/engine/ImageBuffer.cpp \
  source/engine/Pixel.cpp \
  source/engine/Painter.cpp \
  source/engine/BasicText.cpp \
  source/engine/PseudoRandom.cpp \
  source/engine/Messaging/MessageBus.cpp \
  source/engine/Messaging/Mailbox.cpp \
  source/engine/Messaging/Message.cpp \
  source/Udp/UdpServer.cpp \
  source/Udp/UdpListener.cpp \
  source/Udp/UdpMessage.cpp \
  source/lib/lodepng.cpp \
  -lSDL2 -ansi -Wall -Wextra -O3 -std=c++11 -o monkey
