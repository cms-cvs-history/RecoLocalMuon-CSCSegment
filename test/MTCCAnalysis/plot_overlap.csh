#! /bin/csh

# Script to produce plots automatically from 
# CSCMTCCOverlapPlotter.C in ROOT
#
# Author:  Dominique Fortin - UCR
#
# You may need to make this executable by typing:  chmod u+x plot_all.csh
#
echo '.x CSCMTCCOverlapPlotter.C(1)' | root.exe -b
echo '.x CSCMTCCOverlapPlotter.C(2)' | root.exe -b
echo '.x CSCMTCCOverlapPlotter.C(3)' | root.exe -b
echo '.x CSCMTCCOverlapPlotter.C(4)' | root.exe -b

