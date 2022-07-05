/*****************************************************************//**
 * \file   sandbox_app.h
 * \brief  Sandbox application definitions module.
 *
 * \author Sabitov Kirill
 * \date   19 June 2022
 *********************************************************************/

#include <sculpto.h>
#include <core/application/entry_point.h>
using namespace scl;

#include "editor_layer.h"

class sandbox_app: public application
{
public:
    sandbox_app() : application("Editor")
    {
        PushOverlay(new editor::editor_layer());
    }

    ~sandbox_app() override
    {
    }

    void OnInit() override
    {
    }

    void OnClose() override
    {
    }
};

scl::application *scl::CreateApplication()
{
    return new sandbox_app();
}