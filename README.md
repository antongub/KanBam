# KanBam
This application was a school Project and defines a Kanban-Board which is printed in the windows command prompt.


## Rules
Because it was a school Project, our teacher defined some Rules:

* Application must be an c++ project
* needs to be a cmd Application
* OOP is not allowed  
  (which was because of the database connection not completely possible)
* Programm and Code needs to be in German

## Database connection
To include the sqlite header File I used the following Tutorial to install sqlite:
[Sqlite Tutorial](https://cppcodetips.wordpress.com/2014/01/14/including-sqlite-dll-in-vc-project/)

In my Case I downloaded the x86 & x64 Windows binary File and the basic sqlite-amalgamation.
>The x86 Files and amalgamation Files are saved into the following absolute Path:
`C:\sqlite3\x86\`  
>The x64 Files and amalgamation Files are saved into the following absolute Path:
`C:\sqlite3\x64\`


After I saved these Files there, I used the following line in Visual Studios Command Prompt:  
>x86: `lib /DEF:"C:\sqlite3\x86\sqlite3.def" /OUT:"C:\sqlite3\x86\sqlite3.lib"`  
>x64: `lib /DEF:"C:\sqlite3\x64\sqlite3.def" /OUT:"C:\sqlite3\x64\sqlite3.lib" /machine:x64`

Also you need to put the specific `.dll` File into the Project debug Folder of your specific Plattform System.  
This should be the folder where your `.exe` File automaticially goes in after you execute your `.cpp` File.   
In my Case it were the following folders:  
> `${Project_Path}\Debug\`  
> `${Project_Path}\x64\Debug\`

## Links to admit
[SQLite introduction](https://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm).