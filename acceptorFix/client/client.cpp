#include "quickfix/config.h"

#include "quickfix/FileStore.h"
#include "quickfix/SocketInitiator.h"
#ifdef HAVE_SSL
#include "quickfix/ThreadedSSLSocketInitiator.h"
#include "quickfix/SSLSocketInitiator.h"
#endif
#include "quickfix/SessionSettings.h"
#include "quickfix/Log.h"
#include "Application.h"
#include <string>
#include <iostream>
#include <fstream>

#include "src/getopt-repl.h"

int main( int argc, char** argv )
{
  if ( argc < 2 )
  {
    std::cout << "Forma de uso: " << argv[ 0 ]
    << " simulador.cfg" << std::endl;
    return 0;
  }
  std::string file = argv[ 1 ];

#ifdef HAVE_SSL
  std::string isSSL;
  if (argc > 2)
  {
    isSSL.assign(argv[2]);
  }
#endif

  FIX::Initiator * initiator = 0;
  try
  {
    FIX::SessionSettings settings( file );

    Application application;
    FIX::FileStoreFactory storeFactory( settings );
    FIX::ScreenLogFactory logFactory( settings );
    initiator = new FIX::SocketInitiator( application, storeFactory, settings, logFactory );

    initiator->start();
    application.run();
    initiator->stop();
    delete initiator;

    return 0;
  }
  catch ( std::exception & e )
  {
    std::cout << e.what();
    delete initiator;
    return 1;
  }
}