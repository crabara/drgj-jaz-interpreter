#include "InstructionItem.h"

InstructionItem::InstructionItem() {
    this->accessAttempt = 0;
    this->next = NULL;
}

InstructionItem::InstructionItem(string tempCommand,
                                 string tempArg,
                                 int tempLineNumber) {
    this->lineNumber = tempLineNumber;
    this->command = tempCommand;
    this->arg = tempArg;
    InstructionItem();
}

InstructionItem::~InstructionItem() { }

void InstructionItem::setLineNumber(int tempLineNumber) {
    this->lineNumber = tempLineNumber;
}

void InstructionItem::incrementAccess() {
    this->accessAttempt++;
}

void InstructionItem::setCommand(string tempCommand) {
    this->command = tempCommand;
}

void InstructionItem::setArg(string tempArg) {
    this->arg = tempArg;
}

void InstructionItem::setNext(InstructionItem* tempNextPtr) {
    this->next = tempNextPtr;
}

int InstructionItem::getLineNumber() {
    return this->lineNumber;
}

int InstructionItem::getAccess() {
    return this->accessAttempt;
}

string InstructionItem::getCommand() {
    return this->command;
}

string InstructionItem::getArg() {
    return this->arg;
}

InstructionItem* InstructionItem::getNext() {
    return this->next;
}
