#ifndef PHOENIX_MESSAGES_H
#define PHOENIX_MESSAGES_H

#include <iostream>


class Messages
{
public:
    Messages ();                    // constructor
    void help            (void);    // shows usage guide. 'static' => no need to class instance
    void version         (void);    // shows version number
    void verbose         (void);    // shows verbose mode
    void inverted_repeat (void);    // shows inverted_repeat mode
    void number          (int);     // shows integer number mode
    void fnumber         (float);   // shows float number mode
};


#endif //PHOENIX_MESSAGES_H
