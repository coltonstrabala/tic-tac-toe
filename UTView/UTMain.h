/****************************************
 * 
 * Ultrasonic Flaw Detector - UTMain
 * Author(s): Sam McAnelly, 
 * Oklahoma State University
 * ECEN 4013
 * 
 ****************************************/

#ifndef UTMain_h
#define UTMain_h

#include "UTComponent.h"
#include "UTGraph.h"

class UTMain : public UTComponent {
    public:
        UTMain(Adafruit_SSD1306 *_display, UTGraph *_graph){
            display = _display;
            graphActive = false;
            graph = _graph;
        }

        ~UTMain() override {
            delete graph;
        }

        void create(boolean show);
        void showGraph();

        XYPos_t getCursorPositionLocation();
        XYPos_t getPositionXY(uint8_t position);

        void changeCursorPosition(uint8_t new_position);

        view_t buttonPress(input_t i);
    private:
        UTGraph *graph;
        boolean graphActive; //are we currently interfacing with the graph?

        //main screen input positions
        XYPos_t menuPosition = {2, 53};
        XYPos_t startPosition = {45, 53};
        XYPos_t exportPosition = {89, 53};

        XYPos_t mainInputPositions[MAIN_IO_COUNT] = {menuPosition, startPosition, exportPosition};

        String mainViewStrings[MAIN_IO_COUNT] = {"menu", "start", "export"};

        void setCursor(XYPos_t position);

        view_t leftPress();
        view_t rightPress();
        view_t upPress();
        view_t downPress();
        view_t enterPress(); 

        void drawMainViewButtons();
};

#endif