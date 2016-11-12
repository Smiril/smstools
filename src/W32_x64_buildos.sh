#!/bin/bash

sudo zypper install mingw32-cross-gcc-c++ mingw32-cross-gcc-bootstrap mingw32-runtime

make W32smstools
make W32smstools-gtk 
make W32getsms
make x64smstools 
make x64smstools-gtk 
make x64getsms
