#include "command_manager.h"
#include <stdexcept>

void CommandManager::executeCommand(std::unique_ptr<Command> command) {
    command->execute();
    m_undoStack.push(std::move(command));
}

void CommandManager::undoLastCommand() {
    if (m_undoStack.empty()) {
        throw std::runtime_error("No commands to undo");
    }

    auto command = std::move(m_undoStack.top());
    m_undoStack.pop();
    command->undo();
}

bool CommandManager::canUndo() const {
    return !m_undoStack.empty();
}

void CommandManager::clearUndoHistory() {
    while (!m_undoStack.empty()) {
        m_undoStack.pop();
    }
}