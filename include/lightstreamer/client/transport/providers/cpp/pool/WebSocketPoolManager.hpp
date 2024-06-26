/******************************************************************************
    Author: Joaquin Bejar Garcia 
    Email: jb@taunais.com 
    Date: 26/4/24
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

#ifndef LIGHTSTREAMER_LIB_CLIENT_CPP_WEBSOCKETPOOLMANAGER_HPP
#define LIGHTSTREAMER_LIB_CLIENT_CPP_WEBSOCKETPOOLMANAGER_HPP
#include <memory>
#include <utility>
#include "HttpClient.h"
#include "ExtendedNettyFullAddress.h"

#include <optional>
#include <queue>
#include <mutex>
#include <condition_variable>

#include <DotNetty/Transport/Channels/Pool.h>
#include <Lightstreamer/DotNet/Logging/Log.h>

namespace lightstreamer::client::transport::providers::cpp::pool {


    template<typename T>
    class IChannel
    {
    public:
        bool send(T item)
        {
            std::lock_guard<std::mutex> lock(mtx);
            data_queue.push(std::move(item));
            cond_var.notify_one();
            return true;
        }

        std::optional<T> recv()
        {
            std::unique_lock<std::mutex> lock(mtx);
            cond_var.wait(lock, [this]{ return !data_queue.empty(); });
            T result = std::move(data_queue.front());
            data_queue.pop();
            return result;
        }

        bool close()
        {
            // TODO: Implement this
            return true;
        }

    private:
        std::queue<T> data_queue;
        std::mutex mtx;
        std::condition_variable cond_var;
    };


    class IChannelPool
    {
    public:
        IChannelPool(size_t size)
        {
            for(size_t i = 0; i < size; ++i)
            {
                connections.push(new Connection);
            }
        }

        ~IChannelPool()
        {
            while(!connections.empty())
            {
                auto conn = connections.front();
                connections.pop();
                delete conn;
            }
        }

        Connection* acquire()
        {
            std::unique_lock<std::mutex> lock(mtx);
            while(connections.empty())
            {
                cond_var.wait(lock);
            }
            auto conn = connections.front();
            connections.pop();
            return conn;
        }

        void release(Connection* conn)
        {
            std::unique_lock<std::mutex> lock(mtx);
            connections.push(conn);
            cond_var.notify_one();
        }

    private:
        std::queue<Connection*> connections;
        std::mutex mtx;
        std::condition_variable cond_var;
    };



    // Un pool de canales que comparte conexiones WebSocket.
    class WebSocketPoolManager
    {
    public:
        // TEST ONLY
        virtual IChannelPoolHandler* decorateChannelPoolHandler(IChannelPoolHandler* handler)
        {
            return handler;
        }

        // Obtiene un canal del pool.
        virtual IChannelPool* get(ExtendedNettyFullAddress addr)
        {
            return poolMap.Get(addr);
        }

        void Dispose()
        {
            poolMap.Dispose();
        }

        WebSocketPoolManager(HttpPoolManager httpPoolMap)
        {
            this->poolMap = new AbstractChannelPoolMapAnonymousInnerClass(this, httpPoolMap);
        }

    private:
        static ILogger* log = LogManager::GetLogger(Constants::NETTY_POOL_LOG);

        AbstractChannelPoolMap<ExtendedNettyFullAddress, WebSocketChannelPool>* poolMap;

        class AbstractChannelPoolMapAnonymousInnerClass : public AbstractChannelPoolMap<ExtendedNettyFullAddress, WebSocketChannelPool>
        {
        public:
            AbstractChannelPoolMapAnonymousInnerClass(WebSocketPoolManager* outerInstance, HttpPoolManager httpPoolMap)
            {
                this->outerInstance = outerInstance;
                this->httpPoolMap = httpPoolMap;
            }

            WebSocketChannelPool* NewPool(ExtendedNettyFullAddress key)
            {
                HttpPoolManager::HttpChannelPool* httpPool = httpPoolMap.getChannelPool(key.Address);

                IChannelPoolHandler* wsPoolHandler = outerInstance->decorateChannelPoolHandler(new WebSocketChannelPoolHandler());
                WebSocketChannelPool* wsPool = new WebSocketChannelPool(httpPool, key, wsPoolHandler);

                return wsPool;
            }

        private:
            WebSocketPoolManager* outerInstance;
            HttpPoolManager httpPoolMap;
        };

        // Controlador que es llamado por el administrador del pool cuando un canal es adquirido o liberado.
        class WebSocketChannelPoolHandler : public BaseChannelPoolHandler
        {
        public:
            void ChannelReleased(IChannel* ch)
            {
                BaseChannelPoolHandler::ChannelReleased(ch);
                if (log->IsDebugEnabled())
                {
                    log->Debug("WebSocket channel released [" + ch->Id + "]");
                }
            }

            void ChannelAcquired(IChannel* ch)
            {
                BaseChannelPoolHandler::ChannelAcquired(ch);
                if (log->IsDebugEnabled())
                {
                    log->Debug("WebSocket channel acquired [" + ch->Id + "]");
                }
            }

            void ChannelCreated(IChannel* ch)
            {
                BaseChannelPoolHandler::ChannelCreated(ch);
                // PipelineUtils.populateHttpPipeline(ch, key, new NettySocketHandler());
                if (log->IsDebugEnabled())
                {
                    log->Debug("WebSocket channel created [" + ch->Id + "]");
                }
            }
        };
    };

}

#endif //LIGHTSTREAMER_LIB_CLIENT_CPP_WEBSOCKETPOOLMANAGER_HPP
