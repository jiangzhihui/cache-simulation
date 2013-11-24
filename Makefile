run-switch:compile-switch
	./switch edge1.cfg
compile-switch:
	g++ -Wall switch.cc -o switch
run-source:compile-source
	./source source.cfg
compile-source:
	g++ -Wall -g source.cc -o source
