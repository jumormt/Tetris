#include <windows.h>
#include <iostream>
using namespace std;

HANDLE initiate();
BOOL textout(HANDLE hOutput,int x,int y,WORD wColors[],int nColors,LPTSTR lpszString);
