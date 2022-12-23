#ifndef ARGV_HPP_
#define ARGV_HPP_

#include <cstring>
#include <initializer_list>
#include <memory>
#include <vector>

class Argv {
private:
    std::vector<std::unique_ptr<char[]>> m_args{};
    std::unique_ptr<char*[]> m_argv;
    int m_argc;
public:
    Argv(std::initializer_list<const char*> args)
        : m_argv(new char*[args.size()]), m_argc(static_cast<int>(args.size())) {
        int i = 0;
        auto iter = args.begin();
        while (iter != args.end()) {
            int len = strlen(*iter) + 1;
            auto ptr = std::unique_ptr<char[]>(
                new char[len]
            );

            strcpy(ptr.get(), *iter);
            m_args.push_back(std::move(ptr));
            m_argv.get()[i] = m_args.back().get();

            iter++;
            i++;
        }
    }

    char** argv() const { return m_argv.get(); }

    int argc() const { return m_argc; }
};

#endif // ARGV_HPP_
