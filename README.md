protocol:

introduction:
c: CONNECT
s: CONNECT,OK

c: DISCONNECT
s: DISCONNECT,OK


training:
c: TRAIN
s: TRAIN,OK
  probably should include their player id or something

c: TARGET
s: TARGET,123.456,321.654
  (x, y)

c: SHOOT,0.5,100.0
  (angle, force)
s: SHOOT,OK || SHOOT,FAIL
-> s: RESULT,HIT or s: RESULT,MISS

c: SPEED,FAST
c: SPEED,SLOW

RESET
QUIT

server states:
  client
    lobby
    training
      client_waiting_for_target_info
      waiting_for_shot_info_from_client
      simulating
      results_sent_to_client


client states:
  connected
  training
    waiting_for_simulation_results
