/****************************************
 * 
 * Ultrasonic Flaw Detector - UTAdjuster
 * Author(s): Sam McAnelly, 
 * Oklahoma State University
 * ECEN 4013
 * 
 ****************************************/

#include "UTAdjuster.h"

/*
 * 
 * 
 *      Public Functions
 * 
 * 
 * */

void UTAdjuster::create(boolean show) {
    adjusting = false;
    updateAdjusterDigitStrings();

    drawAdjusterContainer();
    drawAdjusterButtons();
    drawAdjusterDigits();
    drawAdjusterUnits();

    changeCursorPosition(input_cursor_position);

    if(show)
        display->display();
}

XYPos_t UTAdjuster::getCursorPositionLocation() {
    return adjusterInputPositions[input_cursor_position];
}

XYPos_t UTAdjuster::getPositionXY(uint8_t position) {
    return adjusterInputPositions[position];
}

void UTAdjuster::changeCursorPosition(uint8_t new_position) {
    XYPos_t *positions;
    uint8_t *cursor_position;
    String *display_strings;

    if(adjusting) {
        positions = adjusterDigitPositions;
        cursor_position = &digit_cursor_position;
        display_strings = adjusterDigitStrings;

        display->setTextSize(2);

        //check bounds
        if(new_position == ADJUSTMENT_VALUE_DIGIT_COUNT) new_position = 0;
        else if(new_position < 0) new_position = ADJUSTMENT_VALUE_DIGIT_COUNT - 1;
    } else {
        Serial.println("Not adjusting...");
        positions = adjusterInputPositions;
        cursor_position = &input_cursor_position;
        display_strings = adjusterViewStrings;

        //check bounds
        if(new_position == ADJUSTMENT_IO_COUNT) new_position = 0;
        else if(new_position < 0) new_position = ADJUSTMENT_IO_COUNT - 1;
    }


    Serial.print("new cursor position:");
    Serial.println(new_position);

    display->setCursor(positions[*cursor_position].x, positions[*cursor_position].y);
    display->setTextColor(WHITE, BLACK);
    display->println(display_strings[*cursor_position]);
    *cursor_position = new_position;

    setCursor(positions[*cursor_position]);
    
    display->display();
    display->setTextSize(1);
}

/*
 * 
 * 
 *      Private Functions
 * 
 * 
 * */

void UTAdjuster::setCursor(XYPos_t position) {
    //sets cursor to draw, not input cursor
    display->setCursor(position.x, position.y);
    display->setTextColor(BLACK, WHITE);

    if(adjusting) {
        display->println(adjusterDigitStrings[digit_cursor_position]);
    } else {
        display->println(adjusterViewStrings[input_cursor_position]);
    }
}

view_t UTAdjuster::leftPress() {
    if(adjusting) {
        if(digit_cursor_position == 0)
            changeCursorPosition(ADJUSTMENT_VALUE_DIGIT_COUNT - 1);
        else
            changeCursorPosition(digit_cursor_position - 1);
    } else {
        if(input_cursor_position == 0)
            changeCursorPosition(ADJUSTMENT_IO_COUNT - 1);
        else
            changeCursorPosition(input_cursor_position - 1);
    }
    return NONE;
}

view_t UTAdjuster::rightPress() {
    if(adjusting) {
        changeCursorPosition(digit_cursor_position + 1);
    } else {
        changeCursorPosition(input_cursor_position + 1);
    }
    
    return NONE;
}

view_t UTAdjuster::upPress() {
    if(adjusting) {
        incrementAdjusterDigit(digit_cursor_position);
    } else {
        adjusting = true;
        //clear old cursor (this might need to be refactored)
        display->setCursor(adjusterInputPositions[input_cursor_position].x, adjusterInputPositions[input_cursor_position].y);
        display->setTextColor(WHITE, BLACK);
        display->println(adjusterViewStrings[input_cursor_position]);
        changeCursorPosition(digit_cursor_position);
    }
    return NONE;
}

view_t UTAdjuster::downPress() {
    if(adjusting) {
        decrementAdjusterDigit(digit_cursor_position);
    }
    return NONE;
}

view_t UTAdjuster::enterPress() {
    switch(input_cursor_position) {
        case 0: //
            //switchView(MENU, 0);
            break;
        case 1: //
            break;
    }
    return BACK;
}

void UTAdjuster::drawAdjusterContainer()
{
    display->fillRect(20, 2, 100, 40, BLACK);
    display->drawRect(20, 2, 100, 40, WHITE);
    display->fillRect(20, 2, 100, 5, WHITE);
    display->display();
}


void UTAdjuster::drawAdjusterButtons() {
    display->setTextColor(WHITE, BLACK);

    for(uint8_t i = 0; i < ADJUSTMENT_STRING_COUNT; i++) {
        display->setCursor(adjusterInputPositions[i].x, adjusterInputPositions[i].y);
        display->println(adjusterViewStrings[i]);
    }
}

void UTAdjuster::drawAdjusterDigits() {
    Serial.println("Drawing digits...");
    display->setTextColor(WHITE, BLACK);
    display->setTextSize(2);

    display->setCursor(decimal_position.x, decimal_position.y);
    display->println(".");

    for(uint8_t i = 0; i < ADJUSTMENT_VALUE_DIGIT_COUNT; i++) {
        display->setCursor(adjusterDigitPositions[i].x, adjusterDigitPositions[i].y);
        display->println(adjusterDigitStrings[i]);
    }

    display->setTextSize(1);
}

void UTAdjuster::drawAdjusterUnits() {
    display->setCursor(unit_position.x, unit_position.y);
    display->println(units);
}

void UTAdjuster::updateAdjusterDigitStrings() {
    for(uint8_t i = 0; i < ADJUSTMENT_VALUE_DIGIT_COUNT; i++) {
        adjusterDigitStrings[i] = String(adjustment_value_digits[i]);
    }
}

void UTAdjuster::incrementAdjusterDigit(uint8_t digit) {
    if(adjustment_value_digits[digit] == 9)
        adjustment_value_digits[digit] = 0;
    else
        adjustment_value_digits[digit] += 1;

    updateAdjusterDigitStrings();
    updateAdjusterDigit(digit);
}

void UTAdjuster::decrementAdjusterDigit(uint8_t digit) {
    if(adjustment_value_digits[digit] == 0)
        adjustment_value_digits[digit] = 9;
    else
        adjustment_value_digits[digit] -= 1;

    updateAdjusterDigitStrings();
    updateAdjusterDigit(digit);
}

void UTAdjuster::updateAdjusterDigit(uint8_t digit) {
    display->setTextSize(2);
    display->setCursor(adjusterDigitPositions[digit].x, adjusterDigitPositions[digit].y);
    display->println(adjusterDigitStrings[digit]);
    display->display();
    display->setTextSize(1);
}

void UTAdjuster::saveAdjustmentValue() {

}

void UTAdjuster::discardAdjustmentValue() {

}