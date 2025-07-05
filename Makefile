debug: $(wildcard src/*.*)
	clang -g -DDEBUG -O0 -std=c11 -Wall -Wextra -fsanitize=address -Wno-pragma-once-outside-header -o debug libaudio/src/main.c

booom: $(wildcard src/*.*)
	clang -std=c11 -Wall -Wextra -fsanitize=address -Wno-pragma-once-outside-header -o booom libaudio/src/main.c

libaudio.a: $(wildcard src/*.*)
	clang -c -O2 -std=c11 -Wall -Wextra -Wno-pragma-once-outside-header libaudio/src/project.c -o project.o
	ar rcs libaudio.a project.o
	rm project.o

clean:
	git clean -Xf

