/*******************************************************************************
 Copyright (c) 2024.

 This program is free software: you can redistribute it and/or modify it
 under the terms of the GNU General Public License as published by the
 Free Software Foundation, either version 3 of the License, or (at your
 option) any later version.

 This program is distributed in the hope that it will be useful, but
 WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 Public License for more details.

 You should have received a copy of the GNU General Public License along
 with this program. If not, see <https://www.gnu.org/licenses/>..
 ******************************************************************************/

/******************************************************************************
    Author: Joaquin Bejar Garcia 
    Email: jb@taunais.com 
    Date: 4/3/24
 ******************************************************************************/

#include <catch2/catch_test_macros.hpp>
#include <Logger.hpp>
#include <ConsoleLogLevel.hpp>
#include <ConsoleLoggerProvider.hpp>

using namespace Lightstreamer::Cpp::Logger ;
using namespace Lightstreamer::Cpp::ConsoleLogLevel;
using namespace Lightstreamer::Cpp::ConsoleLoggerProvider;

auto logger = ConsoleLogger::getInstance(Level::TRACE, "category");

TEST_CASE("Logger Level::TRACE", "[Logger]") {
    logger->Debug("I'm logger debug");
    REQUIRE(logger->IsDebugEnabled());
    REQUIRE(logger->getLevel() == Level::TRACE);
}

TEST_CASE("Logger Level::DEBUG", "[Logger]") {
    ConsoleLoggerProvider consoleLoggerProvider(Level::DEBUG); // logger is already instanciated with Level::TRACE
    auto logger_by_provider = consoleLoggerProvider.getLogger("category");
    logger_by_provider->Debug("I'm logger_by_provider debug");
    REQUIRE(logger_by_provider->IsDebugEnabled());
    REQUIRE(logger_by_provider->getLevel() == Level::TRACE);
}
