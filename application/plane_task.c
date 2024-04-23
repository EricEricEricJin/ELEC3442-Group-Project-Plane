/******************************************************************************
 * Copyright (C) 2024 Eric Jin <https://github.com/ericericericjin>
 *
 * Everyone is permitted to copy and distribute verbatim or modified copies
 * of this program, and changing it is allowed as long as the name is changed.
 *****************************************************************************/

#include <stdlib.h>
#include "plane_task.h"
#include "board.h"
#include "pid.h"
#include "communication.h"
#include "ctrl_surface.h"

#include "plane_data.h"
#include "ground_cmd.h"
#include "shared_mem.h"
#include "msg_id.h"
#include "esc.h"

#define TASK_CYCLE_PERIOD 5

#define PITCH_MAX_DEGPMS (0.03f)
#define ROLL_MAX_DEGPMS (0.03f)
#define YAW_MAX_DEGPMS (0.03f)

struct pid_param aileron_inter_param =
{
    .p = 1.0f,
    .i = 0.1f,
    .max_out = 1.0f,
    .integral_limit = 0.1f
};

struct pid_param aileron_outer_param =
{
    .p = 1.0f,
    .i = 0.1f,
    .max_out = 1.0f,
    .integral_limit = 0.1f
};

struct pid_param rudder_inter_param =
{
    .p = 1.0f,
    .i = 0.1f,
    .max_out = 1.0f,
    .integral_limit = 0.1f
};

struct pid_param rudder_outer_param =
{
    .p = 1.0f,
    .i = 0.1f,
    .max_out = 1.0f,
    .integral_limit = 0.1f
};

struct pid_param elevator_inter_param =
{
    .p = 1.0f,
    .i = 0.1f,
    .max_out = 1.0f,
    .integral_limit = 0.1f
};

struct pid_param elevator_outer_param =
{
    .p = 1.0f,
    .i = 0.1f,
    .max_out = 1.0f,
    .integral_limit = 0.1f
};

struct communication comm;
struct ground_cmd cmd;
struct plane_data data;
struct fdbk_data fdbk;

// const char *IP = "127.0.0.1";
// const uint16_t PORT = 1234;

struct ctrl_surface ctrl_aileron, ctrl_elevator, ctrl_rudder;
struct servo servo_aileron_left, servo_aileron_right, servo_elevator, servo_rudder;
struct esc engine_1, engine_2;

void offline_task();

int offline = 0;

