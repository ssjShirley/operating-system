
Introduction

----------------------

A Process Manager(PMan) is a simple system which includes bg, bglist, bgkill, bgstop, bgstart, and pstat commands. Through these commands, you can run background execution of programs, manage them with bgkill, bgstop, and bgstart, get the total background jobs and get the status of process.



Requirements

-----------------------

[1]If you want to execute a program through this manager, you should compile it first. For example, if you want to run "process.c", you need to type "gcc process.c -o process" to make sure it can execute well during the PMan. What's more, you must make sure your program is in the same folder with PMan. Also, I have already have a process that was compiled called "foo.c", so you can just type bg foo when you want to execute it. However, if you want to process your own program, please copy it into the same fold and compile it.
 
[2]To use PMan, you need to type "make" to compile it first. After that, you can run it with "./Pman".

[3]When you want to use "bgkill", “bgstop”, "bgstart", or "pstat" to manager or view the status of your process, make sure you type the right pid,which can be found in the bglist. For example, if you want to kill the process 1111, you should type "bgkill 1111". (Note: the space between command and pid is necessary).

[4]As this system is an infinit loop, if you want to finish the system, you neet to type "control+c" to terminate it.  



Troubleshooting

-----------------------

If you type a unexisted process name, the bglist will also show it in the list. It is a problem that i didn't figure out.

If you terminate the process by a commond outside of PMan, the bglist won't delete this process. It is a problem that I didn't figure out.



Maintainer

------------------------

*Shu, Shengjie(Shirley) -






  
