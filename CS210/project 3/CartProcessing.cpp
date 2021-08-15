#include <Python.h>
#include <iostream>
#include <Windows.h>
#include <cmath>
#include <string>

using namespace std;

const char* menuDisplay =
	"\n        Please select an option:\n"
	" 1 - Retrieve items purchased in a given day\n"
	" 2 - Retrieve number of sales per given item\n"
	" 3 - View histogram of sales for each item\n"
	" 4 - Exit program\n";
/*
Description:
	To call this function, simply pass the function name in Python that you wish to call.
Example:
	callProcedure("printsomething");
Output:
	Python will print on the screen: Hello from python!
Return:
	None
*/
void CallProcedure(string pName)
{
	char* procname = new char[pName.length() + 1];
	std::strcpy(procname, pName.c_str());

	Py_Initialize();
	PyObject* my_module = PyImport_ImportModule("PythonCode");
	PyErr_Print();
	PyObject* my_function = PyObject_GetAttrString(my_module, procname);
	PyObject* my_result = PyObject_CallObject(my_function, NULL);
	Py_Finalize();

	delete[] procname;
}

/*
Description:
	To call this function, pass the name of the Python functino you wish to call and the string parameter you want to send
Example:
	int x = callIntFunc("PrintMe","Test");
Output:
	Python will print on the screen:
		You sent me: Test
Return:
	100 is returned to the C++
*/
int callIntFunc(string proc, string param)
{
	char* procname = new char[proc.length() + 1];
	std::strcpy(procname, proc.c_str());

	char* paramval = new char[param.length() + 1];
	std::strcpy(paramval, param.c_str());


	PyObject* pName, * pModule, * pDict, * pFunc, * pValue = nullptr, * presult = nullptr;
	// Initialize the Python Interpreter
	Py_Initialize();
// Build the name object
pName = PyUnicode_FromString((char*)"PythonCode");
// Load the module object
pModule = PyImport_Import(pName);
// pDict is a borrowed reference 
pDict = PyModule_GetDict(pModule);
// pFunc is also a borrowed reference 
pFunc = PyDict_GetItemString(pDict, procname);
if (PyCallable_Check(pFunc))
{
	pValue = Py_BuildValue("(z)", paramval);
	PyErr_Print();
	presult = PyObject_CallObject(pFunc, pValue);
	PyErr_Print();
}
else
{
	PyErr_Print();
}
//printf("Result is %d\n", _PyLong_AsInt(presult));
Py_DECREF(pValue);
// Clean up
Py_DECREF(pModule);
Py_DECREF(pName);
// Finish the Python Interpreter
Py_Finalize();

// clean 
delete[] procname;
delete[] paramval;


return _PyLong_AsInt(presult);
}

/*
Description:
	To call this function, pass the name of the Python functino you wish to call and the string parameter you want to send
Example:
	int x = callIntFunc("doublevalue",5);
Return:
	25 is returned to the C++
*/
int callIntFunc(string proc, int param)
{
	char* procname = new char[proc.length() + 1];
	std::strcpy(procname, proc.c_str());

	PyObject* pName, * pModule, * pDict, * pFunc, * pValue = nullptr, * presult = nullptr;
	// Initialize the Python Interpreter
	Py_Initialize();
	// Build the name object
	pName = PyUnicode_FromString((char*)"PythonCode");
	// Load the module object
	pModule = PyImport_Import(pName);
	// pDict is a borrowed reference 
	pDict = PyModule_GetDict(pModule);
	// pFunc is also a borrowed reference 
	pFunc = PyDict_GetItemString(pDict, procname);
	if (PyCallable_Check(pFunc))
	{
		pValue = Py_BuildValue("(i)", param);
		PyErr_Print();
		presult = PyObject_CallObject(pFunc, pValue);
		PyErr_Print();
	}
	else
	{
		PyErr_Print();
	}
	//printf("Result is %d\n", _PyLong_AsInt(presult));
	Py_DECREF(pValue);
	// Clean up
	Py_DECREF(pModule);
	Py_DECREF(pName);
	// Finish the Python Interpreter
	Py_Finalize();

	// clean 
	delete[] procname;

	return _PyLong_AsInt(presult);
}

//display the menu, validate user input, and return menu selection
int getMenuSelection() {
	string tempStr = "";
	int selection = -1;
	bool validSelection = false;

	while (!validSelection) {
		cout << menuDisplay;
		try {
			getline(cin, tempStr);
			selection = stoi(tempStr);
			if (selection < 1 || selection > 4) {
				throw runtime_error("");
			}
			validSelection = true;
		}
		catch (exception) {
			cout << "\nPlease enter a number between 1 and 4\n\n";
		}
	}
	return selection;
}

// get and validate the item the user wants to know more about
string getItemFromUser() {
	string item;
	bool validItem = false;
	while (!validItem) {
		cout << "\nPlease enter an item to look up:\n";
		try {
			getline(cin, item);
			if (item.length() < 3 || item.length() > 12) {
				throw runtime_error("");
			}
			validItem = true;
		}
		catch(exception) {
			cout << "\nplease enter a valid item\n";	
		}

	}
	return item;		
}

// call python function and print number of times purchased
void printNumTimesPurchased() {
	string item = getItemFromUser();
	int numSold = callIntFunc("getNumTimesPurchased", item);
	cout << "\n" << item << " was purchased " << numSold << " time(s). \n";
}

int main()
{
	int selection = getMenuSelection();
	// main menu loop
	while (selection != 4) {

		switch (selection){
			case 1:
				cout << endl;
				CallProcedure("printProduceList");
				break;
			case 2:
				printNumTimesPurchased();
				break;
			case 3:
				CallProcedure("printHistogram");
				break;
			default:
				break;
		}
		selection = getMenuSelection();
	}
	
	return 1;
}