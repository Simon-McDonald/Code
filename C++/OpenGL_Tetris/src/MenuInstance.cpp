/*
 * MenuInstance.cpp
 *
 *  Created on: Dec 8, 2017
 *      Author: Simon
 */

#include "MenuInstance.hpp"

const GLColour<GLfloat> MenuInstance::selectedColour = {0.0, 0.0, 1.0};
const GLColour<GLfloat> MenuInstance::defaultColour = {0.0, 0.0, 0.0};

MenuInstance::MenuInstance(std::vector<std::string> itemNames) : textImageInst("TEXT"), menuIndex(0) {
    items.reserve(itemNames.size());

    for (size_t vectorIdx = 0; vectorIdx < itemNames.size(); ++vectorIdx) {
        items.emplace_back(&this->textImageInst, itemNames.at(vectorIdx), 0.0,
            0.0 - vectorIdx * 0.2, 1.5, 1.5, TextImage::TextAlignment::CENTRE,
            TextImage::TextAlignment::CENTRE, 0.0, 0.0, MenuInstance::defaultColour);
    }

    this->updateSelectedColours(menuIndex, menuIndex);
}

void MenuInstance::updateSelectedColours(size_t oldSelectedIdx, size_t newSelectedIdx) {
    this->items.at(oldSelectedIdx).setColour(MenuInstance::defaultColour);
    this->items.at(oldSelectedIdx).setSize(1.5, 1.5);
    this->items.at(newSelectedIdx).setColour(MenuInstance::selectedColour);
    this->items.at(newSelectedIdx).setSize(2.0, 2.0);
}

bool MenuInstance::update(double deltaTime_ms, const UserInputStruct & userInput) {
    size_t newIdx = menuIndex;

    if (userInput.onClick.up) {
        newIdx = (menuIndex + items.size() - 1) % items.size();
    } else if (userInput.onClick.down) {
        newIdx = (menuIndex + 1) % items.size();
    }

    if (newIdx != menuIndex) {
        this->updateSelectedColours(menuIndex, newIdx);
        menuIndex = newIdx;
    }

    INFO << "Update, menuIndex: " << menuIndex << END;
	return true;
}

bool MenuInstance::render(void) {
	// Render block based stuff
	return true;
}

bool MenuInstance::renderText(void) {

    for (RenderableText &text : this->items) {
        text.Render();
    }

	return true;
}

bool MenuInstance::renderBackground(void) {
	// Render window based stuff
	return true;
}

MenuInstance::~MenuInstance(void) {}


