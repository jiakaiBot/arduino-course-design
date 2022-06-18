#define LED 13
#define Buzzer 10
#define PIR_pin 12

//分配给数码管的数字引脚
const byte pins[] = {9, 8, 7, 6, 5, 4, 3, 2};

//将Arduino控制数码管的端口初始化
void Init_seg()
{
    for (int i = 0; i < 8; i++)
    {
        pinMode(pins[i], OUTPUT); //设置为输出模式
    }
}

//用字节数组存储0~9数字所对应符号编码
byte DIGI_DISP[] = {
    // 前缀B 表示二进制数
    B11111100, //=0(自左向右依次对应a,b,c,d,e,f,g,dp）
    B01100000, // 1
    B11011010, // 2
    B11110010, // 3
    B01100110, // 4
    B10110110, // 5
    B10111110, // 6
    B11100000, // 7
    B11111110, // 8
    B11110110, // 9
};

//显示函数
//参数num：用户需要显示的符号，取值范围：0~255
void display_Num(byte num)
{
    for (int cnt = 0; cnt < 8; cnt++)
    {
        //读出每一位，并输出到对应的引脚
        digitalWrite(pins[cnt], bitRead(DIGI_DISP[num], cnt));
    }
}

//清屏函数
//参数dly:清屏所需时长，单位：毫秒
void Clean_scrn(int dly)
{
    for (int cnt = 0; cnt < 8; cnt++)
    {
        //所有位都为低电平，LED都断电
        digitalWrite(pins[cnt], 0);
    }
    delay(dly);
}

void setup()
{
    Init_LDR();
    Init_Light();
    Init_Buzzer();
    Init_PIR();
    Init_seg(); // Arduino对数码管的端口初始化
}

void loop()
{
    // 0~9数字依次显示
    for (int cnt = 0; cnt <= 9; cnt++)
    {
        if (Val_PIR())
        {
            if (Val_LDR() < 150)
            {
                LED_Off();
                Alarm();
            }
            else
            {
                display_Num(cnt); //显示指定的字符
                delay(1000);      //字符显示1秒钟
                //显示内容切换期间，执行清屏
                Clean_scrn(30);
                LED_PWM(Val_LDR());
                Mute();
            }
        }
        else
        {
            display_Num(cnt); //显示指定的字符
            delay(1000);      //字符显示1秒钟
            //显示内容切换期间，执行清屏
            Clean_scrn(30);
            LED_Off();
            Mute();
        }
    }
}

void Init_LDR()
{
}

void Init_Buzzer()
{
    pinMode(LED, OUTPUT);
    pinMode(Buzzer, OUTPUT);
}

void Alarm()
{
    int cnt = 0;
    for (int Hz = 200; Hz <= 800; Hz++)
    {
        cnt++;
        if (cnt == 40)
        {
            cnt = 0;
            digitalWrite(LED, !digitalRead(LED)); // LED状态翻转
        }
        tone(Buzzer, Hz);
        delay(5);
    }
    delay(1000);
    for (int Hz = 800; Hz >= 200; Hz--)
    {
        cnt++;
        if (cnt == 40)
        {
            cnt = 0;
            digitalWrite(LED, !digitalRead(LED)); // LED状态翻转
        }
        tone(Buzzer, Hz); //向蜂鸣器输出频率
                          //     play_Joy();
                          //     delay(5);
        delay(5);
    }
}

void Mute()
{
    noTone(Buzzer);
    digitalWrite(Buzzer, LOW);
    digitalWrite(LED, LOW);
}

int Val_LDR()
{
    int value, out;
    value = analogRead(A0);
    out = map(value, 0, 1023, 0, 255);
    return out;
}

void Init_Light()
{
    pinMode(11, OUTPUT);
}

void LED_On()
{ //开灯
    digitalWrite(11, HIGH);
}

void LED_Off()
{ //关灯
    digitalWrite(11, LOW);
}

void LED_PWM(byte degree)
{
    analogWrite(11, degree);
}

void Init_PIR()
{
    pinMode(PIR_pin, INPUT);
}

bool Val_PIR()
{
    bool val;
    val = digitalRead(PIR_pin);
    return (val);
}