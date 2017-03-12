import datetime
import os
import unittest

import db_functions


class TestDatabase(unittest.TestCase):
    def setUp(self):
        self.db_name = 'unit_test.sqlite3'
        db_functions.create_db(self.db_name)

    def initialize_list(self):
        added_data = [
            ('Header 1', 'Name 1', 0, datetime.datetime.now()),
            ('Header 2', 'Name 2', 1, datetime.datetime.now()),
            (None, 'Name 3', 1, datetime.datetime.now()),
            (None, None, None, datetime.datetime.now()),
        ]
        for added in added_data:
            db_functions.add_list(header=added[0], author_name=added[1], type=added[2], db_name=self.db_name)
        return added_data

    # def initialize_items(self):
    #     added_data = [
    #         (1, 'Body 1', ),
    #         (1, 'Body 2),
    #         (2, None),
    #         (3, 'Body 4'),
    #         (2, 'Body 5'),
    #         (2, 'Body 6'),
    #         (3, None,),
    #     ]
    #     for added in added_data:
    #         db_functions.add_list(header=added[0], author_name=added[1], type=added[2], db_name=self.db_name)
    #     return added_data

    def check_lists_equal(self, added_data, retrieved_data):
        for i in range(len(added_data)):
            retrieved_data[i] = retrieved_data[i][1:3] + (retrieved_data[i][4],) + (retrieved_data[i][3],)
        for added, retrieved in zip(added_data, retrieved_data):
            for i in range(len(added)):
                if i == 3:
                    self.assertEqual(type(added[i]), type(retrieved[i]),
                                     "added = {}\nretrieved = {}\nCreated field has type '{}', not '{}'"
                                     .format(added, retrieved, type(retrieved[i]), type(added[i])))
                    self.assertLessEqual(retrieved[i] - added[i], datetime.timedelta(minutes=2),
                                         'added = {}\nretrieved = {}\nCreated field differs by more than 2 minutes'
                                         .format(added, retrieved))
                else:
                    self.assertEqual(added[i], retrieved[i],
                                     "added = {}\nretrieved = {}\n'{}' was added but '{}' was retrieved"
                                     .format(added, retrieved, added[i], retrieved[i]))

    def test_adding_and_retrieving_lists(self):
        added_data = self.initialize_list()
        retrieved_data = db_functions.get_all_lists(self.db_name)
        self.check_lists_equal(added_data, retrieved_data)

    def test_updating_lists(self):
        added_data = self.initialize_list()
        updated_data = [
            ('Updated header 1', None, None, None),
            (None, 'Updated name 2', 0, None),
            (None, None, None, None),
            ('Updated header 4', 'Updated name 4', 1, None),
        ]
        for i, updated in enumerate(updated_data):
            db_functions.update_list(i + 1, header=updated[0], author_name=updated[1], type=updated[2],
                                     db_name=self.db_name)
        for i in range(len(updated_data)):
            updated = ()
            for j in range(len(updated_data[i])):
                updated += updated_data[i][j] if updated_data[i][j] is not None else added_data[i][j],
            updated_data[i] = updated
        retrieved_data = db_functions.get_all_lists(self.db_name)
        self.check_lists_equal(updated_data, retrieved_data)

    def test_updating_not_existed_list(self):
        self.assertFalse(db_functions.update_list(1, db_name=self.db_name))

# todo: check if any list_item references to deleting todo_list
    def test_deleting_lists(self):
        added_data = self.initialize_list()
        added_data.pop(3)
        added_data.pop(1)
        db_functions.delete_list(2, db_name=self.db_name)
        db_functions.delete_list(4, db_name=self.db_name)
        retrieved_data = db_functions.get_all_lists(self.db_name)
        self.check_lists_equal(added_data, retrieved_data)

    def tearDown(self):
        os.remove(self.db_name)
