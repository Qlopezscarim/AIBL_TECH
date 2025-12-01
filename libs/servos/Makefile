all:
	g++ -o test_pca9685 test_pca9685.cpp PCA9685.cpp -std=c++11
lib:
	g++ -c PCA9685.cpp -std=c++11 -o PCA9685.o
	ar rcs libpca9685.a PCA9685.o
