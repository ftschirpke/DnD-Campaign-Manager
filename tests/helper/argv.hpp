#ifndef ARGV_HPP_
#define ARGV_HPP_

#include "dnd_config.hpp"

#include <cstring>
#include <initializer_list>
#include <memory>
#include <utility>
#include <vector>

/**
 * @brief A class to help set up command line arguments for testing
 */
class Argv {
public:
    /**
     * @brief Constructs Argv object
     * @param args arguments
     */
    Argv(std::initializer_list<const char*> args);
    /**
     * @brief Get command line arguments
     * @return array of command line arguments
     */
    char** argv() const;
    /**
     * @brief Get the amount of command line arguments
     * @return the amount of command line arguments
     */
    int argc() const;
private:
    std::vector<std::unique_ptr<char[]>> m_args{};
    std::unique_ptr<char*[]> m_argv;
    int m_argc;
};

inline Argv::Argv(std::initializer_list<const char*> args)
    : m_argv(new char*[args.size()]), m_argc(static_cast<int>(args.size())) {
    int i = 0;
    auto iter = args.begin();
    while (iter != args.end()) {
        size_t len = strlen(*iter) + 1;
        auto ptr = std::unique_ptr<char[]>(new char[len]);

        strcpy(ptr.get(), *iter);
        m_args.emplace_back(std::move(ptr));
        m_argv.get()[i] = m_args.back().get();

        iter++;
        i++;
    }
}

inline char** Argv::argv() const { return m_argv.get(); }

inline int Argv::argc() const { return m_argc; }

#endif // ARGV_HPP_
