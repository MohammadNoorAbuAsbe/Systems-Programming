# -*- coding: utf-8 -*-


class supplier_DTO:
 
    def __init__(self, _id, _name):
        self._id = _id
        self._name = _name
        pass
    
    def get_id(self):
        return self._id
    
    def get_name(self):
        return self._name
    
    def set_id(self, new_id):
        self._id = new_id
    
    def set_name(self, new_name):
        self._name = new_name
    