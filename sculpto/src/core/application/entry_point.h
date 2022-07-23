/*!****************************************************************//*!*
 * \file   entry_point.h
 * \brief  Sculpto library entry point definition module.
 * 
 * \author Sabitov Kirill
 * \date   18 June 2022
 *********************************************************************/

#ifndef __ENTRY_POINT_H
#define __ENTRY_POINT_H

#include "application.h"
extern scl::application *scl::CreateApplication();

/*!*
 * Main programm function.
 * 
 * \param argc - application run arguments count.
 * \param argv - application run arguments array.
 * \return system return code.
 */
int main(int argc, char *argv[])
{
    auto app = scl::CreateApplication();
    app->Run();
    delete app, app = nullptr;
}

#endif /*! !__ENTRY_POINT_H */
