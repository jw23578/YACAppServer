#ifndef EMAILLOGIC_H
#define EMAILLOGIC_H

#include <string>

class EMailLogic
{
    std::string smtpSenderName;
    std::string smtpSenderEMail;
    std::string smtpReplyTo;
    std::string smtpHost;
    std::string smtpUser;
    std::string smtpPassword;
public:
    EMailLogic(std::string smtpSenderName,
               std::string smtpSenderEMail,
               std::string smtpReplyTo,
               std::string smtpHost,
               std::string smtpUser,
               std::string smtpPassword);

    void sendPleaseVerifyMail(const std::string &loginEMail,
                              const std::string &verifyToken);
};

#endif // EMAILLOGIC_H
