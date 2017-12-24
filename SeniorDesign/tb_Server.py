from protoServer import Server

def main():
   nServer = Server(8124)
   nServer.bind()
   print("Server created. Port: 8124: Listening for client requests...")
   nServer.run()

main()

