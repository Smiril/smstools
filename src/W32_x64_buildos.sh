#!/bin/bash

sudo zypper install mingw32-cross-gcc mingw32-cross-gcc-bootstrap mingw32-runtime

make W32smstools && make W32smstools-gtk && make W32getsms
