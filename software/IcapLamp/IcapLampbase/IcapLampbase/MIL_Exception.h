/********************************************************************
	created:	2012/09/21
	created:	21:9:2012   13:03
	filename: 	MIL_Exception.h
	author:		张斌
	
	purpose:	封装本系统异常
*********************************************************************/
#ifndef MIL_Exception_h__
#define MIL_Exception_h__

#include <exception>
#include <boost/exception/all.hpp>

using namespace std;

#define MILEXEC_WHAT(e) *get_error_info<err_str>(e)

//定义系统所有异常错误码标识
const int MILEXCE_DB_NOTHAVE_WORKER	= 1;
const int MILEXCE_DB_NOTHAVE_LIGHT	= 2;
const int MILEXCE_DB_WORKER_REPEAT	= 3;
const int MILEXCE_DB_QUERY			= 4;

const string MILEXCE_DB_NOTHAVE_WORKER_STR	= "<err! database has no this worker>";
const string MILEXCE_DB_NOTHAVE_LIGHT_STR	= "<err! database has no this light>";
const string MILEXCE_DB_WORKER_REPEAT_STR	= "<err! database has already had this worker>";
const string MILEXCE_DB_QUERY_STR			= "<err! database query error>";

typedef boost::error_info<struct tag_err_no, int> err_no;
typedef boost::error_info<struct tag_err_str, std::string> err_str;

struct MIL_Exception :
	virtual std::exception,
	virtual boost::exception
{
public:
	MIL_Exception(void);
	~MIL_Exception(void);
};

#endif // MIL_Exception_h__