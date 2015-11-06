/**
 * \file DoubleOr.hpp
 * \author Candice Bentejac
 * \date November 6, 2015
 * \brief Header for the DoubleOr class, which inherits from the Connector class.
 */

#ifndef _DOUBLE_OR_
#define _DOUBLE_OR_

#include "Connector.hpp"

class DoubleOr : public Connector
{
  public:
    /**
     * \brief DoubleOr constructor.
     * \return A DoubleOr object.
     */
    DoubleOr();
    /**
     * \brief DoubleOr destructor.
     */
    ~DoubleOr();
};

#endif
