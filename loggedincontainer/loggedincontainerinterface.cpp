#include "loggedincontainerinterface.h"

LoggedInContainerInterface::LoggedInContainerInterface()
{

}

void LoggedInContainerInterface::clear(const sole::uuid &userId)
{
    auto it(loggedInUsers.begin());
    while (it != loggedInUsers.end())
    {
        if (it->second.userId == userId)
        {
            it = loggedInUsers.erase(it);
        }
        else
        {
            ++it;
        }
    }
}
