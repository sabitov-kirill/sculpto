/*****************************************************************//**
 * \file   application.cpp
 * \brief  
 * 
 * \author Sabitov Kirill
 * \date   19 June 2022
 *********************************************************************/

#include "sclpch.h"
#include "application.h"

 /**
  * Default applicatino construcotr.
  * Initialises all depended systems.
  *
  * \param Name - Application name. Will be shown in window title.
  */
scl::application::application(const std::string &Name) :
    window(700, 400, Name + " (Sculpto application).")
{
}

 /**
  * Run application loop and show window function.
  *
  * \param None.
  * \return None.
  */
void scl::application::Run()
{
    window::Run();
}