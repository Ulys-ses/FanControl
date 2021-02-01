from flask import Flask
from flask import request
from flask import make_response
import sqlite3
import datetime
import json

class FanControl:
	strCurrentTimeName = "Current"

	def __init__(self, strRootURL, strRootPath):
		self.strRootURL = strRootURL
		self.strRootPath = strRootPath
		self.strDataBase = strRootPath + "/Data/FanControl.db"

	def Route(self, flaskApplication):
		flaskApplication.add_url_rule(self.strRootURL, 'FanControl', self.GetARMPage)
		flaskApplication.add_url_rule(self.strRootURL + "/<FileName>", 'FanControlFiles', self.GetStaticFile,    methods=['GET'])
		flaskApplication.add_url_rule(self.strRootURL + '/Parameters/', 'fcParameterSetList_Get',  self.ParameterSetList_Get,    methods=['GET'])
		flaskApplication.add_url_rule(self.strRootURL + '/Parameters/', 'fcParameterSet_Add',      self.ParameterSet_Add,        methods=['POST'])
		flaskApplication.add_url_rule(self.strRootURL + '/Parameters/<strUpdateTime>/', 'fcParameterSet_Get',     self.ParameterSet_Get,        methods=['GET'])
		flaskApplication.add_url_rule(self.strRootURL + '/Parameters/<strUpdateTime>/', 'fcParameterSet_Update',  self.ParameterSet_Update,     methods=['PUT'])
		flaskApplication.add_url_rule(self.strRootURL + '/Parameters/<strDeleteTime>/', 'fcParameterSet_Delete',  self.ParameterSet_Delete,     methods=['DELETE'])
		flaskApplication.add_url_rule(self.strRootURL + '/Parameters/<strUpdateTime>/Comment', 'fcParameterSetComment_Get',  self.ParameterSetComment_Get,     methods=['GET'])
		flaskApplication.add_url_rule(self.strRootURL + '/Parameters/<strUpdateTime>/Comment', 'fcParameterSetComment_Set',  self.ParameterSetComment_Set,     methods=['PUT'])

		flaskApplication.add_url_rule(self.strRootURL + '/Probes/', 'fcProbes_Get',    self.Probes_Get,    methods=['GET'])
		flaskApplication.add_url_rule(self.strRootURL + '/Probes/', 'fcProbe_Set',     self.Probe_Set,     methods=['POST'])
		flaskApplication.add_url_rule(self.strRootURL + '/Probes/', 'fcProbes_Delete', self.Probes_Delete, methods=['DELETE'])

	def GetARMPage(self):
		return self.GetStaticFile("ARM.htm")

	def GetStaticFile(self, FileName):
		f = open(self.strRootPath + "/Static/" + FileName , 'r')
		return f.read()

	def ParameterSetList_Get(self):
		# Читаем список из базы
		SQLiteConnect = sqlite3.connect(self.strDataBase)
		SQLiteConnect.row_factory = sqlite3.Row
		cursFanControl = SQLiteConnect.cursor()
		cursFanControl.execute('select Time, Comment from  Params order by Time desc')
		# Формируем json
		listResult = []
		for rowParamSet in cursFanControl:
			ParamSetInfo = {"Date": rowParamSet["Time"], "Comment":rowParamSet["Comment"]}
			listResult.append(ParamSetInfo)
		return json.dumps(listResult, ensure_ascii=False, indent=1)

	def ParameterSet_Add(self):
		# Готовим значения для записи
		Parameters = request.get_data(as_text = True)
		Now = datetime.datetime.today().strftime("%Y%m%d-%H%M%S")
		# Открываем базу
		SQLiteConnect = sqlite3.connect(self.strDataBase)
		cursFanControl = SQLiteConnect.cursor()
        # Сохраниям
		cursFanControl.execute('insert into Params values (?, ?, "")', (Now, Parameters))
		SQLiteConnect.commit()
		return "OK"

	def ParameterSet_Get(self, strUpdateTime):
		# Открываем базу
		SQLiteConnect = sqlite3.connect(self.strDataBase)
		SQLiteConnect.row_factory = sqlite3.Row
		cursFanControl = SQLiteConnect.cursor()
		# Читаем параметры
		if strUpdateTime == FanControl.strCurrentTimeName:
			cursFanControl.execute('select Parameters from  Params order by Time desc limit 1')
		else:
			cursFanControl.execute('select Parameters from  Params where Time = ?', (strUpdateTime, ))
		# Формируем ответ
		Row = cursFanControl.fetchone()
		if (Row == None):
			return "[]"
		return Row['Parameters']

	def ParameterSet_Update(self, strUpdateTime):
		# Готовим значения для записи
		Parameters = request.get_data(as_text = True)
		Now = datetime.datetime.today().strftime("%Y%m%d-%H%M%S")
		# Открываем базу
		SQLiteConnect = sqlite3.connect(self.strDataBase)
		cursFanControl = SQLiteConnect.cursor()
        # Сохраниям
		if strUpdateTime == FanControl.strCurrentTimeName:
			cursFanControl.execute('update Params set Time=?, Parameters = ? where Time in (select max(Time) from Params)', (Now, Parameters))
			if cursFanControl.rowcount < 1:
				cursFanControl.execute('insert into Params values (?, ?, "")', (Now, Parameters))
		else:
			cursFanControl.execute('update Params set Time=?, Parameters = ? where Time = ?', (Now, Parameters, strUpdateTime))
		SQLiteConnect.commit()
		return "OK"

	def ParameterSet_Delete(self, strDeleteTime):
		# Открываем базу
		SQLiteConnect = sqlite3.connect(self.strDataBase)
		cursFanControl = SQLiteConnect.cursor()
		# Удаляем
		cursFanControl.execute('delete from Params where Time = ?', (strDeleteTime, ))
		SQLiteConnect.commit()
		return "OK"

	def ParameterSetComment_Get(self, strUpdateTime):
		# Открываем базу
		SQLiteConnect = sqlite3.connect(self.strDataBase)
		SQLiteConnect.row_factory = sqlite3.Row
		cursFanControl = SQLiteConnect.cursor()
		# Читаем параметры
		if strUpdateTime == FanControl.strCurrentTimeName:
			cursFanControl.execute('select Comment from  Params order by Time desc limit 1')
		else:
			cursFanControl.execute('select Comment from  Params where Time = ?', (strUpdateTime, ))
		# Формируем ответ
		Row = cursFanControl.fetchone()
		Comment = ""
		if (Row != None):
			Comment = Row['Comment']
		jsonResult = {"Comment": Comment}
		return json.dumps(jsonResult, ensure_ascii=False, indent=1)

	def ParameterSetComment_Set(self, strUpdateTime):
		# Готовим комментарий для записи
		Parameters = request.json["Comment"]
		# Открываем базу
		SQLiteConnect = sqlite3.connect(self.strDataBase)
		cursFanControl = SQLiteConnect.cursor()
        # Сохраниям
		if strUpdateTime == FanControl.strCurrentTimeName:
			cursFanControl.execute('update Params set Comment = ? where Time in (select max(Time) from Params)', (Parameters, ))
		else:
			cursFanControl.execute('update Params set Comment=? where Time = ?', (Parameters, strUpdateTime))
		SQLiteConnect.commit()
		return "OK"

	def Probes_Get(self):
		return "Hello, Probes!"

	def Probe_Set(self):
		return "Hello, ProbeSet!"

	def Probes_Delete(self):
		return "Hello, ProbesDelete!"
