/* 
 * File:   InstructionItem.cpp
 * Author: Jag
 * 
 * Created on February 11, 2012, 6:20 AM
 */

#include "InstructionItem.h"

InstructionItem::InstructionItem() {
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
    return lineNumber;
}

string InstructionItem::getCommand() {
    return command;
}

string InstructionItem::getArg() {
    return arg;
}

InstructionItem* InstructionItem::getNext() {
    return next;
}
