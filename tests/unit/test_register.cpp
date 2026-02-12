#include <gtest/gtest.h>
#include "Register.h"

// Test Register construction
TEST(RegisterTest, DefaultConstruction)
{
    Register reg;
    
    EXPECT_FALSE(reg.m_flitEnable);
    EXPECT_FALSE(reg.m_creditEnable);
}

// Test Register pushbackFlit and popfrontFlit
TEST(RegisterTest, FlitOperations)
{
    Register reg;
    
    // Initially empty
    EXPECT_TRUE(reg.isFlitRegisterEmpty());
    
    // Push a flit
    Flit flit1(1, {1, 2, 3});
    reg.pushbackFlit(flit1);
    EXPECT_FALSE(reg.isFlitRegisterEmpty());
    
    // Push another flit
    Flit flit2(2, {4, 5, 6});
    reg.pushbackFlit(flit2);
    
    // Pop first flit
    Flit popped1 = reg.popfrontFlit();
    EXPECT_EQ(popped1.m_source, 1);
    EXPECT_FALSE(reg.isFlitRegisterEmpty());
    
    // Pop second flit
    Flit popped2 = reg.popfrontFlit();
    EXPECT_EQ(popped2.m_source, 2);
    EXPECT_TRUE(reg.isFlitRegisterEmpty());
}

// Test Register pushbackCredit and popfrontCredit
TEST(RegisterTest, CreditOperations)
{
    Register reg;
    
    // Initially empty
    EXPECT_TRUE(reg.isCreditRegisterEmpty());
    
    // Push a credit
    Credit credit1(0, false);
    reg.pushbackCredit(credit1);
    EXPECT_FALSE(reg.isCreditRegisterEmpty());
    
    // Push another credit
    Credit credit2(1, true);
    reg.pushbackCredit(credit2);
    
    // Pop first credit
    Credit popped1 = reg.popfrontCredit();
    EXPECT_EQ(popped1.m_creditVirtualChannel, 0);
    EXPECT_FALSE(popped1.m_isTail);
    EXPECT_FALSE(reg.isCreditRegisterEmpty());
    
    // Pop second credit
    Credit popped2 = reg.popfrontCredit();
    EXPECT_EQ(popped2.m_creditVirtualChannel, 1);
    EXPECT_TRUE(popped2.m_isTail);
    EXPECT_TRUE(reg.isCreditRegisterEmpty());
}

// Test Register enable flags
TEST(RegisterTest, EnableFlags)
{
    Register reg;
    
    // Initially disabled
    EXPECT_FALSE(reg.m_flitEnable);
    EXPECT_FALSE(reg.m_creditEnable);
    
    // Enable flags
    reg.m_flitEnable = true;
    reg.m_creditEnable = true;
    
    EXPECT_TRUE(reg.m_flitEnable);
    EXPECT_TRUE(reg.m_creditEnable);
}

// Test Register with multiple operations
TEST(RegisterTest, MultipleOperations)
{
    Register reg;
    
    // Add multiple flits
    for (int i = 0; i < 5; ++i) {
        Flit flit(i, {i, i + 1, i + 2});
        reg.pushbackFlit(flit);
    }
    
    EXPECT_FALSE(reg.isFlitRegisterEmpty());
    
    // Pop all flits
    for (int i = 0; i < 5; ++i) {
        Flit popped = reg.popfrontFlit();
        EXPECT_EQ(popped.m_source, i);
    }
    
    EXPECT_TRUE(reg.isFlitRegisterEmpty());
    
    // Add multiple credits
    for (int i = 0; i < 3; ++i) {
        Credit credit(i, i % 2 == 0);
        reg.pushbackCredit(credit);
    }
    
    EXPECT_FALSE(reg.isCreditRegisterEmpty());
    
    // Pop all credits
    for (int i = 0; i < 3; ++i) {
        Credit popped = reg.popfrontCredit();
        EXPECT_EQ(popped.m_creditVirtualChannel, i);
    }
    
    EXPECT_TRUE(reg.isCreditRegisterEmpty());
}

// Test Register empty state after operations
TEST(RegisterTest, EmptyStateAfterOperations)
{
    Register reg;
    
    // Add and remove flit
    Flit flit(1, {1, 2});
    reg.pushbackFlit(flit);
    reg.popfrontFlit();
    EXPECT_TRUE(reg.isFlitRegisterEmpty());
    
    // Add and remove credit
    Credit credit(0, false);
    reg.pushbackCredit(credit);
    reg.popfrontCredit();
    EXPECT_TRUE(reg.isCreditRegisterEmpty());
}

// Test Register with different flit types
TEST(RegisterTest, DifferentFlitTypes)
{
    Register reg;
    
    // Head flit
    Flit headFlit(1, {1, 2, 3});
    reg.pushbackFlit(headFlit);
    
    // Body flit
    std::vector<float> data = {1.0f, 2.0f, 3.0f};
    Flit bodyFlit(data, 5);
    reg.pushbackFlit(bodyFlit);
    
    // Tail flit
    Flit tailFlit(42);
    reg.pushbackFlit(tailFlit);
    
    // Pop and verify
    Flit popped1 = reg.popfrontFlit();
    EXPECT_EQ(popped1.m_flitType, FlitType::H);
    
    Flit popped2 = reg.popfrontFlit();
    EXPECT_EQ(popped2.m_flitType, FlitType::B);
    
    Flit popped3 = reg.popfrontFlit();
    EXPECT_EQ(popped3.m_flitType, FlitType::T);
}

// Test Register with credit isTail flag
TEST(RegisterTest, CreditIsTailFlag)
{
    Register reg;
    
    Credit credit1(0, false);
    Credit credit2(1, true);
    
    reg.pushbackCredit(credit1);
    reg.pushbackCredit(credit2);
    
    Credit popped1 = reg.popfrontCredit();
    EXPECT_FALSE(popped1.m_isTail);
    
    Credit popped2 = reg.popfrontCredit();
    EXPECT_TRUE(popped2.m_isTail);
}
