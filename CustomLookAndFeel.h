/*
  ==============================================================================

    CustomLookAndFeel.h
    Created: 11 Oct 2020 1:21:11am
    Author:  David

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

//==============================================================================
/*
*/
class CustomLookAndFeel : public juce::LookAndFeel_V4


{
public:

	void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider&)
	{
		float diameter = juce::jmin(width, height);
		float radius = diameter / 2;
		float centreX = x + width / 2;
		float centreY = y + height / 2;
		float rx = centreX - radius;
		float ry = centreY - radius;
		float angle = rotaryStartAngle + (sliderPosProportional * (rotaryEndAngle - rotaryStartAngle));

		juce::Rectangle<float> dialArea(rx, ry, diameter, diameter);

		juce::Path dialTick, backgndTick;

		dialTick.addPieSegment(dialArea, rotaryStartAngle, angle, 0.6);
		backgndTick.addPieSegment(dialArea, rotaryStartAngle, rotaryEndAngle, 0.6);

		g.setColour(juce::Colour(20, 31, 51));
		g.fillPath(backgndTick);

		g.setColour(juce::Colour(223, 116, 12));
		g.fillPath(dialTick);
		//PathStrokeType::JointStyle::beveled

	}

	void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle, juce::Slider&) override
	{

		juce::Rectangle<float> sliderArea(x, y, width, height);


		g.setColour(juce::Colour(20, 31, 51));
		g.fillRect(x, y, width, height);

		g.setColour(juce::Colour(223, 116, 12));
		g.fillRect((float)x, sliderPos, (float)width, y + (height - sliderPos));

	}

	void drawLabel(juce::Graphics& g, juce::Label& label) override
	{

		if (!label.isBeingEdited())
		{
			auto alpha = label.isEnabled() ? 1.0f : 0.5f;
			const juce::Font font(12, 1);

			g.setColour(juce::Colour(223, 116, 12));
			g.setFont(font);

			auto textArea = getLabelBorderSize(label).subtractedFrom(label.getLocalBounds());

			g.drawFittedText(label.getText(), textArea, label.getJustificationType(),
				juce::jmax(1, (int)(textArea.getHeight() / font.getHeight())),
				label.getMinimumHorizontalScale());

		}

	}

	void drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
		bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
	{
		auto fontSize = juce::jmin(15.0f, button.getHeight() * 0.75f);
		auto tickWidth = fontSize * 1.1f;

		juce::Colour fillColour = (button.getToggleState() == true ? juce::Colour(223, 116, 12) : juce::Colour(20, 31, 51));
		g.fillAll(fillColour);

		drawTickBox(g, button, 4.0f, (button.getHeight() - tickWidth) * 0.5f,
			tickWidth, tickWidth,
			button.getToggleState(),
			button.isEnabled(),
			shouldDrawButtonAsHighlighted,
			shouldDrawButtonAsDown);

		g.setColour(button.findColour(juce::ToggleButton::textColourId));
		g.setFont(fontSize);

		if (!button.isEnabled())
			g.setOpacity(0.5f);

		g.drawFittedText(button.getButtonText(),
			button.getLocalBounds().withTrimmedLeft(juce::roundToInt(tickWidth) + 10)
			.withTrimmedRight(2),
			juce::Justification::centredLeft, 10);
	}

};


class CustomLookAndFeel2 : public juce::LookAndFeel_V4

{
public:

	void drawLabel(juce::Graphics& g, juce::Label& label) override
	{

		if (!label.isBeingEdited())
		{
			const juce::Font font(14, 0);

			g.setColour(juce::Colour(250, 250, 250));
			g.setFont(font);

			juce::BorderSize<int> borderSize(0, 0, 0, 0);
			label.setBorderSize(borderSize);

			juce::Rectangle<int> rect(label.getX(), label.getY(), label.getWidth(), label.getHeight());

			auto textArea = getLabelBorderSize(label).subtractedFrom(rect);

			g.drawFittedText(label.getText(), textArea, juce::Justification::horizontallyCentred,
				juce::jmax(1, (int)(textArea.getHeight() / font.getHeight())),
				label.getMinimumHorizontalScale());


		}

	}



	void drawComboBox(juce::Graphics& g, int width, int height, const bool isMouseButtonDown, int buttonX, int buttonY, int buttonW, int buttonH, juce::ComboBox& box)
	{

		juce::Rectangle<int> boxBounds(0, 0, width, height);


		g.setColour(juce::Colour(20, 31, 51));
		g.fillRect(boxBounds.toFloat());


	}

