/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "Socket.h"

#include <decaf/net/SocketImpl.h>
#include <decaf/net/SocketImplFactory.h>

#include <decaf/internal/net/tcp/TcpSocket.h>

using namespace decaf;
using namespace decaf::net;
using namespace decaf::io;
using namespace decaf::internal;
using namespace decaf::internal::net;
using namespace decaf::internal::net::tcp;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
SocketImplFactory* Socket::factory = NULL;

////////////////////////////////////////////////////////////////////////////////
Socket::Socket() : impl(NULL), created(false), connected(false), closed(false),
                   bound(false), inputShutdown(false), outputShutdown(false) {

    if( this->factory != NULL ) {
        this->impl = factory->createSocketImpl();
        return;
    }

    this->impl = new TcpSocket();
}

////////////////////////////////////////////////////////////////////////////////
Socket::Socket( SocketImpl* impl ) : impl(impl), created(false), connected(false), closed(false),
                                     bound(false), inputShutdown(false), outputShutdown(false) {

    if( impl == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "SocketImpl pointer passed was Null." );
    }
}

////////////////////////////////////////////////////////////////////////////////
Socket::Socket( const std::string& host, int port ) : impl(NULL), created(false), connected(false),
                                                      closed(false), bound(false),
                                                      inputShutdown(false), outputShutdown(false){

    if( port < 0 || port > 65535 ) {
        throw IllegalArgumentException(
            __FILE__, __LINE__, "Port specified is out of range: %d", port );
    }

    try{

        if( this->factory != NULL ) {
            this->impl = factory->createSocketImpl();
        } else {
            this->impl = new TcpSocket();
        }

        this->initSocketImpl( host, port, "", 0 );
    }
    DECAF_CATCH_RETHROW( UnknownHostException )
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
Socket::Socket( const std::string& host, int port, const std::string& localAddress, int localPort ) :
    impl(NULL), created(false), connected(false), closed(false), bound(false),
    inputShutdown(false), outputShutdown(false) {

    if( port < 0 || port > 65535 ) {
        throw IllegalArgumentException(
            __FILE__, __LINE__, "Port specified is out of range: %d", port );
    }

    try{

        if( this->factory != NULL ) {
            this->impl = factory->createSocketImpl();
        } else {
            this->impl = new TcpSocket();
        }

        this->initSocketImpl( host, port, localAddress, localPort );
    }
    DECAF_CATCH_RETHROW( UnknownHostException )
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
Socket::~Socket() {
    try{
        close();
        delete this->impl;
    }
    DECAF_CATCH_NOTHROW( Exception )
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void Socket::initSocketImpl( const std::string& host, int port, const std::string& localAddress, int localPort )
    throw( decaf::io::IOException, decaf::net::UnknownHostException ) {

    try{

        ensureCreated();

        try {
            //this->impl->bind( localAddress, localPort );
            this->bound = true;
            this->impl->connect( host, port, -1 );
            this->connected = true;
        } catch( IOException& ex ) {
            this->impl->close();
            throw ex;
        }
    }
    DECAF_CATCH_RETHROW( UnknownHostException )
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
int Socket::getPort() const {
    return this->connected ? this->impl->getPort() : 0;
}

////////////////////////////////////////////////////////////////////////////////
int Socket::getLocalPort() const {
    return this->bound ? this->impl->getLocalPort() : -1;
}

////////////////////////////////////////////////////////////////////////////////
std::string Socket::getInetAddress() const {
    return this->connected ? this->impl->getInetAddress() : "";
}

////////////////////////////////////////////////////////////////////////////////
std::string Socket::getLocalAddress() const {
    return this->bound ? this->impl->getLocalAddress() : "0.0.0.0";
}

////////////////////////////////////////////////////////////////////////////////
void Socket::bind( const std::string& ipaddress, int port )
    throw( decaf::io::IOException, decaf::lang::exceptions::IllegalArgumentException ) {

    checkClosed();

    if( isBound() ) {
        throw IOException(
            __FILE__, __LINE__, "The Socket is already bound." );
    }

    try{

        ensureCreated();

        try {
            this->impl->bind( ipaddress, port );
            this->bound = true;
        } catch( IOException& e ) {
            this->impl->close();
            throw e;
        }
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_RETHROW( IllegalArgumentException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void Socket::close() throw( decaf::io::IOException ) {
    try{
        this->closed = true;
        this->impl->close();
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void Socket::connect( const std::string& host, int port )
    throw( decaf::io::IOException,
           decaf::lang::exceptions::IllegalArgumentException ) {

    if( port < 0 || port > 65535 ) {
        throw IllegalArgumentException(
            __FILE__, __LINE__, "Port specified is out of range: %d", port );
    }

    try{
        this->connect( host, port, 0 );
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_RETHROW( IllegalArgumentException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void Socket::connect( const std::string& host, int port, int timeout )
    throw( decaf::io::IOException,
           decaf::net::SocketTimeoutException,
           decaf::lang::exceptions::IllegalArgumentException ) {

    checkClosed();

    if( timeout < 0 ) {
        throw IllegalArgumentException(
            __FILE__, __LINE__, "Timeout value specified is invalid: %d", timeout );
    }

    if( isConnected() ) {
        throw SocketException(
            __FILE__, __LINE__, "The Socket is already connected." );
    }

    if( host.empty() ) {
        throw IllegalArgumentException(
            __FILE__, __LINE__, "Host cannot be empty." );
    }

    try{

        ensureCreated();

        try {

            if( !isBound() ) {
                //this->impl->bind( "", 0 );
                this->bound = true;
            }

            this->impl->connect( host, port, timeout );
            this->connected = true;

        } catch( IOException& ex ) {
            this->impl->close();
            throw ex;
        }
    }
    DECAF_CATCH_RETHROW( SocketTimeoutException )
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_RETHROW( IllegalArgumentException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
InputStream* Socket::getInputStream() throw( decaf::io::IOException ) {

    checkClosed();

    try{

        if( isInputShutdown() ) {
            throw IOException( __FILE__, __LINE__, "Input was shutdown on this Socket." );
        }

        return this->impl->getInputStream();
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
OutputStream* Socket::getOutputStream() throw( decaf::io::IOException ) {

    checkClosed();

    try{

        if( isOutputShutdown() ) {
            throw IOException( __FILE__, __LINE__, "Output was shutdown on this Socket." );
        }

        return this->impl->getOutputStream();
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void Socket::shutdownInput() throw( decaf::io::IOException ) {

    if( isInputShutdown() ) {
        throw IOException( __FILE__, __LINE__, "Socket input has already been shutdown." );
    }

    checkClosed();

    try{
        this->impl->shutdownInput();
        this->inputShutdown = true;
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void Socket::shutdownOutput() throw( decaf::io::IOException ) {

    if( isOutputShutdown() ) {
        throw IOException( __FILE__, __LINE__, "Socket output has already been shutdown." );
    }

    checkClosed();

    try{
        this->impl->shutdownOutput();
        this->outputShutdown = true;
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
int Socket::getSoLinger() const throw( SocketException ) {

    checkClosed();

    try{
        ensureCreated();
        return this->impl->getOption( SocketOptions::SOCKET_OPTION_LINGER );
    }
    DECAF_CATCH_RETHROW( SocketException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, SocketException )
    DECAF_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
void Socket::setSoLinger( bool state, int timeout )
    throw( SocketException, IllegalArgumentException ) {

    checkClosed();

    try{
        ensureCreated();

        if( state && timeout < 0 ) {
            throw IllegalArgumentException(
                __FILE__, __LINE__, "Timeout value passed is invalid: %d", timeout );
        }

        int value = state ? ( 65535 > timeout ? timeout : 65535 ) : -1;
        this->impl->setOption( SocketOptions::SOCKET_OPTION_LINGER, value );
    }
    DECAF_CATCH_RETHROW( SocketException )
    DECAF_CATCH_RETHROW( IllegalArgumentException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, SocketException )
    DECAF_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
bool Socket::getKeepAlive() const throw( SocketException ) {

    checkClosed();

    try{
        ensureCreated();
        return this->impl->getOption( SocketOptions::SOCKET_OPTION_KEEPALIVE ) == 0 ? false : true;
    }
    DECAF_CATCH_RETHROW( SocketException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, SocketException )
    DECAF_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
void Socket::setKeepAlive( bool keepAlive ) throw( SocketException ) {

    checkClosed();

    try{
        ensureCreated();
        this->impl->setOption( SocketOptions::SOCKET_OPTION_KEEPALIVE, keepAlive ? 1 : 0 );
    }
    DECAF_CATCH_RETHROW( SocketException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, SocketException )
    DECAF_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
int Socket::getReceiveBufferSize() const throw( SocketException ) {

    checkClosed();

    try{
        ensureCreated();
        return this->impl->getOption( SocketOptions::SOCKET_OPTION_RCVBUF );
    }
    DECAF_CATCH_RETHROW( SocketException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, SocketException )
    DECAF_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
void Socket::setReceiveBufferSize( int size )
    throw( SocketException, decaf::lang::exceptions::IllegalArgumentException ) {

    checkClosed();

    if( size <= 0 ) {
        throw IllegalArgumentException(
            __FILE__, __LINE__, "Buffer size given was invalid: %d", size );
    }

    try{
        ensureCreated();
        this->impl->setOption( SocketOptions::SOCKET_OPTION_RCVBUF, size );
    }
    DECAF_CATCH_RETHROW( SocketException )
    DECAF_CATCH_RETHROW( IllegalArgumentException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, SocketException )
    DECAF_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
bool Socket::getReuseAddress() const throw( SocketException ) {

    checkClosed();

    try{
        ensureCreated();
        return this->impl->getOption( SocketOptions::SOCKET_OPTION_REUSEADDR ) == 0 ? false : true;
    }
    DECAF_CATCH_RETHROW( SocketException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, SocketException )
    DECAF_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
void Socket::setReuseAddress( bool reuse ) throw( SocketException ) {

    checkClosed();

    try{
        ensureCreated();
        this->impl->setOption( SocketOptions::SOCKET_OPTION_REUSEADDR, reuse ? 1 : 0 );
    }
    DECAF_CATCH_RETHROW( SocketException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, SocketException )
    DECAF_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
int Socket::getSendBufferSize() const throw( SocketException ) {

    checkClosed();

    try{
        ensureCreated();
        return this->impl->getOption( SocketOptions::SOCKET_OPTION_SNDBUF );
    }
    DECAF_CATCH_RETHROW( SocketException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, SocketException )
    DECAF_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
void Socket::setSendBufferSize( int size )
    throw( SocketException, decaf::lang::exceptions::IllegalArgumentException ) {

    checkClosed();

    if( size <= 0 ) {
        throw IllegalArgumentException(
            __FILE__, __LINE__, "Buffer size given was invalid: %d", size );
    }

    try{
        ensureCreated();
        this->impl->setOption( SocketOptions::SOCKET_OPTION_SNDBUF, size );
    }
    DECAF_CATCH_RETHROW( SocketException )
    DECAF_CATCH_RETHROW( IllegalArgumentException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, SocketException )
    DECAF_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
int Socket::getSoTimeout() const throw( SocketException ) {

    checkClosed();

    try{
        ensureCreated();
        return this->impl->getOption( SocketOptions::SOCKET_OPTION_TIMEOUT );
    }
    DECAF_CATCH_RETHROW( SocketException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, SocketException )
    DECAF_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
void Socket::setSoTimeout( int timeout )
    throw( SocketException, decaf::lang::exceptions::IllegalArgumentException ) {

    checkClosed();

    if( timeout < 0 ) {
        throw IllegalArgumentException(
            __FILE__, __LINE__, "Socket timeout given was invalid: %d", timeout );
    }

    try{
        ensureCreated();
        this->impl->setOption( SocketOptions::SOCKET_OPTION_TIMEOUT, timeout );
    }
    DECAF_CATCH_RETHROW( SocketException )
    DECAF_CATCH_RETHROW( IllegalArgumentException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, SocketException )
    DECAF_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
bool Socket::getTcpNoDelay() const throw ( SocketException ) {

    checkClosed();

    try{
        ensureCreated();
        return this->impl->getOption( SocketOptions::SOCKET_OPTION_TCP_NODELAY ) == 0 ? false : true;
    }
    DECAF_CATCH_RETHROW( SocketException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, SocketException )
    DECAF_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
void Socket::setTcpNoDelay( bool value ) throw ( SocketException ) {

    checkClosed();

    try{
        ensureCreated();
        this->impl->setOption( SocketOptions::SOCKET_OPTION_TCP_NODELAY, value ? 1 : 0 );
    }
    DECAF_CATCH_RETHROW( SocketException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, SocketException )
    DECAF_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
int Socket::getTrafficClass() const throw( SocketException ) {

    checkClosed();

    try{
        ensureCreated();
        return this->impl->getOption( SocketOptions::SOCKET_OPTION_IP_TOS );
    }
    DECAF_CATCH_RETHROW( SocketException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, SocketException )
    DECAF_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
void Socket::setTrafficClass( int value )
    throw( SocketException, decaf::lang::exceptions::IllegalArgumentException ) {

    checkClosed();

    if( value < 0 || value > 255 ) {
        throw IllegalArgumentException(
            __FILE__, __LINE__, "Value of the Traffic class option was invalid: %d", value );
    }

    try{
        ensureCreated();
        this->impl->setOption( SocketOptions::SOCKET_OPTION_IP_TOS, value );
    }
    DECAF_CATCH_RETHROW( SocketException )
    DECAF_CATCH_RETHROW( IllegalArgumentException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, SocketException )
    DECAF_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
bool Socket::getOOBInline() const throw( SocketException ) {

    checkClosed();

    try{
        ensureCreated();
        return this->impl->getOption( SocketOptions::SOCKET_OPTION_OOBINLINE ) == 0 ? false : true;
    }
    DECAF_CATCH_RETHROW( SocketException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, SocketException )
    DECAF_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
void Socket::setOOBInline( bool value ) throw( SocketException ) {

    checkClosed();

    try{
        ensureCreated();
        this->impl->setOption( SocketOptions::SOCKET_OPTION_OOBINLINE, value ? 1 : 0 );
    }
    DECAF_CATCH_RETHROW( SocketException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, SocketException )
    DECAF_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
void Socket::sendUrgentData( int data ) throw( decaf::io::IOException ) {

    checkClosed();

    try{
        ensureCreated();
        this->impl->sendUrgentData( data & 0xFF );
    }
    DECAF_CATCH_RETHROW( SocketException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, SocketException )
    DECAF_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
void Socket::checkClosed() const throw( decaf::io::IOException ) {
    if( this->closed ) {
        throw IOException( __FILE__, __LINE__, "Socket already closed." );
    }
}

////////////////////////////////////////////////////////////////////////////////
void Socket::ensureCreated() const throw( decaf::io::IOException ) {

    try{
        if( !this->created ) {
            this->impl->create();
            this->created = true;
        }
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void Socket::setSocketImplFactory( SocketImplFactory* factory )
    throw( decaf::io::IOException,
           decaf::net::SocketException ) {

    if( Socket::factory != NULL ) {
        throw SocketException(
            __FILE__, __LINE__, "A SocketInplFactory was already set." );
    }

    Socket::factory = factory;
}

////////////////////////////////////////////////////////////////////////////////
void Socket::accepted() {

    this->created = true;
    this->bound = true;
    this->connected = true;
}
