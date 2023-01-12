#target: prerequisites
#	command to build target

APPNAME=$(shell basename $(shell pwd))  # get the name of the current directory (the app name)
APPARGS=10 # number of arguments to pass to the app (the number of arguments)
CFLAGS=-g -Wall -Wextra # compiler flags (debug, warnings, etc)

$(APPNAME): $(APPNAME).c # the target is the app name
	cc $(CFLAGS) $< -o $@ -pthread 

.PHONY: lint # the .PHONY target is the lint target
lint: # the lint target, checks for syntax errors
	cpplint --filter=-readability/casting $(APPNAME).c

.PHONY: memcheck
memcheck: $(APPNAME) # the memcheck target, checks for memory leaks
	valgrind --tool=memcheck --leak-check=full ./$(APPNAME) $(APPARGS)

.PHONY: helgrind
helgrind: $(APPNAME) # the helgrind target, checks for memory leaks
	valgrind --tool=helgrind ./$(APPNAME) $(APPARGS)

.PHONY: asan # invalid access & memory leaks
asan: $(APPNAME).c # the asan target, checks for invalid access and memory leaks
	cc $(CFLAGS) -fsanitize=address $(APPNAME).c -o $(APPNAME) -pthread

.PHONY: msan # uninitialized memory
msan: $(APPNAME).c # the msan target, checks for uninitialized memory
	clang $(CFLAGS) -fsanitize=memory $(APPNAME).c -o $(APPNAME) -pthread

.PHONY: tsan # thread sanitizer
tsan: $(APPNAME).c # the tsan target, checks for thread sanitizer
	cc $(CFLAGS) -fsanitize=thread $(APPNAME).c -o $(APPNAME) -pthread

.PHONY: ubsan # undefined behavior
ubsan: $(APPNAME).c # the ubsan target, checks for undefined behavior
	cc $(CFLAGS) -fsanitize=undefined $(APPNAME).c -o $(APPNAME) -pthread

.PHONY: clean
clean: # the clean target, removes the app and the executable
	rm -f $(APPNAME)
