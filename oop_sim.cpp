#include "oop_sim.h"

namespace oop {

	void State::on_input() {
	}

	const std::string& State::get_input() const {
		return input;
	}

	void Question::operator>>(std::string& collector)
	{
		collector = "" + get_input();
	}
}