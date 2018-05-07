//
//  CustomDirector.h
//  KitkitSchool
//
//  Created by Gunho Lee on 6/28/16.
//
//

#ifndef CustomDirector_h
#define CustomDirector_h


NS_CC_BEGIN

class CustomDirector : public Director
{
public:
    template<class T> void popSceneWithTransition(float duration)
    {
        CCASSERT(_runningScene != NULL, "running scene should not null");
        
        _scenesStack.popBack();
        ssize_t c = _scenesStack.size();
        
        if (c == 0)
        {
            end();
        }
        else
        {
            Scene *scene = (Scene *)T::create(duration, (Scene *)_scenesStack.at(c-1));
            _scenesStack.replace(c-1, scene);
            _sendCleanupToScene = true;
            _nextScene = (Scene*)_scenesStack.at(c - 1);
        }
    }
};

NS_CC_END

#endif /* CustomDirector_h */
