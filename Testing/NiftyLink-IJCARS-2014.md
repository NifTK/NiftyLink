Introduction
------------

These instructions were written to describe how to reproduce the results in
Tables 1,2,3 and 4 in the IJCARS 2014 paper.

Assumptions
-----------

For these instructions, we assume:

  - We are running on two Linux/Mac machines
  - Clocks are synchronised (eg. use ptpd)
  - NiftyLink already compiled

Note:

  - Results appear in MachineB console window
  - Test images are in NiftyLink/Testing/Data 

Table 1
-------

The aim is to time tracking data from MachineA to MachineB.

For OpenIGTLink:

    MachineB:> bin/TestServerOIGTL 18944
    MachineA:> bin/TestClientOIGTLTracking MachineBIPAddress 18944 128 16 10000
    MachineB:> bin/TestServerOIGTL 18944
    MachineA:> bin/TestClientOIGTLTracking MachineBIPAddress 18944 512 16 10000
    MachineB:> bin/TestServerOIGTL 18944
    MachineA:> bin/TestClientOIGTLTracking MachineBIPAddress 18944 1024 16 10000
    
For NiftyLink

    MachineB:> bin/TestServerQt 18944 0 0 -1 600 
    MachineA:> bin/TestClientNifTKQtTracking MachineBIPAddress 18944 128 16 10000
    MachineA:> bin/TestClientNifTKQtTracking MachineBIPAddress 18944 512 16 10000
    MachineA:> bin/TestClientNifTKQtTracking MachineBIPAddress 18944 1024 16 10000

Table 2
-------

The aim is to time imaging data from MachineA to MachineB.

For OpenIGTLink

  MachineB:> bin/TestServerOIGTL 18944
  MachineA:> bin/TestClientOIGTLImaging MachineBIPAddress 18944 32 100 DirectoryContainingNiftyLink/Testing/Data/TestImageBlank512x256is128kb.pgm
  MachineB:> bin/TestServerOIGTL 18944
  MachineA:> bin/TestClientOIGTLImaging MachineBIPAddress 18944 32 100 DirectoryContainingNiftyLink/Testing/Data/TestImageBlank512x512is256kb.pgm
  MachineB:> bin/TestServerOIGTL 18944
  MachineA:> bin/TestClientOIGTLImaging MachineBIPAddress 18944 32 100 DirectoryContainingNiftyLink/Testing/Data/TestImageBlank1024x512is512kb.pgm 
  MachineB:> bin/TestServerOIGTL 18944
  MachineA:> bin/TestClientOIGTLImaging MachineBIPAddress 18944 32 100 DirectoryContainingNiftyLink/Testing/Data/TestImageBlank2048x1024is2048kb.pgm

For NiftyLink

  MachineB:> bin/TestServerQt 18944 0 0 -1 600
  MachineA:> bin/TestClientNifTKQtImaging MachineBIPAddress 18944 32 100 DirectoryContainingNiftyLink/Testing/Data/TestImageBlank512x256is128kb.pgm
  MachineA:> bin/TestClientNifTKQtImaging MachineBIPAddress 18944 32 100 DirectoryContainingNiftyLink/Testing/Data/TestImageBlank512x512is256kb.pgm
  MachineA:> bin/TestClientNifTKQtImaging MachineBIPAddress 18944 32 100 DirectoryContainingNiftyLink/Testing/Data/TestImageBlank1024x512is512kb.pgm
  MachineA:> bin/TestClientNifTKQtImaging MachineBIPAddress 18944 32 100 DirectoryContainingNiftyLink/Testing/Data/TestImageBlank2048x1024is2048kb.pgm 

Table 3 and 4
-------------

The aim is to time image and tracking data together, from MachineA to MachineB.

For OpenIGTLink

  MachineB:> bin/TestServerOIGTL 18944 
  MachineA:> bin/TestClientOIGTLTrackingAndImaging MachineBIPAddress 18944 2 16 100 DirectoryContainingNiftyLink/Testing/Data/TestImageBlank2048x1024is2048kb.pgm
  MachineB:> bin/TestServerOIGTL 18944
  MachineA:> bin/TestClientOIGTLTrackingAndImaging MachineBIPAddress 18944 16 16 100 DirectoryContainingNiftyLink/Testing/Data/TestImageBlank512x512is256kb.pgm
  MachineB:> bin/TestServerOIGTL 18944
  MachineA:> bin/TestClientOIGTLTrackingAndImaging MachineBIPAddress 18944 32 16 100 DirectoryContainingNiftyLink/Testing/Data/TestImageBlank512x256is128kb.pgm

For NiftyLink

  MachineB:> bin/TestServerQt 18944 0 0 -1 600
  MachineA:> bin/TestClientNifTKQtTrackingAndImaging MachineBIPAddress 18944 2 16 100 DirectoryContainingNiftyLink/Testing/Data/TestImageBlank2048x1024is2048kb.pgm
  MachineA:> bin/TestClientNifTKQtTrackingAndImaging MachineBIPAddress 18944 2 16 100 DirectoryContainingNiftyLink/Testing/Data/TestImageBlank512x512is256kb.pgm
  MachineA:> bin/TestClientOIGTLTrackingAndImaging MachineBIPAddress 18944 32 16 100  DirectoryContainingNiftyLink/Testing/Data/TestImageBlank512x256is128kb.pgm 
  
