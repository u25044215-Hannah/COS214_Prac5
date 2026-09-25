#include "OperatorConsole.h"
#include "Command.h"
#include <iostream>

OperatorConsole::OperatorConsole() {}
OperatorConsole::~OperatorConsole() {}

void OperatorConsole::execute(std::unique_ptr<Command> command) {
    if (!command) {
        std::cerr << "[Invoker:OperatorConsole] ERROR: null command rejected." << std::endl;
        return;
    }

    std::cout << "\n[Invoker:OperatorConsole] Executing -> "
              << command->description() << std::endl;
    command->execute();
    history_.push_back(std::move(command));
}

Command* OperatorConsole::lastCommand() {
    return history_.empty() ? 0 : history_.back().get();
}

std::size_t OperatorConsole::commandCount() const {
    return history_.size();
}
