// TheGameEngineProject.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MainExec.h"

int main(int argc, char *argv[])
{	
	CMainExec* main = new CMainExec(1000, 600);
	main->setNextState(STATE_GAME);
	main->exec();
	delete main;

	return 0;
}

/*
string json_example = "{\"array\": \
[\"item1\", \
\"item2\"], \
\"not an array\": \
\"asdf\" \
}";

// Let's parse it
Json::Value root;
Json::Reader reader;
bool parsedSuccess = reader.parse(json_example,
root,
false);

if (!parsedSuccess)
{
// Report failures and their locations
// in the document.
cout << "Failed to parse JSON" << endl << reader.getFormattedErrorMessages() << endl;
return 1;
}

// Let's extract the array contained
// in the root object
const Json::Value array = root["array"];

// Iterate over sequence elements and
// print its values
for (unsigned int index = 0; index<array.size(); ++index)
{
cout << "Element "
<< index
<< " in array: "
<< array[index].asString()
<< endl;
}

// Lets extract the not array element
// contained in the root object and
// print its value
const Json::Value notAnArray =
root["not an array"];

if (!notAnArray.isNull())
{
cout << "Not an array: "
<< notAnArray.asString()
<< endl;
}

// If we want to print JSON is as easy as doing:
cout << "Json Example pretty print: "
<< endl << root.toStyledString()
<< endl;
system("PAUSE");

return 0;
//printf("What is this\n");
//system("PAUSE");
//return 0;
*/


