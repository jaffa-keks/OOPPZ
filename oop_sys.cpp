#include <fstream>
#include <string>
#include <utility>
#include <map>
#include <iostream>

#include "oop_sys.h"
#include "oop_lib.h"

namespace oop {

	void Database::init_links() {
		for (Stop* s : stops)
			s->init_links();
	}

	bool Database::load_stops(const std::string& file_name) {
		std::ifstream inp_file(file_name);
		if (!inp_file) return false;

		std::string line;
		while (std::getline(inp_file, line)) {
			size_t id_off = line.find(" ");

			int s_id = stoi(line.substr(0, id_off));
			std::string name = line.substr(id_off + 1, line.size() - id_off - 1);

			stops.push_back(new Stop(s_id, name));
		}
		inp_file.close();
		return true;
	}

	bool Database::load_lines(const std::string& file_name) {
		std::ifstream inp_file(file_name);
		if (!inp_file) return false;

		std::string line;
		while (std::getline(inp_file, line)) {
			size_t id_off = line.find(" ");

			std::string l_id = line.substr(0, id_off);
			std::string rest = line.substr(id_off + 1, line.size() - id_off - 1);

			std::vector<std::string> paths = split(rest, "#");

			lines.push_back(new Line(l_id, paths[0], paths[1], stops));
		}
		inp_file.close();

		init_links();
		return true;
	}

	void ps_format1(std::ofstream&, Stop*);

	void Database::print_stop(int id) {
		for (Stop* s : stops) {
			if (s->get_id() == id) {
				std::ofstream out_file("stajaliste_" + std::to_string(id) + ".txt");
				if (!out_file) return;

				ps_format1(out_file, s);
				out_file.close();
				return;
			}
		}
	}

	void pl_format1(std::ofstream&, Line*);

	void Database::print_line(const std::string& id) {
		for (Line* l : lines) {
			if (l->get_id() == id) {
				std::ofstream out_file("linija_" + id + ".txt");
				if (!out_file) return;

				pl_format1(out_file, l);
				out_file.close();
				return;
			}
		}
	}

	void ps_format1(std::ofstream& of, Stop* s) {
		of << s->get_id() << " " << s->get_name();
		of << " [";
		bool lf = false;
		for (Line* l : s->get_lines()) {
			if (!lf) lf = true; else of << " ";
			of << l->get_id();
		}
		of << "]";
	}

	void pl_format1(std::ofstream& of, Line* l) {
		of << l->get_id() << " ";
		of << l->get_stops_a()[0]->get_name() << "->";
		of << l->get_stops_a()[l->get_stops_a().size() - 1]->get_name() << "\n";
		for (Stop* s : l->get_stops_a())
			of << s->get_id() << " " << s->get_name() << "\n";
		for (Stop* s : l->get_stops_b())
			of << s->get_id() << " " << s->get_name() << "\n";
	}

	Stop* Database::get_stop(int id) const {
		for (Stop* s : stops)
			if (s->get_id() == id)
				return s;
		return nullptr;
	}

	Line* Database::get_line(std::string& id) const {
		for (Line* l : lines)
			if (l->get_id() == id)
				return l;
		return nullptr;
	}

	void Database::line_stats(const std::string& id) {
		for (Line* l : lines) {
			if (l->get_id() == id) {
				std::ofstream out_file("statistika_" + id + ".txt");
				if (!out_file) return;

				std::string out1;
				int max = 0;
				for (auto& i : l->get_intersects()) {
					if (i.second.size() > max) max = i.second.size();
					out1 += i.first->get_id() + " ";
				}

				std::string out2;
				for (auto& i : l->get_intersects())
					if (i.second.size() == max)
						out2 += i.first->get_id() + ",";

				out_file << id << "\n";
				out_file << out1.substr(0, out1.size() - 1) << "\n";
				out_file << out2.substr(0, out2.size() - 1) << "\n";
				out_file.close();

				return;
			}
		}
	}

	void Route::print(std::ofstream& of) const {
		if (a) of << a->get_id();
		of << "->";
		if (b) of << b->get_id();
		of << "\n";
		for (int i = 0; i < stops.size(); i++) {
			of << stops[i]->get_id();
			if (i < stops.size() - 1) of << " ";
		}
		of << "\n";
	}

	std::vector<Route> Database::least_stops(Stop& a, Stop& b) const {
		std::map<Stop*, std::pair<int, std::pair<Stop*, Link>>> table;
		std::vector<Stop*> visited, unvisited = stops;

		for (Stop* s : unvisited) table[s] = std::make_pair(1e8, std::make_pair(nullptr, Link()));
		table[&a].first = 0;

		while (unvisited.size() > 0) {
			Stop* current = unvisited[0];
			for (Stop* uv : unvisited) if (table[uv].first < table[current].first) current = uv;
			for (const Link& l : current->get_links()) {
				for (Stop* sv : visited) if (sv == l.stop) continue;
				int dist = l.weight + table[current].first;
				if (dist < table[l.stop].first) {
					table[l.stop].first = dist;
					table[l.stop].second.first = current;
					table[l.stop].second.second = l;
				}
			}
			visited.push_back(current);
			_for (i, 0, unvisited.size())
				if (unvisited[i] == current) {
					unvisited.erase(unvisited.begin() + i);
					break;
				}
		}

		std::vector<Route> path;
		Stop* rt = &b;
		while (table[rt].second.first != &a) {
			Stop* prev_node = table[rt].second.first;
			Link& prev_link = table[prev_node].second.second;
			Link& curr_link = table[rt].second.second;
			Line& from = *prev_link.line;
			Line& to = *curr_link.line;
			path.push_back(Route(&from, &to, curr_link.route));
			rt = prev_node;
		}
		path.push_back(Route(nullptr, table[rt].second.second.line, table[rt].second.second.route));
		std::reverse(path.begin(), path.end());
		return path;
	}

	std::vector<Route> Database::least_changes(Stop& a, Stop& b) const {
		std::map<Stop*, std::pair<int, std::pair<Stop*, Link>>> table;
		std::vector<Stop*> visited, unvisited = stops;

		for (Stop* s : unvisited) table[s] = std::make_pair(1e8, std::make_pair(nullptr, Link()));
		table[&a].first = 0;

		while (unvisited.size() > 0) {
			Stop* current = unvisited[0];
			for (Stop* uv : unvisited) if (table[uv].first < table[current].first) current = uv;
			for (const Link& l : current->get_links()) {
				for (Stop* sv : visited) if (sv == l.stop) continue;
				int dist = 1 + table[current].first;
				if (dist < table[l.stop].first) {
					table[l.stop].first = dist;
					table[l.stop].second.first = current;
					table[l.stop].second.second = l;
				}
			}
			visited.push_back(current);
			_for(i, 0, unvisited.size())
				if (unvisited[i] == current) {
					unvisited.erase(unvisited.begin() + i);
					break;
				}
		}

		std::vector<Route> path;
		Stop* rt = &b;
		while (table[rt].second.first != &a) {
			Stop* prev_node = table[rt].second.first;
			Link& prev_link = table[prev_node].second.second;
			Link& curr_link = table[rt].second.second;
			Line& from = *prev_link.line;
			Line& to = *curr_link.line;
			path.push_back(Route(&from, &to, curr_link.route));
			rt = prev_node;
		}
		path.push_back(Route(nullptr, table[rt].second.second.line, table[rt].second.second.route));
		std::reverse(path.begin(), path.end());
		return path;
	}
}