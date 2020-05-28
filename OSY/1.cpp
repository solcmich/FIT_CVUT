#ifndef __PROGTEST__

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <climits>
#include <cfloat>
#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <vector>
#include <set>
#include <list>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <stack>
#include <deque>
#include <memory>
#include <functional>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include "progtest_solver.h"
#include "sample_tester.h"
#include <string>

using namespace std;
#endif /* __PROGTEST__ */

template <typename T>
class SafeQueue
{
public:
    void Push(T val)
    {
        unique_lock ul(m_mtx);
        self.push(val);
        cv_Pop.notify_one();
    }

    bool Pop(T & val)
    {
        unique_lock ul(m_mtx); // Lock before poping
        while(self.empty() && !end)
            cv_Pop.wait(ul);

        if(self.empty())
            return false;

        val = self.front();
        self.pop();
        cv_Empty.notify_all();
        return true;
    }
    void Flush()
    {
        end = true;
        cv_Pop.notify_all();
        unique_lock ul(m_mtx); // Lock before poping
        while(!self.empty())
        {
            cv_Pop.notify_all();
            cv_Empty.wait(ul);
        }
    }

    bool Empty()
    {
        unique_lock ul(m_mtx);
        bool b = self.empty();
        return b;
    }

    bool EmptyNonLocked()
    {
        return self.empty();
    }
    condition_variable cv_Pop;
    condition_variable cv_Empty;
    mutex m_mtx;
    bool end = false;
private:
    queue<T> self;


};

struct Message {
    Message(uint32_t id, vector<uint64_t> * fragments, CBigInt res) : id(id), fragments(fragments), res(res){}
    uint64_t id;
    vector <uint64_t> *fragments;
    CBigInt res;
};
class CSentinelHacker;

class CSentinelHacker {
public:
    CSentinelHacker()
    {
        toSendSafe = new SafeQueue<Message*>();
        messagesMap = map<uint32_t, Message*>();
        toSolveSafe = new SafeQueue<Message*>();
        toProcessSafe = new SafeQueue<uint64_t>();
    }
    static bool SeqSolve(const vector <uint64_t> &fragments, CBigInt &res)
    {
        int cnt = 0;
        auto callback = [&](const uint8_t *data, size_t bits) {
            const uint8_t * d = data + 4;
            res = CountExpressions(d, bits - 32);
            cnt++;
        };
        FindPermutations(fragments.data(), fragments.size(), callback);
        return cnt > 0;
    }
    void AddTransmitter(ATransmitter x)
    {
        m_transmitters.push_back(x);
    }

    void AddReceiver(AReceiver x)
    {
        m_receivers.push_back(x);
    }

    void AddFragment(uint64_t x)
    {
        toProcessSafe->Push(x);
    }
    void Producer(AReceiver t)
    {
        uint64_t data;
        while (t->Recv(data))
            toProcessSafe->Push(data);

        unique_lock<mutex> ul (recieversOut_mtx);
        recieversOut++;
    }
    void ProcessData(uint64_t data)
    {
        uint64_t g = 0xffffffe000000000;
        uint64_t id = data & g;
        id = id / 0x2000000000;
        Message * m;
        unique_lock ll(map_mtx);
        auto i = messagesMap.find(id);
        if(i != messagesMap.end())
        {
            m = i->second;
            m->fragments->push_back(data);
        }
        else{
            auto fragments = new vector<uint64_t>();
            fragments->push_back(data);
            CBigInt dummy;
            m = new Message(id, fragments, dummy);
            messagesMap.insert(make_pair(id, m));
        }
        ll.unlock();
        toSolveSafe->Push(m);
    }

