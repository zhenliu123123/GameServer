#include "ZinxTimer.h"
#include<sys/timerfd.h>
#include<iostream>
#include<list>
#include<vector>

TimerOutManager::TimerOutManager() 
{
    //创建十个齿
    for (int i = 0; i < 10; i++)
    {
        std::list<TimeOutProc*> tmp;
        m_Timer_Wheel.push_back(tmp);
    }
}
TimerOutManager::~TimerOutManager() 
{
}

//定时器通道类
//创建文件描述符
bool ZinxTimer::Init()
{
    bool iRet = false;
    int iFd = timerfd_create(CLOCK_MONOTONIC, 0);
    if (iFd >= 0)
    {
        struct itimerspec timerValue = { {1,0} ,{1,0} };
        if (timerfd_settime(iFd, 0, &timerValue, NULL) == 0)
        {
            iRet = true;
            m_timerFd = iFd;
        }
    }
    return iRet;
}

//读取超时次数
bool ZinxTimer::ReadFd(std::string& _input)
{
    bool rRet = false;
    char buf[8] = "";
    if (sizeof(buf)== read(m_timerFd, buf, sizeof(buf)))
    {
        rRet = true;
        //char类型写入string
        _input.assign(buf, sizeof(buf));
    }
    return rRet;
}

bool ZinxTimer::WriteFd(std::string& _output)
{
    return false;
}

//关闭文件描述符
void ZinxTimer::Fini()
{
    close(m_timerFd);
    m_timerFd = -1;//防止之后程序访问，提高健壮性
}

//返回当前定时器的文件描述符
int ZinxTimer::GetFd()
{
    return m_timerFd;
}

//筛选
std::string ZinxTimer::GetChannelInfo()
{
    return "TimerFd";
}

//返回处理超时事件的对象
AZinxHandler* ZinxTimer::GetInputNextStage(BytesMsg& _oInput)
{
    return &TimerOutManager::getInstance();
}

//定时器管理类
//单例模式3:类加载时创建实例
TimerOutManager TimerOutManager::Single;
IZinxMsg* TimerOutManager::InternelHandle(IZinxMsg& _oInput)
{
    //八字节无符号数
    unsigned long iTimeoutCount = 0;//超时次数
    GET_REF2DATA(BytesMsg, obytes, _oInput);
    //obytes是八字节string
    obytes.szData.copy((char*)&iTimeoutCount, sizeof(iTimeoutCount), 0);
    //一个一个字节拷过来
    while ((iTimeoutCount--) > 0)//为了防止子函数执行时间过长导致定时器少执行
    {
        //移动当前的刻度
        curTooth++;
        curTooth %= 10;
        std::list<TimeOutProc*>m_cache;
        //遍历齿中的任务列表
        //1.若圈数为0，执行任务，摘除本节点，重新添加
        //2.否则，圈数-1
        //用迭代器遍历，而不用based for原因：
        //要摘除任务并移动，摘除任务时列表会断，遍历不下去了。
        for (auto itr = m_Timer_Wheel[curTooth].begin(); itr != m_Timer_Wheel[curTooth].end(); )
        {
            if ((*itr)->iCount <= 0)
            {
                //把执行函数缓存起来最后统一执行，防止执行函数对遍历产生冲突(比如删除自己)
                m_cache.push_back((*itr));//缓存执行任务列表
                auto p_tmp = *itr;//存起未删除时的itr值，防止溢出
                itr = m_Timer_Wheel[curTooth].erase(itr);//删除当前itr后，返回下一个itr
                addTask(p_tmp);
            }
            else
            {
                (*itr)->iCount--;
                ++itr;//不能在for循环里++，因为删除后会溢出
            }
            //统一执行任务
            for (auto task : m_cache)
            {
                task->Proc();
            }
        }

    }
    return nullptr;
}

AZinxHandler* TimerOutManager::GetNextHandler(IZinxMsg& _oNextMsg)
{
    return nullptr;
}

//管理类里的添加任务函数
void TimerOutManager::addTask(TimeOutProc* p_task)
{
    //如果先计算偏移量后相加会溢出
    //计算插入到哪个齿
    int insertTooth = (p_task->getTimeSec()+curTooth) % 10;
    //插入
    m_Timer_Wheel[insertTooth].push_back(p_task);
    //计算圈数
    p_task->iCount = p_task->getTimeSec() / 10;
}

//管理类的删除任务函数
void TimerOutManager::delTask(TimeOutProc* p_task)
{
    //引用为了同步修改原始值
    for (std::list<TimeOutProc *> &task_list : m_Timer_Wheel)
    {
        for (TimeOutProc *task : task_list)
        {
            if (task == p_task)
            {
                //摘除指定的task
                task_list.remove(task);
                //返回调用函数的位置
                return;
            }
        }
    }
}
