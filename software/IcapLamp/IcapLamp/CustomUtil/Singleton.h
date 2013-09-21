#include <iostream>
using namespace std;

//////////////////Singleton����
/**
* @class CSingleton
* @brief ʵ����Ѹ�����Ϊ��Ԫ, ���Ұѹ��캯����Ϊ�ǹ���
*         ��:
*          Class C
*          {
*          friend class CSingleton<C>;
*          protected:
*              C(){};
*          }
*/
template<class T>
class CSingleton
{
public:
	static T* instance()
	{
		//double check. ��ǰ�������⣬��֤Ч�ʺͶ��߳���ȷ��
		if (!m_pInstance)
		{
			//TODO: ����. CMutexGuard guard(m_lock);

			if (!m_pInstance)
			{
				//static T t;
				//m_pInstance = &t;
				m_pInstance = new T;
			}
		}
		return m_pInstance;
	};

protected:
	CSingleton(){}; //��ֹ����ʵ��
	CSingleton(const CSingleton&){}; //��ֹ����������һ��ʵ��
	CSingleton &operator =(const CSingleton&){}; //��ֹ��ֵ�������һ��ʵ��
	virtual ~CSingleton()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}
	};

private:
	static T* m_pInstance; //���Ψһʵ��
	//TODO: ʡ���˻�������Աm_lock
};

//////////////////Singletonʵ�ֲ���
template<class T> T* CSingleton<T>::m_pInstance = NULL;