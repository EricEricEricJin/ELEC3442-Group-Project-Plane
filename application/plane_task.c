#include <stdlib.h>
#include "plane_task.h"
#include "board.h"
#include "pid.h"
#include "communication.h"
#include "ctrl_surface.h"

#define TASK_CYCLE_PERIOD 5

struct pid_param aileron_param = 
{
    .p = 1.0f,
    .i = 0.1f,
    .max_out = 1.0f,
    .integral_limit = 0.1f
};

struct pid_param rudder_param = 
{
    .p = 1.0f,
    .i = 0.1f,
    .max_out = 1.0f,
    .integral_limit = 0.1f
};

struct pid_param elevator_param = 
{
    .p = 1.0f,
    .i = 0.1f,
    .max_out = 1.0f,
    .integral_limit = 0.1f
};

struct communication comm;
struct ground_cmd cmd;
struct plane_data data;

const char* IP = "127.0.0.1";
const uint16_t PORT = 1234;

struct ctrl_surface ctrl_aileron, ctrl_elevator, ctrl_rudder;
struct servo servo_aileron_left, servo_aileron_right, servo_elevator, servo_rudder;

void plane_task(void const* argument)
{
    // Initialize controllers
    ctrl_surface_init(&ctrl_aileron, aileron_param);
    ctrl_surface_init(&ctrl_elevator, elevator_param);
    ctrl_surface_init(&ctrl_rudder, rudder_param);

    // Initialize servos
    servo_init(&servo_aileron_left, SERVO_180DEG, 0, 0, 45, -45);
    servo_init(&servo_aileron_right, SERVO_180DEG, 1, 0, -45, 45);
    servo_init(&servo_elevator, SERVO_180DEG, 2, 0, 45, -45);
    servo_init(&servo_rudder, SERVO_180DEG, 3, 0, 45, -45);

    // Initialize communication
    communication_init(&comm, &cmd, &data, IP, PORT, 0xffff);
    communication_set_send_rate(&comm, 50000);
    communication_start(&comm);

    int comm_state;
    float aileron_out, elevator_out, rudder_out;

    while (1)
    {
        // Disable actuators if plane offline
        if (communication_get_state(&comm) & COMM_RECV_TIMEOUT)
        {
            servo_turn_off(&servo_aileron_left);
            servo_turn_off(&servo_aileron_right);
            servo_turn_off(&servo_elevator);
            servo_turn_off(&servo_rudder);
            goto task_loop_end;
        }
        
        // Keep servos on
        servo_turn_on(&servo_aileron_left);
        servo_turn_on(&servo_aileron_right);
        servo_turn_on(&servo_elevator);
        servo_turn_on(&servo_rudder);
    
        // update controller 
        ctrl_surface_set_input(&ctrl_aileron, cmd.aileron);
        ctrl_surface_set_feedback(&ctrl_aileron, data.angle_x, data.omega_x);

        ctrl_surface_set_input(&ctrl_elevator, cmd.elevator);
        ctrl_surface_set_feedback(&ctrl_elevator, data.angle_y, data.omega_y);

        ctrl_surface_set_input(&ctrl_rudder, cmd.rudder);
        ctrl_surface_set_feedback(&ctrl_rudder, data.angle_z, data.omega_z);
        // Currently use imu heading, change to magnetic heading later

        // Calculate outputs
        aileron_out = ctrl_surface_calculate(&ctrl_aileron);
        elevator_out = ctrl_surface_calculate(&ctrl_elevator);
        rudder_out = ctrl_surface_calculate(&ctrl_rudder);
        
        // Apply to actuators
        servo_set_deg_trimmed(&servo_aileron_left, aileron_out * 45.0f);
        servo_set_deg_trimmed(&servo_aileron_right, aileron_out * (-45.0f));
        servo_set_deg_trimmed(&servo_elevator, elevator_out * 45.0f);
        servo_set_deg_trimmed(&servo_rudder, rudder_out * 45.0f);

        task_loop_end:
        usleep(TASK_CYCLE_PERIOD); // change to sleep_to later
    }
    
}