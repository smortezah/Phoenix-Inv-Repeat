#ifndef PHOENIX_MESSAGES_H
#define PHOENIX_MESSAGES_H

#include <iostream>


class Messages
{
public:
    Messages ();                                // constructor
    static void help            (void);         // shows usage guide. 'static' => no need to class instance
    static void version         (void);         // shows version number
    static void verbose         (void);         // shows verbose mode
    static void inverted_repeat (void);         // shows inverted_repeat mode
    static void number          (int);          // shows integer number mode
    static void fnumber         (float);        // shows float number mode
};


#endif //PHOENIX_MESSAGES_H
