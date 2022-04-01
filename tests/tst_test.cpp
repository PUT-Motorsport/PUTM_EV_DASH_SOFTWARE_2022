#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "../Dash/canhandler.h"

class CanHandler;

using namespace testing;

class CanTest: public Test {
protected:
    QCanBusDevice * testCan;
    CanHandler canHandler;
    QCanBusFrame testFrame;
    BMS_LV_main bms {123, 78, 34, 34, BMS_LV_states::Normal};
    void SetUp() override {
        canHandler.connect();
        //create second can
        testCan = QCanBus::instance()->createDevice(QStringLiteral("socketcan"),
                                                      QStringLiteral("vcan0"), nullptr);
        testCan->connectDevice();
        testFrame.setFrameId(BMS_LV_MAIN_CAN_ID);
    }
    void TearDown() override {
        delete testCan;
    }
};

TEST_F(CanTest, FrameSending) {
    EXPECT_TRUE(testCan->state());
    ASSERT_EQ(canHandler.connected(), true);

}

TEST_F(CanTest, FrameParsing) {
    EXPECT_TRUE(testCan->state());
    char * data = new char[sizeof(BMS_LV_main)];
    std::memcpy(data, &bms, sizeof(BMS_LV_main));
    testFrame.setPayload(data);

    testCan->writeFrame(testFrame);
    EXPECT_EQ(canHandler.getCanData().bms_lv_main.data.current, bms.current);
    EXPECT_EQ(canHandler.getCanData().bms_lv_main.data.device_state, bms.device_state);
    EXPECT_EQ(canHandler.getCanData().bms_lv_main.data.soc, bms.soc);
    EXPECT_EQ(canHandler.getCanData().bms_lv_main.data.temp_avg, bms.temp_avg);
    EXPECT_EQ(canHandler.getCanData().bms_lv_main.data.voltage_sum, bms.voltage_sum);
}

