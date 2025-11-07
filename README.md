It should be noted all code included here is built for the debian kernel:<br>
Linux beaglebone 4.14.108-ti-r119 #1 SMP PREEMPT Mon Sep 30 02:30:24 UTC 2019 armv7l GNU/Linux<br>

please enter uname -a to your terminal to ensure you are running the same kernel on the beaglebone: <br>
This code is not expected to work on newer kernels due to differences in Capes/Device tree configurations:<br>
The code is setup so that kernel initialization could be swapped in the future with greater development effort - minus some references <br>
to sysfys which may have to be altered manually. <br>


Build instructions:<br>
The current method for bulding is setup as thus:<br>
Go into each director in the libs/ directory and type "make libs"<br>
then type make inside INIT - it has dependencies on other libs, thus it must be run in this order.<br>
<br>
Then perform a make clean and a make in the AIBL directory.<br>
