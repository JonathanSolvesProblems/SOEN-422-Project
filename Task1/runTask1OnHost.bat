cl /EHsc COutForAUnit_task1_todo.c TestAUnit_task1_todo.c /DHost /link /out:task1OnHost.exe
task1OnHost.exe > task1OnHost.txt
aunit task1OnHost.txt
pause