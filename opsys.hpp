#ifndef OP_SYS_HPP
#define OP_SYS_HPP

#include <algorithm>
#include <cstdint>
#include <string>

#include "types.hpp"

//------------------------------------------------------------------------------------------------------------------
// helper union to cast pointer to member
//------------------------------------------------------------------------------------------------------------------
template<typename memberT>
union u_ptm_cast {

    u_ptm_cast(memberT _pmember) : pmember(_pmember) {};
    u_ptm_cast(void *_pvoid) : pvoid(_pvoid) {};

    memberT  pmember;
    void*     pvoid;
};

template<typename memberT>
inline memberT memberFromPtr(void *pvoid) {
    return u_ptm_cast<memberT>(pvoid).pmember;
}
template<typename memberT>
inline void* ptrFromMember(memberT pmember) {
    return u_ptm_cast<memberT>(pmember).pvoid;
}

typedef void* t_member_ptr;

//------------------------------------------------------------------------------------------------------------------

struct Op;

extern "C" void callFunc(const void *funcPtr, const size_t *stack, size_t numArgs, Op* opPtr);

enum OpParamType
{
    OPT_INT,
    OPT_FLT,
    OPT_STR
};

struct OpParam
{
    OpParamType type;
    int         chans;
 
    union
    {
        float   flts[4];
        int     ints[4];
    };

    std::string str; // can't be in union
};

struct Op
{
    Op(const char* name, const void *execFunc, const OpParam *params, size_t numParams) : 
        m_execFunc(execFunc),
        m_numParams(numParams),
        m_name(name)
    {
        std::copy(params, params+numParams, m_params);
    }

    void execute()
    {
        size_t args[16] = { 0 };

        for (uint32_t i=0; i<m_numParams; i++)
        {
            const OpParam &p = m_params[i];
            switch (p.type)
            {
            case OPT_STR:
                args[i] = (size_t)&p.str;
                break;
            default: // float and integer types (it's a union!)
                args[i] = (p.chans > 1 ? (size_t)p.ints : (size_t)p.ints[0]);
                break;
            }
        }

        callFunc(m_execFunc, args, m_numParams, this);
    }


    void FooOpExec1(const Size2 &size, float val, const std::string &str)
    {
        std::cout << "m_name = " << this->m_name << std::endl;
        std::cout << size.x << ", " << size.y << std::endl;
        std::cout << val << std::endl;
        std::cout << str << std::endl;        
        std::cout << "this = " << this << std::endl; 
        std::cout << "m_numParams = " << this->m_numParams << std::endl;
        std::cout << "m_execFunc = " << m_execFunc << std::endl;        
        std::cout << "-----------------------" << std::endl;
    }

    void FooOpExec2(const Size2 &size, float val, const Rect &rect, float val2, const std::string &str)
    {
        std::cout << "m_name = " << this->m_name << std::endl;
        std::cout << size.x << ", " << size.y << std::endl;
        std::cout << val << std::endl;
        std::cout << val2 << std::endl;
        std::cout << rect.x0 << ", " << rect.y0 << ", " << rect.x1 << ", " << rect.y1 << std::endl;
        std::cout << str << std::endl;
        std::cout << "this = " << this << std::endl;
        std::cout << "m_numParams = " << this->m_numParams << std::endl;
        std::cout << "m_execFunc = " << m_execFunc << std::endl;        
        std::cout << "-----------------------" << std::endl;
    }
    
    void FooOpExec3()
    {
        std::cout << "m_name = " << this->m_name << std::endl;
        //std::cout << val << std::endl;
        std::cout << "this = " << this << std::endl;
        std::cout << "m_numParams = " << this->m_numParams << std::endl;
        std::cout << "m_execFunc = " << m_execFunc << std::endl;
        std::cout << "-----------------------" << std::endl;
    }
 
public:
    const void * m_execFunc;
    size_t       m_numParams;
    OpParam      m_params[16];
    const char*  m_name;
};

#endif