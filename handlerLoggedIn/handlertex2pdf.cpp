#include "handlertex2pdf.h"
#include "base64.h"
#include "temporaryfile.h"
#include "extrapidjson.h"

HandlerTex2Pdf::HandlerTex2Pdf(ORMPersistenceInterface &opi,
                               PistacheServerInterface &serverInterface,
                               LoggedInAppUsersContainer &loggedInAppUsersContainer):
    HandlerLoggedInInterface(serverInterface,
                             opi,
                             loggedInAppUsersContainer)
{
    addMethod(serverInterface, methodNames.tex2pdf, MethodInfo::TypePost);
}

void HandlerTex2Pdf::method(CurrentContext &context)
{
    MACRO_GetMandatoryString(texBase64);
    std::string tex(base64_decode(texBase64));
    if (!tex.size())
    {
        answerOk("tex2Base64 is not valid base64", false);
        return;
    }
    ExtRapidJSON postData(getPostedData());
    std::vector<std::unique_ptr<TemporaryFile>> imageFiles;
    if (postData.hasArray("images"))
    {
        const rapidjson::Value &images(postData.getObject("images"));
        for (size_t i(0); i < images.Size(); ++i)
        {
            ExtRapidJSON image(images[i]);
            std::string name(image.getString("name"));
            std::string extension(ExtString::extractFileExtension(name, true));
            std::unique_ptr<TemporaryFile> imageFile(new TemporaryFile(extension));
            imageFile->write(base64_decode(image.getString("dataBase64")));
            ExtString::replaceAll(tex, name, imageFile->getFilenameWithExtension());
            imageFiles.push_back(std::move(imageFile));
        }
    }
    if (postData.hasArray("variables"))
    {
        const rapidjson::Value &variables(postData.getObject("variables"));
        for (size_t i(0); i < variables.Size(); ++i)
        {
            const rapidjson::Value &v(variables[i]);
            ExtRapidJSON variable(v);
            ExtString::replaceAll(tex, variable.getString("name"), variable.getString("value"));
        }
    }
    if (postData.hasArray("tableVariables"))
    {
        const rapidjson::Value &tableVariables(postData.getObject("tableVariables"));
        for (size_t i(0); i < tableVariables.Size(); ++i)
        {
            ExtRapidJSON tv(tableVariables[i]);
            std::string name(tv.getString("name"));
            if (tv.hasArray("values"))
            {
                const rapidjson::Value &values(tv.getObject("values"));
                for (size_t c(0); c < values.Size(); ++c)
                {
                    std::string value(values[c].GetString());
                    if (value != "\\hline")
                    {
                        value += " \\\\";
                    }
                    if (c < values.Size() - 1)
                    {
                        value += "\r";
                        value += name;
                    }
                    ExtString::replaceAll(tex, name, value);
                }
            }
        }
    }
    TemporaryFile texFile(".tex");
    texFile.write(tex);
    std::string command("pdflatex ");
    command += texFile.getFilenameWithExtension();
    system(command.c_str());
    TemporaryFile pdfFile(texFile.getFilename(), ".pdf");
    TemporaryFile auxFile(texFile.getFilename(), ".aux");
    TemporaryFile logFile(texFile.getFilename(), ".log");

    rapidjson::Document document;
    document.SetObject();
    ExtRapidJSONWriter answer(document, document.GetAllocator());
    answer.addMember("pdfBase64", base64_encode(pdfFile.readAll()));
    answer.addMember("auxBase64", base64_encode(auxFile.readAll()));
    answer.addMember("logBase64", base64_encode(logFile.readAll()));
    answerOk(pdfFile.exists(), document);
}
