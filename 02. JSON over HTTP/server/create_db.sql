PRAGMA foreign_keys = ON;
DROP TABLE IF EXISTS list_item;
DROP TABLE IF EXISTS todo_list;
CREATE TABLE todo_list (
list_id INTEGER PRIMARY KEY AUTOINCREMENT,
header TEXT,
author_name TEXT,
created TIMESTAMP,
type INTEGER -- 0 is marked list, 1 is numeric list
);
CREATE TABLE list_item (
  item_id INTEGER PRIMARY KEY AUTOINCREMENT,
  list_id INTEGER,
  body TEXT,
  number INTEGER,
  status INTEGER, -- 0 is open task, 1 is closed
  FOREIGN KEY (list_id) REFERENCES todo_list (list_id)
);
