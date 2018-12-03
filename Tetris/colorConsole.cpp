#include "colorConsole.h"

HANDLE initiate()
{
	HANDLE hOutput; 
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	return hOutput;
}

BOOL textout(HANDLE hOutput,int x,int y,WORD wColors[],int nColors,LPTSTR lpszString)
{
    DWORD cWritten; 
    BOOL fSuccess; 
    COORD coord; 
   
	coord.X = x;              // start at first cell 
    coord.Y = y;              //   of first row      
    fSuccess = WriteConsoleOutputCharacter( 
        hOutput,              // screen buffer handle 
        lpszString,           // pointer to source string 
        lstrlen(lpszString),  // length of string 
        coord,                // first cell to write to 
        &cWritten);           // actual number written 
    if (! fSuccess) 
        cout<<"error:WriteConsoleOutputCharacter"<<endl;
    
	    
	for (;fSuccess && coord.X < lstrlen(lpszString)+x; coord.X += nColors) 
    {
        fSuccess = WriteConsoleOutputAttribute( 
            hOutput,          // screen buffer handle 
            wColors,          // pointer to source string 
            nColors,                // length of string 
            coord,            // first cell to write to 
            &cWritten);       // actual number written 
    }
    if (! fSuccess) 
    	cout<<"error:WriteConsoleOutputAttribute"<<endl;

	return 0;
}
