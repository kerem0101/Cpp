class Fan{
private: 
    int Speed;

public: 

    Fan(int speed) : Speed(speed) {}

    void StartFan(){
        printf("Fan started, speed: %d", Speed);
    }

    //static trampoline function 
    static void Wrapper(void* obj){

        Fan* fan = static_cast<Fan*>(obj);

        if(fan != nullptr){
            fan->StartFan();
        }
    } 

};


class Button{
public:
    //callback function pointer
    typedef void (*CallBackFunction)(void*);

private:
    CallBackFunction userCallBack; //function address
    void* userObj; //object address (this)

public:
    Button() : userCallBack(nullptr), userObj(nullptr) {}

    //user use this function for save callback
    void Attach(CallBackFunction func, void* obj){
        userCallBack = func;
        userObj = obj;
    }

    void Click(){
        if(userCallBack != nullptr){
            userCallBack(userObj);
        }
    }

};

int main(){
    Fan fan1(1);
    Fan fan2(2);

    Button button1;
    Button button2;

    button1.Attach(&Fan::Wrapper, &fan1);
    button2.Attach(&Fan::Wrapper, &fan2);

    button1.Click();
    button2.Click();

    return 0;
}