#include "beginendtrack.h"
#include "logstatcontroller.h"
#include "utils/extstring.h"

BeginEndTrack::BeginEndTrack(std::string const &file, int line, const std::string &n):trackCount(0),
    name(n),
    begin(std::chrono::system_clock::now())
{
    LogStatController::slog(file, line, LogStatController::verbose, std::string("Start: ") + name);
}

void BeginEndTrack::track(std::string const &file, int line)
{
    ++trackCount;
    auto ms(milliSeconds());
    LogStatController::slog(file, line, LogStatController::verbose, std::string("Step ") + ExtString::toString(trackCount) + " " + name + " " + ExtString::toString(ms));
}

long long BeginEndTrack::milliSeconds() const
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - begin).count();
}
