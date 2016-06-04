#include <iostream>
#include <Python.h>
#include <ratio>
#include <ctime>
#include <chrono>
using namespace std;

int main(int argc, char* argv[])
{
    Py_Initialize();   //初始化
    PySys_SetArgv(argc,argv);
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./')");
    PyObject* pModule = NULL;
    PyObject* pFunc = NULL;
    PyObject* pParam = NULL;
    PyObject* pResult = NULL;
    char* pBuffer = "";

    pModule = PyImport_ImportModule("pSigma");

    if (!pModule)
    {
	PyErr_Print();
        cout << "get module failed!" << endl;
        exit (0);
    }
    using namespace std::chrono;
    steady_clock::time_point clock_begin = steady_clock::now();
    for (int i = 1; i <= 200; ++i){
        pFunc = PyObject_GetAttrString(pModule, "player");
        if (!pFunc) {
            cout << "get func failed!" << endl;
            exit (0);
        }
        pParam = Py_BuildValue("(s)", "a.in");
        pResult = PyEval_CallObject(pFunc,pParam);
        int p1, p2;
        /*if(pResult)
        {
            if(PyArg_Parse(pResult, "(ii)", &p1, &p2))
            {
                cout << p1 << endl;
                cout << p2 << endl;
            }
        }*/
        Py_DECREF(pParam);
        Py_DECREF(pFunc);
    }
    steady_clock::time_point clock_end = steady_clock::now();
    duration<double> time_span = duration_cast<duration<double> >(clock_end - clock_begin);
    std::cout << "Evaluation INFO: Time span: "
                <<  (double)time_span.count() << "s" << std::endl;
    Py_Finalize();
    //cout << "hello" << endl;
    return 0;
}
