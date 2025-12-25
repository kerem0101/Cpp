class CriticalSection {
public:
    CriticalSection() {
        // disable interrupts
        __disable_irqs();
    }

    ~CriticalSection() {
        // enable interrupts
        __enable_irqs();
    }
};

void UpdateSensor(){
    {
        CriticalSection lock; // interrupts are disabled here
    
        // Critical section code to update sensor data safely
    
        if (ReadData() == ERROR) {
            // destructer of CriticalSection will re-enable interrupts
            return;
        }
    
        Data = Process();
        
    } // interrupts are re-enabled here
}
        

class ScopedPin {
private:
    int pin_;

public:
    ScopedPin(int pin) : pin_(pin) {
        HAL_GPIO_WritePin(pin_, GPIO_PIN_SET);
    }

    ~ScopedPin() {
        HAL_GPIO_WritePin(pin_, GPIO_PIN_RESET);
    }

};

void ControlLED(int pin) {
    {
        ScopedPin led(pin); // LED is turned on here

        // Perform operations while LED is on

    } // LED is turned off here
}