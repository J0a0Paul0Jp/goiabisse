#ifdef _MSC_VER
#pragma warning( disable : 4503 4355 4786 )
#endif

#include "Application.h"
#include "quickfix/config.h"
#include "quickfix/Session.h"
#include "quickfix/fix42/ExecutionReport.h"

void Application::onCreate(const FIX::SessionID&) {}
void Application::onLogon(const FIX::SessionID&) {}
void Application::onLogout(const FIX::SessionID&) {}
void Application::toAdmin(FIX::Message&, const FIX::SessionID&) {}
void Application::toApp( FIX::Message& message,
                         const FIX::SessionID& sessionID )
EXCEPT( FIX::DoNotSend ) {}

void Application::fromAdmin( const FIX::Message& message,
                             const FIX::SessionID& sessionID )
EXCEPT( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::RejectLogon ) {}

void Application::fromApp( const FIX::Message& message,
                           const FIX::SessionID& sessionID )
EXCEPT( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType )
{ crack( message, sessionID ); }

void Application::onMessage( const FIX40::NewOrderSingle& message,
                             const FIX::SessionID& sessionID )
{
  FIX::Symbol symbol;
  FIX::Side side;
  FIX::OrdType ordType;
  FIX::OrderQty orderQty;
  FIX::Price price;
  FIX::ClOrdID clOrdID;
  FIX::Account account;

  message.get( ordType );

  if ( ordType != FIX::OrdType_LIMIT )
    throw FIX::IncorrectTagValue( ordType.getField() );

  message.get( symbol );
  message.get( side );
  message.get( orderQty );
  message.get( price );
  message.get( clOrdID );

  FIX40::ExecutionReport executionReport = FIX40::ExecutionReport
      ( FIX::OrderID( genOrderID() ),
        FIX::ExecID( genExecID() ),
        FIX::ExecTransType( FIX::ExecTransType_NEW ),
        FIX::OrdStatus( FIX::OrdStatus_FILLED ),
        symbol,
        side,
        orderQty,
        FIX::LastShares( orderQty ),
        FIX::LastPx( price ),
        FIX::CumQty( orderQty ),
        FIX::AvgPx( price ) );

  executionReport.set( clOrdID );

  if( message.isSet(account) )
    executionReport.setField( message.get(account) );

  try
  {
    FIX::Session::sendToTarget( executionReport, sessionID );
  }
  catch ( FIX::SessionNotFound& ) {}
}

// int main()
// {
//   std::cout<<"Start Fix!"<<std::endl;
// }

