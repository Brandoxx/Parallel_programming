# rule
#target: prerequisites
#	commands to build target

hello: hello.c  ## this is a rule, the target is hello, the prerequisites are hello.c
	cc -g -Wall -Wextra hello.c -o hello -pthread 


.PHONY: asan msan tsan ubsan ## this is a phony rule, it is not a target, it is a prerequisite

asan: ## this is a rule, asan is a google sanitizer, the prerequisites are the targets of the rule
	cc -g -Wall -Wextra -fsanitize=address hello.c -o hello -pthread

msan: ## this is a rule, msan is a google sanitizer, the prerequisites are the targets of the rule
	clang -g -Wall -Wextra -fsanitize=memory hello.c -o hello -pthread

tsan: ## this is a rule, tsan is a google sanitizer, the prerequisites are the targets of the rule
	cc -g -Wall -Wextra -fsanitize=thread hello.c -o hello -pthread

ubsan: ## this is a rule, ubsan is a google sanitizer, the prerequisites are the targets of the rule
	cc -g -Wall -Wextra -fsanitize=undefined hello.c -o hello -pthread

.PHONY: memcheck ## this is a phony rule, it is not a target, it is a prerequisite
memcheck: ## memcheck checks for memory leaks
	valgrind --tool=memcheck --leak-check=full ./hello

.PHONY: helgrind
helgrind: ## helgrind checks for memory leaks, theadsafety, and undefined behavior
	valgrind --tool=helgrind ./hello

.PHONY: clean 
clean:
	rm -f hello
