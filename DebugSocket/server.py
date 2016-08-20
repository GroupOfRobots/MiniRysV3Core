import datetime
import tornado.httpserver
import tornado.websocket
import tornado.ioloop
import tornado.web
import socket



def send_to_all_clients(msg):
    for client in clients:
        client.write_message(message)

class WSHandler(tornado.websocket.WebSocketHandler):

    clients = []
    def open(self):
        print 'new connection'
	self.write_message("Hello World")
        WSHandler.clients.append(self)
      
    def on_message(self, message):
        print 'message received:  %s' % message
	#self.write_message('ECHO: ' + message)
 
    def on_close(self):
        print 'connection closed'
	WSHandler.clients.remove(self)
 
    def check_origin(self, origin):
        return True

    @classmethod
    def write_to_clients(cls):
        print "Writing to clients"
        for client in cls.clients:
            client.write_message("Hello")
 
application = tornado.web.Application([
    (r'/ws', WSHandler),
])
 
 
if __name__ == "__main__":
    http_server = tornado.httpserver.HTTPServer(application)
    http_server.listen(8888)
    myIP = socket.gethostbyname(socket.gethostname())
    print '*** Websocket Server Started at %s***' % myIP
    #tornado.ioloop.IOLoop.instance().add_timeout(datetime.timedelta(seconds=15), WSHandler.write_to_clients)
    tornado.ioloop.IOLoop.instance().start()
 



