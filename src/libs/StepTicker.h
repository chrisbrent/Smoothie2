/*
      This file is part of Smoothie (http://smoothieware.org/). The motion control part is heavily based on Grbl (https://github.com/simen/grbl).
      Smoothie is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
      Smoothie is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
      You should have received a copy of the GNU General Public License along with Smoothie. If not, see <http://www.gnu.org/licenses/>.
*/



#ifndef STEPTICKER_H
#define STEPTICKER_H

#include <stdint.h>
#include <vector>
#include <bitset>
#include <functional>
#include <atomic>

#include "mbed.h"

class StepperMotor;

class StepTicker{
    public:
        static StepTicker* global_step_ticker;

        StepTicker();
        ~StepTicker();
        void set_frequency( float frequency );
        void signal_a_move_finished();
        void set_reset_delay( float seconds );
        int register_motor(StepperMotor* motor);
        void add_motor_to_active_list(StepperMotor* motor);
        void remove_motor_from_active_list(StepperMotor* motor);
        void set_acceleration_ticks_per_second(uint32_t acceleration_ticks_per_second);
        float get_frequency() const { return frequency; }
        void unstep_tick();
        void step_tick();
        uint32_t get_tick_cnt() const { return tick_cnt; }
        uint32_t ticks_since(uint32_t last) const { return (tick_cnt>=last) ? tick_cnt-last : (UINT32_MAX-last) + tick_cnt + 1; }
        
        void register_acceleration_tick_handler(std::function<void(void)> cb){
            acceleration_tick_handlers.push_back(cb);
        }
        void acceleration_tick();
        void synchronize_acceleration(bool fire_now);

        void start();

        friend class StepperMotor;
    
        // TOADDBACK was private
        volatile uint32_t tick_cnt;
        uint32_t period_us;
        uint32_t reset_delay_us;
    private:
        float frequency;
        uint32_t acceleration_period_us;
        std::vector<std::function<void(void)>> acceleration_tick_handlers;
        std::vector<StepperMotor*> motor;
        std::bitset<32> active_motor; // limit to 32 motors
        std::bitset<32> unstep;       // limit to 32 motors
        std::atomic_uchar do_move_finished;
        uint8_t num_motors;
        volatile bool a_move_finished;

        Ticker* step_timer;
        Ticker* acceleration_timer;
        Timeout* unstep_timer;


};



#endif
