#! /bin/csh

# Script to produce plots automatically for all CSC segments using 
# CSCMTCCPlotter_X/Y.C in ROOT
#
# Author:  Dominique Fortin - UCR
#
# You may need to make this executable by typing:  chmod u+x plot_all.csh
#
echo '.x CSCMTCCPlotter_X.C(2,2,27)' | root.exe -b
echo '.x CSCMTCCPlotter_Y.C(2,2,27)' | root.exe -b
