#ifndef OPERATORCONSOLE_H
#define OPERATORCONSOLE_H

#include <memory>
#include <vector>

class Command;

class OperatorConsole {
private:
    std::vector<std::unique_ptr<Command> > history_; // owns command objects

public:
    OperatorConsole();
    ~OperatorConsole();

    void execute(std::unique_ptr<Command> command);
    Command* lastCommand();
    std::size_t commandCount() const;
};

#endif
