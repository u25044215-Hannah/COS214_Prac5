#ifndef NOTIFICATIONSERVICE_H
#define NOTIFICATIONSERVICE_H

#include <string>

// Subsystem service: campus-wide broadcast (PA, SMS, app push).
class NotificationService {
public:
    NotificationService();
    void broadcast(const std::string& channel, const std::string& message);
    int sentCount() const;
private:
    int sent_;
};

#endif
