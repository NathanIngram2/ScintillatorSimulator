#pragma once

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <stdlib.h>

#include "./MenuOptions.h"
#include "./CommandIf.h"
#include "./CommandFactory.h"

class UI
{
public:
    UI();
    ~UI();
    UI(const UI&) = delete;
	void operator=(const UI&) = delete;

    void displayResponse(std::vector<std::string>) const;
    std::string retrieveUserInput();
    void printUI() const;

private:
    CommandFactory* m_commandFactory;
    std::map<MenuOptions, CommandIf*>* m_commandMap;
};