# KanBam
Kanban-Board in windows command prompt


## Rules
This project has the following predefined Rules
(Important to admit => not defined by me):

* Application must be an c++ project
* need to be a cmd Application
* OOP is not allowed

## Database connection
To include the sqlite header File I used the following Tutorial to install sqlite: 
[Sqlite Tutorial](https://cppcodetips.wordpress.com/2014/01/14/including-sqlite-dll-in-vc-project/)
</br>

</br>
In my Case I downloaded the x86 & x64 Windows binary File and the basic sqlite-amalgamation.
</br>
The x86 Files and amalgamation Files are saved into the following absolute Path: 
`C:\sqlite3\x86\`
</br>
The x64 Files and amalgamation Files are saved into the following absolute Path: 
`C:\sqlite3\x64\`
</br>
</br>
After I saved these Files there, I used the following line in Visual Studios Command Prompt:
x86: `lib /DEF:"C:\sqlite3\x86\sqlite3.def" /OUT:"C:\sqlite3\x86\sqlite3.lib"`
x64: `lib /DEF:"C:\sqlite3\x64\sqlite3.def" /OUT:"C:\sqlite3\x64\sqlite3.lib" /machine:x64`
</br>
Also you need to put the specific `.dll` File into the Project debug Folder of your specific Plattform System.
This should be the folder where your `.exe` File automaticially goes in after you execute your `.cpp`File. In my Case it were the following folders:
`${Project_Path}\Debug\`
`${Project_Path}\x64\Debug\`

## Nice to collect
[SQLite introduction](https://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm).