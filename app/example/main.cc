// {{{ GPL License 

// This file is part of gringo - a grounder for logic programs.
// Copyright (C) 2013  Benjamin Kaufmann
// Copyright (C) 2013  Roland Kaminski

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

#include <clingo/clingocontrol.hh>

void toGringoValue(const char* p_string, std::vector<Gringo::Value>* vec)
{
	char* end;

	int number = std::strtol(p_string, &end, 10);

	if ((end == p_string) || (*end != '\0'))
	{
		vec->push_back(Gringo::Value::createId(p_string));
	}
	else
	{
		vec->push_back(Gringo::Value::createNum(number));
	}
}

Gringo::Value stringToValue(char* p_aspString)
{
	if (std::strlen(p_aspString) == 0)
		return Gringo::Value();

	std::vector<Gringo::Value> vec;

	char* values = std::strchr(p_aspString, '(');
	char* valuesEnd = std::strrchr(p_aspString, ')');
	char* nameEnd = values;
	char tmpEnd, tmp;
	char *index1, *index2;

	if (values == nullptr || valuesEnd == nullptr || valuesEnd < values)
	{
		return Gringo::Value::createId(p_aspString);
	}

	tmpEnd = *valuesEnd;
	*valuesEnd = '\0';

	++values;

	while (valuesEnd > values)
	{
//              std::cout << "##" << values << std::endl;
		index1 = std::strchr(values, '(');
		index2 = std::strchr(values, ',');

		if (index1 >= valuesEnd)
			index1 = nullptr;

		if (index2 >= valuesEnd)
			index2 = nullptr;

		if (index1 == nullptr && index2 == values)
		{
			++values;
			continue;
		}

		if (index2 != nullptr && index1 == nullptr)
		{
			tmp = *index2;
			*index2 = '\0';
			toGringoValue(values, &vec);
			*index2 = tmp;
			values = index2 + 1;
		}
		else if (index2 == nullptr && index1 != nullptr)
		{
			int count = 0;
			++index1;
			while (index1 != valuesEnd)
			{
				if (*index1 == ')')
				{
					if (count == 0)
						break;
					else
						--count;
				}
				else if (*index1 == '(')
					++count;

				++index1;
			}
			++index1;
			//  index1 = std::strrchr(values, ')') +1;
			tmp = *index1;
			*index1 = '\0';
			vec.push_back(stringToValue(values));
			*index1 = tmp;
			values = index1 + 1;
		}
		else if (index2 == nullptr && index1 == nullptr)
		{
			toGringoValue(values, &vec);
			values = valuesEnd;
		}
		else if (index2 < index1)
		{
			tmp = *index2;
			*index2 = '\0';
			toGringoValue(values, &vec);
			*index2 = tmp;
			values = index2 + 1;
		}
		else
		{
			int count = 0;
			++index1;
			while (index1 != valuesEnd)
			{
				if (*index1 == ')')
				{
					if (count == 0)
						break;
					else
						--count;
				}
				else if (*index1 == '(')
					++count;

				++index1;
			}
			++index1;
			tmp = *index1;
			*index1 = '\0';
			vec.push_back(stringToValue(values));
			*index1 = tmp;
			values = index1 + 1;
		}
	}

	*valuesEnd = tmpEnd;

	return Gringo::Value::createFun(std::string(p_aspString, nameEnd), vec);
}

Gringo::Value stringToValue(const char* p_aspString)
{
	char *aspString = new char[std::strlen(p_aspString) + 1];
	std::strcpy(aspString, p_aspString);

	Gringo::Value value = stringToValue(aspString);
	delete[] aspString;

	return value;
}


bool checkMatchValues(const Gringo::Value* value1, const Gringo::Value* value2)
{
	if (value2->type() != Gringo::Value::Type::FUNC)
		return false;

	if (value1->name() != value2->name())
		return false;

	if (value1->args().size() != value2->args().size())
		return false;

	for (uint i = 0; i < value1->args().size(); ++i)
	{
		Gringo::Value arg = value1->args()[i];

		if (arg.type() == Gringo::Value::Type::ID && arg.name() == "wildcard")
			continue;

		if (arg.type() == Gringo::Value::Type::FUNC && value2->args()[i].type() == Gringo::Value::Type::FUNC)
		{
			if (false == checkMatchValues(&arg, &value2->args()[i]))
				return false;
		}
		else if (arg != value2->args()[i])
			return false;
	}

	return true;
}

void example2()
{
	std::cout << "Example2 started" << std::endl;
	std::vector<char const *> args {"clingo", nullptr};

	DefaultGringoModule module;
	ClingoLib lib(module, args.size() - 2, args.data());
	lib.add("base", {}, "a(1..2). b(5..10). a(X) :- b(X). a(1,3). a(tut). a(tut, 3). c(1..10).");
	lib.add("base", {}, "a(b(c(3,4))). a(b(c(3,3))). a(b(c(2,3))). a(b(c(2,3),2)). a(b(c(Y,X))) :- a(1), b(Y), c(X).");
	lib.ground( { {"base", {}}}, nullptr);

	// Note that you need to adapt the method "checkMatchValues" with
	// the corresponding wildcard-identifier for the following two options.

	// 1st Option with '?' as wildcard (needs parse method from above)
	//Gringo::Value queryGringoValue = stringToValue("a(b(c(?,3)))");

	// 2nd Option with gringo-module parser (no need for extra method, but give "wildcard" a special meaning)
	Gringo::Value queryGringoValue = module.parseValue("a(b(c(wildcard,3)))");

	lib.solve([queryGringoValue](Gringo::Model const &m)
	{
		//std::cout << "Inside Lambda!" << std::endl;
			ClingoModel& clingoModel = (ClingoModel&) m;
			auto it = clingoModel.out.domains.find(queryGringoValue.sig());
			std::vector<Gringo::Value> gringoValues;
			std::vector<Gringo::AtomState const *> atomStates;
			if (it != clingoModel.out.domains.end())
			{
				//std::cout << "In Loop" << std::endl;
				for (auto& domainPair : it->second.domain)
				{
					//std::cout << domainPair.first << " " << std::endl;
					if (&(domainPair.second) && clingoModel.model->isTrue(clingoModel.lp.getLiteral(domainPair.second.uid())))
					{
						if (checkMatchValues(&queryGringoValue, &domainPair.first))
						{
							gringoValues.push_back(domainPair.first);
							atomStates.push_back(&(domainPair.second));
						}
					}
				}
			}

			for (auto &gringoValue : gringoValues)
			{
				std::cout << gringoValue << " ";
			}
			std::cout << std::endl;

			return true;
		},
			{});
}

void example1()
{
	std::vector<char const *> args {"clingo", "-e", "brave", nullptr};
	DefaultGringoModule module;
	ClingoLib lib(module, args.size() - 2, args.data());
	lib.load("/home/emmeda/Research/dev/mslws/src/symrock/alica_asp_test/src/etc/asp_background_knowledge/alica-background-knowledge.lp");
	lib.ground( { {"alicaBackground", {}}}, nullptr);
	lib.ground( { {"testTopLevel", {}}}, nullptr);
	lib.solve([](Gringo::Model const &m)
	{
		for (auto &atom : m.atoms(Gringo::Model::SHOWN))
		{
			std::cout << atom << " ";
		}
		std::cout << std::endl;
		return true;
	},
				{});
}

int main()
{
	example1();
}

