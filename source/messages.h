#ifndef PHOENIX_MESSAGES_H
#define PHOENIX_MESSAGES_H


#include <iostream>


class Messages
{
public:
    Messages ();            // constructor
    void help ();           // show usage guide
    void about ();          // show About Phoenix
    void verbose ();        // show verbose mode
    void number (int);      // show integer number mode
    void fnumber (float);   // show float number mode
};


#endif //PHOENIX_MESSAGES_H
