#pragma once
#include <string>
#include <iostream>
#include <exception>

#include "oop_sys.h"

namespace oop {

	class Simulator {
		Database db;
		State current;
	public:
		Simulator() : current(Welcome()) {
			while (1) {
				try {
					current = current.go_to_next();
				}
				catch (...) {
					//On welcome if input is 0 throw QuitException
				}
			}
		}
	};

	class State {
		std::string display;
		std::string input;

	public:
		State(const std::string& display) : display(display) {}

		void on_display() {
			std::cout << display << "\n";
			std::cin >> input;
			on_input();
		}
		const std::string& get_input() const;

		virtual void on_input();
		virtual State go_to_next() const { return State(""); };
	};

	class Question : public State {
	public:
		Question(const std::string& display) : State(display) {}

		void on_input() override {};
		State go_to_next() const override { return State(""); };

		void operator>>(std::string& collector);
	};

	class Welcome : public State {
	public:
		Welcome() : State("Dobrodosli u simulator mreze gradskog prevoza. Molimo Vas, odaberite opciju:\n1.Ucitavanje podataka o mrezi\n2.Kraj rada") {}
		State go_to_next() const override;
	};
}