#ifndef PHOENIX_MESSAGES_H
#define PHOENIX_MESSAGES_H


class Messages
{
public:
    Messages ();                // constructor
    static void help    (void); // shows usage guide. 'static' => no need to class instance
    static void version (void); // shows version number
    static void verbose (void); // shows verbose mode
};


#endif //PHOENIX_MESSAGES_H
