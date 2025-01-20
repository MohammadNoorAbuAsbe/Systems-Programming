# -*- coding: utf-8 -*-
import sqlite3
import atexit
from PersistenceLayer.DAO.hats_DAO import _hats_DAO
from PersistenceLayer.DAO.supplier_DAO import _supplier_DAO
from PersistenceLayer.DAO.orders_DAO import _orders_DAO
from PersistenceLayer.DTO.hats_DTO import hats_DTO
from PersistenceLayer.DTO.supplier_DTO import supplier_DTO
from PersistenceLayer.DTO.orders_DTO import orders_DTO

class _Repository:    
    def __init__(self):
        self._conn = sqlite3.connect('database.db')
        self.h_DAO = _hats_DAO(self._conn)
        self.s_DAO = _supplier_DAO(self._conn)
        self.o_DAO = _orders_DAO(self._conn)
        pass
    
    def get_connection(self):
        return self._conn
    
    def _close(self):
        self._conn.commit()
        self._conn.close()
    
    def create_tables(self):
        try:
            self._conn.executescript("""        
            CREATE TABLE suppliers (
                id INTEGER PRIMARY KEY,
                name STRING NOT NULL
            );
     
            CREATE TABLE orders (
                id INTEGER PRIMARY KEY,
                location STRING NOT NULL,
                hat INTEGER NOT NULL,
                FOREIGN KEY(hat) REFERENCES hats(id)
            );
            
            CREATE TABLE hats (
                id INTEGER PRIMARY KEY,
                topping STRING NOT NULL,
                supplier INTEGER,
                quantity INTEGER NOT NULL,
                FOREIGN KEY(supplier) REFERENCES suppliers(id)
            );
            """)
            self._conn.commit()
        except Exception as error:
            print(error) 
            pass
        pass
    
    def store(self, database_records):
        indicator = database_records.pop(0)
        for i in range(len(indicator)):
            indicator[i] = int(indicator[i].split('\n')[0])
        number_of_hats = indicator[0]
        number_of_suppliers = indicator[1]
        
        while(number_of_hats > 0):
            record = database_records.pop(0)
            hat = hats_DTO(int(record[0]), record[1], int(record[2]), int(record[3].split('\n')[0]))# _topping, _supplier, _quantity
            self.h_DAO.insert(hat)
            number_of_hats-=1
        pass
        while(number_of_suppliers > 0):
            record = database_records.pop(0)
            supplier = supplier_DTO(int(record[0]), record[1].split('\n')[0])
            self.s_DAO.insert(supplier)
            number_of_suppliers-=1
        pass
    
    def place_orders(self, orders, output_file):
        num = 1
        while (len(orders) > 0):
            orderList = orders.pop(0)
            cur = self.h_DAO._conn.cursor()
            cur.execute("SELECT * FROM hats WHERE topping=? ORDER BY hats.supplier", (orderList[1].split('\n')[0],))
            row = cur.fetchone()
            hat = hats_DTO(row[0], row[1], row[2], row[3])
            order = orders_DTO(int(num), orderList[0], row[0])
            num += 1
            self.o_DAO.insert(order)
            try:
                self.h_DAO.update_amount_and_process(hat)                  
                with open(output_file, "a") as file_object:
                    output_str = str(hat.get_topping()) +','+ str(self.s_DAO.get_supplier_by_id(hat.get_supplier()).get_name()) +"," + str(orderList[0]) + "\n"
                    file_object.write(output_str)
            except Exception as error:
                print(error)
        pass

repo = _Repository()
atexit.register(repo._close)
