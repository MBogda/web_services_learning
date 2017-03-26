from xmlrpc.server import SimpleXMLRPCServer

import db_functions

with SimpleXMLRPCServer(('localhost', 8080), allow_none=True) as server:
    server.register_introspection_functions()
    server.register_function(db_functions.get_all_lists)
    server.register_function(db_functions.add_list)
    server.register_function(db_functions.update_list)
    server.register_function(db_functions.delete_list)
    server.register_function(db_functions.get_list_items)
    server.register_function(db_functions.add_item)
    server.register_function(db_functions.update_item)
    server.register_function(db_functions.delete_item)
    server.serve_forever()
