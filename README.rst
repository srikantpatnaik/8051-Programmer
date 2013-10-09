README
======

.. NOTE:: Need help to make it USB compatible. Please contact us for more details. 

This application is hosted at `sourceforge <http://sourceforge.net/projects/linux-isp-89sxx/>`_ , 
a clone is also available at `github <https://github.com/srikantpatnaik/8051-Programmer>`_.

This is the version-2.0 of the software, which runs on GNU/Linux based systems.

This is primarily written to program ISP based 8051 controllers(89SXX). The 
software decodes the hex file entered from the command line and send it to 
controller's flash memory using PC's parallel port. The hardware connections 
are very minimal.

Prefer ``SDCC`` to compile your 8051 embedded C code.

The hardware circuit shown in the ``programmer.JPG`` is very simple and can be 
prepared in few hours.This is the minimum hardware configuration, you can add 
LEDs etc where ever you want.


Running the code
----------------

1. To compile ``linux_isp_89SXX.c`` open your terminal and become root user ::


	[srikant@localhost ~]$ sudo -s


   after becoming root a ``#`` symbol will appear at your prompt, like this ::


	[root@localhost ]#

#. Now locate your directory where you download the source and compile ::
	
	[root@localhost ]# cc -o 8051_LINUX_ISP linux_isp_89SXX.c 


#. This will create an output executable file ``8051_LINUX_ISP`` 
   (you can choose any name in place of 8051_LINUX_ISP), now to dump your hex file ::


	[root@localhost ]# ./8051_LINUX_ISP blink.hex 


#. If you don't want to use and ``./8051_LINUX_ISP`` every time and want to get 
   rid of keeping the hex file and executable file in same directory, just move your 
   executable file to ``/usr/bin`` directory ::

	[root@localhost ]# mv 8051_LINUX_ISP /usr/bin



Precautions with hardware
-------------------------

1. Try to give clean 5V supply, though it will work from 3.5V to 5.5V, and 
   also check the earthing of your system. In very few cases due to improper 
   earthing the programmer will not detect the device.

#. Try to use minimum wire length from parallel port connector and circuit 
   board, and if you are using wire length of more than 20cm it is advisable 
   to use LAN wire which is cheaper and less noise prone. Otherwise you may
   purchase one DB25 extension cable, which will definitely work.

#. Use the IC bases for controller and buffer IC 74HC126, its better to use ZIF 
   socket for controller. 


Please give your suggestions and feed backs so that we can modify and make 
the program compatible for other controller devices.

We thank `Kevin Towers` and `Steven bolt` of sp12 software (originally inspired
by their assistance), to all blogs which helped us and of course the almighty `google`.

Feel free to communicate. 


Srikant Patnaik  (email :  u.srikant.patnaik@gmail.com)
	
Karsten Albrektsen (email :  kaal@privat.dk)

