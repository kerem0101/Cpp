#include "FreeRTOS.h"
#include "task.h"

// Abstract Class Thread
class Thread {
private:
    TaskHandle_t taskHandle;
    const char* taskName;
    uint16_t stackDepth;
    UBaseType_t priority;

    // TRAMBOLİN (Static Wrapper)
    // FreeRTOS see only this function. This function, given 'void* param', converts it to
    // 'Thread*' and call Run() method.
    static void TaskEntryPoint(void* params) {
        // 1. void* -> Thread* conversion
        Thread* task = static_cast<Thread*>(params);
        
        // 2. virtual function call (Polymorphism)
        task->Run(); 

        // 3. delete task when Run() exits
        vTaskDelete(NULL); 
    }

public:
    // Constructor: setup task parameters
    Thread(const char* name, uint16_t stackSize, UBaseType_t prio) 
        : taskName(name), stackDepth(stackSize), priority(prio), taskHandle(nullptr) {}

    // Start the task in FreeRTOS
    bool Start() {
        BaseType_t result = xTaskCreate(
            TaskEntryPoint, // C fonction (Wrapper)
            taskName,       // debug name
            stackDepth,     // Stack size
            this,           // attended param (this pointer)
            priority,       // priority
            &taskHandle     // Handle
        );
        return (result == pdPASS);
    }

    // Pure Virtual Function
    // Derived classes must implement this method
    virtual void Run() = 0; 
};

class BlinkerTask : public Thread {
private:
    int delayMs;

public: 
    BlinkerTask(int delayMs) : Thread("Blinker", 128, 1), delayMs(delayMs) {}

    void Run() override {
        while (true) {
            // on/off toggle LED
            HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
            vTaskDelay(pdMS_TO_TICKS(delayMs)); // delayMs wait
        }
    }
};

// two blinker tasks with different delays
BlinkerTask led1(500); // 500ms
BlinkerTask led2(100); // 100ms

int main() {
    HAL_Init();
    
    // Start tasks in FreeRTOS
    led1.Start();
    led2.Start();
    
    // Start the RTOS scheduler
    vTaskStartScheduler();
    
    while(1); // should never reach here
}