    void Consumer(ATransmitter t)
    {
        while(true)
        {
            unique_lock<mutex> ul (workersOut_mtx);
            if(workersOut == m_workers && toSendSafe->Empty())
                break;
            ul.unlock();

            Message * m;
            if(!toSendSafe->Pop(m))
                continue;

            t->Send(m->id, m->res);

            unique_lock<mutex> ull (map_mtx);
            messagesMap.erase(m->id);
            ull.unlock();
        }
    }
    void Worker()
    {
        while(true)
        {
            uint64_t d;
            if(m_receiversCnt == recieversOut && toProcessSafe->Empty() && toSolveSafe->Empty() && !run)
                break;
            if(!toProcessSafe->Pop(d))
                continue;
            ProcessData(d);

            Message * m;
            if(!toSolveSafe->Pop(m))
                continue;

            CBigInt res;

            if(SeqSolve(*m->fragments, res))
            {
                m->res = res;
                toSendSafe->Push(m);
            }


        }

        unique_lock<mutex> ul (workersOut_mtx);
        workersOut++;
        cv_workersOut.notify_all();

    }
    void Start(unsigned thrCount)
    {
        run = true;
        m_workers = thrCount;
        for(auto a : m_receivers)
        {
            t_receivers.emplace_back( &CSentinelHacker::Producer, this, a );
            m_receiversCnt++;
        }

        for(auto a : m_transmitters)
            t_tramsitters.emplace_back( &CSentinelHacker::Consumer, this, a );

        for ( size_t i = 0; i < thrCount; i ++ )
            t_workers.emplace_back( &CSentinelHacker::Worker, this);
    }

    void Stop(void)
    {
        run = false;

        for(auto & a : t_receivers)
            a.join();

        toProcessSafe->Flush(); //Flush dafa processing queue after we joined all the recievers threads
        toSolveSafe->Flush();

        for(auto & a : t_workers)
            a.join();

        toSendSafe->Flush();
        for(auto & a : t_tramsitters)
            a.join();

        if(m_transmitters.size() == 0)
            return;

        unique_lock<mutex> ul (map_mtx);
        for(auto a : messagesMap)
            m_transmitters[0]->Incomplete(a.second->id);

        ul.unlock();
    }

private:
    vector<ATransmitter> m_transmitters;
    vector<AReceiver> m_receivers;
    int m_receiversCnt = 0;
    mutex                map_mtx;
    mutex                fragments_mutex;

    mutex                workersOut_mtx;
    mutex                recieversOut_mtx;

    condition_variable   cv_receiversOut;
    condition_variable   cv_workersOut;

    int recieversOut = 0;
    int workersOut = 0;
    int m_workers;

    vector<thread> t_workers;
    vector<thread> t_receivers;
    vector<thread> t_tramsitters;
    bool run;
    SafeQueue<Message*> * toSendSafe;
    SafeQueue<Message*> * toSolveSafe;
    SafeQueue<uint64_t> * toProcessSafe;

    map<uint32_t, Message*> messagesMap;
};

#ifndef __PROGTEST__

int main(void) {

    using namespace std::placeholders;
    for (int i = 0; i < 500; ++i) {


        CSentinelHacker test;
        auto trans = make_shared<CExampleTransmitter>();
        AReceiver recv = make_shared<CExampleReceiver>(initializer_list<uint64_t>
                                                               {0x02230000000c, 0x071e124dabef, 0x02360037680e,
                                                                0x071d2f8fe0a1,
                                                                0x055500150755});

        test.AddTransmitter(trans);
        test.AddTransmitter(trans);
        test.AddReceiver(recv);
        test.Start(1);

        static initializer_list<uint64_t> t1Data = {0x071f6b8342ab, 0x0738011f538d, 0x0732000129c3, 0x055e6ecfa0f9,
                                                    0x02ffaa027451, 0x02280000010b, 0x02fb0b88bc3e};
        thread t1(FragmentSender, bind(&CSentinelHacker::AddFragment, &test, _1), t1Data);

        static initializer_list<uint64_t> t2Data = {0x073700609bbd, 0x055901d61e7b, 0x022a0000032b, 0x016f0000edfb};
        thread t2(FragmentSender, bind(&CSentinelHacker::AddFragment, &test, _1), t2Data);
        FragmentSender(bind(&CSentinelHacker::AddFragment, &test, _1),
                       initializer_list<uint64_t>{0x017f4cb42a68, 0x02260000000d, 0x072500000025});
        t1.join();
        t2.join();
        test.Stop();
        assert(trans->TotalSent() == 4);
        assert(trans->TotalIncomplete() == 2);
    }
    return 0;
}

#endif /* __PROGTEST__ */
