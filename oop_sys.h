#pragma once
#include <vector>
#include "oop_structs.h"

namespace oop {

	struct Route;

	class Database {
		std::vector<Stop*> stops;
		std::vector<Line*> lines;

		void init_links();
	public:

		void load_stops(const std::string& file_name);
		void load_lines(const std::string& file_name);

		void print_stop(int id);
		void print_line(const std::string& id);
		void line_stats(const std::string& id);

		Stop* get_stop(int id) const;
		Line* get_line(std::string& id) const;

		std::vector<Route> least_stops(Stop& a, Stop& b) const;
		std::vector<Route> least_changes(Stop& a, Stop& b) const;

	};

	struct Route {
		Line *a, *b;
		std::vector<Stop*> stops;

		Route(Line* a, Line* b, std::vector<Stop*> stops)
			: a(a), b(b), stops(stops) {}
		void print(std::ofstream& of) const;
	};
}