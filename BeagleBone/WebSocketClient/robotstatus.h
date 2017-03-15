#ifndef ROBOTSTATUS
#define ROBOTSTATUS

#include <cstddef>

namespace irys {

const std::size_t STATUS_TEXT_MESSAGE_LENGTH = 31;

/// Structure describing robot's status
///
/// It is sent to the whole swarm from each robot
struct RobotStatus {
    float inclination;      /// robot's tilt angle
    float sensorsData[3];   /// radius distances from sonars
    float linearVelocity;   /// robot's horizontal velocity
    float angularVelocity;  /// robot's wheel's angular velocity
    float positionXYZ[3];   /// robot's position in the area
    float orientation;      /// orientation relative to the XY-axes
    float batteryState;     /// battery state in %
    int id;                 /// robot's ID
    bool busy;              /// tells whether the robot is performing some task
    char textMessage[STATUS_TEXT_MESSAGE_LENGTH];  /// additional message
};

} // namespace irys

#endif // ROBOTSTATUS

