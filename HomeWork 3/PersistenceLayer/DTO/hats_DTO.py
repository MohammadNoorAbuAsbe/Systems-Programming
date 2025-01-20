# -*- coding: utf-8 -*-

class hats_DTO:
    
    def __init__(self, _id, _topping, _supplier, _quantity):
        self._id = _id
        self._topping = _topping
        self._supplier = _supplier
        self._quantity = _quantity
        pass
    
    def get_id(self):
        return self._id
    
    def get_topping(self):
        return self._topping
    
    def get_supplier(self):
        return self._supplier
    
    def get_quantity(self):
        return self._quantity
    
    def set_id(self, new_id):
        self._id = new_id
    
    def set_topping(self, new_topping):
        self._topping = new_topping
    
    def set_supplier(self, new_supplier):
        self._supplier = new_supplier
    
    def set_quantity(self, new_quantity):
        self._quantity = new_quantity