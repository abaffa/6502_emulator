INCLUDES= -I ./include
FLAGS = -O3 -std=c++11

OBJECTS=./build/hw_web.o ./build/linkedlist.o ./build/queue_ll.o ./build/tasm_opcode.o ./build/utils.o ./build/memory.o ./build/keyboard.o ./build/screen.o ./build/6502.o ./build/6821.o

all: ${OBJECTS} 
		gcc ${FLAGS} ${INCLUDES} ./src/main.cpp ${OBJECTS} -L ./lib -lmingw32  -lwsock32 -o ./bin/main.exe	
		
./build/hw_web.o:src/hw_web.cpp
	gcc ${FLAGS} ${INCLUDES} ./src/hw_web.cpp -c -o ./build/hw_web.o

./build/hw_tty.h.o:src/hw_tty.h.cpp
	gcc ${FLAGS} ${INCLUDES} ./src/hw_tty.h.cpp -c -o ./build/hw_tty.h.o

./build/linkedlist.o:src/linkedlist.cpp
	gcc ${FLAGS} ${INCLUDES} ./src/linkedlist.cpp -c -o ./build/linkedlist.o

./build/queue_ll.o:src/queue_ll.cpp
	gcc ${FLAGS} ${INCLUDES} ./src/queue_ll.cpp -c -o ./build/queue_ll.o

./build/tasm_opcode.o:src/tasm_opcode.cpp
	gcc ${FLAGS} ${INCLUDES} ./src/tasm_opcode.cpp -c -o ./build/tasm_opcode.o

./build/utils.o:src/utils.cpp
	gcc ${FLAGS} ${INCLUDES} ./src/utils.cpp -c -o ./build/utils.o

./build/memory.o:src/memory.cpp
	gcc ${FLAGS} ${INCLUDES} ./src/memory.cpp -c -o ./build/memory.o

./build/keyboard.o:src/keyboard.cpp
	gcc ${FLAGS} ${INCLUDES} ./src/keyboard.cpp -c -o ./build/keyboard.o

./build/screen.o:src/screen.cpp
	gcc ${FLAGS} ${INCLUDES} ./src/screen.cpp -c -o ./build/screen.o

./build/6502.o:src/6502.cpp
	gcc ${FLAGS} ${INCLUDES} ./src/6502.cpp -c -o ./build/6502.o

./build/6821.o:src/6821.cpp
	gcc ${FLAGS} ${INCLUDES} ./src/6821.cpp -c -o ./build/6821.o	
	gcc ${FLAGS} ${INCLUDES} ./src/6502.cpp -c -o ./build/6502.o


clean:
	del build\* /q
	del bin\main.exe /q
