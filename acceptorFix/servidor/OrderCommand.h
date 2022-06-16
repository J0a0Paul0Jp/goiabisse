/* -*- C++ -*- */

/****************************************************************************
** Copyright (c) 2001-2014
**
** This file is part of the QuickFIX FIX Engine
**
** This file may be distributed under the terms of the quickfixengine.org
** license as defined by quickfixengine.org and appearing in the file
** LICENSE included in the packaging of this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.quickfixengine.org/LICENSE for licensing information.
**
** Contact ask@quickfixengine.org if any conditions of this licensing are
** not clear to you.
**
****************************************************************************/


#include "Market.h"
#include <map>
#include <iostream>

class OrderCommand
{
  typedef std::map < std::string, Market > Markets;
public:
  bool append( const Order& order )
  {
    Markets::iterator i = markets.find( order.getSymbol() );
    if ( i == markets.end() )
      i = markets.insert( std::make_pair( order.getSymbol(), Market() ) ).first;
    return i->second.append( order );
  }

  void remove( const Order& order )
  {
    Markets::iterator i = markets.find( order.getSymbol() );
    if ( i == markets.end() ) return ;
    i->second.remove( order );
  }

  Order& find( std::string symbol, Order::Side side, std::string id )
  {
    Markets::iterator i = markets.find( symbol );
    if ( i == markets.end() ) throw std::exception();
    return i->second.find( side, id );
  }

  bool match( std::string symbol, std::queue < Order > & orders )
  {
    Markets::iterator i = markets.find( symbol );
    if ( i == markets.end() ) return false;
    return i->second.match( orders );
  }

  bool match( std::queue < Order > & orders )
  {
    Markets::iterator i;
    for ( i = markets.begin(); i != markets.end(); ++i )
      i->second.match( orders );
    return orders.size() != 0;
  }


  void print() const
  {
    std::cout << "SYMBOLS:" << std::endl;
    std::cout << "--------" << std::endl;

    Markets::const_iterator i;
    for ( i = markets.begin(); i != markets.end(); ++i )
      std::cout << i->first << std::endl;
  }

private:
  Markets markets;
};
