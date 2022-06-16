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

#ifndef ORDERCOMMAND_MARKET_H
#define ORDERCOMMAND_MARKET_H

#include "Order.h"
#include <map>
#include <queue>
#include <string>
#include <functional>

class Market
{
  public:
    bool append( const Order& order );
    void remove( const Order& order );
    Order& find( Order::Side side, std::string id );
    bool match( std::queue < Order > & );
    void print() const;

  private:
    //bind
    typedef std::multimap < double, Order, std::greater < double > > BidOrders;
    //new offer
    typedef std::multimap < double, Order, std::less < double > > AskOrders;

    void match( Order& bid, Order& ask );

    std::queue < Order > orderUpdates;
    BidOrders bidOrders;
    AskOrders askOrders;
};

#endif