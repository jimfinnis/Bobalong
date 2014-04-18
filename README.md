Bobalong
=======

This repository contains all the code that is and will be on the autonomous sailing boat Robert A Long.



## Goals

The goals of this project is to take part in Microtransat transatlantic race (http://www.microtransat.org/) with the aim of creating a autonomous sailing boat capable of sailing across the Atlantic Ocean.


## Building

To build you need to install ino tool, I recomend cloning its git repo and doing 'make install' so you have an up-to-date version.

go to the root directory and run 'ino clean' and then 'ino build' 

if you get a complaint about arguments being too long or something then you probably need to delete the wifi library directory for arduino. run 'ino build -v' and look through the output for a bunch of -I arguments quoting the offending derectory.