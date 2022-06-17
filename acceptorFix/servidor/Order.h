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

#ifndef ORDERMATCH_ORDER_H
#define ORDERMATCH_ORDER_H

#include <string>
#include <iomanip>
#include <ostream>

class Order
{
  friend std::ostream& operator<<( std::ostream&, const Order& );

public:
  enum Side { buy, sell };
  enum Type { market, limit };

  Order( const std::string& clientId, const std::string& symbol,
         const std::string& owner, const std::string& target,
         Side side, char type, double price, long quantity )

  {
    openQuantity = quantity;
    executedQuantity = 0;
    avgExecutedPrice = 0;
    lastExecutedPrice = 0;
    lastExecutedQuantity = 0;
  }

  const std::string& getClientID() const { return clientId; }
  const std::string& getSymbol() const { return symbol; }
  const std::string& getOwner() const { return owner; }
  const std::string& getTarget() const { return target; }
  
  Side getSide() const { return side; }
  char getType() const { return type; }
  double getPrice() const { return price; }
  long getQuantity() const { return quantity; }

  long getOpenQuantity() const { return openQuantity; }
  long getExecutedQuantity() const { return executedQuantity; }
  double getAvgExecutedPrice() const { return avgExecutedPrice; }
  double getLastExecutedPrice() const { return lastExecutedPrice; }
  long getLastExecutedQuantity() const { return lastExecutedQuantity; }

  bool isFilled() const { return quantity == executedQuantity; }
  bool isClosed() const { return openQuantity == 0; }

  void execute( double price, long quantity )
  {
    avgExecutedPrice =
      ( ( quantity * price ) + ( avgExecutedPrice * executedQuantity ) )
      / ( quantity + executedQuantity );

    openQuantity -= quantity;
    executedQuantity += quantity;
    lastExecutedPrice = price;
    lastExecutedQuantity = quantity;
  }
  void cancel()
  {
    openQuantity = 0;
  }

private:
  std::string clientId;
  std::string symbol;
  std::string owner;
  std::string target;
  Side side;
  char type;
  double price;
  long quantity;

  long openQuantity;
  long executedQuantity;
  double avgExecutedPrice;
  double lastExecutedPrice;
  long lastExecutedQuantity;
};

inline std::ostream& operator<<( std::ostream& ostream, const Order& order )
{
  return ostream
         << "ID: " << std::setw( 10 ) << "," << order.getClientID()
         << " OWNER: " << std::setw( 10 ) << "," << order.getOwner()
         << " PRICE: " << std::setw( 10 ) << "," << order.getPrice()
         << " QUANTITY: " << std::setw( 10 ) << "," << order.getQuantity();
}

#endif