all:
	g++ -o VM main.cpp -I/include
	$(shell cd asm)

clean:
	rm VM
