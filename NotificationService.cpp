#include "NotificationService.h"
#include <iostream>

NotificationService::NotificationService() : sent_(0) {}

void NotificationService::broadcast(const std::string& channel, const std::string& message) {
    ++sent_;
    std::cout << "  [Notify:" << channel << "] " << message << "\n";
}

int NotificationService::sentCount() const { return sent_; }
