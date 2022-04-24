//
//  Director.hpp
//  BreakoutEx
//
//  Created by chen caibin on 2022/1/5.
//

#ifndef Director_hpp
#define Director_hpp

#include "scene.h"
#include <vector>

class Director{
public:
    
    static Director* GetInstance()
        {
            if(s_instance == nullptr)
            {
                s_instance = new Director();
            }
            return s_instance;
        }
    
    IScene* GetTopScene()
    {
        return runningScenes.back();
    }
    
    std::vector<IScene*> GetAllScenes()
    {
        return runningScenes;
    }
    
    void PopScene()
    {
        if(runningScenes.size()>1){
            runningScenes.back()->OnExit();
            runningScenes.pop_back();
        }
    }
    
    void PushScene(IScene *scene)
    {
        runningScenes.push_back(scene);
        scene->OnEnter();
    }
    
    void SetRootScene(IScene *scene)
    {
        runningScenes.clear();
        runningScenes.push_back(scene);
        scene->OnEnter();
    }
    
protected:
    Director() {}
    ~Director() {}

    // Use auto_ptr to make sure that the allocated memory for instance
    // will be released when program exits (after main() ends).
    static Director *s_instance;

private:
    Director(const Director&);
    Director& operator =(const Director&);
    std::vector<IScene*> runningScenes;
};

#endif /* Director_hpp */
