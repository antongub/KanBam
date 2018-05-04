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
</br>
[Sqlite Tutorial](https://cppcodetips.wordpress.com/2014/01/14/including-sqlite-dll-in-vc-project/)
</br>
In my Case I downloaded the x86 & x64 Windows binary File and the basic sqlite-amalgamation.
</br>
The x86 files are saved into the following absolute Path: 
`C:\sqlite3\x86\`
</br>
The x64 files are saved into the following absolute Path: 
`C:\sqlite3\x64\`

Also you need to put the specific `.dll` File into the debug Folder of your specific Plattform System.



</br>
</br>
My sources are inspired by [this SQLite introduction](https://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm) .