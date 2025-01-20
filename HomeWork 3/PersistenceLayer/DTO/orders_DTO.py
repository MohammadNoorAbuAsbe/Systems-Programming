# -*- coding: utf-8 -*-


class  orders_DTO:
   
    def __init__(self, _id, _location, _hat):
        self._id = _id
        self._location = _location
        self._hat = _hat
        pass
    
    def get_id(self):
        return self._id
    
    def get_location(self):
        return self._location
    
    def get_hat(self):
        return self._hat
    
    def set_id(self, new_id):
        self._id = new_id
    
    def set_location(self, new_location):
        self._location = new_location
    
    def set_hat(self, new_hat):
        self._hat = new_hat
        