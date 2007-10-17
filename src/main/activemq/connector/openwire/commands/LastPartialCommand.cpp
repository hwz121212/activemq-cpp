/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <activemq/connector/openwire/commands/LastPartialCommand.h>
#include <activemq/exceptions/NullPointerException.h>

using namespace std;
using namespace activemq;
using namespace activemq::exceptions;
using namespace activemq::connector;
using namespace activemq::connector::openwire;
using namespace activemq::connector::openwire::commands;

/*
 *
 *  Command and marshaling code for OpenWire format for LastPartialCommand
 *
 *
 *  NOTE!: This file is autogenerated - do not modify!
 *         if you need to make a change, please see the Java Classes in the
 *         activemq-core module
 *
 */
////////////////////////////////////////////////////////////////////////////////
LastPartialCommand::LastPartialCommand()
{
}

////////////////////////////////////////////////////////////////////////////////
LastPartialCommand::~LastPartialCommand()
{
}

////////////////////////////////////////////////////////////////////////////////
LastPartialCommand* LastPartialCommand::cloneDataStructure() const {
    LastPartialCommand* lastPartialCommand = new LastPartialCommand();

    // Copy the data from the base class or classes
    lastPartialCommand->copyDataStructure( this );

    return lastPartialCommand;
}

////////////////////////////////////////////////////////////////////////////////
void LastPartialCommand::copyDataStructure( const DataStructure* src ) {

    // Copy the data of the base class or classes
    PartialCommand::copyDataStructure( src );

    const LastPartialCommand* srcPtr = dynamic_cast<const LastPartialCommand*>( src );

    if( srcPtr == NULL || src == NULL ) {
    
        throw exceptions::NullPointerException(
            __FILE__, __LINE__,
            "LastPartialCommand::copyDataStructure - src is NULL or invalid" );
    }
}

////////////////////////////////////////////////////////////////////////////////
unsigned char LastPartialCommand::getDataStructureType() const {
    return LastPartialCommand::ID_LASTPARTIALCOMMAND; 
}

////////////////////////////////////////////////////////////////////////////////
std::string LastPartialCommand::toString() const {

    ostringstream stream;

    stream << "Begin Class = LastPartialCommand" << std::endl;
    stream << " Value of LastPartialCommand::ID_LASTPARTIALCOMMAND = 61" << std::endl; 
    stream << PartialCommand::toString();
    stream << "End Class = LastPartialCommand" << std::endl;

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
bool LastPartialCommand::equals( const DataStructure* value ) const {
    const LastPartialCommand* valuePtr = dynamic_cast<const LastPartialCommand*>( value );

    if( valuePtr == NULL || value == NULL ) {
        return false;
    }
    if( !PartialCommand::equals( value ) ) {
        return false;
    }
    return true;
}

