
#include <iostream>
#include <vector>
#include <cstdio>
#include <string>
#include <unordered_map>
#include <thread> 
#include "ringbuffer.h"

using namespace std;
my_ring_buffer<int, 1000> g_ringbuffer;

void my_push()
{
    for (int i = 0; i < 10000; i++)
    {
        g_ringbuffer.push(99999);
    }
}

void my_pop()
{
    for (int i = 0; i < 10000; i++)
    {
        int output;
        if (g_ringbuffer.pop(output))
        {
            //cout << output << endl;
        }
    }
}

int main(int argc, const char** argv)
{
    if (argc <= 2)
    {
        cout << "argc:" << argc <<  " invalid" << endl;
        return -1;
    }
    
    int producer = atoi(argv[1]);
    int consumer = atoi(argv[2]);
    //vector<thread> tv;
    thread tv[producer + consumer];
    //tv.reserve(producer + consumer + 1);
    int count = 0;
    while (producer-- > 0)
    {
        tv[count++] = thread(my_push);
        //tv.emplace_back(t);
        //tv[count++] = t;
        //t.join();
    }

    while (consumer-- > 0)
    {
        tv[count++] = thread(my_pop);
        //thread t(my_pop);
        //tv.emplace_back(t);
        //tv[count++] = t;
        //t.join();
    }

    for (int i = 0; i < count; i++)
    {
        thread& t = tv[i];
        t.join();
    }

    cout << "all thread finished" << endl;
    return 0;
}