void plane_task(void const *argument)
{
    // Initialize controllers
    ctrl_surface_init(&ctrl_aileron, aileron_inter_param, aileron_outer_param);
    ctrl_surface_init(&ctrl_elevator, elevator_inter_param, elevator_outer_param);
    ctrl_surface_init(&ctrl_rudder, rudder_inter_param, rudder_outer_param);

    // Initialize servos
    servo_init(&servo_aileron_left, SERVO_180DEG, 1, 0, -45, 45);
    servo_init(&servo_aileron_right, SERVO_180DEG, 0, 0, 45, -45);
    servo_init(&servo_elevator, SERVO_180DEG, 4, 0, 45, -45);
    servo_init(&servo_rudder, SERVO_180DEG, 3, 0, 45, -45);

    // Initialize engines
    esc_init(&engine_1, 14, CURRENT_CH1, 1000, 2000);
    esc_init(&engine_2, 15, CURRENT_CH2, 1000, 2000);

    // // Initialize communication
    // communication_init(&comm, &cmd, &data, IP, PORT, 0xffff);
    // communication_set_send_rate(&comm, 50000);
    // communication_start(&comm);

    int comm_state;
    float aileron_out, elevator_out, rudder_out;

    // struct plane_data data;
    // struct ground_cmd cmd;

    float locked_pitch, locked_roll, locked_yaw;

    while (1)
    {
        // Update command and data
        shared_mem_get(CMD_MSG_ID, (void*)&cmd);
        shared_mem_get(DATA_MSG_ID, (void*)&data);

        // printf("t1 = %d\tt2 = %d\n", cmd.thrust_1, cmd.thrust_2);

        // consider as plane offlined if command error or time out
        // if (board_get_millis() - cmd.update_time_ms > CMD_MAX_DELAY_MS)
        // {
        //     offline_task();
        //     goto task_loop_end;
        // }

        // // force to manual mode if cannot get sensor data
        // if (board_get_millis() - data.update_time_ms > DATA_MAX_DELAY_MS)
        // {
        //     cmd.opmode_elevator = OPMODE_MANUAL;
        //     cmd.opmode_aileron = OPMODE_MANUAL;
        //     cmd.opmode_rudder = OPMODE_MANUAL;
        // }

        offline = 0;

        // Keep servos on
        servo_turn_on(&servo_aileron_left);
        servo_turn_on(&servo_aileron_right);
        servo_turn_on(&servo_elevator);
        servo_turn_on(&servo_rudder);

        // update elevator
        ctrl_surface_set_mode(&ctrl_aileron, cmd.opmode_elevator);
        ctrl_surface_set_feedback(&ctrl_elevator, data.pitch, data.w_y);
        if (cmd.opmode_elevator == CTRL_SURFACE_MODE_LOCK_ATT)
        {
            locked_pitch += cmd.elevator / 32768.0f * (PITCH_MAX_DEGPMS * TASK_CYCLE_PERIOD);
            ctrl_surface_set_input(&ctrl_elevator, locked_pitch);
        }
        else
        {
            locked_pitch = data.pitch;
            ctrl_surface_set_input(&ctrl_elevator, cmd.elevator / 32768.0f);
        }

        // update aileron
        ctrl_surface_set_mode(&ctrl_aileron, cmd.opmode_aileron);
        ctrl_surface_set_feedback(&ctrl_aileron, data.roll, data.w_x);
        if (cmd.opmode_aileron == CTRL_SURFACE_MODE_LOCK_ATT)
        {
            locked_roll += cmd.aileron / 32768.0f * (ROLL_MAX_DEGPMS * TASK_CYCLE_PERIOD);
            ctrl_surface_set_input(&ctrl_aileron, locked_roll);
        }
        else 
        {
            locked_roll = data.roll;
            ctrl_surface_set_input(&ctrl_aileron, cmd.aileron / 32768.0f);
        }

        // update rudder
        ctrl_surface_set_mode(&ctrl_rudder, cmd.opmode_rudder);
        ctrl_surface_set_feedback(&ctrl_rudder, data.yaw, data.w_z);
        if (cmd.opmode_rudder == CTRL_SURFACE_MODE_LOCK_ATT)
        {
            locked_yaw += cmd.rudder / 32768.0f * (YAW_MAX_DEGPMS * TASK_CYCLE_PERIOD);
            ctrl_surface_set_input(&ctrl_rudder, locked_yaw);
        }
        else 
        {
            locked_yaw = data.yaw;
            ctrl_surface_set_input(&ctrl_rudder, cmd.rudder / 32768.0f);
        }

        // Calculate outputs
        aileron_out = ctrl_surface_calculate(&ctrl_aileron);
        elevator_out = ctrl_surface_calculate(&ctrl_elevator);
        rudder_out = ctrl_surface_calculate(&ctrl_rudder);

        // Apply to actuators
        servo_set_deg_trimmed(&servo_aileron_left, aileron_out * 45.0f);
        servo_set_deg_trimmed(&servo_aileron_right, aileron_out * (-45.0f));
        servo_set_deg_trimmed(&servo_elevator, elevator_out * 45.0f);
        servo_set_deg_trimmed(&servo_rudder, rudder_out * 45.0f);

        // printf("eng1=%d:%f\teng2=%d:%f\t\n", cmd.eng_1, (float)(cmd.thrust_1) / UINT16_MAX, cmd.eng_2, (float)(cmd.thrust_2) / UINT16_MAX);
        esc_set_thrust(&engine_1, (float)(cmd.thrust_1) / UINT16_MAX);
        esc_set_thrust(&engine_2, (float)(cmd.thrust_2) / UINT16_MAX);
        
        // Set engines
        if (cmd.eng_1)
            esc_start(&engine_1);
        else
            esc_stop(&engine_1);
        if (cmd.eng_2)
            esc_start(&engine_2);
        else
            esc_stop(&engine_2);
        

        // Update feedback
        fdbk.elevator = (int8_t)(-elevator_out * 127.0f);
        fdbk.aileron_l = (int8_t)(aileron_out * 127.0f);
        fdbk.aileron_r = (int8_t)(-aileron_out * 127.0f);
        fdbk.rudder = (int8_t)(rudder_out * 127.0f);

        if (esc_get_state(&engine_1) == ESC_RUNNING)
            fdbk.eng_1 = (int8_t)(esc_get_thrust(&engine_1) * 100.0f);
        else
            fdbk.eng_1 = -1;

        if (esc_get_state(&engine_2) == ESC_RUNNING)
            fdbk.eng_2 = (int8_t)(esc_get_thrust(&engine_2) * 100.0f);
        else
            fdbk.eng_2 = -1;
         
        shared_mem_update(FDBK_MSG_ID, &fdbk);

    task_loop_end:
        board_delay_ms(TASK_CYCLE_PERIOD);
    }
}

void offline_task()
{
    offline = 1;

    // Turn off servos
    servo_turn_off(&servo_aileron_left);
    servo_turn_off(&servo_aileron_right);
    servo_turn_off(&servo_elevator);
    servo_turn_off(&servo_rudder);

    // Turn off engines
    esc_stop(&engine_1);
    esc_stop(&engine_2);
}