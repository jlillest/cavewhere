#!/bin/bash

export PACKAGES="xfce4 virtualbox-guest-dkms virtualbox-guest-utils virtualbox-guest-x11"
export DPKG_INSTALL="sudo dpkg -i"
export CAVEWHERE=$(find /vagrant/ -name cavewhere*)

function install_package {
	local INSTALL="sudo apt-get -y install"
        echo "Installing $1: ($INSTALL $1)"
        $INSTALL $1 #> /dev/null 2>&1
        if [ $? -gt 0 ]; then
            sudo apt-get -f --force-yes --yes install #> /dev/null 2>&1
            $INSTALL $1 > /dev/null 2>&1
        fi
}

function install_dpkg {
	local INSTALL="sudo dpkg -i"
        echo "Installing $1: ($INSTALL $1)"
        $INSTALL $1 > /dev/null 2>&1
        if [ $? -gt 0 ]; then
            sudo apt-get -f --force-yes --yes install > /dev/null 2>&1
            $INSTALL $1 > /dev/null 2>&1
        fi
}

function install_aptpackages {
    for package in $@; do
        echo "Install $package"
        install_package $package
    done
}

echo "Update and upgrade our aptitude sources"
sudo apt-get -y update
sudo apt-get -y upgrade

echo "Install packages: $PACKAGES"
install_aptpackages $PACKAGES
echo "Install cavewhere: $CAVEWHERE"
install_dpkg $CAVEWHERE "$DPKG_INSTALL"

sudo ldconfig
