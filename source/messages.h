#ifndef PHOENIX_MESSAGES_H
#define PHOENIX_MESSAGES_H


#include <iostream>


class Messages
{
public:
    Messages ();            // constructor
    void help (void);       // show usage guide
    void about (void);      // show About Phoenix
    void verbose (void);    // show verbose mode
    void number (int);      // show integer number mode
    void fnumber (float);   // show float number mode
};


#endif //PHOENIX_MESSAGES_H
