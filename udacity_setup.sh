#!/bin/bash
# author: Blake Swadling

# run this script in your base projects folder to set up the environment
# for the Udacity C++ Route Planning project
# this script is for Ubuntu 22.04, it may work on other versions but has not been tested
# usage: ./udacity_setup.sh
set -e  # exit on error

# update and install deps
# tested on 22.04
sudo apt update && sudo apt upgrade -y
sudo apt install -y build-essential libcairo2-dev libgraphicsmagick1-dev libpng-dev cmake
sudo apt install -y libncurses5-dev libncursesw5-dev

#clone the skeleton route planner project
git clone https://github.com/udacity/CppND-System-Monitor.git --recurse-submodules

cd CppND-System-Monitor

# I want to work in my own repo, so switch origin to point to mine
git remote rename origin udacity
git remote add origin git@github.com:thecodemonkeyau/system-monitor.git
git fetch origin && git rebase origin/master

# build the sample
#mkdir -p build && cd build
#pwd
make build

# run the sample code
./build/monitor
