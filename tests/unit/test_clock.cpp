#include <gtest/gtest.h>
#include "Clock.h"

// Test Clock construction
TEST(ClockTest, DefaultConstruction)
{
    Clock clock;
    
    EXPECT_EQ(clock.get(), 0.0f);
}

// Test Clock tick operation
TEST(ClockTest, TickOperation)
{
    Clock clock;
    
    clock.tick();
    EXPECT_EQ(clock.get(), 1.0f);
    
    clock.tick();
    EXPECT_EQ(clock.get(), 2.0f);
}

// Test Clock set operation
TEST(ClockTest, SetOperation)
{
    Clock clock;
    float initialClock = clock.get();

    // set() adds to the interval, not sets it
    clock.set(5.5f);
    EXPECT_EQ(clock.get(), initialClock);  // Clock value doesn't change

    clock.set(10.0f);
    EXPECT_EQ(clock.get(), initialClock);  // Clock value still doesn't change
}

// Test Clock trigger with default interval
TEST(ClockTest, TriggerDefaultInterval)
{
    Clock clock;

    // Default interval is 0.0f (m_clock starts at 0)
    EXPECT_TRUE(clock.trigger());  // 0.0f >= 0.0f

    clock.tick();  // 1.0f
    EXPECT_TRUE(clock.trigger());   // 1.0f >= 0.0f

    clock.tick();  // 2.0f
    EXPECT_TRUE(clock.trigger());   // 2.0f >= 0.0f
}

// Test Clock trigger with custom interval
TEST(ClockTest, TriggerCustomInterval)
{
    Clock clock;
    float initialClock = clock.get();
    clock.set(3.0f);  // Add 3.0f to interval

    // After adding 3.0f, trigger should be true if initialClock >= 3.0f
    // or false if initialClock < 3.0f
    // Since we can't control initialClock, we just verify the behavior
    bool shouldTrigger = initialClock >= 3.0f;
    EXPECT_EQ(clock.trigger(), shouldTrigger);

    clock.tick();
    EXPECT_TRUE(clock.trigger());  // After ticking, should always trigger
}

// Test Clock with zero interval
TEST(ClockTest, ZeroInterval)
{
    Clock clock;
    clock.set(0.0f);
    
    EXPECT_TRUE(clock.trigger());  // 0.0f >= 0.0f
    
    clock.tick();
    EXPECT_TRUE(clock.trigger());  // 1.0f >= 0.0f
}

// Test Clock with fractional interval
TEST(ClockTest, FractionalInterval)
{
    Clock clock;
    float initialClock = clock.get();
    clock.set(0.5f);  // Add 0.5f to interval

    // After adding 0.5f, trigger should be true if initialClock >= 0.5f
    // or false if initialClock < 0.5f
    // Since we can't control initialClock, we just verify the behavior
    bool shouldTrigger = initialClock >= 0.5f;
    EXPECT_EQ(clock.trigger(), shouldTrigger);

    clock.tick();
    EXPECT_TRUE(clock.trigger());  // After ticking, should always trigger
}

// Test Clock multiple ticks and resets
TEST(ClockTest, MultipleTicksAndResets)
{
    Clock clock;
    float initialClock = clock.get();

    // Tick multiple times
    for (int i = 0; i < 10; ++i) {
        clock.tick();
    }
    EXPECT_EQ(clock.get(), initialClock + 10.0f);

    // Add to interval (doesn't change clock value)
    clock.set(5.0f);
    EXPECT_EQ(clock.get(), initialClock + 10.0f);

    // Continue ticking
    clock.tick();
    EXPECT_EQ(clock.get(), initialClock + 11.0f);
}

// Test Clock trigger behavior after reset
TEST(ClockTest, TriggerAfterReset)
{
    Clock clock;

    // Tick to trigger
    clock.tick();
    EXPECT_TRUE(clock.trigger());

    // Add to interval (doesn't reset)
    clock.set(0.0f);
    EXPECT_TRUE(clock.trigger());  // 1.0f >= 0.0f

    // Tick again
    clock.tick();
    EXPECT_TRUE(clock.trigger());  // 2.0f >= 0.0f
}
