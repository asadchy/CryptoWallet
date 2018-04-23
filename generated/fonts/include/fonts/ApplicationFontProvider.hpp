/* DO NOT EDIT THIS FILE */
/* This file is autogenerated by the text-database code generator */

#ifndef APPLICATIONFONTPROVIDER_HPP
#define APPLICATIONFONTPROVIDER_HPP

#include <touchgfx/FontManager.hpp>

struct Typography
{
  static const touchgfx::FontId TRANSACTION = 0;
  static const touchgfx::FontId KEYBOARD = 1;
  static const touchgfx::FontId MODE = 2;
  static const touchgfx::FontId LOGO = 3;
  static const touchgfx::FontId TRANSACTION_HEAD = 4;
  static const touchgfx::FontId LOGO_TEXT = 5;
  static const touchgfx::FontId BUTTON_TEXT = 6;
  static const touchgfx::FontId ENTERED = 7;
  static const touchgfx::FontId DIALOG = 8;
  static const touchgfx::FontId ALPHA_TEXT_BTN = 9;
  static const touchgfx::FontId ALPHA_TEXT = 10;
  static const touchgfx::FontId CURR_TEXT_T = 11;
  static const touchgfx::FontId BLOCKED = 12;
};

class ApplicationFontProvider : public touchgfx::FontProvider
{
public:
  virtual touchgfx::Font* getFont(touchgfx::FontId fontId);
};

#endif /* APPLICATIONFONTPROVIDER_HPP */
