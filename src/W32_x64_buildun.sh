#!/bin/bash

sudo apt-get install mingw-w64 mingw-w64-tools mingw-w64-common mingw-w64-i686-dev mingw-w64-x86-64-dev g++-mingw-w64-i686

make W32smstools 
make W32smstools-gtk 
make W32getsms
make x64smstools 
make x64smstools-gtk 
make x64getsms
