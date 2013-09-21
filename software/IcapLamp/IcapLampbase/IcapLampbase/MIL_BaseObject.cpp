#include <iostream>
#include <string>
#include "MIL_BaseObject.h"

using namespace std;

MIL_BaseObject::MIL_BaseObject(void)
{
}


MIL_BaseObject::~MIL_BaseObject(void)
{
}

void MIL_BaseObject::ReportEvent()
{
	cout<<"ReportEvent"<<endl;
}

void MIL_BaseObject::Serialize()
{
	cout<<"Serialize"<<endl;
}