//Forward Declaration
class MotorContext; 

class MotorState {
public:
    virtual ~MotorState() {}

    // when entering the state (e.g., Start the motor)
    virtual void Enter(MotorContext* ctx) = 0;

    // when exiting the state (e.g., Stop the motor)
    virtual void Exit(MotorContext* ctx) = 0;

    // working in the loop
    // Returns a pointer to the next state. If the state should change, return the new state,
    // otherwise return nullptr.
    virtual MotorState* Update(MotorContext* ctx) = 0;
};

class MotorContext {
private:
    MotorState* currentState; // current state pointer
    int speed; // motor speed variable

public:
    MotorContext(MotorState* startState) : currentState(startState), speed(0) {
        // enter the initial state
        if (currentState) currentState->Enter(this);
    }

     // This function is called continuously in the main loop
    void Process() {
        if (currentState) {
            // Call the Update function of the current state
            MotorState* nextState = currentState->Update(this);

            // If a state change is requested
            if (nextState != nullptr) {
                currentState->Exit(this); // exit the current state
                currentState = nextState; // transition to the new state
                currentState->Enter(this); // enter the new state
            }
        }
    }

    // Getter/Setter for speed
    void SetSpeed(int s) { speed = s; }
    int GetSpeed() { return speed; }
};

class RunningState : public MotorState {
public:
    void Enter(MotorContext* ctx) override {
        // start the motor
        ctx->SetSpeed(50); // exp: set speed to 50
        printf("Motor Running State: Motor started with speed %d\n", ctx->GetSpeed());
        // additional initialization if needed
    }

    void Exit(MotorContext* ctx) override {
        // stop the motor
        ctx->SetSpeed(0); // set speed to 0
        printf("Motor Running State: Motor stopped.\n");
        // Other cleanup operations
    }

    MotorState* Update(MotorContext* ctx) override {
        // If motor speed exceeds 100, transition to ErrorState
        if (ctx->GetSpeed() > 100) {
            return &ErrorState::Instance(); // Error state
        }
        // state remains the same
        return nullptr;
    }

    static RunningState& Instance() {
        static RunningState instance;
        return instance;
    }
};

class ErrorState : public MotorState {
public:
    void Enter(MotorContext* ctx) override {
        // Handle motor error
        printf("Motor Error State: An error has occurred!\n");
    }

    void Exit(MotorContext* ctx) override {
        // Exit error state
        printf("Motor Error State: Exiting error state.\n");
    }

    MotorState* Update(MotorContext* ctx) override {
        // stay in error state until reset
        return nullptr;
    }

    static ErrorState& Instance() {
        static ErrorState instance;
        return instance;
    }
};

class IdleState : public MotorState {
public:
    void Enter(MotorContext* ctx) override {
        // Idle state initialization
        ctx->SetSpeed(0); // set speed to 0
        printf("Motor Idle State: Motor is idle.\n");
        // Other initialization operations
    }

    void Exit(MotorContext* ctx) override {
        // Exit idle state
        printf("Motor Idle State: Exiting idle state.\n");
    }

    MotorState* Update(MotorContext* ctx) override {
        // If motor speed is greater than 0, transition to RunningState
        if (ctx->GetSpeed() > 0) {
            return &RunningState::Instance(); // Running state
        }
        // state remains the same
        return nullptr;
    }

    static IdleState& Instance() {
        static IdleState instance;
        return instance;
    }
};

int main() {
    // 1. Create the motor context with the initial state as IdleState
    MotorContext motor(&IdleState::Instance());

    while (1) {
        // 2. Process the state machine continuously
        motor.Process();

        // 3. Simulation: Increase speed to trigger transition from Running to Error
        HAL_Delay(100);
        motor.SetSpeed(motor.GetSpeed() + 10); 
    }
}