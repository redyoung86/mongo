// message_server.h

/*    Copyright 2009 10gen Inc.
 *
 *    This program is free software: you can redistribute it and/or  modify
 *    it under the terms of the GNU Affero General Public License, version 3,
 *    as published by the Free Software Foundation.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Affero General Public License for more details.
 *
 *    You should have received a copy of the GNU Affero General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *    As a special exception, the copyright holders give permission to link the
 *    code of portions of this program with the OpenSSL library under certain
 *    conditions as described in each individual source file and distribute
 *    linked combinations including the program with the OpenSSL library. You
 *    must comply with the GNU Affero General Public License in all respects
 *    for all of the code used other than as permitted herein. If you modify
 *    file(s) with this exception, you may extend this exception to your
 *    version of the file(s), but you are not obligated to do so. If you do not
 *    wish to do so, delete this exception statement from your version. If you
 *    delete this exception statement from all source files in the program,
 *    then also delete it in the license file.
 */

/*
  abstract database server
  async io core, worker thread system
 */

#pragma once

#include "mongo/platform/basic.h"

namespace mongo {

class MessageHandler {
public:
    virtual ~MessageHandler() {}

    /**
     * Called once when a socket is connected.
     */
    virtual void connected(AbstractMessagingPort* p) = 0;

    /**
     * Called every time a message comes in. Handler is responsible for responding to client.
     */
    virtual void process(Message& m, AbstractMessagingPort* p) = 0;

    /**
     * Called once, either when the client disconnects or when the process is shutting down. After
     * close() is called, this handler's AbstractMessagingPort pointer (passed in via the
     * connected() method) is no longer valid.
     */
    virtual void close() = 0;
};

class MessageServer {
public:
    struct Options {
        int port;            // port to bind to
        std::string ipList;  // addresses to bind to

        Options() : port(0), ipList("") {}
    };

    virtual ~MessageServer() {}
    virtual void run() = 0;
    virtual void setAsTimeTracker() = 0;
    virtual bool setupSockets() = 0;
};

// TODO use a factory here to decide between port and asio variations
MessageServer* createServer(const MessageServer::Options& opts, MessageHandler* handler);
}
