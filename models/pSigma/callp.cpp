/*
g++ -o callp callp.cpp -I/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/include/python2.7 -I/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/include/python2.7 -fno-strict-aliasing -fno-common -dynamic -g -O2 -DNDEBUG -g -fwrapv -O3 -Wall -Wstrict-prototypes -L/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/lib/python2.7/config -lpython2.7 -ldl -framework CoreFoundation
*/
#include <Python.h>
#include <iostream>
#include <string>
using std::cin;
using std::cout;
using std::endl;
using std::string;

int main(){

	Py_Initialize(); //初始化 python
	if (!Py_IsInitialized()){
		cout << "initialized error" << endl;
		return -1;
	}
	//PyRun_SimpleString("import sys");
	//PyRun_SimpleString("sys.path.append('./')");
	PyObject *pName(0), *pModule(0), *pDct(0), *pFunc(0), *pArgs(0);

	pName = PyString_FromString("pSigma"); //载入名为 pytest的脚本
	pModule = PyImport_Import(pName);

	if (!pModule){
		cout << "can not find pSigma.py" << endl;
		return -1;
	}
	else
		cout << "open Module" << endl;

	pDct = PyModule_GetDict(pModule);

	if (!pDct)
	{
		cout << "pDct error" << endl;
		return -1;
	}

	pFunc = 0;
	pFunc = PyDict_GetItemString(pDct, "player"); //找到名为 add 的函数
	if (!pFunc || !PyCallable_Check(pFunc)){
		cout << "pFunc error" << endl;
		return -1;
	}

	pArgs = PyTuple_New(1); //为传入形参开辟空间

	//放置传入的形参，类型说明：
	//s 字符串 , 均是C 风格的字符串
	//i 整型
	//f 浮点数
	//o 表示一个 python 对象
	PyTuple_SetItem(pArgs, 0, Py_BuildValue("s", "a.in"));

	PyObject_CallObject(pFunc, pArgs);
	Py_Finalize(); 
}