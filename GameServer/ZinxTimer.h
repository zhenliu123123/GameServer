#pragma once
#include<zinx.h>
#include<list>
#include"ZinxTimer.h"
#include<vector>

class ZinxTimer :
    public Ichannel
{
    // 通过 Ichannel 继承
    virtual bool Init() override;
    virtual bool ReadFd(std::string& _input) override;
    virtual bool WriteFd(std::string& _output) override;
    virtual void Fini() override;
    virtual int GetFd() override;
    virtual std::string GetChannelInfo() override;
    virtual AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override;
public:
    int m_timerFd=-1;
};

//定义纯虚类来管理
class TimeOutProc
{
public:
    virtual void Proc() = 0;
    virtual int getTimeSec() = 0;
    //任务所剩圈数
    int iCount = -1;
};

//定义定时器管理类
class TimerOutManager :
    public AZinxHandler
{
    //单例模式1:构造析构函数私有化
    //声明静态成员变量存储，唯一单例
    TimerOutManager();
    virtual ~TimerOutManager();
    static TimerOutManager Single;
    //创建时间轮
    std::vector<std::list<TimeOutProc*>> m_Timer_Wheel;
    //当前刻度(当前在哪个齿上)
    int curTooth = 0;
    //时间轮齿
    int tooth = 10;
public:
    // 通过 AZinxHandler 继承
    virtual IZinxMsg* InternelHandle(IZinxMsg& _oInput) override;
    virtual AZinxHandler* GetNextHandler(IZinxMsg& _oNextMsg) override;
    void addTask(TimeOutProc *p_task);
    void delTask(TimeOutProc *p_task);
    //单例模式2:提供获取单例的接口(引用类型接口)
    static TimerOutManager &getInstance()
    {
        return Single;
    }
};


