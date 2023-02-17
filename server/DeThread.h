#ifndef _DETHREAD_H_
#define _DETHREAD_H_
#define NON_NULL(x) ((x)?(x):"")

#define ASSERT_MSG(COND, FMT, ARGS...)\
{\
    if(!(COND)){\
        char msg[1024] = {0,};\
        snprintf(msg, sizeof(msg), FMT, ##ARGS);\
        throw std::logic_error(msg);\
        abort();\
    }\
}

/**
 * @brief 
 * 
 * @param HDL      线程id
 * @param STACK_KB 分配的栈大小
 * @param JOIN     线程是否和主线程分离
 * @param FUNC     线程执行函数
 * @param PARAM    传入的参数
 * @param NAME     线程的名字
 * 
 * @return int 
 */
#define CREATE_THREAD(HDL,STACK_KB,JOIN,FUNC,PARAM,NAME)\
{\
    char nctThdName[16] = {0,};/*max length is 16*/\
    int nctThdNameLen = strlen(NON_NULL(NAME));\
    if(nctThdNameLen){snprintf(nctThdName, sizeof(nctThdName), "%s", NON_NULL(NAME));}\
    pthread_t nctThdId = 0;\
    pthread_attr_t threadAttr;\
    pthread_attr_init(&threadAttr);\
    ASSERT_MSG(STACK_KB <= 1024 * 8, "Thread stack size[%d]KB is too large !!", STACK_KB);\
    if(STACK_KB > 0){pthread_attr_setstacksize(&threadAttr, 1024*STACK_KB);}\
    pthread_attr_setdetachstate(&threadAttr, JOIN ? PTHREAD_CREATE_JOINABLE : PTHREAD_CREATE_DETACHED);\
    pthread_create(&nctThdId, &threadAttr, FUNC, (void *)PARAM);\
    if((HDL))\
    {\
        *(pthread_t *)(HDL) = nctThdId;\
    }\
    ASSERT_MSG(nctThdId != 0, "Failed to create thread ! name: %s", nctThdName);\
    if(nctThdNameLen){pthread_setname_np(nctThdId, nctThdName);}\
    pthread_attr_destroy(&threadAttr);\
}
#endif