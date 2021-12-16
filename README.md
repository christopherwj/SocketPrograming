# socketProgramming
The Assignment: Create three programs – two client programs and one server program. Client 1 will 
send a C struct comprising of an integer value, a character value and a float value to the server process.
 The server changes each of the value in the following way – it doubles the integer value (a 2 becomes a 
4), adds 1 to the float value (a 2.3 becomes a 3.3) and increases the character value to the next higher 
character with a roll over (i.e a ‘z’ becomes ‘a’, ‘b’ becomes a ‘c’). The server then prints the original 
values that it received and the modified values as well (label these values as “Before” and “After” values). 
The server then transmits these values to Client 2.
Client 2 prints the values that it received from the server and terminates the connection. All other 
processes are terminated as well.
Compile and run this exercise in the Unix domain.
Helpful hints: First try the above program with Client1 sending just an integer or a character to the 
server. Read through the example codes that I have posted on Canvas and the one in the Tannenbaum 
book – they are sure to help. Start early – it is not a difficult program; however it could be time-
consuming. Also, keep in mind that acquiring accounts on three different machines may be a task. If you 
have access to three different machines through your work or otherwise – please feel free to use them. 
Otherwise please reach out for computer account requests online at https://ost.sdsu.edu/
What to turn in: A soft copy of your well documented source code and a snap shot of the screen to show 
me that your program was actually running. Have two clients and a server window on the screen with 
messages which explicitly tells me that the server (for example jason.sdsu.edu) requested a connection 
with a specific client (say volta.sdsu.edu) and that the connection was established. 
Grading Criteria: (points 100)
Code works amongst 3 different machines (e.g, volta, Jason, license server) and every criterion is met: 
100 points
Code works between 2 different machines (e.g volta to Jason and Jason to volta) and every criterion is 
met: 80 points
Code works between separate ports on same machine and every criterion is met: 60points
Sophistication in code writing for example querying the DNS server for IP address of a machine instead 
of hard-coding the address will garner extra credit points as well. 
