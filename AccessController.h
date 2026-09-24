#ifndef ACCESSCONTROLLER_H
#define ACCESSCONTROLLER_H

#include <stdexcept>
#include <string>

enum AccessLevel { LEVEL_OPEN, LEVEL_LOCKED, LEVEL_RESTRICTED };

inline std::string toString(AccessLevel l) {
    if (l == LEVEL_OPEN) return "OPEN";
    if (l == LEVEL_LOCKED) return "LOCKED";
    return "RESTRICTED";
}

// Thrown when the underlying access hardware cannot honour a request.
class AccessFault : public std::runtime_error {
public:
    explicit AccessFault(const std::string& msg) : std::runtime_error(msg) {}
};

// ADAPTER: Target interface - what CampusGuard wants to talk to.
class AccessController {
public:
    virtual ~AccessController() {}
    virtual void lock(const std::string& areaId) = 0;
    virtual void unlock(const std::string& areaId) = 0;
    virtual void restrictToStaff(const std::string& areaId) = 0;
    virtual AccessLevel statusOf(const std::string& areaId) const = 0;
};

#endif
