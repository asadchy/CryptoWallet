/*
 * ListElement.hpp
 *
 *  Created on: Feb 28, 2018
 *      Author: artyom
 */

#ifndef LISTELEMENT_HPP_
#define LISTELEMENT_HPP_

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/Bitmap.hpp>

using namespace touchgfx;

class ListElement : public Container
{
public:
	ListElement();

	void setListElement(const Bitmap &bmp);

private:
	Image background;
};

#endif /* LISTELEMENT_HPP_ */
