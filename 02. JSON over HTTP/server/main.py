import json

from bottle import route, run, request, response

import db_functions


@route('/', method='POST')
def index():
    response.content_type = 'application/json'
    json_response = {'action': 'unknown', 'status': 'Error'}
    if request.content_type == 'application/json':
        try:
            # todo: use logs
            # print(request.body.read().decode('utf-8'))
            json_request = json.loads(request.body.read().decode('utf-8'))
        except json.JSONDecodeError:
            pass
        else:
            if isinstance(json_request, dict):
                json_response['action'] = json_request.get('action', 'unknown')
                is_ok = False
                if json_request.get('action') == 'add_list':
                    todo_list = json_request.get('todo_list')
                    if todo_list is not None:
                        is_ok = add_list(todo_list)
                elif json_request.get('action') == 'update_list':
                    todo_list = json_request.get('todo_list')
                    if todo_list is not None:
                        is_ok = update_list(todo_list)
                elif json_request.get('action') == 'delete_list':
                    todo_list = json_request.get('todo_list')
                    if todo_list is not None:
                        is_ok = delete_list(todo_list)
                elif json_request.get('action') == 'get_lists':
                    json_response['todo_lists'] = get_lists()
                    is_ok = True

                elif json_request.get('action') == 'add_item':
                    list_item = json_request.get('list_item')
                    if list_item is not None:
                        is_ok = add_item(list_item)
                elif json_request.get('action') == 'update_item':
                    list_item = json_request.get('list_item')
                    if list_item is not None:
                        is_ok = update_item(list_item)
                elif json_request.get('action') == 'delete_item':
                    list_item = json_request.get('list_item')
                    if list_item is not None:
                        is_ok = delete_item(list_item)
                elif json_request.get('action') == 'get_items':
                    list_item = json_request.get('list_item')
                    if list_item is not None:
                        json_list_items = get_items(list_item)
                        if json_list_items is not False:
                            json_response['list_items'] = json_list_items
                            is_ok = True

                if is_ok:
                    json_response['action'] = json_request['action']
                    json_response['status'] = 'OK'

    return json.dumps(json_response)


# todo: don't call db_functions with parameters from client, but processing them
def add_list(todo_list):
    try:
        db_functions.add_list(**todo_list)
    except TypeError:
        return False
    else:
        return True


def update_list(todo_list):
    try:
        result = db_functions.update_list(**todo_list)
    except TypeError:
        return False
    else:
        return result


def delete_list(todo_list):
    try:
        result = db_functions.delete_list(**todo_list)
    except TypeError:
        return False
    else:
        return result


def get_lists():
    todo_lists = db_functions.get_all_lists()
    for i in range(len(todo_lists)):
        todo_lists[i]['created'] = todo_lists[i]['created'].isoformat()
    return todo_lists


def add_item(list_item):
    try:
        result = db_functions.add_item(**list_item)
    except TypeError:
        return False
    else:
        return result


def update_item(list_item):
    try:
        result = db_functions.update_item(**list_item)
    except TypeError:
        return False
    else:
        return result


def delete_item(list_item):
    try:
        result = db_functions.delete_item(**list_item)
    except TypeError:
        return False
    else:
        return result


def get_items(list_item):
    try:
        result = db_functions.get_list_items(**list_item)
    except TypeError:
        return False
    else:
        return result


if __name__ == '__main__':
    run(host='localhost', port=8080, debug=True)
