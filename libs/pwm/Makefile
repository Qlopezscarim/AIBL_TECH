all:
	echo "This file needs sudo permissions and is auto-run by this makefile"
	c++ pwm.cpp test.cpp -o pwm_test
lib:
	c++ -c pwm.cpp -o pwm.o
	ar rcs libpwm.a pwm.o
