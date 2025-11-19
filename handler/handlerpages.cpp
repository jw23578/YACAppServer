#include "handlerpages.h"

HandlerPages::HandlerPages(PistacheServerInterface &serverInterface,
                           ORMPersistenceInterface &opi):
    PistacheHandlerInterface(serverInterface,
                             opi,
                             "*",
                             MethodInfo::TypeGet,
                             TypeNoLoginNeeded)
{
    appIdNeeded = false;
}

void HandlerPages::method(CurrentContext &context)
{
    if (requestRessource() == "/methods.html")
    {
        std::string answer("<!DOCTYPE html>"
                           "<html lang=\"de\"> "
                           "<head>"
                           "<meta charset=\"utf-8\">"
                           "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
                           "<title>YACAppServer Methods</title>"
                           "</head>"
                           "<body>");
        answer += serverInterface.getBaseUrl() + "<br><br>";
        for (const auto &mi: serverInterface.allMethods)
        {
            answer += mi.getTypeAsString() + " " + mi.getMethod() + "<br>";
        }
        answer += std::string("</body>"
                           "</html>");
        answerOkHtml(answer);
        return;
    }
    if (requestRessource() == "/info.html")
    {
        answerOk("info.html", true);
        return;
    }
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
