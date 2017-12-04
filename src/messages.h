#ifndef PHOENIXIR_MESSAGES_H
#define PHOENIXIR_MESSAGES_H

#include <iostream>


class Messages
{
public:
    Messages () = default;                                 // constructor
    
    void help    () const;                                 // show usage guide
    void about   () const;                                 // show About Phoenix
    void verbose () const;                                 // show verbose mode
};

#endif //PHOENIXIR_MESSAGES_H