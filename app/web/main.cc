// {{{ GPL License 

// This file is part of gringo - a grounder for logic programs.
// Copyright (C) 2013  Benjamin Kaufmann

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// }}}

#include "clingo_app.hh"
#include <iterator>

extern "C" int run(char const *program, char const *options) {
    std::streambuf* orig = std::cin.rdbuf();
    auto exit(Gringo::onExit([orig]{ std::cin.rdbuf(orig); }));
    std::istringstream input(program);
    std::cin.rdbuf(input.rdbuf());
    Gringo::reset_message_printer();
    std::vector<std::vector<char>> opts;
    opts.emplace_back(std::initializer_list<char>{'c','l','i','n','g','o','\0'});
    std::istringstream iss(options);
    for (std::istream_iterator<std::string> it(iss), ie; it != ie; ++it) {
        opts.emplace_back(it->c_str(), it->c_str() + it->size() + 1);
    }
    std::vector<char*> args;
    for (auto &opt : opts) {
        args.emplace_back(opt.data());
    }
    ClingoApp app;
    args.emplace_back(nullptr);
    return app.main(args.size()-2, args.data());
}

