#include "Model.h"
#include "Singleton.h"

sqlite3* CModel::MGetDatabase()
{
	if (!bIsOpened)
	{
		db.open("IcapLamp.db");
		bIsOpened = true;
	}
	return db.getHandle();
}

int CModel::GetDeviceIDbyIP(std::string ip)
{
	Field definition_tbLampDevice[] = 
	{
		Field(FIELD_KEY),
		Field("ip", type_text),
		Field(DEFINITION_END),
	};

	RecordSet recordset(MGetDatabase(), definition_tbLampDevice);
	std::string str_sql = std::string("SELECT _ID FROM LampDevice where ip = \"") + ip + std::string("\"");
	recordset.query(str_sql);
	Record r = recordset.getRecord(0);
	return r.getValue("_ID")->asInteger();
}

int CModel::GetDeviceIDbyMAC(std::string mac)
{
	Field definition_tbLampDevice[] = 
	{
		Field(FIELD_KEY),
		Field("mac", type_text),
		Field(DEFINITION_END),
	};

	RecordSet recordset(MGetDatabase(), definition_tbLampDevice);
	std::string str_sql = std::string("SELECT _ID FROM LampDevice where mac = \"") + mac + std::string("\"");
	recordset.query(str_sql);
	if(recordset.count() == 0)
	{
		return 0;
	}

	Record r = recordset.getRecord(0);
	return r.getValue("_ID")->asInteger();
}

int CModel::GetAPIDbyIP(std::string ip)
{
	Field definition_tbAP[] = 
	{
		Field(FIELD_KEY),
		Field("ip", type_text),
		Field(DEFINITION_END),
	};

	RecordSet recordset(MGetDatabase(), definition_tbAP);
	std::string str_sql = std::string("SELECT _ID FROM AP where ip = \"") + ip + std::string("\"");
	recordset.query(str_sql);
	Record r = recordset.getRecord(0);
	return r.getValue("_ID")->asInteger();
}

//获取所有AP的ip地址
std::vector<std::string> CModel::GetAPIPList(void)
{
	std::vector<std::string> vec_ip_pool;

	Field definition_tbAP[] = 
	{
		Field(FIELD_KEY),
		Field("direction", type_text),
		Field("ip", type_text),
		Field(DEFINITION_END),
	};

	Table tbAP(MGetDatabase(), "AP", definition_tbAP);

	tbAP.open();

	for (int index = 0; index < tbAP.recordCount(); index++)
	{
		if (Record* record = tbAP.getRecord(index))
				vec_ip_pool.push_back(std::string(record->getValue("ip")->asString()));
	}
	return vec_ip_pool;
}

//获取所有设备的ip地址
std::vector<std::string> CModel::GetDeviceIPList(void)
{
	std::vector<std::string> vec_ip_pool;

	Field definition_tbDevice[] = 
	{
		Field(FIELD_KEY),
		Field("ip", type_text),
		Field(DEFINITION_END),
	};

	Table tbDevice(MGetDatabase(), "LampDevice", definition_tbDevice);

	tbDevice.open();

	for (int index = 0; index < tbDevice.recordCount(); index++)
	{
		if (Record* record = tbDevice.getRecord(index))
			vec_ip_pool.push_back(std::string(record->getValue("ip")->asString()));
	}
	return vec_ip_pool;
}