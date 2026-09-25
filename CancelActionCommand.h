#ifndef CANCELACTIONCOMMAND_H
#define CANCELACTIONCOMMAND_H

#include "Command.h"

class CancelActionCommand : public Command {
private:
    Command& target_; 
    bool executed_;

public:
    explicit CancelActionCommand(Command& target);

    void execute();
    void undo();
    std::string description() const;
};

#endif
