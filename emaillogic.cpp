#include "emaillogic.h"
#include "jw78smtpwrapper.h"

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
void EMailLogic::sendVerifyTokenMail(const std::string &loginEMail,
                                     const std::string &verifyToken,
                                     const std::string &verifyUrl)
{
    // TODo Implement E-Mails
    sendPleaseVerifyMail(loginEMail,
                         verifyToken,
                         verifyUrl);
}

void EMailLogic::sendPleaseVerifyMail(const std::string &loginEMail,
                                      const std::string &verifyToken,
                                      const std::string &verifyUrl)
{
    jw78::SMTPWrapper *smtp(new jw78::SMTPWrapper);
    smtp->createEmptyEMail("Please Verify your Account",
                           verifyToken +"<br><br>" + verifyUrl,
                           smtpSenderName,
                           smtpSenderEMail);
    smtp->to_addr.push_back(loginEMail);
    smtp->reply_to = smtpReplyTo;
    smtp->host = smtpHost;
    smtp->user = smtpUser;
    smtp->password = smtpPassword;
    smtp->send();
}

void EMailLogic::sendPleaseUpdatePasswordMail(const std::string &loginEMail,
                                              const std::string &updatePasswordToken)
{
    jw78::SMTPWrapper *smtp(new jw78::SMTPWrapper);
    smtp->createEmptyEMail("Change your Password",
                           updatePasswordToken,
                           smtpSenderName,
                           smtpSenderEMail);
    smtp->to_addr.push_back(loginEMail);
    smtp->reply_to = smtpReplyTo;
    smtp->host = smtpHost;
    smtp->user = smtpUser;
    smtp->password = smtpPassword;
    smtp->send();
}
