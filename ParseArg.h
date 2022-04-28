//
// Created by charles on 10/04/2022.
//

#ifndef TP2REGIONGROWING_PARSEARG_H
#define TP2REGIONGROWING_PARSEARG_H

#include <map>
#include <utility>
#include <vector>
#include <string>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>

class ParseArg {
public:
    class Arg {
    public:
        static const int TYPE_STRING = 0;
        static const int TYPE_INT = 1;
        static const int TYPE_FLOAT = 2;
        static const int TYPE_BOOL = 3;

        std::string name;
        std::vector<std::string> commands = {};
        std::string default_val = {};
        std::string val;
        bool is_set = false;
        bool is_set_required = false;
        int type = TYPE_STRING;

        Arg() = default;

        Arg(const std::string &argName, const std::string &argCommand,
            const std::string &defaultVal, const int type) {
            std::vector<std::string> argCommands = {};
            argCommands.push_back(argCommand);
            init(argName, argCommands, defaultVal, false, type);
        }

        Arg(const std::string &argName, const std::string &argCommand,
            const std::string &defaultVal) {
            std::vector<std::string> argCommands = {};
            argCommands.push_back(argCommand);
            init(argName, argCommands, defaultVal, false, TYPE_STRING);
        }

        Arg(const std::string &argName, const std::string &argCommand,
            const std::string &defaultVal,
            const bool isSetRequired, const int type) {
            std::vector<std::string> argCommands = {};
            argCommands.push_back(argCommand);
            init(argName, argCommands, defaultVal, isSetRequired, type);
        }

        Arg(const std::string &argName, const std::vector<std::string> &argCommands,
            const std::string &defaultVal,
            const bool isSetRequired, const int type) {
            init(argName, argCommands, defaultVal, isSetRequired, type);
        }

        void init(const std::string &argName, const std::vector<std::string> &argCommands,
                  const std::string &defaultVal,
                  const bool isSetRequired, const int argType) {
            name = argName;
            commands = argCommands;
            type = argType;
            is_set_required = isSetRequired;
            if (!is_set_required) {
                default_val = defaultVal;
                val = defaultVal;
            }
        }

        std::string get_type_name() const {
            switch (type) {
                case TYPE_INT:
                    return "int";
                case TYPE_FLOAT:
                    return "float";
                case TYPE_BOOL:
                    return "bool";
                default:
                    return "string";
            }
        }

        void display() const {
            std::cout << "Argument : " << name << "(" << get_type_name() << ")\n";
            for (const auto &c: commands) {
                std::cout << "\tSyntaxe :\"" << c << "=val\"\n";
            }
            if (is_set_required) {
                std::cout << "\tRequired argument\n";
            } else {
                std::cout << "\tDefault value : \"" << default_val << "\"\n";
            }
            std::cout << "\n";
        }

        bool replies_to(const char *m) const {
            return replies_to(std::string(m));
        }

        bool replies_to(const std::string &m) const {
            return std::any_of(commands.begin(), commands.end(),
                               [&m](const std::string &c) {
                                   return m.substr(0, c.size() + 1) == (c + "=");
                               });
        }

        void set_val(const char *m) {
            set_val(std::string(m));
        }

        void set_val(const std::string &m) {
            is_set = false;
            for (const auto &c: commands) {
                if (m.substr(0, c.size() + 1) == (c + "=")) {
                    val = m.substr(c.size() + 1);
                    is_set = true;
                    break;
                }
            }
        }

        std::string get_string() const {
            check_set();
            return val;
        }

        int get_int() const {
            check_set();
            if (type == TYPE_INT) {
                return std::stoi(val);
            } else {
                throw std::invalid_argument(
                        "Argument '" + name + "'(" + get_type_name() + ") does not support get_int().");
            }
        }

        float get_float() const {
            check_set();
            if (type == TYPE_FLOAT || type == TYPE_INT) {
                return std::stof(val);
            } else {
                throw std::invalid_argument(
                        "Argument '" + name + "'(" + get_type_name() + ") does not support get_float().");
            }
        }

        bool get_bool() const {
            check_set();
            if (type == TYPE_BOOL) {
                return val == "true";
            } else if (type == TYPE_INT) {
                return std::stoi(val);
            } else if (type == TYPE_FLOAT) {
                return std::stof(val);
            } else {
                throw std::invalid_argument(
                        "Argument '" + name + "'(" + get_type_name() + ") does not support get_bool().");
            }
        }

    private:
        void check_set() const {
            if (is_set_required && !is_set) {
                throw std::invalid_argument("Argument '" + name + "' required but has not been set.");
            }
        }
    };

    ParseArg() = default;

    void add_argument(const Arg &argument) {
        arguments[argument.name] = argument;
    }

    void add_argument(const char *argName, const char *command, const char *defaultVal) {
        add_argument(Arg(std::string(argName), std::string(command), std::string(defaultVal)));
    }

    void add_argument(const char *argName, const char *command, const int defaultVal) {
        add_argument(Arg(std::string(argName), std::string(command),
                         std::to_string(defaultVal), Arg::TYPE_INT));
    }

    void add_argument(const char *argName, const char *command, const float defaultVal) {
        add_argument(Arg(std::string(argName), std::string(command),
                         std::to_string(defaultVal), Arg::TYPE_FLOAT));
    }

    void add_argument(const char *argName, const char *command, const bool defaultVal) {
        add_argument(Arg(std::string(argName), std::string(command),
                         (defaultVal ? "true" : "false"), Arg::TYPE_BOOL));
    }

    void display() {
        std::cout << "-h or --help to display that beauty\n";
        for (const auto &a: arguments) {
            a.second.display();
        }
    }

    void parse(int argc, char **argv) {
        for (int i = 1; i < argc; ++i) {
            std::string tmp(argv[i]);
            if (tmp == "-h" || tmp == "--help") {
                display();
                exit(0);
            }
        }

        for (int i = 1; i < argc; ++i) {
            for (auto &a: arguments) {
                if (a.second.replies_to(argv[i])) {
                    a.second.set_val(argv[i]);
                }
            }
        }
    }

    const Arg &operator[](const char *arg_name) const {
        auto tmp = std::string(arg_name);
        auto lookup = arguments.find(tmp);
        if (lookup != arguments.end()) {
            return (*lookup).second;
        } else {
            throw std::invalid_argument("Argument '" + tmp + "' is requested but has not been defined.");
        }
    }

private:
    std::map<std::string, Arg> arguments;
};

#endif //TP2REGIONGROWING_PARSEARG_H

