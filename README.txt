Hello user,

We are glad to introduce you the ISP programmer version 2.0 for Linux.


Now Linux users can compile their embedded C code using SDCC and then dump their hex file using this software,so no need to restart your system to switch for Windows based programmer.

The hardware circuit shown in the "programmer.JPG" is very simple and can be prepared in few hours.This is the minimum hardware configuration,you can add LED's etc where ever you want.


Now some important steps:

1>  To compile linux_isp_89SXX.c first open your terminal and become root user by typing 'su' and hit enter and then put password for root user.


[chinto@localhost ~]$ su
Password:



after becoming root a '#' symbol will appear at your prompt,like this


[root@localhost chinto]#


now put  linux_isp_89SXX.c file in the home folder or go to the folder where you have downloaded by 'cd' command.
You can verify the contents of the folder with 'ls' command


[root@localhost chinto]# ls
 Documents   linux_isp_89SXX.c  blink.hex  Pictures    Videos
Desktop  Download   ISP project  Music   Public   



Now compile it using cc command,before that check weather GCC is installed in your system or not,if GCC is not present then error will flash that”command not found”.


[root@localhost chinto]# cc -o 8051_LINUX_ISP linux_isp_89SXX.c 


This will create an output executable file '8051_LINUX_ISP'(you can choose any name in place of 8051_LINUX_ISP),so to dump your hex file to controller we suggest you to put your hex file also in the same folder and run as shown.


[root@localhost chinto]# ./8051_LINUX_ISP blink.hex 


If you don't want to use and ./8051_LINUX_ISP every time and want to get rid of keeping the hex file and executable file in same folder,just make your executable file as a command by copying it to /bin folder.


[root@localhost chinto]# cp 8051_LINUX_ISP /bin



2 > Regarding the hardware:

Try to give clean 5V supply,though it will work from 3.5V to 5.5V ,and also check the earthing connection of your system.In very few cases due to improper earthing the programmer will not detect the device.

Also try to use minimum wire length from parallel port connector and circuit board(use general purpose PCB's for given schematic),and if you are using wire length of more than 20cm it is advisable to use LAN wire(which is cheaper and less noisy).Otherwise you can purchase one DB25 extension cable,which will definitely work.


Use the IC bases for controller and buffer IC 74HC126,or you can use ZIF socket for controller which is bit costlier but easy to replace controller every time after programming.


Please give your suggestions and feed backs so that we can modify and make the program compatible for other controller devices also.

We thank Kevin Towers and Steven bolt of sp12 software(originally inspired by their assistance),to all blogs which helped us and ofcourse google.com(impossible to think of any project without it).

Feel free to talk to us.




Warm regards

U.Srikant Patnaik  (email :  srikant.patnaik2008@gmail.com)
	
Karsten Albrektsen (email :  kaal@privat.dk)

