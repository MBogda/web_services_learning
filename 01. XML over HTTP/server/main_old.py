import xml.etree.ElementTree as ET
from io import BytesIO

from bottle import route, run, request, response

import db_functions


# todo? check value of field 'type'
@route('/todo_list', method='POST')
def post_todo_list():
    if request.content_type != 'text/xml':
        # todo it
        return
    todo_list = ET.fromstring(request.body.read())
    kwargs = {}
    for child in todo_list:
        kwargs[child.tag] = child.text
    try:
        db_functions.add_list(**kwargs)
    except TypeError:
        # todo right status?
        response.status = '405 Method Not Allowed'
        # todo? content-type
    else:
        response.status = '201 Created'


# todo? check value of field 'type'
@route('/todo_list/<list_id:int>', method='PUT')
def put_todo_list(list_id):
    if request.content_type != 'text/xml':
        # todo it
        return
    todo_list = ET.fromstring(request.body.read())
    kwargs = {}
    for child in todo_list:
        kwargs[child.tag] = child.text
    try:
        updated = db_functions.update_list(list_id, **kwargs)
    except TypeError:
        # todo right status?
        response.status = '405 Method Not Allowed'
        # todo? content-type
    else:
        if updated:
            response.status = '204 No Content'
        else:
            response.status = '404 Not Found'


# todo? 404 Not Found
@route('/todo_list/<list_id:int>', method='DELETE')
def delete_todo_list(list_id):
    if db_functions.delete_list(list_id):
        response.status = '204 No Content'
    else:
        # todo right status?
        response.status = '403 Forbidden'
        # todo? content-type


@route('/todo_list', method='GET')
def get_todo_list():
    todo_lists = db_functions.get_all_lists()
    xml_todo_lists = ET.Element('todo_lists')
    for todo_list in todo_lists:
        xml_todo_list = ET.SubElement(xml_todo_lists, 'todo_list')
        for field, value in todo_list.items():
            ET.SubElement(xml_todo_list, field).text = str(value) if value is not None else ''
    xml_todo_lists = ET.ElementTree(xml_todo_lists)
    temp_file = BytesIO()
    xml_todo_lists.write(temp_file, encoding='UTF-8', xml_declaration=True)
    response.content_type = 'text/xml'
    return temp_file.getvalue()


if __name__ == '__main__':
    run(host='localhost', port=8080, debug=True)
