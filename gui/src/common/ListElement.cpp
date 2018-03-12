/*
 * ListElement.cpp
 *
 *  Created on: Feb 28, 2018
 *      Author: artyom
 */

#include <gui/common/ListElement.hpp>


ListElement::ListElement()
	: Container()
{
}

void ListElement::setListElement(const Bitmap &bmp)
{
	background.setBitmap(bmp);
	background.setXY(0,0);

	setHeight(background.getHeight());
	setWidth(background.getWidth());

	add(background);
}
