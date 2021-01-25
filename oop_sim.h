#pragma once
#include <string>
#include <iostream>
#include <exception>
#include <fstream>

#include "oop_sys.h"

namespace oop {

	class Simulator {

	public:
		Simulator() {
			while (1) {
				Database db;

				std::cout << "Dobrodosli u simulator mreze gradskog prevoza. Izaberite zeljenu opciju:\n";
				std::cout << "1. Ucitavanje podataka o mrezi gradskog prevoza\n";
				std::cout << "0. Kraj rada\n";

				int input;
				std::cin >> input;

				if (input == 0)
					return;
				else if (input != 1)
					continue;

				std::string f1, f2;
				std::cout << "Naziv fajla sa podacima o stanicama:\n";
				std::cin >> f1;
				if (!db.load_stops(f1)) continue;

				std::cout << "Naziv fajla sa podacima o linijama:\n";
				std::cin >> f2;
				db.load_lines(f2);
				if (!db.load_lines(f2)) continue;

				std::cout << "Mreza je uspesno ucitana. Molimo Vas, odaberite opciju:\n";
				std::cout << "1. Prikaz informacija o stajalistu" << "\n";
				std::cout << "2. Prikaz osnovnih informacija o liniji gradskog prevoza" << "\n";
				std::cout << "3. Prikaz statistickih informacija o liniji gradskog prevoza" << "\n";
				std::cout << "4. Pronalazak putanje izmedju dva stajalista" << "\n";
				std::cout << "0. Kraj rada" << "\n";

				int opt;
				std::cin >> opt;
				std::string arg;
				std::vector<Route> path;
				std::ofstream out_f;

				switch (opt) {
				case 1:
					std::cout << "Unesite broj stajalista:\n";
					int s_n;
					std::cin >> s_n;
					db.print_stop(s_n);
				case 2:
					std::cout << "Unesite oznaku linije:\n";
					std::cin >> arg;
					db.print_line(arg);
				case 3:
					std::cout << "Unesite oznaku linije:\n";
					std::cin >> arg;
					db.line_stats(arg);
				case 4:
					std::cout << "Unesite brojeve pocetnog i kranjeg stajalista\n";
					int a, b;
					std::cin >> a >> b;
					std::cout << "Izaberite vrstu putanje:\n";
					std::cout << "1. Najmanji broj stanica\n";
					std::cout << "2. Najmanji broj presedanja\n";
					int k;
					std::cin >> k;
					if (k == 1)
						path = db.least_stops(*db.get_stop(a), *db.get_stop(b));
					else if (k == 2)
						path = db.least_changes(*db.get_stop(a), *db.get_stop(b));
					out_f.open("putanja_" + std::to_string(a) + "_" + std::to_string(b) + ".txt");
					for (Route& r : path)
						r.print(out_f);
					out_f.close();
				case 0:
					return;
				}
			}
		}
	};
}