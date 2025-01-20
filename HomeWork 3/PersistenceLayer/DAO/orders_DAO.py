# -*- coding: utf-8 -*-

from PersistenceLayer.DTO.orders_DTO import orders_DTO
class _orders_DAO:
    def __init__(self, conn):
        self._conn = conn


    def insert(self, orders):
        try:
            self._conn.execute("""
                INSERT INTO orders (id, location, hat) VALUES (?, ?, ?);
            """, [orders.get_id(), orders.get_location(), orders.get_hat()])
            self._conn.commit()
        except Exception as error:
            print(error)

    
    def update_orders(self, location, topping):  # should support it with some triggers before and after about the case of ZERO
        try:
            self._conn.execute(""" 
                UPDATE orders
                WHERE location = ?   
                WHERE topping = ?
           ; """, [location, topping])
            self._conn.commit()
        except Exception as error:
            print(error)
            
    def get_order_by_location(self, location):
        order = None
        try:
            cursor = self._conn.cursor() 
            order_row = cursor.execute(""" SELECT * FROM orders WHERE location=? ;""",[location]).fetchone()
            order = orders_DTO(order_row[0], order_row[1], order_row[2], order_row[3])
            cursor.close()
        except Exception as error:
            print(error)
        return order