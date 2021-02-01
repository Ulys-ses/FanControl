import sqlite3

SQLiteConnect = sqlite3.connect("Data/FanControl.db")
SQLiteConnect.row_factory = sqlite3.Row
cursFanControl = SQLiteConnect.cursor()

#cursFanControl.execute('insert into Params values("20210130-193015", ?, "")', [('[{"Name":"TimeGetParams", "Value":9},{"Name":"bbb", "Value":11}]')])
#SQLiteConnect.commit()

cursFanControl.execute('select * from  Params order by Time desc limit 3')
print(cursFanControl.rowcount)
for result in cursFanControl:
    print(result['Time'])
    print(result['Parameters'])
