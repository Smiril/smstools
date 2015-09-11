#!/bin/bash

sudo zypper install libjansson4 libjansson4-32bit libjansson-devel curl libcurl4 libcurl4-32bit libcurl-devel at-spi2-atk-common at-spi2-atk-gtk2 glib glib-32bit gtk gtk2-branding-SLED gtk2-data gtk2-devel gtk2-immodule-amharic gtk2-immodule-inuktitut gtk2-metatheme-adwaita gtk2-tools gtk2-tools-32bit patterns-openSUSE-base patterns-openSUSE-devel_basis patterns-openSUSE-devel_C_C++ git
cd src/
sh setup.sh
cd ../
echo "Make Windows Binary?(only Opensuse 13.2 x64) Do that? [Y,n]"
read input
if [[ $input == "Y" || $input == "y" ]]; then
        echo "doing that ..."
        cd src/
        sh W32_x64_opensuse_build.sh
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
