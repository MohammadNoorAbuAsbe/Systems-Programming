# -*- coding: utf-8 -*-

from PersistenceLayer.DTO.hats_DTO import hats_DTO

class _hats_DAO:
    def __init__(self, conn):
        self._conn = conn  
        pass
    
    def insert(self, hat):
        try:
            self._conn.execute("""INSERT INTO hats (id, topping, supplier, quantity) VALUES (?, ?, ?, ?);""", [hat.get_id(), hat.get_topping(), hat.get_supplier(), hat.get_quantity()])
            self._conn.commit()
        except Exception as error:
            print(error)
        pass
    
    def delete(self, _id):
        try:
            self._conn.execute(""" DELETE FROM hats where id=? ;""",[_id])
            self._conn.commit()
        except Exception as error:
            print(error)
            
    def update_amount_and_process(self, hat):
        try:
            update = """ UPDATE hats SET quantity=? where id=? ;"""
            delete = """ DELETE FROM hats WHERE id=? ;"""
            if hat.get_quantity() == 1:
                self._conn.execute(delete, [hat.get_id()])
            elif hat.get_quantity() > 1:
                self._conn.execute(update, [hat.get_quantity() - 1, hat.get_id()])           
            self._conn.commit()
        except Exception as error:
            print(error)     

      
    def get_hat_by_supplier(self):
        hat = None
        try:
            cursor = self._conn.cursor()
            vac_row = cursor.execute("""
            SELECT *
            FROM hats
            ORDER BY hats.topping ASC
            LIMIT 1;                          
            """).fetchone()
            hat = hats_DTO(vac_row[0], vac_row[1], vac_row[2], vac_row[3])
            self._conn.commit()
            cursor.close()
        except Exception as error:
            print(error)
        return hat


    def get_hat_id(self, hat):
        id = None
        return id

