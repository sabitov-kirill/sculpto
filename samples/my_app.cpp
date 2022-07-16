/*****************************************************************//**
 * \file   my_app.cpp
 * \brief  Sculpto sample client application main module.
 *
 * \author Sabitov Kirill
 * \date   15 July 2022
 *********************************************************************/

#include <sculpto.h>
#include <core/application/entry_point.h>
using namespace scl;

class my_app: public application
{
private:

public:
    my_app() : application("Editor") {}
    ~my_app() override {}

    void OnInit() override
    {
    }

    void OnClose() override
    {
    }

    void OnUpdate(float DeltaTime) override
    {
    }

    void OnGuiUpdate()
    {
    }
};

scl::application *scl::CreateApplication()
{
    return new my_app();
}
