import datetime
import sqlite3


class TodoListDataBase:
    def __init__(self, db_name='todo_lists.db'):
        self.db_name = db_name

    def _create_db(self):
        """
        This function will drop existed tables!
        :return: None
        """
        conn = sqlite3.connect(self.db_name)
        with open('create_db.sql', 'r') as f:
            conn.executescript(f.read())
        conn.commit()
        conn.close()

    @staticmethod
    def _list_existed(list_id, conn):
        return conn.execute('SELECT list_id FROM todo_list WHERE list_id = ?', (list_id,)).fetchone()

    def add_list(self, header=None, author_name=None, type_=None):
        conn = sqlite3.connect(self.db_name, detect_types=sqlite3.PARSE_DECLTYPES)
        conn.execute('INSERT INTO todo_list (header, author_name, type) VALUES (?, ?, ?)',
                     (header, author_name, type_))
        conn.commit()
        conn.close()
        return True

    def update_list(self, list_id, header=None, author_name=None, type_=None):
        conn = sqlite3.connect(self.db_name)
        if not self._list_existed(list_id, conn):
            conn.close()
            return False
        if header is None and author_name is None and type_ is None:
            return False
        query = 'UPDATE todo_list SET '
        args = []
        if header is not None:
            query += 'header = ? '
            args.append(header)
        if author_name is not None:
            query += 'author_name = ? '
            args.append(author_name)
        if type_ is not None:
            query += 'type = ? '
            args.append(type_)
        query += 'WHERE list_id = ?'
        conn.execute(query, (*args, list_id))
        conn.commit()
        conn.close()
        return True

    def delete_list(self, list_id):
        conn = sqlite3.connect(self.db_name)
        conn.execute('PRAGMA FOREIGN_KEYS = ON;')
        try:
            conn.execute('DELETE FROM todo_list WHERE list_id = ?', (list_id,))
        except sqlite3.IntegrityError:
            conn.close()
            return False
        conn.commit()
        conn.close()
        return True

    def get_all_lists(self):
        conn = sqlite3.connect(self.db_name, detect_types=sqlite3.PARSE_DECLTYPES)
        conn.row_factory = sqlite3.Row
        result = conn.execute('SELECT * FROM todo_list ORDER BY list_id').fetchall()
        conn.close()
        result = [dict(row) for row in result]
        return result

    def add_item(self, list_id, body=None, number=None, status=None):
        conn = sqlite3.connect(self.db_name)
        if not self._list_existed(list_id, conn):
            conn.close()
            return False
        # todo? maybe use trigger for this
        if number is None:
            (number,) = conn.execute('SELECT MAX(number + 1) FROM list_item WHERE list_id = ?', (list_id,)).fetchone()
            if number is None:
                number = 1
        conn.execute('INSERT INTO list_item (list_id, body, number, status) VALUES (?, ?, ?, ?)',
                     (list_id, body, number, status))
        conn.commit()
        conn.close()
        return True

    def update_item(self, item_id, list_id=None, body=None, number=None, status=None):
        conn = sqlite3.connect(self.db_name)
        if list_id is not None:
            if not self._list_existed(list_id, conn):
                conn.close()
                return False
        if list_id is None and body is None and number is None and status is None:
            return False
        query = 'UPDATE list_item SET '
        args = []
        if list_id is not None:
            query += 'list_id = ? '
            args.append(list_id)
        if body is not None:
            query += 'body = ? '
            args.append(body)
        if number is not None:
            query += 'number = ? '
            args.append(number)
        if status is not None:
            query += 'status = ? '
            args.append(status)
        query += 'WHERE item_id = ?'
        conn.execute(query, (*args, item_id))
        conn.commit()
        conn.close()
        return True

    def delete_item(self, item_id):
        conn = sqlite3.connect(self.db_name)
        conn.execute('DELETE FROM list_item WHERE item_id = ?', (item_id,))
        conn.commit()
        conn.close()
        return True

    def get_list_items(self, list_id):
        conn = sqlite3.connect(self.db_name)
        conn.row_factory = sqlite3.Row
        result = conn.execute('SELECT * FROM list_item WHERE list_id = ? ORDER BY number', (list_id,)).fetchall()
        conn.close()
        result = [dict(row) for row in result]
        return result
