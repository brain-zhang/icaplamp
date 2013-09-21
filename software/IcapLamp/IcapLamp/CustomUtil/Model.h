#include "../stdafx.h"
#include "../easySQLite/SqlCommon.h"
#include "../easySQLite/SqlField.h"
#include "../easySQLite/SqlDatabase.h"
#include "../easySQLite/SqlTable.h"

using namespace sql;

template<class T> class CSingleton;

class CModel
{

friend class CSingleton<CModel>;

public:
	sqlite3* MGetDatabase();
	int GetDeviceIDbyIP(std::string ip);
	int CModel::GetDeviceIDbyMAC(std::string mac);
	int GetAPIDbyIP(std::string ip);
	std::vector<std::string> GetAPIPList(void);
	std::vector<std::string> GetDeviceIPList(void);

private:
	CModel(void):bIsOpened(false){};

private:
	sql::Database db;
	bool bIsOpened;

};
