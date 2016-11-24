#!/bin/bash
#
# script to deploy library files from the local user's arduino library directory
# useful when hacking on the library.

INSTALL_DIR=~/Documents/Arduino/libraries/andys_menu;

if [ ! -e $INSTALL_DIR ]; then
	echo making dir: $INSTALL_DIR;
	mkdir $INSTALL_DIR;
fi

cp -v ./andys_menu.cpp $INSTALL_DIR
cp -v ./andys_menu.h $INSTALL_DIR
cp -v LICENSE $INSTALL_DIR
cp -v README.md $INSTALL_DIR

