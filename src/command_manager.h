#ifndef COMMAND_MANAGER_H
#define COMMAND_MANAGER_H

#include <stack>
#include <memory>
#include <functional>

class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
};

class CommandManager {
public:
    void executeCommand(std::unique_ptr<Command> command);
    void undoLastCommand();
    bool canUndo() const;
    void clearUndoHistory();

private:
    std::stack<std::unique_ptr<Command>> m_undoStack;
};

// Specific command classes can be defined here or in separate files
class AddFoodCommand : public Command {
public:
    AddFoodCommand(/* parameters needed for adding food */);
    void execute() override;
    void undo() override;

private:
    // Store necessary state to undo the action
};

class RemoveFoodCommand : public Command {
public:
    RemoveFoodCommand(/* parameters needed for removing food */);
    void execute() override;
    void undo() override;

private:
    // Store necessary state to undo the action
};

#endif // COMMAND_MANAGER_H