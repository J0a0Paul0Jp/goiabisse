#include <iostream>

#include "quickfix/config-all.h"
#include "quickfix/MessageCracker.h"
#include "quickfix/Values.h"
#include "quickfix/Utility.h"
#include "quickfix/Mutex.h"
#include "quickfix/Application.h"
#include "quickfix/fix42/NewOrderSingle.h"

using namespace std;

void Application::onCreate(const FIX::SessionID&) {

}

void onLogon(const FIX::SessionID&) {

}


int main() {
   cout << "FIX!" <<endl;
}


