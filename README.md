# Dppr
Trying out fun WinAPI stuff
- Do not use this on anyone or on yourself
- This software likely won't do anything since it cannot bypass PatchGuard nor the Driver Signature requirement

Currently the program creates a driver that can hide processes

# How?
Well, since Windows's kernel drivers have all the access in the world, they can also access a special and beautiful structure called EPROCESS
The EPROCESS structure has a field pointing to a linked list, which connects all the current running processes on your computer.
Who cares? The Task Manager for example, does! If we can take out our caller process's EPROCESS from the task manager we can make it invisible, and no one can detect it!
Except, PatchGuard.. :( 
But that is okay, with some exception hooking I mean..
