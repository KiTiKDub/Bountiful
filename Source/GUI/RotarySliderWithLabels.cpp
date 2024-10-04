/*
  ==============================================================================

    rotarySliderWithLabels.cpp
    Created: 6 Nov 2023 5:57:57pm
    Author:  kylew

  ==============================================================================
*/

#include "RotarySliderWithLabels.h"

void RotarySliderWithLabels::paint(juce::Graphics& g) {
    using namespace juce;

    auto startAng = degreesToRadians(180.f + 45.f);
    auto endAng = degreesToRadians(180.f - 45.f) + MathConstants<float>::twoPi;

    auto range = getRange();

    auto sliderBounds = getSliderBounds();

    //auto bounds = getLocalBounds();
    g.setColour(Colours::white);

    getLookAndFeel().drawRotarySlider(g,
        sliderBounds.getX(),
        sliderBounds.getY(),
        sliderBounds.getWidth(),
        sliderBounds.getHeight(),
        jmap(getValue(), range.getStart(), range.getEnd(), 0.0, 1.0),
        startAng,
        endAng,
        *this);

    auto center = sliderBounds.toFloat().getCentre();
    auto radius = sliderBounds.getHeight() * .5f;

    g.setColour(Colours::white);

    //g.drawRect(sliderBounds);
    //g.setColour(juce::Colours::orange);
    //g.drawRect(bounds);

    auto numChoices = labels.size();

    for(int i = 0; i < numChoices; ++i)
    {
        Rectangle<float> r;
        Point<float> c;
        //float ang;

        auto pos = labels[i].pos;
        auto str = labels[i].label;
        auto textHeight = labels[i].fontSize;
        g.setFont(textHeight);
        auto strWidth = g.getCurrentFont().getStringWidth(str);
        //float extraPush = 0;
        
        r.setSize(strWidth, textHeight); //draww text on edge of slider bounds, or create a slightly bigger bounds and draw them on that

        if (pos == 1) //Will need to do something based on ratios. Normal sliders this does not work for 2 & 4
        {
            c = center.getPointOnCircumference(radius + textHeight*.8, 0); //this may also be bad for smaller sliders
            r.setCentre(c);
        }
        else if (pos == 2)
        {
            c = center.getPointOnCircumference(radius + strWidth/2, MathConstants<float>::pi / 2);
            r.setCentre(c);
            
        }
        else if (pos == 3)
        {
            c = center.getPointOnCircumference(radius, MathConstants<float>::pi);
            r.setCentre(c);
        }
        else 
        {
            c = center.getPointOnCircumference(radius + strWidth/2, 3 * MathConstants<float>::pi / 2);
            r.setCentre(c);
        }

        //g.drawRect(r);

        g.drawFittedText(str, r.toNearestInt(), juce::Justification::centred, 2, 1);
    }
}

juce::Rectangle<int> RotarySliderWithLabels::getSliderBounds() const {

    auto bounds = getLocalBounds();

    auto reduceWidth = juce::jmax(bounds.getWidth() * .15, (double)18); //Need to reduce this so that they are square
    auto reduceHeight = juce::jmax(bounds.getHeight() * .15, (double)18);
    auto boundsShrink = bounds.reduced(reduceWidth, reduceHeight);

    return boundsShrink;
}


void RotarySliderWithLabels::changeParam(juce::RangedAudioParameter* p)
{
    param = p;
    repaint();
}

juce::String getValString(const juce::RangedAudioParameter& param, bool isName, juce::String suffix, std::vector<juce::String> vector)
{
    juce::String str;

    auto name = param.getName(20);
    auto val = param.getValue();
    auto range = param.getNormalisableRange();
    auto normVal = juce::jmap(val, 0.f, 1.f, range.start, range.end);
    normVal = floor(normVal * 100);
    normVal /= 100;

    if (isName)
    {
        str << name;
    }
    else
    {
        if (vector.size() > 0)
        {
            auto type = vector.at(normVal);
            str << type;
        }
        else
        {
            str << normVal << suffix;
        }
    }

    return str;
}
