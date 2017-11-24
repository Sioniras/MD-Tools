# --------------------------------------------------------------------------
# CompareContacts makefile
# --------------------------------------------------------------------------
# General Compilation Options
OBJECTS = main.o ContactList.o
CC = g++ -std=c++11		# Compiler to use
DEBUG = -g				# Add this to LFLAGS/CFLAGS to be able to debug
LFLAGS = -Wall -O2		# Linker Flags
CFLAGS = -Wall -c -O2	# Compile flags to .o
# --------------------------------------------------------------------------
# Compilation of the main program
# --------------------------------------------------------------------------
CompareContacts: $(OBJECTS)
	$(CC) $(LFLAGS) $(OBJECTS) -o compcontacts

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp -o main.o
# --------------------------------------------------------------------------
# The matrix class
ContactList.o: ContactList.cpp ContactList.h
	$(CC) $(CFLAGS) ContactList.cpp -o ContactList.o
# --------------------------------------------------------------------------
# Misc tasks
# --------------------------------------------------------------------------
clean:
	rm *.o compcontacts
