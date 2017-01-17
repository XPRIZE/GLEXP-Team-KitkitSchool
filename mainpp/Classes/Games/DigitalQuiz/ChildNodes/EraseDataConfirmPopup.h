//
//  EraseDataConfirmPopup.h -- Let the user decide erase/resume the app
//  TodoSchool on Nov 2, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Utils/MainNS.h"


BEGIN_NS_DIGITALQUIZ

class EraseDataConfirmPopup: public Node {
    typedef Node Super;

public:
    Chain<string> Classroom;
    Chain<int> StudentNumber;
    Chain<string> ProblemName;
    Chain<bool> Finished;

    function<void()> OnEraseAndRestart;
    function<void()> OnResume;
    function<void()> OnCancel;

public:
    CREATE_FUNC(EraseDataConfirmPopup);
    EraseDataConfirmPopup();
    bool init() override;

private:
    void clearInternals();
    void refreshChildNodes();
};

END_NS_DIGITALQUIZ
