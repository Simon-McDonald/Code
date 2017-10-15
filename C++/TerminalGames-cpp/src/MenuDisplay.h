/*
 * MenuDisplay.h
 *
 *  Created on: Mar 8, 2016
 *      Author: Simon
 */

#ifndef MENUDISPLAY_H_
#define MENUDISPLAY_H_

#include <string>
#include <vector>

#include "ConsoleWindow.h"

namespace Window {

	class MenuDisplay {
	public:
		MenuDisplay (std::string title);

		void addMenuItem (std::string menuString, void (*menuItemCb) (void));
		void printMenu (void);
		void updateMenu (void);
		void enterMenu (void);

		~MenuDisplay (void);

	private:
		struct menuItem {
			std::string menuString;
			void (*menuItemCb) (void);
		};

		std::string title;
		std::vector<menuItem> menuOptions;
		unsigned int currentIndex;
	};
}

#endif /* MENUDISPLAY_H_ */
