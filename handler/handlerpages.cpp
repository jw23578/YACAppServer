#include "handlerpages.h"

HandlerPages::HandlerPages(PistacheServerInterface &serverInterface):
    PistacheHandlerInterface(serverInterface,
                             "*",
                             TypeGet,
                             TypeNoLoginNeeded)
{

}

void HandlerPages::method()
{
    if (requestRessource() != "/index.html")
    {
        answerBad("here only index.html is served");
        return;
    }
    std::string answer("<!DOCTYPE html>"
                       "<html lang=\"de\"> "
                       "<head>"
                       "<meta charset=\"utf-8\">"
                       "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
                       "<title>YACAppServer</title>"
                       "</head>"
                       "<body>"
                       "Hi, this is the YACAppServer"
                       "</body>"
                       "</html>");
    answerOkHtml(answer);
}
