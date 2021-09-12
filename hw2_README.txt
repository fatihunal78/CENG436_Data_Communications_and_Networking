Sir, my code is in these, bach, it works without any problems.

I wrote a threaded server, so more than one client can connect. The debugging option is enabled.

When you give the think <time> command, it finds the time difference between it and the first time command before it by adding <time>. That is, it waits first and then responds. If you do not give an argument to think time, it will give an error message.

In the next diff command, the server waits for the specified time to respond, but when you enter the diff command again, it does not wait, so you need to give the think command again if you want it to wait for the specified time again.

It can receive a random port between 0 and 8000 and it states this port as a comment.

Using the difftime function in the diff command, it finds the time in seconds in bole bole minute hour and second

The client and server read and write only once in each command. While the loop is active on the client and server until the quit command, I ensure continuous communication between the two.

Working examples of client and server:
>client -s localhost -p 3561 -d
>server -p 3561 -d