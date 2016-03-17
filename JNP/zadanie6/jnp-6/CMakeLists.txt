cmake_minimum_required(VERSION 3.1)
project(zad)

set(CMAKE_CXX_COMPILER g++)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -g -Wall")

set(SOURCE_FILES Bank.h Bank.cpp InterstellarClock.h InterstellarClock.cpp GwiezdnaKomisjaBankowa.h GwiezdnaKomisjaBankowa.cpp
        BankBuilder.cpp CurrencyTypes.h BankAccount.cpp BankAccount.h IdType.cpp IdType.h
        BankExchangeTable.cpp BankExchangeTable.h gsb.h Person.cpp Person.h AccountInfo.h
        Planets.cpp Planets.h
        AccountRegistry.cpp AccountRegistry.h Utils.h
        History.cpp History.h)

add_executable(zad gsb_example.cpp ${SOURCE_FILES})
add_executable(planet_person_test planet_person_test.cpp ${SOURCE_FILES})
add_executable(marek_test gsb_test01.cpp ${SOURCE_FILES})
add_executable(marek_test2 gsb_test02.cpp ${SOURCE_FILES})

add_executable(currency_account_test currency_account_test.cpp ${SOURCE_FILES})
