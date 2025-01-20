all: main

# Tool invocations
# Executable "hello" depends on the files hello.o and run.o.

main: bin/Studio.o	bin/Action.o	bin/Customer.o	bin/Trainer.o	bin/Workout.o	bin/main.o
	echo "Linking"
	g++ -o bin/Studio bin/Studio.o	bin/Action.o	bin/Customer.o	bin/Trainer.o	bin/Workout.o	bin/main.o
	echo "Done"

bin/Studio.o: src/Studio.cpp
	g++	-g	-Wall	-Weffc++	-std=c++11	-c	-Iinclude	-o	bin/Studio.o	src/Studio.cpp 

bin/Action.o: src/Action.cpp
	g++	-g	-Wall	-Weffc++	-std=c++11	-c	-Iinclude	-o	bin/Action.o	src/Action.cpp

bin/Customer.o: src/Customer.cpp
	g++	-g	-Wall	-Weffc++	-std=c++11	-c	-Iinclude	-o	bin/Customer.o	src/Customer.cpp

bin/Trainer.o: src/Trainer.cpp
	g++	-g	-Wall	-Weffc++	-std=c++11	-c	-Iinclude	-o	bin/Trainer.o	src/Trainer.cpp

bin/Workout.o: src/Workout.cpp
	g++	-g	-Wall	-Weffc++	-std=c++11	-c	-Iinclude	-o	bin/Workout.o	src/Workout.cpp

bin/main.o: src/main.cpp
	g++	-g	-Wall	-Weffc++	-std=c++11	-c	-Iinclude	-o	bin/main.o	src/main.cpp

clean:
	rm	-f	bin/*