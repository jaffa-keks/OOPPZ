#pragma once
#include <string>
#include <vector>
#include <utility>
#include <map>

namespace oop {

	class Line;
	struct Link;

	class Stop {
	private:
		int id;
		std::string name;
		std::vector<Line*> lines;
		std::vector<Link> links;

	public:
		Stop(int id, const std::string& name) : id(id), name(name) {}
		void init_links();

		int get_id() const;
		const std::string& get_name() const;
		const std::vector<Line*>& get_lines() const;
		const std::vector<Link>& get_links() const;

		void add_line(Line* l); //add sorted
		Link connected(Stop& s) const;
	};

	class Line {
	private:
		std::string id;
		std::vector<Stop*> stops_a, stops_b, full_path;
		std::map<Line*, std::vector<Stop*>> intersects;

		std::vector<Stop*> analyze_path(const std::string& path, const std::vector<Stop*>& stops);
		void analyze_intersects();

	public:
		Line(const std::string& id, const std::string& path_a, const std::string& path_b, const std::vector<Stop*>& stops)
			: id(id), stops_a(analyze_path(path_a, stops)), stops_b(analyze_path(path_b, stops)) {
			analyze_intersects();
			full_path.reserve(stops_a.size() + stops_b.size() - 2);
			full_path.insert(full_path.end(), stops_a.begin(), stops_a.end() - 1);
			full_path.insert(full_path.end(), stops_b.begin(), stops_b.end() - 1);
		}

		const std::string& get_id() const;
		const std::vector<Stop*>& get_stops_a() const;
		const std::vector<Stop*>& get_stops_b() const;
		const std::vector<Stop*>& get_full_path() const;
		std::map<Line*, std::vector<Stop*>>& get_intersects();
		
		Stop* intersect_with(Line* a);
	};

	struct Link {
		Stop* stop;
		Line* line;
		int weight;
		std::vector<Stop*> route; // weight should always be equal to route.size() - 1

		Link(Stop& s, Line& l, int w) : stop(&s), line(&l), weight(w) {}
		Link() : stop(nullptr), line(nullptr), weight(1e8) {}
	};
}