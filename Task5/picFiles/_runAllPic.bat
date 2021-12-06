@echo Running all Small Edison (15) programs on Host.
del -f _allPicTests.txt

se f0.pic >  _allPicTests.txt
se f1.pic >> _allPicTests.txt
se f2.pic >> _allPicTests.txt
se f3.pic >> _allPicTests.txt
se f4.pic >> _allPicTests.txt
se f5.pic >> _allPicTests.txt
se f6.pic >> _allPicTests.txt
se a0.pic >> _allPicTests.txt
se a1.pic >> _allPicTests.txt
se a2.pic >> _allPicTests.txt
se a3.pic >> _allPicTests.txt
se c0.pic >> _allPicTests.txt
se c1.pic >> _allPicTests.txt
se m0.pic >> _allPicTests.txt
se r0.pic >> _allPicTests.txt
type         _allPicTests.txt
aunit.exe    _allPicTests.txt
pause
