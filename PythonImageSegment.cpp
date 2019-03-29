#include "PythonImageSegment.h"

using namespace std;

//��const char *cת wchar_t * ����ΪPy_SetPythonHome()����ƥ��
wchar_t *GetWC(const char *c)
{
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc, c, cSize);

	return wc;
}

void PythonImageSegment(const char* fileSrc, const char* fileGrowing, const char* fileEdge)
{
	//char currentDir[PATH_MAX];
	char currentDir[1024];
	char* flag = _getcwd(currentDir, sizeof(currentDir) - 1);
	string pythonHome(currentDir);
	pythonHome.append("\\Python\\Python64");
	Py_SetPythonHome(GetWC(pythonHome.c_str()));
	Py_Initialize();

	//PyRun_SimpleString("print ('hello I am come from Python!')");
	PyRun_SimpleString("import sys");

	if (flag != NULL)
	{
		string m_scriptPath(currentDir);
		m_scriptPath.append("\\Python");
		//m_scriptPath.replace("\\", "/");
		PyRun_SimpleString((std::string("if not r'") + m_scriptPath
			+ "' in sys.path: sys.path.append(r'" + m_scriptPath + "')").c_str());
		printf("currentDir ==>> %s\n", m_scriptPath.c_str());
	}
	else
	{
		printf("Show currentDir error\n");
	}

	//PyRun_SimpleString("print(sys.path)");
	string filePath(fileSrc);
									   
	string file_growing(fileGrowing);
													 
	string file_edge(fileEdge);
											   
	PyObject * pModule = NULL;
	PyObject * pFunc = NULL;
	PyObject * pArg = NULL;

	pModule = PyImport_ImportModule("start");    //ȷ������import��ģ�鶼��
	if (pModule == NULL)
	{
		printf("�޷�����ģ��");
		getchar();
		return ;
	}


	pFunc = PyObject_GetAttrString(pModule, "start");
	if (pFunc == NULL)
	{
		printf("�޷����غ���");
	}
	pArg = Py_BuildValue("(sss)", filePath.c_str(), file_growing.c_str(), file_edge.c_str());
	printf("%s\n", filePath.c_str());
	PyEval_CallObject(pFunc, pArg);


	Py_Finalize();

	printf("�ɹ�������\n");
	getchar();
	return ;
}