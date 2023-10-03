CC = gcc
CFLAGS = -ansi -Wall -pedantic -g

assembler: assembler.o pre.o pre_help.o first_pass.o linked_list.o second_pass.o help_second_pass.o general_functions.o general_errors.o syntax_errors.o command_errors.o create_ent_ext.o create_extern_data_lists.o
	$(CC) $(CFLAGS) -o assembler assembler.o pre.o pre_help.o first_pass.o linked_list.o second_pass.o help_second_pass.o general_functions.o general_errors.o syntax_errors.o command_errors.o create_ent_ext.o create_extern_data_lists.o

assembler.o: assembler.c pre.h first_pass.h second_pass.h 
	$(CC) $(CFLAGS) -c assembler.c -o assembler.o

pre.o: pre.c pre_help.h
	$(CC) $(CFLAGS) -c pre.c -o pre.o

pre_help.o: pre_help.c general_functions.h pre_help.h
	$(CC) $(CFLAGS) -c pre_help.c -o pre_help.o

first_pass.o: first_pass.c general_functions.h command_errors.h syntax_errors.h create_extern_data_lists.h create_ent_ext.h
	$(CC) $(CFLAGS) -c first_pass.c -o first_pass.o

linked_list.o: linked_list.c linked_list.h
	$(CC) $(CFLAGS) -c linked_list.c -o linked_list.o

second_pass.o: second_pass.c general_functions.h help_second_pass.h
	$(CC) $(CFLAGS) -c second_pass.c -o second_pass.o

help_second_pass.o: help_second_pass.c general_functions.h help_second_pass.h
	$(CC) $(CFLAGS) -c help_second_pass.c -o help_second_pass.o

general_functions.o: general_functions.c
	$(CC) $(CFLAGS) -c general_functions.c -o general_functions.o

general_errors.o: general_errors.c general_errors.h
	$(CC) $(CFLAGS) -c general_errors.c -o general_errors.o

syntax_errors.o: syntax_errors.c syntax_errors.h general_functions.h command_errors.h general_errors.h
	$(CC) $(CFLAGS) -c syntax_errors.c -o syntax_errors.o

command_errors.o: command_errors.c general_functions.h command_errors.h syntax_errors.h
	$(CC) $(CFLAGS) -c command_errors.c -o command_errors.o

create_ent_ext.o: create_ent_ext.c create_ent_ext.h
	$(CC) $(CFLAGS) -c create_ent_ext.c -o create_ent_ext.o

create_extern_data_lists.o: create_extern_data_lists.c create_extern_data_lists.h general_functions.h
	$(CC) $(CFLAGS) -c create_extern_data_lists.c -o create_extern_data_lists.o

clean:
	rm -f assembler *.o
