import datetime
import sqlite3

default_db_name = 'todo_lists.db'


def create_db(db_name=default_db_name):
    """
    This function will drop existed tables!
    :param db_name: name of data base file
    :return: None
    """
    conn = sqlite3.connect(db_name)
    with open('create_db.sql', 'r') as f:
        conn.executescript(f.read())
    conn.commit()
    conn.close()


def list_existed(list_id, conn):
    return conn.execute('SELECT list_id FROM todo_list WHERE list_id = ?', (list_id,)).fetchone()


def add_list(header=None, author_name=None, type=0, db_name=default_db_name):
    conn = sqlite3.connect(db_name, detect_types=sqlite3.PARSE_DECLTYPES)
    created = datetime.datetime.now()
    conn.execute('INSERT INTO todo_list (header, author_name, created, type) VALUES (?, ?, ?, ?)',
                 (header, author_name, created, type))
    conn.commit()
    conn.close()


def update_list(list_id, header=None, author_name=None, type=None, db_name=default_db_name):
    conn = sqlite3.connect(db_name)
    if not list_existed(list_id, conn):
        conn.close()
        return False
    current_values = conn.execute('SELECT header, author_name, type FROM todo_list WHERE list_id = ?', (list_id,))\
        .fetchone()
    new_values = [header, author_name, type]
    for i in range(len(new_values)):
        new_values[i] = new_values[i] if new_values[i] is not None else current_values[i]
    conn.execute('UPDATE todo_list SET header = ?, author_name = ?, type = ? WHERE list_id = ?',
                 (*new_values, list_id))
    conn.commit()
    conn.close()
    return True


def get_all_lists(db_name=default_db_name):
    conn = sqlite3.connect(db_name, detect_types=sqlite3.PARSE_DECLTYPES)
    conn.row_factory = sqlite3.Row
    result = conn.execute('SELECT * FROM todo_list ORDER BY list_id').fetchall()
    conn.close()
    result = [dict(row) for row in result]
    return result


def delete_list(list_id, db_name=default_db_name):
    conn = sqlite3.connect(db_name)
    conn.execute('PRAGMA FOREIGN_KEYS = ON;')
    try:
        conn.execute('DELETE FROM todo_list WHERE list_id = ?', (list_id,))
    except sqlite3.IntegrityError:
        conn.close()
        return False
    conn.commit()
    conn.close()
    return True


def add_item(list_id, body=None, number=None, status=0, db_name=default_db_name):
    conn = sqlite3.connect(db_name)
    if not list_existed(list_id, conn):
        conn.close()
        return False
    if number is None:
        (number,) = conn.execute('SELECT MAX(number + 1) FROM list_item WHERE list_id = ?', (list_id,)).fetchone()
        if number is None:
            number = 1
    conn.execute('INSERT INTO list_item (list_id, body, number, status) VALUES (?, ?, ?, ?)',
                 (list_id, body, number, status))
    conn.commit()
    conn.close()
    return True


def update_item(item_id, list_id=None, body=None, number=None, status=None, db_name=default_db_name):
    conn = sqlite3.connect(db_name)
    if list_id is not None:
        if not list_existed(list_id, conn):
            conn.close()
            return False
    current_values = conn.execute('SELECT list_id, body, number, status FROM list_item WHERE item_id = ?',
                                  (item_id,)).fetchone()
    new_values = [list_id, body, number, status]
    for i in range(len(new_values)):
        new_values[i] = new_values[i] if new_values[i] is not None else current_values[i]
    conn.execute('UPDATE list_item SET list_id = ?, body = ?, number = ?, status = ? WHERE item_id = ?',
                 (*new_values, item_id))
    conn.commit()
    conn.close()
    return True


def get_list_items(list_id, db_name=default_db_name):
    conn = sqlite3.connect(db_name)
    conn.row_factory = sqlite3.Row
    result = conn.execute('SELECT * FROM list_item WHERE list_id = ? ORDER BY number', (list_id,)).fetchall()
    conn.close()
    result = [dict(row) for row in result]
    return result


def delete_item(item_id, db_name=default_db_name):
    conn = sqlite3.connect(db_name)
    conn.execute('DELETE FROM list_item WHERE item_id = ?', (item_id,))
    conn.commit()
    conn.close()
    return True
