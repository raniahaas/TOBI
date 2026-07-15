/**
07/14/2026 - RH - Modified main.cpp for unit testing on state changes
*/

#include <unity.h>
#include "flightState.h"

//Core functions requireed by Unity
void setUp(void) {}
void tearDown(void) {}

/*
Method for testing with flightState.h:
    -Each function is passed as an argument, s
        Which clears each time it is ran
    -Then use TEST_ASSERT_TRUE/FALSE
        Evaluates the code in its current condition and will fail if it is false
        Additionally, they can be more than just true or false like bool or uint32
    -Call each test repeditivley
        This is the best way to simulate threads, by constantly 'checking' and updating information
    -Mark out edge cases
        This is great for simulating and testing logic, but also must consider edge cases espically here
        there are a lot of unknowns and lots to be checked
*/

//Liftoff
void test_launch_detected_on_accel_spike() {
    FlightState s;

    //{AccelZ, altitude, elapsedTime(ms)}
    FlightInputs in{ 16.0f, 100.0f, 1000 };
    s = updateFlightState(s, in);
    TEST_ASSERT_TRUE(s.launched);
    TEST_ASSERT_EQUAL_FLOAT(100.0f, s.launchAltitude);
}

//Threshold for determining decent
void test_no_launch_below_threshold() {
    FlightState s;
    FlightInputs in{ 10.0f, 100.0f, 1000 };
    s = updateFlightState(s, in);
    TEST_ASSERT_FALSE(s.launched);
}

//Apogee testing, similar stripped logic where we are looking for <50m drop
void test_apogee_detected_after_50m_drop() {
    FlightState s;
    //Launch
    s = updateFlightState(s, {16.0f, 100.0f, 0});
    //Peak
    s = updateFlightState(s, {9.81f, 500.0f, 100});
    //Drop 60m
    s = updateFlightState(s, {9.81f, 440.0f, 200});

    TEST_ASSERT_TRUE(s.apogeeReached);
    TEST_ASSERT_EQUAL_FLOAT(500.0f, s.apogee);
}

//Test for landing
void test_landing_requires_3_seconds_stable() {
    FlightState s;

    //Update several times, decreasing accel and altitude
    s = updateFlightState(s, {16.0f, 100.0f, 0});
    s = updateFlightState(s, {9.81f, 500.0f, 100});
    s = updateFlightState(s, {9.81f, 440.0f, 200}); 

    //Landed and unmoving, start confirm time at t=1200
    s = updateFlightState(s, {9.81f, 110.0f, 1200});
    TEST_ASSERT_FALSE(s.landed);

    //2.1 s
    s = updateFlightState(s, {9.81f, 110.0f, 3300});
    TEST_ASSERT_FALSE(s.landed);

    //3.1 s, can determine as landed
    s = updateFlightState(s, {9.81f, 110.0f, 4300});
    TEST_ASSERT_TRUE(s.landed);
}

//Test for limiting/restarting system --added later for lockout angle detection 
void test_landing_confirm_resets_if_unstable() {
    FlightState s;
    s = updateFlightState(s, {16.0f, 100.0f, 0});
    s = updateFlightState(s, {9.81f, 500.0f, 100});
    s = updateFlightState(s, {9.81f, 440.0f, 200});
    //Confirm with device
    s = updateFlightState(s, {9.81f, 110.0f, 1000}); 
    //Spike, need to restart
    s = updateFlightState(s, {20.0f, 110.0f, 1500});
    TEST_ASSERT_EQUAL_UINT32(0, s.landedConfirmedStart);
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_launch_detected_on_accel_spike);
    RUN_TEST(test_no_launch_below_threshold);
    RUN_TEST(test_apogee_detected_after_50m_drop);
    RUN_TEST(test_landing_requires_3_seconds_stable);
    RUN_TEST(test_landing_confirm_resets_if_unstable);
    return UNITY_END();
}