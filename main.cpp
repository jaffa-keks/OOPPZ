#include "oop_sim.h"

int main() {
	std::string pitanje;
	oop::Question("Da li ide gas") >> pitanje;
	std::cout << pitanje;
}