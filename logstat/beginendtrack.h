#ifndef BEGINENDTRACK_H
#define BEGINENDTRACK_H

#include <chrono>
#include <string>
#include "logstatcontroller.h"

class BeginEndTrack
{
    Log::Scope scope;
    int trackCount;
    std::string name;
    std::chrono::system_clock::time_point begin;
    long long milliSeconds() const;
public:
    BeginEndTrack(const std::string &file, int line, std::string const &n);
    void track(const std::string &file, int line);

};

#endif // BEGINENDTRACK_H
