/******************************************************************************
    Author: Joaquin Bejar Garcia 
    Email: jb@taunais.com 
    Date: 12/3/24
 ******************************************************************************/

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

#ifndef LIGHTSTREAMER_LIB_CLIENT_CPP_OFFLINESTATUSINTERFACE_HPP
#define LIGHTSTREAMER_LIB_CLIENT_CPP_OFFLINESTATUSINTERFACE_HPP

#include <string>

namespace lightstreamer::client::platform_data::offline {

    class OfflineStatusInterface {
    public:
        virtual ~OfflineStatusInterface() = default;

        virtual bool isOffline(const std::string &server) = 0;
    };
}
#endif //LIGHTSTREAMER_LIB_CLIENT_CPP_OFFLINESTATUSINTERFACE_HPP
