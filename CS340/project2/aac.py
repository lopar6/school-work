from pymongo import MongoClient
import datetime

class Aac(object):
  """ CRUD operations for Animal collection """

  def __init__(self, username, password):
    # server connection timeout set to 500ms
    self.client = MongoClient('mongodb://%s:%s@localhost:27017/?authMechanism=DEFAULT&authSource=admin' % (username, password), serverSelectionTimeoutMS=500)
    self.database = self.client['AAC']
    self.animals = self.database['animals']
  
  def create(self, record):
    if record is not None:
      record["datetime"] = datetime.datetime.now().strftime("%m-%d-%Y, %H:%M:%S")
      result = self.animals.insert_one(record)
      return bool(result.acknowledged)
    else:
      raise Exception("No animal data provided")

  def read(self, record, limit = 50):
    cursor = self.animals.find(record, {'_id': False}, limit=limit)
    return cursor

  def update(self, record, change):
    if change is not None:
      cursor = self.animals.find_one_and_update(record, change)
      return bool(cursor)
    else:
      raise Exception("No change provided")

  def delete(self, record, multiple = False):
    if record is not None:
      result = ( self.animals.delete_many(record) if multiple
                  else self.animals.delete_one(record))
      if not result.acknowledged:
        raise Exception("Could not process delete")
      return result.deleted_count
    else:
      raise Exception("No animal data provided")
