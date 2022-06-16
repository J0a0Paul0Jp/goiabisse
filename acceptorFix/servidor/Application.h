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

#ifndef EXECUTOR_APPLICATION_H
#define EXECUTOR_APPLICATION_H

#include "quickfix/Application.h"
#include "Order.h"
#include "OrderCommand.h"
#include "IDGenerator.h"
#include "quickfix/MessageCracker.h"
#include "quickfix/Values.h"
#include "quickfix/Utility.h"
#include "quickfix/Mutex.h"

#include "quickfix/fix42/NewOrderSingle.h"

class Application: public FIX::Application, public FIX::MessageCracker
{

  // Application overloads
  void onCreate( const FIX::SessionID& );
  void onLogon( const FIX::SessionID& sessionID );
  void onLogout( const FIX::SessionID& sessionID );
  void toAdmin( FIX::Message&, const FIX::SessionID& );
  void toApp( FIX::Message&, const FIX::SessionID& )
    EXCEPT( FIX::DoNotSend );
  void fromAdmin( const FIX::Message&, const FIX::SessionID& )
    EXCEPT( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::RejectLogon );
  void fromApp( const FIX::Message& message, const FIX::SessionID& sessionID )
    EXCEPT( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType );

  // MessageCracker overloads
  void onMessage( const FIX42::NewOrderSingle&, const FIX::SessionID& );

  //Order - process a new order of client
  void newOrder( const Order& );
  //Order - reject a order
  void recuseOrder( const Order& order );
  void recuseOrder( const FIX::SenderCompID&, const FIX::TargetCompID&,
                    const FIX::ClOrdID& clOrdID, const FIX::Symbol& symbol,
                    const FIX::Side& side, const std::string& message );
  //Order - Cancel a order
  void cancelOrder( const Order& order );
  void updateOrder( const Order&, char status );
  void acceptOrder( const Order& order ){ updateOrder( order, FIX::OrdStatus_NEW ); }
  void fillOrder( const Order& order )
  {
    updateOrder( order,
                 order.isFilled() ? FIX::OrdStatus_FILLED
                 : FIX::OrdStatus_PARTIALLY_FILLED );
  }
  
  // Type conversions
  Order::Side convert( const FIX::Side& );
  Order::Type convert( const FIX::OrdType& );
  FIX::Side convert( Order::Side );
  FIX::OrdType convert( Order::Type );

  OrderCommand _orderCommand;
  IDGenerator m_generator;
  
  std::string genOrderID() {
    std::stringstream stream;
    stream << ++m_orderID;
    return stream.str();
  }
  
  std::string genExecID() {
    std::stringstream stream;
    stream << ++m_execID;
    return stream.str();
  }
  private:
    int m_orderID, m_execID;

  public:
    const OrderCommand& orderCommand() { return _orderCommand; }
};
#endif