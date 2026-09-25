#include "CancelActionCommand.h"
#include <iostream>

CancelActionCommand::CancelActionCommand(Command& target)
    : target_(target), executed_(false) {}

void CancelActionCommand::execute() {
    std::cout << "[CancelActionCommand] Cancelling previous action: "
              << target_.description() << std::endl;
    target_.undo();
    executed_ = true;
}

void CancelActionCommand::undo() {
    if (!executed_) {
        std::cout << "[CancelActionCommand] Nothing to undo." << std::endl;
        return;
    }

    std::cout << "[CancelActionCommand] Re-applying cancelled action: "
              << target_.description() << std::endl;
    target_.execute();
    executed_ = false;
}

std::string CancelActionCommand::description() const {
    return "Cancel previous action: " + target_.description();
}
