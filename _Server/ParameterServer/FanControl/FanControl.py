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

	# Подключение обработчиков запросов
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

		flaskApplication.add_url_rule(self.strRootURL + '/Probes/<strCode>/<timeBegin>/<timeEnd>/', 'fcProbes_Get',    self.Probes_Get,    methods=['GET'])
		flaskApplication.add_url_rule(self.strRootURL + '/Probes/', 'fcProbe_Add',     self.Probe_Add,     methods=['POST'])
		flaskApplication.add_url_rule(self.strRootURL + '/Probes/<strCode>/<timeEnd>/', 'fcProbes_Delete', self.Probes_Delete, methods=['DELETE'])

	# --- Функции получения статических страниц

	def GetARMPage(self):
		return self.GetStaticFile("ARM.htm")

	def GetStaticFile(self, FileName):
		f = open(self.strRootPath + "/Static/" + FileName , 'r')
		return f.read()

	# --- Функции обработки запросов к API

	def ParameterSetList_Get(self):
		# Читаем список из базы
		(SQLiteConnect, cursFanControl) = self._DataBaseConnect(True)
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
		Now = FanControl._GetNow()
        # Сохраниям
		(SQLiteConnect, cursFanControl) = self._DataBaseConnect()
		cursFanControl.execute('insert into Params (Time, Parameters, Comment) values (?, ?, "")', (Now, Parameters))
		SQLiteConnect.commit()
		return "OK"

	def ParameterSet_Get(self, strUpdateTime):
		(SQLiteConnect, cursFanControl) = self._DataBaseConnect(True)
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
		Now = FanControl._GetNow()
        # Сохраниям
		(SQLiteConnect, cursFanControl) = self._DataBaseConnect()
		if strUpdateTime == FanControl.strCurrentTimeName:
			cursFanControl.execute('update Params set Time=?, Parameters = ? where Time in (select max(Time) from Params)', (Now, Parameters))
			if cursFanControl.rowcount < 1:
				cursFanControl.execute('insert into Params (Time, Parameters, Comment) values (?, ?, "")', (Now, Parameters))
		else:
			cursFanControl.execute('update Params set Time=?, Parameters = ? where Time = ?', (Now, Parameters, strUpdateTime))
		SQLiteConnect.commit()
		return "OK"

	def ParameterSet_Delete(self, strDeleteTime):
		# Удаляем
		(SQLiteConnect, cursFanControl) = self._DataBaseConnect()
		cursFanControl.execute('delete from Params where Time = ?', (strDeleteTime, ))
		SQLiteConnect.commit()
		return "OK"

	def ParameterSetComment_Get(self, strUpdateTime):
		# Читаем параметры
		(SQLiteConnect, cursFanControl) = self._DataBaseConnect(True)
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
        # Сохраниям
		(SQLiteConnect, cursFanControl) = self._DataBaseConnect()
		if strUpdateTime == FanControl.strCurrentTimeName:
			cursFanControl.execute('update Params set Comment = ? where Time in (select max(Time) from Params)', (Parameters, ))
		else:
			cursFanControl.execute('update Params set Comment=? where Time = ?', (Parameters, strUpdateTime))
		SQLiteConnect.commit()
		return "OK"

	def Probes_Get(self, strCode, timeBegin, timeEnd):
		# Читаем список из базы
		(SQLiteConnect, cursFanControl) = self._DataBaseConnect(True)
		cursFanControl.execute('select Time, Code, Value from  ProbeValues where Time >= ? and Time <= ? and Code like ? order by Time desc', (timeBegin, timeEnd, strCode))
		# Формируем json
		listResult = []
		for rowProbeInfo in cursFanControl:
			ParamSetInfo = {"Date": rowProbeInfo["Time"], "Code":rowProbeInfo["Code"], "Value":rowProbeInfo["Value"]}
			listResult.append(ParamSetInfo)
		return json.dumps(listResult, ensure_ascii=False, indent=1)

	def Probe_Add(self):
		# Читаем параметры
		strCode   = request.json["Code"]
		strValue  = request.json["Value"]

		Now = FanControl._GetNow()

        # Сохраниям
		(SQLiteConnect, cursFanControl) = self._DataBaseConnect()
		cursFanControl.execute('insert into ProbeValues (Code, Time, Value) values (?, ?, ?)', (strCode, Now, strValue))
		SQLiteConnect.commit()
		return "OK"

	def Probes_Delete(self, strCode, timeEnd):
        # Удаляем
		(SQLiteConnect, cursFanControl) = self._DataBaseConnect()
		cursFanControl.execute('delete from ProbeValues where Time <= ? and Code like ?', (timeEnd, strCode))
		SQLiteConnect.commit()
		return "OK"

	# --- Служебные функции

	# Подключение к БД
	def _DataBaseConnect(self, bUseRow=False):
		SQLiteConnect = sqlite3.connect(self.strDataBase)
		if bUseRow:
			SQLiteConnect.row_factory = sqlite3.Row
		cursFanControl = SQLiteConnect.cursor()
		return (SQLiteConnect, cursFanControl)

	def _FormatDate(dtDate):
		return dtDate.strftime("%Y%m%d-%H%M%S")

	def _GetNow():
		return FanControl._FormatDate(datetime.datetime.today())
