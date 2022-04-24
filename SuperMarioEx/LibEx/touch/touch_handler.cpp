//
//  touch_handler.cpp
//  BreakoutEx
//
//  Created by chen caibin on 2021/12/29.
//

#include "touch_handler.hpp"

TouchDelegate* TouchHandler::getDelegate(void)
{
    return m_pDelegate;
}

void TouchHandler::setDelegate(TouchDelegate *pDelegate)
{
    m_pDelegate = pDelegate;
}

int TouchHandler::getPriority(void)
{
    return m_nPriority;
}

void TouchHandler::setPriority(int nPriority)
{
    m_nPriority = nPriority;
}

int TouchHandler::getEnabledSelectors(void)
{
    return m_nEnabledSelectors;
}

void TouchHandler::setEnalbedSelectors(int nValue)
{
    m_nEnabledSelectors = nValue;
}

TouchHandler* TouchHandler::handlerWithDelegate(TouchDelegate *pDelegate, int nPriority)
{
    TouchHandler *pHandler = new TouchHandler();

    if (pHandler)
    {
        pHandler->initWithDelegate(pDelegate, nPriority);
    }
    
    return pHandler;
}

bool TouchHandler::initWithDelegate(TouchDelegate *pDelegate, int nPriority)
{
    m_pDelegate = pDelegate;

    m_nPriority = nPriority;
    m_nEnabledSelectors = 0;

    return true;
}

TouchHandler::~TouchHandler(void)
{
   
}
