//
//  TouchDispatcher.cpp
//  BreakoutEx
//
//  Created by chen caibin on 2021/12/29.
//

#include "touch_dispatcher.hpp"
#include "director.hpp"

#include <string>
#include <iostream>

TouchDispatcher* TouchDispatcher::m_instance_ptr = nullptr;

bool TouchDispatcher::isDispatchEvents(void)
{
    return dispatchEvents;
}

void TouchDispatcher::setDispatchEvents(bool bDispatchEvents)
{
    dispatchEvents = bDispatchEvents;
}

bool TouchDispatcher::init(void)
{
    dispatchEvents = true;

    return true;
}

TouchDispatcher::~TouchDispatcher(void)
{
    
}

void TouchDispatcher::addHandler(int scene,TouchHandler *handler)
{
    //int i = 0;
    for (auto iter : touchHandlers)
    {
        if(scene == iter.first)
        {
            /*
            auto v = iter.second;
            for(TouchHandler *h : v )
            {
                if( h->getPriority() < handler->getPriority() )
                    i++;
                
                if( h->getDelegate() == handler->getDelegate() )
                    assert("Delegate already added to touch dispatcher.");
            }
            std::vector<TouchHandler*>::iterator it;
            it = v.begin();
            v.insert(it+i, handler);
            */
            touchHandlers[scene].push_back(handler);
            return;
        }
    }
    std::vector<TouchHandler*> touchVec;
    touchVec.push_back(handler);
    touchHandlers[scene] = touchVec;
}

void TouchDispatcher::addTargetedDelegate(int scene, TouchDelegate *pDelegate, int nPriority)
{
    TouchHandler *handler = TouchHandler::handlerWithDelegate(pDelegate, nPriority);
    this->addHandler(scene,handler);
}

void TouchDispatcher::removeDelegate(TouchDelegate *delegate)
{
    if( delegate == nullptr )
        return;
    
    int i = 0;
    for (auto iter : touchHandlers)
    {
        auto v = iter.second;
        std::vector<TouchHandler*>::iterator it;
        it = v.begin();
        for( TouchHandler *handler : v ) {
            if( handler->getDelegate() ==  delegate ) {
                v.erase(it+i);
                break;
            }
            i++;
        }
    }
}

void TouchDispatcher::removeAllDelegates(void)
{
    touchHandlers.clear();
}

void TouchDispatcher::setPriority(int nPriority, TouchDelegate *pDelegate)
{
    
}

void TouchDispatcher::touchesBegan(double x, double y)
{
    int scene = Director::GetInstance()->GetTopScene()->GetSceneID();
    if( dispatchEvents )  {
        for (auto iter : touchHandlers)
        {
            if(scene == iter.first)
            {
                auto v = iter.second;
                for( TouchHandler *handler : v ) {
                    handler->getDelegate()->TouchBegan(x, y);
                }
            }
        }
    }
}

void TouchDispatcher::touchesMoved(double x, double y)
{
    int scene = Director::GetInstance()->GetTopScene()->GetSceneID();
    if( dispatchEvents )  {
        for (auto iter : touchHandlers)
        {
            if(scene == iter.first)
            {
                auto v = iter.second;
                for( TouchHandler *handler : v ) {
                    handler->getDelegate()->TouchMoved(x, y);
                }
            }
        }
    }
}

void TouchDispatcher::touchesEnded(double x, double y)
{
    int scene = Director::GetInstance()->GetTopScene()->GetSceneID();
    if( dispatchEvents )  {
        for (auto iter : touchHandlers)
        {
            if(scene == iter.first)
            {
                auto v = iter.second;
                for( TouchHandler *handler : v ) {
                    handler->getDelegate()->TouchEnded(x, y);
                }
            }
        }
    }
}

void TouchDispatcher::touchesCancelled(double x, double y)
{
    int scene = Director::GetInstance()->GetTopScene()->GetSceneID();
    if( dispatchEvents )  {
        for (auto iter : touchHandlers)
        {
            if(scene == iter.first)
            {
                auto v = iter.second;
                for( TouchHandler *handler : v ) {
                    handler->getDelegate()->TouchCancelled(x, y);
                }
            }
        }
    }
}

void TouchDispatcher::touchesMovedOffset(double x, double y)
{
    int scene = Director::GetInstance()->GetTopScene()->GetSceneID();
    if( dispatchEvents )  {
        for (auto iter : touchHandlers)
        {
            if(scene == iter.first)
            {
                auto v = iter.second;
                for( TouchHandler *handler : v ) {
                    handler->getDelegate()->TouchMovedOffset(x, y);
                }
            }
        }
    }
}
