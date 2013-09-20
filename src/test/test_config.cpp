/*
 * Copyright 2012, 2013 Moritz Hilscher
 *
 * This file is part of mapcrafter.
 *
 * mapcrafter is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * mapcrafter is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with mapcrafter.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "../config/extended_ini.h"
#include "../config/parser.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/test/unit_test.hpp>

namespace config = mapcrafter::config2;

BOOST_AUTO_TEST_CASE(config_test) {
	/*
	config::ConfigSection section("world", "myworld");
	section.set("foo", "bar");
	section.set("foo2", "test");
	section.remove("foo2");
	section.set("test", "73");
	section.set("test", "42");
	*/

	/*
	config::ConfigSection section("test", "");
	section.set("foo", "42");

	config::ConfigFile c;

	config::ValidationMessage msg;
	if (!c.loadFile("test.conf", msg))
		std::cout << msg << std::endl;
	else {
		c.getRootSection().set("hello", "world");
		c.addSection(section);
		c.write(std::cout);
	}
	*/

	if (!42)
		return;

	config::ConfigParser parser;
	config::ValidationMap validation;
	bool ok = parser.parse("test.conf", validation);

	if (validation.size() > 0) {
		std::cout << (ok ? "Some notes on your configuration file:" : "Your configuration file is invalid!") << std::endl;
		for (auto it = validation.begin(); it != validation.end(); ++it) {
			std::cout << it->first << ":" << std::endl;
			for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
				std::cout << " - " << *it2 << std::endl;
			}
		}
	}
}

BOOST_AUTO_TEST_CASE(config_testReadWrite) {
	config::ConfigFile c;
	if (!c.loadFile("data/config/test.conf"))
		BOOST_ERROR("Unable to load test config file test.conf!");
	std::ifstream in("data/config/test.conf");
	std::string in_data((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

	std::ostringstream out;
	c.write(out);
	std::string out_data = out.rdbuf()->str();

	BOOST_CHECK_EQUAL(in_data, out_data);
}
