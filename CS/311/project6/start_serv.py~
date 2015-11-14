'''
Created on Nov 25, 2012

@author: Robert Phillips III
'''
from threading import Thread
from threading import Lock
import signal
import socket
import os

class MutableNumber():
    def __init__(self):
        self.bound = 0;

    def add_bound(self, num):
        self.bound += num

    def get_bound(self):
        return self.bound
            
    def set_bound(self, num):
        self.bound = num


class Server():
    def __init__(self):
        self.connections = []
        self.offset = MutableNumber()
        self.offset.set_bound(3)

        self.perfects = []
        self.lock = Lock()

    def get_connections(self):
        return self.connections
    
    def start(self):       
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.bind(('', 1148))

        while True:
            listener = ClientListener(self.connections, self.offset, self.perfects, sock, self.lock, os.getpid())

            listener.start()
        

class ClientListener(Thread):
    def __init__(self, lst, offset, perfects, sock, lock, pid):
        Thread.__init__(self)
        
        self.conns = lst
        self.offset = offset
        self.perfects = perfects
        self.lock = lock
        self.pid = pid
        self.bound_socket = sock

        sock.listen(1)
        print 'waiting for a new connnection'

        self.conn, self.addr = sock.accept()
        print 'connection from ' + self.addr[0]
                
    def run(self):
        try:  
            cmd = self.conn.recv(64)
            if (cmd == 'report') or (cmd == 'kill'):
                self.conn.send('Active Connections\r\n')
                for connection in self.conns:
                    self.conn.send(str(connection[0][0]) + '\r\n')

                self.conn.send('Perfect Numbers\r\n')
                for p in self.perfects:
                    self.conn.send(p + '\r\n')

                if (cmd == 'kill'):
                    self.conn.send('KILL SIGNAL BEING EXECUTED\n')
                    for connection in self.conns:
                        connection[1].send('kill');

                    self.bound_socket.close()
                    os.kill(self.pid, signal.SIGKILL)
            else:
                print cmd
                self.lock.acquire()
                self.conns.append((self.addr, self.conn))

                self.conn.send(str(self.offset.get_bound()))
                bound = self.get_next_bound(int(cmd))

                self.offset.set_bound(bound)
                self.conn.send(str(bound))
                self.lock.release()

                resp = self.conn.recv(4096)

                if resp:
                    p_nums = resp.split('\r\n')

                    for num in p_nums:
                        self.perfects.append(num)

                self.conns.remove((self.addr, self.conn))
            
            self.conn.close()
        except socket.error, msg:
            print 'connection error:' + str(msg[0])
            self.offset.set_bound(-1)

    
    def get_next_bound(self, specs):
        offset = self.offset.get_bound()
        
        bound = 0
        while (bound < specs):
            bound += offset
            offset += 1

        return offset
        
if __name__ == '__main__':
    print 'Server IP:  ' + socket.gethostbyname(socket.getfqdn())
    
    serv = Server()
    serv.start()
    
