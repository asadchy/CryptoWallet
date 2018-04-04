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
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>

using namespace touchgfx;

class ListElement : public Container
{
public:
	ListElement();

	void setListElement(const Bitmap &bmp);

	void setCurr(Unicode::UnicodeChar *name);
	void setAmount(Unicode::UnicodeChar *amount);
	void setAmountDollars(Unicode::UnicodeChar *amount);
	void setCurrIco(const Bitmap &bmp);

private:
	Image background;
	Image ico;

	/*
	 * Member Declarations
	 */
	TextAreaWithOneWildcard curr_name;
	TextAreaWithOneWildcard curr_amount;
	TextAreaWithOneWildcard curr_amount_dollars;

	/*
	 * Wildcard Buffers
	 */
	static const uint16_t BUF_SIZE = 16;
	Unicode::UnicodeChar curr_nameBuffer[BUF_SIZE];
	Unicode::UnicodeChar curr_amountBuffer[BUF_SIZE];
	Unicode::UnicodeChar curr_amount_dollarsBuffer[BUF_SIZE];

};

#endif /* LISTELEMENT_HPP_ */
