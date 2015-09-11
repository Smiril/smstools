#!/bin/bash

sudo apt-get install build-essential libgtk2.0-0 libglib2.0-0 libgdk-pixbuf2.0-0 libcairo2 libc6 git
cd src/
sh setup.sh
cd ../
echo "Make Windows Binary?(only Unbuntu x64) Do that? [Y,n]"
read input
if [[ $input == "Y" || $input == "y" ]]; then
        echo "doing that ..."
        cd src/
        sh W32_x64_buildun.sh
        cd ../
else
        echo "don't doing that ..."
fi
echo "Make Install Linux Binary? Do that? [Y,n]"
read input
if [[ $input == "Y" || $input == "y" ]]; then
        echo "doing that ..."
        sudo make -C src/ install
else
        echo "don't doing that ..."
fi

