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

#ifndef LIGHTSTREAMER_LIB_CLIENT_CPP_SUBSCRIPTIONLISTENERENDEVENT_HPP
#define LIGHTSTREAMER_LIB_CLIENT_CPP_SUBSCRIPTIONLISTENERENDEVENT_HPP
#include <string>
#include <memory>
#include <lightstreamer/client/ClientMessageListener.hpp>
#include <lightstreamer/client/events/Event.hpp>
#include <utility>
#include <lightstreamer/client/SubscriptionListener.hpp>
#include <lightstreamer/client/Subscription.hpp>

namespace lightstreamer::client::events {

    class SubscriptionListenerEndEvent : public Event<SubscriptionListener> {
    private:
        std::shared_ptr<Subscription> subscription;

    public:
        explicit SubscriptionListenerEndEvent(std::shared_ptr<Subscription> subscription)
                : subscription(std::move(subscription)) {}

        void applyTo(SubscriptionListener& listener) const override {
            listener.onListenEnd(*subscription);
        }
    };

} // namespace lightstreamer::client::events

#endif //LIGHTSTREAMER_LIB_CLIENT_CPP_SUBSCRIPTIONLISTENERENDEVENT_HPP
