/*
 * EnumMacro.h
 *
 *  Created on: Feb 6, 2016
 *      Author: Simon
 */

#ifndef ENUMMACRO_H_
#define ENUMMACRO_H_

#include <string>
#include <string.h>

/**
 * To use the DECLARE_ENUM macro, define list of enum states as follows:
 *   #define SUIT_ENUM_LIST(X) \
 *     X(HEARTS), \
 *     X(CLUBS), \
 *     X(SPADES), \
 *     X(DIAMONDS)
 *
 * Use the DECLARE_ENUM macro:
 *   DECLARE_ENUM(suits, SUIT_ENUM_LIST)
 *
 * Using the enum:
 *   suitsEnum var_name = suits::CLUBS;
 *
 * The following functions are defined for the enum:
 *   suitsEnum succ (suitsEnum);
 *   suitsEnum pred (suitsEnum);
 *   std::string enumToString (suitsEnum);
 *   suitsEnum stringToEnum (std::string);
 */

#define _PLAIN(x) x
#define _STRINGY(x) #x

#define DECLARE_ENUM(typeName, enumList) \
	namespace typeName { \
		enum typeName##EnumValues { \
			enumList(_PLAIN), \
			EnumLimit \
		}; \
	} \
	typedef typeName::typeName##EnumValues typeName##Enum; \
	typeName##Enum succ (typeName##Enum currEnum) { \
		typeName##Enum nextEnum = typeName##Enum (currEnum + 1); \
		if (nextEnum == typeName::EnumLimit) { \
			nextEnum = typeName##Enum (0); \
		} \
		return nextEnum; \
	} \
	typeName##Enum pred (typeName##Enum currEnum) { \
		typeName##Enum prevEnum; \
		if (currEnum == 0) { \
			prevEnum = typeName##Enum (typeName::EnumLimit - 1); \
		} else { \
			prevEnum = typeName##Enum (currEnum + 1); \
		} \
		return prevEnum; \
	} \
	std::string enumToString (typeName##Enum enumValue) { \
		const char *enumStringList[] = { enumList(_STRINGY) }; \
		if (enumValue >= typeName::EnumLimit) { \
			return std::string("Invalid Enum Index"); \
		} \
		return enumStringList[enumValue]; \
	} \
	typeName##Enum stringToEnum (std::string enumString) { \
		const char *enumStringList[] = { enumList(_STRINGY) }; \
		typeName##Enum enumIdx = typeName##Enum (0); \
		do { \
			if (!strcmp (enumStringList[enumIdx], enumString.c_str())) { \
				return enumIdx; \
			} \
			enumIdx = succ (enumIdx); \
		} while (enumIdx != typeName##Enum (0)); \
		return typeName::EnumLimit; \
	}

#endif /* ENUMMACRO_H_ */
