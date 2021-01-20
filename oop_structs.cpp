#include "oop_structs.h"
#include "oop_lib.h"

namespace oop {

	int Stop::get_id() const {
		return id;
	}

	const std::string& Stop::get_name() const {
		return name;
	}

	const std::vector<Line*>& Stop::get_lines() const {
		return lines;
	}

	const std::vector<Link>& Stop::get_links() const {
		return links;
	}

	void Stop::add_line(Line* l) {
		_for (i, 0, lines.size()) {
			if (l->get_id() == lines[i]->get_id()) return;
			if (l->get_id() < lines[i]->get_id()) {
				lines.insert(lines.begin() + i, l);
				return;
			}
		}
		lines.push_back(l);
	}

	Link Stop::connected(Stop& s) const {
		for (const Link& l : links)
			if (l.stop == &s)
				return l;
		return Link();
	}

	std::map<Line*, std::vector<Stop*>>& Line::get_intersects() {
		return intersects;
	}

	std::vector<Stop*> Line::analyze_path(const std::string& path, const std::vector<Stop*>& stations) {
		std::vector<std::string> stops = split(path, " ");
		std::vector<Stop*> s_path;
		for (std::string& stop : stops) {
			int s_id = stoi(stop);
			for (Stop* s_ref : stations) {
				if (s_ref->get_id() == s_id) {
					s_ref->add_line(this);
					s_path.push_back(s_ref);
					break;
				}
			}
		}
		return s_path;
	}

	void Line::analyze_intersects() {
		std::vector<Stop*>* paths[] = { &stops_a, &stops_b };
		_for(pp, 0, 2) {
			std::vector<Stop*>& path = *(paths[pp]);
			for (Stop* ss : path) {
				if (pp == 1) {
					bool next = false;
					for (Stop* sp : *paths[0])
						if (sp->get_id() == ss->get_id()) {
							next = true; break;
						}
					if (next) continue;
				}
				for (Line* sl : ss->get_lines()) {
					if (sl == this) continue;
					intersects[sl].push_back(ss);
					sl->get_intersects()[this].push_back(ss);
				}
			}
		}
	}

	const std::string& Line::get_id() const {
		return id;
	}

	const std::vector<Stop*>& Line::get_stops_a() const {
		return stops_a;
	}

	const std::vector<Stop*>& Line::get_stops_b() const {
		return stops_b;
	}

	const std::vector<Stop*>& Line::get_full_path() const {
		return full_path;
	}

	// make sure database calls this function for all stops on init
	void Stop::init_links() {
		for (Line* l : lines) {
			int local_index = std::find(l->get_full_path().begin(), l->get_full_path().end(), this) - l->get_full_path().begin();
			std::vector<Stop*> path;
			path.reserve(l->get_full_path().size());
			path.insert(path.end(), l->get_full_path().begin() + local_index, l->get_full_path().end());
			path.insert(path.end(), l->get_full_path().begin(), l->get_full_path().begin() + local_index);

			_for(i, 1, path.size()) {
				Link conn = connected(*path[i]);
				if (conn.stop) {
					int k = 0;
					for (; k < links.size(); k++) if (links[k].stop == path[i]) break;
					if (i < links[k].weight) {
						links[k].weight = i;
						links[k].line = l;
						links[k].route.clear();
						links[k].route.insert(links[k].route.end(), path.begin(), path.begin() + i + 1);
					}
				}
				else {
					links.push_back(Link(*path[i], *l, i)); // i = dist
					std::vector<Stop*>& s_ref = links[links.size() - 1].route;
					s_ref.insert(s_ref.end(), path.begin(), path.begin() + i + 1);
				}
			}
		}
	}
}