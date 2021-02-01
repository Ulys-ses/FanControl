import sqlite3

SQLiteConnect = sqlite3.connect("Data/FanControl.db")
cursFanControl = SQLiteConnect.cursor()
 
# Создание таблиц
# Параметры
cursFanControl.execute("""CREATE TABLE Params  -- Параметры
                   (
                   Time       text,    -- Дата-время сохранения параметровYYYYMMDD-HHMMSS
                   Parameters text,    -- Значение параметров в json
                   Comment    text     -- Комментарий в набору
                   )""")
# Датчики
cursFanControl.execute("""CREATE TABLE ProbeValues  -- Показания датчиков
                   (
                   Code  text,        -- Код датчика
                   Time  text,        -- Дата-время YYYYMMDD-HHMMSS
                   Value integer      -- Значение показаний датчика
                   )""")
