/* 
 * File:   InstructionItem.cpp
 * Author: Jag
 * 
 * Created on February 11, 2012, 6:20 AM
 */

#include "InstructionItem.h"

InstructionItem::InstructionItem() {
    this->command = "";
    this->arg = "";
    this->next = NULL;
}

InstructionItem::InstructionItem(string tempCommand, string tempArg) {
    this->command = tempCommand;
    this->arg = tempArg;
    this->next = NULL;
}

InstructionItem::~InstructionItem() { }

void InstructionItem::setCommand(string tempCommand) {
    this->command = tempCommand;
}

void InstructionItem::setArg(string tempArg) {
    this->arg = tempArg;
}

void InstructionItem::setNext(InstructionItem* tempNextPtr) {
    this->next = tempNextPtr;
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
