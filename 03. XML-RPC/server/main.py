from xmlrpc.server import SimpleXMLRPCServer

from db_functions import TodoListDataBase

with SimpleXMLRPCServer(('localhost', 8080), allow_none=True) as server:
    server.register_introspection_functions()
    server.register_instance(TodoListDataBase())
    server.serve_forever()
