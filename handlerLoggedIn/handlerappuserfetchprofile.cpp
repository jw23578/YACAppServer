#include "handlerappuserfetchprofile.h"

HandlerAppUserFetchProfile::HandlerAppUserFetchProfile(DatabaseLogics &databaseLogics,
                                                       PistacheServerInterface &serverInterface,
                                                       LoggedInAppUsersContainer &loggedInAppUsersContainer):
    HandlerLoggedInInterface(serverInterface,
                               databaseLogics.getOpi(),
                             methodNames.fetchProfile,
                             TypeGet,
                             loggedInAppUsersContainer),
    databaseLogics(databaseLogics)
{
    addMethod(serverInterface,
              methodNames.fetchMyProfile,
              TypeGet);
}

void HandlerAppUserFetchProfile::method(CurrentContext &context)
{
    rapidjson::Document profile;
    profile.SetObject();
    if (isMethod(methodNames.fetchMyProfile))
    {
        t0002_user fetchProfile;
        if (!fetchProfile.load(context, context.userId))
        {
            answerOk(std::string("could not load profile with id: ") + context.userId.str(), false);
            return;
        }
        ExtRapidJSONWriter jw(profile, profile.GetAllocator());
        jw.addMember("id", fetchProfile.user_id.asString());
        jw.addMember(fetchProfile.message_font_color.name(), fetchProfile.message_font_color);
        jw.addMember(fetchProfile.color.name(), fetchProfile.color);
        jw.addMember(fetchProfile.fstname.name(), fetchProfile.fstname);
        jw.addMember(fetchProfile.surname.name(), fetchProfile.surname);
        jw.addMember(fetchProfile.visible_name.name(), fetchProfile.visible_name);
        jw.addMember(fetchProfile.public_key_base64.name(), fetchProfile.public_key_base64);
        jw.addMember(fetchProfile.image_id.name(), fetchProfile.image_id.asString());
        jw.addMember(fetchProfile.searching_exactly_allowed.name(), fetchProfile.searching_exactly_allowed);
        jw.addMember(fetchProfile.searching_fuzzy_allowed.name(), fetchProfile.searching_fuzzy_allowed);
    }
    if (isMethod(methodNames.fetchProfile))
    {
        MACRO_GetMandatoryUuid(profileId);
        t0002_user fetchProfile;
        if (!fetchProfile.load(context, profileId))
        {
            answerOk(std::string("could not load profile with id: ") + profileId.str(), false);
            return;
        }
        ExtRapidJSONWriter jw(profile, profile.GetAllocator());
        jw.addMember("id", fetchProfile.user_id.asString());
        jw.addMember(fetchProfile.message_font_color.name(), fetchProfile.message_font_color);
        jw.addMember(fetchProfile.color.name(), fetchProfile.color);
        jw.addMember(fetchProfile.visible_name.name(), fetchProfile.visible_name);
        jw.addMember(fetchProfile.public_key_base64.name(), fetchProfile.public_key_base64);
        jw.addMember(fetchProfile.image_id.name(), fetchProfile.image_id.asString());
    }
    answerOk(true, profile);
}
