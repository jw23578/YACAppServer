#include "beginendtrack.h"
#include "utils/extstring.h"

BeginEndTrack::BeginEndTrack(std::string const &file, int line, const std::string &n):
    scope(n),
    trackCount(0),
    name(n),
    begin(std::chrono::system_clock::now())
{
    LogStatController::slog(file, line, LogStatController::verbose, std::string("Trackstart: ") + name);
}

void BeginEndTrack::track(std::string const &file, int line)
{
    ++trackCount;
    auto ms(milliSeconds());
    LogStatController::slog(file, line, LogStatController::verbose, std::string("Trackstep ") + ExtString::toString(trackCount) + " " + name + " " + ExtString::toString(ms));
}

long long BeginEndTrack::milliSeconds() const
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - begin).count();
}
