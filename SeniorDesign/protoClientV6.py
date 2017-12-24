#########################
#########################
#File: protoClient.py
#
#To run: use the following line
#
#from protoClient import Client
#
#########################
#########################
#when creating a new Client
#
# nClient = Client(IP_ADD, PORT)
# nClient.connect()#connect to the server
# nClient.send("Error Code")#send an error code
# nClient.receive() #server response\
# nClient.close() #close the server
#
#########################
#########################
#Summary: A protoClient that sends a message to the server and
#         receives a response back
#
#########################
#########################

import socket
import time

IP_ADD = '129.110.92.15'#the ip address of the server
PORT = 9189 #the port number

class Client:

   def __init__(self, host, port):#initalize the Client
      self.host = host
      self.port = port
      self.isConnected = 0
      self.socket = socket.socket()
      self.isWorking = True

   def connect(self):#connect to the Server
      if(self.isConnected == 0):
         self.socket.connect((self.host,self.port))
         self.isConnected = 1
         self.cSetup()
      else:
         print("Error: Client is already connected to a server!")

   def reconnect(self):#reconnect to the server after breaking the connection
      self.socket.close()
      self.socket = socket.socket()
      self.socket.connect((self.host,self.port))

   def send(self,code):#send a formatted message
      message =  str(self.getID()) + ',' + str(self.getTime()) + ',' + str(code) + ',' + "CONNECTED" + ',' + str(self.getDay())
      self.socket.send(message.encode('ascii'))

   def receive(self):#receive a message from the Server
      bytemessage = self.socket.recv(1024)
      rmessage = bytemessage.decode('ascii')
      print("Client: Received from the server " + rmessage)
      return rmessage

   def close(self): #close the connection
      self.socket.close()
      print("Client: Connection closed.")

   def address(self):#get the address of the Client
      return socket.gethostbyname(socket.gethostname())

   def getTime(self):#Get the time
      return time.strftime("%b %d %Y, %H:%M:%S", time.gmtime())
  
   def getDay(self):#get the day
      return time.strftime("%j")

   def cSetup(self):#setup the Client files
      try:
         r = open('cLog.txt', 'r')
         r.close()
      except IOError as err:
         self.socket.send('setup'.encode('ascii'))
         rmessage = self.receive()
         w = open('cLog.txt', 'w')
         w.write("Device ID, Device Type\n")
         w.write(rmessage + "\n")
         w.close()
         self.reconnect()

   def getID(self):
      try: 
         r = open('cLog.txt', 'r')
         r.readline()
         tempstring = r.readline().split(",")
         return int(tempstring[0])
      except:
         print("cLog.txt not found! Run the setup first!")
         return 0
       
