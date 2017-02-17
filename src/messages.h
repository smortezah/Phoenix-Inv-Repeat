#ifndef PHOENIX_MESSAGES_H
#define PHOENIX_MESSAGES_H


#include <iostream>


class Messages
{
public:
    Messages ();                /// constructor
    
    void help () const;         /// show usage guide
    void about () const;        /// show About Phoenix
    void verbose () const;      /// show verbose mode
    void fnumber (float) const; /// show float number mode
};


#endif //PHOENIX_MESSAGES_H