	void drawPopupMenuBackground(juce::Graphics& g, int width, int height) override
	{
		auto background = juce::Colour(20, 31, 51);

		g.fillAll(background);
	}

	juce::Font getComboBoxFont(juce::ComboBox& box) override
	{
		juce::Font font(12, 0);

		return font;
	}

	void positionComboBoxText(juce::ComboBox& box, juce::Label& label) override
	{
		label.setBounds(0, 0,
			box.getWidth(),
			box.getHeight() - 2);

		label.setFont(getComboBoxFont(box));

		label.setJustificationType(juce::Justification::centred);
	}

	void drawPopupMenuItem(juce::Graphics& g, const juce::Rectangle<int>& area,
		const bool isSeparator, const bool isActive,
		const bool isHighlighted, const bool isTicked,
		const bool hasSubMenu, const juce::String& text,
		const juce::String& shortcutKeyText,
		const juce::Drawable* icon, const juce::Colour* const textColourToUse) override
	{
		if (isSeparator)
		{
			auto r = area.reduced(5, 0);
			r.removeFromTop(juce::roundToInt((r.getHeight() * 0.5f) - 0.5f));

			g.setColour(findColour(juce::PopupMenu::textColourId).withAlpha(0.3f));
			g.fillRect(r.removeFromTop(1));
		}
		else
		{
			auto textColour = (textColourToUse == nullptr ? findColour(juce::PopupMenu::textColourId)
				: *textColourToUse);

			auto r = area.reduced(1);

			if (isHighlighted && isActive)
			{
				g.setColour(juce::Colour(8, 12, 20));
				g.fillRect(r);

				g.setColour(findColour(juce::PopupMenu::highlightedTextColourId));
			}
			else
			{
				g.setColour(textColour.withMultipliedAlpha(isActive ? 1.0f : 0.5f));
			}

			r.reduce(juce::jmin(5, area.getWidth() / 20), 0);

			auto font = getPopupMenuFont();

			auto maxFontHeight = r.getHeight() / 1.3f;

			if (font.getHeight() > maxFontHeight)
				font.setHeight(maxFontHeight);

			g.setFont(font);

			auto iconArea = r.removeFromLeft(juce::roundToInt(maxFontHeight)).toFloat();

			if (icon != nullptr)
			{
				icon->drawWithin(g, iconArea, juce::RectanglePlacement::centred | juce::RectanglePlacement::onlyReduceInSize, 1.0f);
				r.removeFromLeft(juce::roundToInt(maxFontHeight * 0.5f));
			}
			else if (isTicked)
			{
				auto tick = getTickShape(1.0f);
				g.fillPath(tick, tick.getTransformToScaleToFit(iconArea.reduced(iconArea.getWidth() / 5, 0).toFloat(), true));
			}

			if (hasSubMenu)
			{
				auto arrowH = 0.6f * getPopupMenuFont().getAscent();

				auto x = static_cast<float> (r.removeFromRight((int)arrowH).getX());
				auto halfH = static_cast<float> (r.getCentreY());

				juce::Path path;
				path.startNewSubPath(x, halfH - arrowH * 0.5f);
				path.lineTo(x + arrowH * 0.6f, halfH);
				path.lineTo(x, halfH + arrowH * 0.5f);

				g.strokePath(path, juce::PathStrokeType(2.0f));
			}

			r.removeFromRight(3);
			g.drawFittedText(text, r, juce::Justification::centredLeft, 1);

			if (shortcutKeyText.isNotEmpty())
			{
				auto f2 = font;
				f2.setHeight(f2.getHeight() * 0.75f);
				f2.setHorizontalScale(0.95f);
				g.setFont(f2);

				g.drawText(shortcutKeyText, r, juce::Justification::centredRight, true);
			}
		}
	}

	juce::Label* createSliderTextBox(juce::Slider& slider) override
	{
		auto* l = LookAndFeel_V2::createSliderTextBox(slider);

		if (getCurrentColourScheme() == LookAndFeel_V4::getGreyColourScheme() && (slider.getSliderStyle() == juce::Slider::LinearBar
			|| slider.getSliderStyle() == juce::Slider::LinearBarVertical))
		{
			l->setColour(juce::Label::textColourId, juce::Colours::black.withAlpha(0.7f));
		}

		return l;
	}
};
