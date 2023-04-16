make:
	gcc -o crates src/*.c src/wrappers/*.c src/parser/*.c

install:
	sudo cp crates /bin/crates