import xml.etree.ElementTree as ET
from io import BytesIO

from bottle import route, run, request, response

import db_functions


@route('/', method='POST')
def index():
    response.content_type = 'text/xml'
    xml_response = ET.Element('response')
    xml_response.set('action', 'unknown')
    xml_response.set('status', 'Error')
    if request.content_type == 'text/xml':
        try:
            # print(request.body.read())
            xml_request = ET.fromstring(request.body.read())
        except ET.ParseError:
            pass
        else:
            xml_response.set('action', xml_request.get('action', 'unknown'))
            is_ok = False
            if xml_request.get('action') == 'add_list':
                todo_list = xml_request.find('todo_list')
                if todo_list is not None:
                    is_ok = add_list(todo_list)
            elif xml_request.get('action') == 'update_list':
                todo_list = xml_request.find('todo_list')
                if todo_list is not None:
                    is_ok = update_list(todo_list)
            elif xml_request.get('action') == 'delete_list':
                todo_list = xml_request.find('todo_list')
                if todo_list is not None:
                    is_ok = delete_list(todo_list)
            elif xml_request.get('action') == 'get_lists':
                xml_response.insert(0, get_lists())
                is_ok = True

            elif xml_request.get('action') == 'add_item':
                list_item = xml_request.find('list_item')
                if list_item is not None:
                    is_ok = add_item(list_item)
            elif xml_request.get('action') == 'update_item':
                list_item = xml_request.find('list_item')
                if list_item is not None:
                    is_ok = update_item(list_item)
            elif xml_request.get('action') == 'delete_item':
                list_item = xml_request.find('list_item')
                if list_item is not None:
                    is_ok = delete_item(list_item)
            elif xml_request.get('action') == 'get_items':
                list_item = xml_request.find('list_item')
                if list_item is not None:
                    xml_list_items = get_items(list_item)
                    if xml_list_items is not None:
                        xml_response.insert(0, xml_list_items)
                        is_ok = True

            if is_ok:
                xml_response.attrib['action'] = xml_request.get('action')
                xml_response.attrib['status'] = 'OK'

    xml_response = ET.ElementTree(xml_response)
    temp_file = BytesIO()
    xml_response.write(temp_file, encoding='UTF-8', xml_declaration=True)
    return temp_file.getvalue()


def add_list(todo_list):
    kwargs = {}
    for child in todo_list:
        kwargs[child.tag] = child.text
    try:
        db_functions.add_list(**kwargs)
    except TypeError:
        return False
    else:
        return True


def update_list(todo_list):
    kwargs = {}
    for child in todo_list:
        kwargs[child.tag] = child.text
    try:
        result = db_functions.update_list(**kwargs)
    except TypeError:
        return False
    else:
        return result


def delete_list(todo_list):
    try:
        list_id = todo_list.find('list_id').text
    except AttributeError:
        return False
    else:
        return db_functions.delete_list(list_id)


def get_lists():
    todo_lists = db_functions.get_all_lists()
    xml_todo_lists = ET.Element('todo_lists')
    for todo_list in todo_lists:
        xml_todo_list = ET.SubElement(xml_todo_lists, 'todo_list')
        for field, value in todo_list.items():
            ET.SubElement(xml_todo_list, field).text = str(value) if value is not None else ''
    return xml_todo_lists


def add_item(list_item):
    kwargs = {}
    for child in list_item:
        kwargs[child.tag] = child.text
    try:
        result = db_functions.add_item(**kwargs)
    except TypeError:
        return False
    else:
        return result


def update_item(list_item):
    kwargs = {}
    for child in list_item:
        kwargs[child.tag] = child.text
    try:
        result = db_functions.update_item(**kwargs)
    except TypeError:
        return False
    else:
        return result


def delete_item(list_item):
    try:
        item_id = list_item.find('item_id').text
    except AttributeError:
        return False
    else:
        db_functions.delete_item(item_id)
        return True


def get_items(list_item):
    try:
        list_id = list_item.find('list_id').text
    except AttributeError:
        return None
    else:
        xml_list_items = ET.Element('list_items')
        list_items = db_functions.get_list_items(list_id)
        for list_item in list_items:
            xml_list_item = ET.SubElement(xml_list_items, 'list_item')
            for field, value in list_item.items():
                ET.SubElement(xml_list_item, field).text = str(value) if value is not None else ''
        return xml_list_items


if __name__ == '__main__':
    run(host='localhost', port=8080, debug=True)
