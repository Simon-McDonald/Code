/*
 * MenuInstance.cpp
 *
 *  Created on: Dec 8, 2017
 *      Author: Simon
 */

#include "MenuInstance.hpp"

const GLColour<GLfloat> MenuInstance::selectedColour = {0.0, 0.0, 1.0};
const GLColour<GLfloat> MenuInstance::defaultColour = {0.0, 0.0, 0.0};

MenuInstance::MenuInstance(std::vector<std::pair<InstanceType, std::string>> itemNames) :
        textImageInst("TEXT"), menuIndex(0), title(7, 25), blockTextureId(-1u) {

    items.reserve(itemNames.size());
    instanceTypes.reserve(itemNames.size());

    for (size_t vectorIdx = 0; vectorIdx < itemNames.size(); ++vectorIdx) {
        items.emplace_back(&this->textImageInst, itemNames.at(vectorIdx).second, 0.0,
            0.15 - vectorIdx * 0.15, 1.5, 1.5, TextImage::TextAlignment::CENTRE,
            TextImage::TextAlignment::CENTRE, 0.0, 0.0, MenuInstance::defaultColour);
        instanceTypes.emplace_back(itemNames.at(vectorIdx).first);
    }

    this->updateSelectedColours(menuIndex, menuIndex);

    // Setup the title. Could be done more cleanly.
    blockTextureId = ResourceManager::loadTexture("block.bmp");

    //150: xxxxxxxxxxxxxxxxxxxxxxxxx : 174
    //125: xxRRRxBBBxRRRxYYxxGxxOOxx : 149
    //100: xxxRxxBxxxxRxxYxYxGxOxxxx : 124
    //75:  xxxRxxBBxxxRxxYYxxGxxOxxx :  99
    //50:  xxxRxxBxxxxRxxYxYxGxxxOxx :  74
    //25:  xxxRxxBBBxxRxxYxYxGxOOxxx :  49
    //0:   xxxxxxxxxxxxxxxxxxxxxxxxx :  24
    this->title.insertIntoBottom(0, {255, 0, 0}, {28, 53, 78, 103, 127, 128, 129}); // T
    this->title.insertIntoBottom(0, {0, 0, 255}, {31, 32, 33, 56, 81, 82, 106, 131, 132, 133}); // E
    this->title.insertIntoBottom(0, {255, 0, 0}, {36, 61, 86, 111, 135, 136, 137}); // T
    this->title.insertIntoBottom(0, {255, 255, 0}, {39, 41, 64, 66, 89, 90, 114, 116, 139, 140}); // R
    this->title.insertIntoBottom(0, {0, 255, 0}, {43, 68, 93, 118, 143}); // I
    this->title.insertIntoBottom(0, {255, 165, 0}, {45, 46, 72, 96, 120, 146, 147}); // S
    this->title.insertIntoBottom(0, {150, 150, 150}, {0, 24, 150, 174}); // Corners
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

    if (userInput.onClick.enter) {
        return false;
    }

	return true;
}

void MenuInstance::render(void) {
    this->title.setGridDimsInShader(); // sets 'uWindowDims'

    this->getShaderManager().setUniformInt("uBlockMod", this->title.getWidth());
    this->getShaderManager().bindVector2i("uPieceOffset", 0, 0);

    this->getShaderManager().setUniformBool("uUniformSettings", false);
    this->getShaderManager().setUniformInt("uGridWidth", this->title.getWidth());

    GLint dims[] = {1, 1};
    MenuInstance::getShaderManager().bindVector2iv("uTextureDims", 1, &dims[0]);
    MenuInstance::getShaderManager().bindTextureArray("gSamplerArray", 1, &this->blockTextureId, 0);

    GLfloat windowCentre[] = {-1.0, 0.35};
    GLfloat windowDims[] = {2.0, 2.0f * this->title.getHeight() / this->title.getWidth()};

    this->getShaderManager().bindVector2("uWindowSize", windowDims[0], windowDims[1]);
    this->getShaderManager().bindVector2("uWindowPos", windowCentre[0], windowCentre[1]);

    this->title.render();
}

void MenuInstance::renderText(void) {
    for (RenderableText &text : this->items) {
        text.Render();
    }
}

void MenuInstance::renderBackground(void) {
	// pass
}

InstanceType MenuInstance::endState(void) {
    return instanceTypes.at(this->menuIndex);
}

void MenuInstance::updateSelectedColours(size_t oldSelectedIdx, size_t newSelectedIdx) {
    this->items.at(oldSelectedIdx).setColour(MenuInstance::defaultColour);
    this->items.at(oldSelectedIdx).setSize(1.5, 1.5);
    this->items.at(newSelectedIdx).setColour(MenuInstance::selectedColour);
    this->items.at(newSelectedIdx).setSize(2.0, 2.0);
}

MenuInstance::~MenuInstance(void) {
    ResourceManager::deleteTexture(this->blockTextureId);
    this->blockTextureId = -1u;
}
