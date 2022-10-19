#include "emaillogic.h"

EMailLogic::EMailLogic(std::string smtpSenderName,
                       std::string smtpSenderEMail,
                       std::string smtpReplyTo,
                       std::string smtpHost,
                       std::string smtpUser,
                       std::string smtpPassword):
    smtpSenderName(smtpSenderName),
    smtpSenderEMail(smtpSenderEMail),
    smtpReplyTo(smtpReplyTo),
    smtpHost(smtpHost),
    smtpUser(smtpUser),
    smtpPassword(smtpPassword)
{

}
