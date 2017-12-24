#########################
#########################
#File: protoServer.py
#
#To run: use the following line
#
#from protoServer import Server
#
#########################
#########################
#when creating a new Server
#
# nServer = Server(PORT)
# nServer.bind()
# nServer.run()
#
#########################
#########################
#Summary: Prototype server code that takes a message from a client and
#         stores the error code in a datalog
#
#########################

import socket
import threading

LIMIT = 9 #number of timeouts allowed before closing server
TIMEOUT = 100# number of seconds to wait for a timeout

PORT = 7123

class Server:

   def __init__(self, port=PORT, host=socket.gethostname()):#initalize the server
      self.port = port
      self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
      self.host = host
      self.lock = threading.Lock()

   def bind(self):#bind the Server to the address/port
      self.socket.settimeout(TIMEOUT)#wait 5 seconds before timing out on an accept
      self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
      self.socket.bind((self.host, self.port))
      self.socket.listen(5) # max of 5 connections at once
      
   def receive(self,connection):#receive a message from the Client
      bstring = connection.recv(128)
      buf = bstring.decode('ascii')
      if(len(buf) > 0):
         self.lock.acquire()#make the string all uppercase, lock the process
         if(buf == 'setup'):
            self.sSetup(connection)
         else:
            cID = buf.split(",")[0]
            print("Server: Client # " + str(cID) + " sent the string: " +  buf)
            self.write(buf)
            connection.send(buf.encode('ascii'))#send the Client the uppercase string
            print("Sent Client# " + str(cID) + " the string: " +  buf)
      self.lock.release()#release the lock

   def sSetup(self, connection, nfile = 'datalog.txt'):#setup The Client information
      try:
         r = open(nfile, 'r')
         r.close()
      except IOError as err:
         print("Server: Setting up datalog.txt")
         w = open(nfile, 'w')
         w.write("ID,DATE,TIME,CODE,IS_WORKING,DAY\n")
         w.close()
      dType = input("Server: Enter the device type:")
      count = 0
      for line in open(nfile, 'r'):
         count+= 1
      iD = str(count)
      setupData = iD + "," + dType
      connection.send(setupData.encode('ascii'))
      print("Server: Sent the client " + setupData)
      

   @staticmethod
   def address():#address of the Server
      return socket.gethostbyname(socket.gethostname())

   def run(self):#run the Server
      threads = []
      timeout = 0
      while True:
         if timeout > LIMIT:
            for x in threads:
               x.join()
            print("Server timed out!")
            self.close()
            break
         try:
            connection, address = self.socket.accept()
            t = threading.Thread(target=self.receive, args=(connection,))
            threads += [t]
            timeout = 0
            t.start()
         except:
            timeout += 1
            print("Server: Socket time out " +  str(timeout))

   def close(self):#close the Server
      self.socket.close()
      print("Server closed.")

   def write(self, message):#write to a file
      nfile = FileOps(message)
#      nfile.mSetup()
      f = nfile.readLog(message)
      nfile.writeLog(f[1])
      if(f[0] == False):
         nfile.appendLog(message,f[1])

class FileOps:#File operations
   def __init__(self, message, nfile = 'datalog.txt'):
      self.message = message
      self.nfile = nfile

#   def mSetup(self):#create datalog.txt if not already created
#      try:
#         r = open(self.nfile, 'r')
#         r.close()
#      except IOError as err:
#         print("Server: Setting up datalog.txt")
#         w = open(self.nfile, 'w')
#         w.write("ID,DATE,TIME,CODE,IS_WORKING,DAY\n")
#         w.close()


   def readLog(self, message):#read datalog.txt and store it as an array of strings
      r = open(self.nfile, 'r+')#note: datalog.txt must already be created
      tempstring = message.split(",")
      found = False
      count = 0
      files = [r.readline()]
      
      #ID,DEST_IP,DATE,TIME,CODE,IS_WORKING,DAY

      for line in r:
         templine = line.split(",")

         isUpdated = self.checkDay(templine[5].rstrip('\n'), tempstring[5].rstrip('\n'))
         tempstring[4] = str(isUpdated)
         templine[4] = str(isUpdated)
         if(templine[0] == tempstring[0]):
            found = True
            for ix in range(1, 5):
               templine[ix] = tempstring[ix]
            templine[5] = tempstring[5] + "\n"
         files += [(",").join(templine)]
      r.close()
      w = open(self.nfile, 'w')
      return (found, files)

   def appendLog(self, message, files):#add a new entry
      a = open(self.nfile, 'a')
      a.write(message + "\n")
      a.close()
   
   def writeLog(self, files):#update the file
      w = open(self.nfile, 'w')
      for line in files:
         w.write(line)
      w.close()

   def checkDay(self, prevDay, currentDay): #time goes from 001 to 366
      lastUpdate = int(currentDay) - int(prevDay)
#      print(str(currentDay) + ":" + str(prevDay) + "  :  " + str(lastUpdate))
      if(lastUpdate > 1):
         return "NC"
      return "CONNECTED"
