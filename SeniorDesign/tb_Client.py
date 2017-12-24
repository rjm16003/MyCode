from protoClient import Client

IP_ADD = '129.110.92.15'#the ip address of the server
PORT = 8124 #the port number

def main():
   nClient0 = Client(IP_ADD, PORT)
   nClient1 = Client(IP_ADD, PORT)
   nClient2 = Client(IP_ADD, PORT)
   
   nClient0.connect()
   nClient1.connect()
   nClient2.connect()

   nClient0.address()
   nClient1.address()
   
   nClient0.send("Hello World")
   nClient1.send("Afternoon!")
   nClient2.send("Goodbye friends!")
   
   nClient0.receive()#throws an error if connection is reset/closed
   nClient1.receive()
   nClient2.receive()

   nClient0.close()
   nClient1.close()
   nClient2.close()

   nClient3 = Client(IP_ADD, PORT)
   nClient3.connect()
   nClient3.connect()

   nClient3.send("Error 404")
#   nClient3.send("Error 101")

   nClient3.receive()#doing a sendx2 only counts as 1 string
#   nClient3.receive()

   nClient3.close()


main()

