g++ -c .\CSE_Core\scstream\istream.cc -o .\x64\Build_GCC\istream.o
g++ -c .\CSE_Core\scstream\ostream.cc -o .\x64\Build_GCC\ostream.o
g++ -c .\CSE_Core\datetime\Date_Time.cpp -o .\x64\Build_GCC\Date_Time.o

ar crv -o .\x64\Build_GCC\libCSE.a .\x64\Build_GCC\istream.o .\x64\Build_GCC\ostream.o .\x64\Build_GCC\Date_Time.o
pause