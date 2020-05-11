
Introduction
----------------------

A Airline Check-in System(ACS) is a simple system which includes 2 queues and 4 clerks. One queue(Queue = 1) for business class and the other (Queue = 0) for economy class. We assume that the customers in each queue are served FIFO, and the customers in the business class have a higher priority than the customers in the economy class. In other words, when a clerk is available, the clerk picks a customer in the business class, if any, to serve, and picks  a customer in the economy class to serve only if the business class queue is empty. When a clerk is available and there is no customer in any queue, the clerk remains idle. We assume that the service time for a customer is known when the customer enters the system. We assume that initially all queues are empty, and all clerks are idle. We assume customers are given before hand.



Requirements
-----------------------

[1] You need an input file which includes customers' information. The first line contains the total number of customers that will be simulated. After that, each line contains the information about a single customer(Id:CLASS number,arrive time,service time). A sample input file called "customers.txt" as follows:(included in the p2 folder)

7
1:0,2,60
2:0,4,70
3:0,5,50
4:1,7,30
5:1,7,40
6:1,8,50
7:0,10,30


 
[2] To use ACS, you need to type "make" to compile it first. After that, you can run it with "./ACS customers.txt".

[3] The system will output sentence when a customer arrives(custimer id), a customer enters a queue(queue id, queue length), a clerk start serving(start time - in seconds, customer id, clerk id), and a clerk end serving(end time - in seconds, customer id, clerk id).

[4] After clerks finish serving all customers, the average waiting time(in seconds) of all customers, business-class customers, economy-class customers will be output. Then, the system will automatically finish.



Troubleshooting
-----------------------

As the start time and end time is relative machine time,calculated by the machine time when the output event occurs minus the machine time, the output of the times may not exactly matches but it is really close to the results.  



Maintainer
------------------------

*Shu, Shengjie(Shirley) 

