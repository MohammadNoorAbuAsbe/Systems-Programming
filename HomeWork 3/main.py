# -*- coding: utf-8 -*-
import sys
from PersistenceLayer.Repository import repo

def database_building(database_records_as_strings):
    database_records = list()
    for i in range(len(database_records_as_strings)):
        database_records.append(database_records_as_strings[i].split(','))
    repo.create_tables()
    repo.store(database_records)

def manage_orders(orders_as_strings, output_file):
    orders = list()
    for i in range(len(orders_as_strings)):
        orders.append(orders_as_strings[i].split(','))
    repo.place_orders(orders, output_file)

def main(config_file, orders_file, output_file, database):
    with open(config_file, "r") as cfile:
        lines = cfile.readlines()
        database_building(lines)
    with open(orders_file, "r") as ofile:
        lines = ofile.readlines()
        manage_orders(lines, str(output_file))

if __name__ == '__main__':
    main(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4])
