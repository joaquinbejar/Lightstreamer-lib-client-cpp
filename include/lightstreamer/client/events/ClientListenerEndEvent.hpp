/******************************************************************************
    Author: Joaquin Bejar Garcia 
    Email: jb@taunais.com 
    Date: 8/3/24
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

#ifndef LIGHTSTREAMER_LIB_CLIENT_CPP_CLIENTLISTENERENDEVENT_HPP
#define LIGHTSTREAMER_LIB_CLIENT_CPP_CLIENTLISTENERENDEVENT_HPP

#include <memory>
#include <lightstreamer/client/ClientListener.hpp>
#include <lightstreamer/client/events/Event.hpp>
#include <utility>

namespace lightstreamer::client::events {
    class ClientListenerEndEvent : public Event<ClientListener> {
    private:
        std::shared_ptr<LightstreamerClient> client;

    public:
        explicit ClientListenerEndEvent(std::shared_ptr<LightstreamerClient> &client) : client(client) {}

        void applyTo(ClientListener &listener) const override {
            listener.onListenEnd(client);
        }
    };
}


#endif //LIGHTSTREAMER_LIB_CLIENT_CPP_CLIENTLISTENERENDEVENT_HPP
