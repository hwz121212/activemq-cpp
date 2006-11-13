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

#include <activemq/connector/openwire/marshal/v2/JournalTopicAckMarshaller.h>

#include <activemq/connector/openwire/commands/JournalTopicAck.h>

//
//     NOTE!: This file is autogenerated - do not modify!
//            if you need to make a change, please see the Java Classes in the
//            activemq-core module
//

using namespace std;
using namespace activemq;
using namespace activemq::io;
using namespace activemq::connector;
using namespace activemq::connector::openwire;
using namespace activemq::connector::openwire::commands;
using namespace activemq::connector::openwire::marshal;
using namespace activemq::connector::openwire::utils;
using namespace activemq::connector::openwire::marshal::v2;

///////////////////////////////////////////////////////////////////////////////
DataStructure* JournalTopicAckMarshaller::createObject() const {
    return new JournalTopicAck();
}

///////////////////////////////////////////////////////////////////////////////
unsigned char JournalTopicAckMarshaller::getDataStructureType() const {
    return JournalTopicAck::ID_JOURNALTOPICACK;
}

///////////////////////////////////////////////////////////////////////////////
void JournalTopicAckMarshaller::tightUnmarshal( OpenWireFormat* wireFormat, DataStructure* dataStructure, DataInputStream* dataIn, BooleanStream* bs ) throw( io::IOException ){
   BaseDataStreamMarshaller::tightUnmarshal( wireFormat, dataStructure, dataIn, bs );

    JournalTopicAck* info =
        dynamic_cast<JournalTopicAck*>( dataStructure );
    info->setDestination( dynamic_cast< ActiveMQDestination* >(
        tightUnmarshalNestedObject( wireFormat, dataIn, bs ) ) );
    info->setMessageId( dynamic_cast< MessageId* >(
        tightUnmarshalNestedObject( wireFormat, dataIn, bs ) ) );
    info->setMessageSequenceId( tightUnmarshalLong( wireFormat, dataIn, bs ) );
    info->setSubscritionName( tightUnmarshalString( dataIn, bs ) );
    info->setClientId( tightUnmarshalString( dataIn, bs ) );
    info->setTransactionId( dynamic_cast< TransactionId* >(
        tightUnmarshalNestedObject( wireFormat, dataIn, bs ) ) );
}

///////////////////////////////////////////////////////////////////////////////
int JournalTopicAckMarshaller::tightMarshal1( OpenWireFormat* wireFormat, DataStructure* dataStructure, BooleanStream* bs ) throw( io::IOException ){

    JournalTopicAck* info =
        dynamic_cast<JournalTopicAck*>( dataStructure );

    int rc = BaseDataStreamMarshaller::tightMarshal1( wireFormat, dataStructure, bs );
    rc += tightMarshalNestedObject1( wireFormat, info->getDestination(), bs );
    rc += tightMarshalNestedObject1( wireFormat, info->getMessageId(), bs );
    rc += tightMarshalLong1( wireFormat, info->getMessageSequenceId(), bs );
    rc += tightMarshalString1( info->getSubscritionName(), bs );
    rc += tightMarshalString1( info->getClientId(), bs );
    rc += tightMarshalNestedObject1( wireFormat, info->getTransactionId(), bs );

    return rc + 0;
}

///////////////////////////////////////////////////////////////////////////////
void JournalTopicAckMarshaller::tightMarshal2( OpenWireFormat* wireFormat, DataStructure* dataStructure, DataOutputStream* dataOut, BooleanStream* bs ) throw( io::IOException ){

    BaseDataStreamMarshaller::tightMarshal2( wireFormat, dataStructure, dataOut, bs );

    JournalTopicAck* info =
        dynamic_cast<JournalTopicAck*>( dataStructure );
    tightMarshalNestedObject2( wireFormat, info->getDestination(), dataOut, bs );
    tightMarshalNestedObject2( wireFormat, info->getMessageId(), dataOut, bs );
    tightMarshalLong2( wireFormat, info->getMessageSequenceId(), dataOut, bs );
    tightMarshalString2( info->getSubscritionName(), dataOut, bs );
    tightMarshalString2( info->getClientId(), dataOut, bs );
    tightMarshalNestedObject2( wireFormat, info->getTransactionId(), dataOut, bs );
}

///////////////////////////////////////////////////////////////////////////////
void JournalTopicAckMarshaller::looseUnmarshal( OpenWireFormat* wireFormat, DataStructure* dataStructure, DataInputStream* dataIn ) throw( io::IOException ){
    BaseDataStreamMarshaller::looseUnmarshal( wireFormat, dataStructure, dataIn );
    JournalTopicAck* info = 
        dynamic_cast<JournalTopicAck*>( dataStructure );
    info->setDestination( dynamic_cast< ActiveMQDestination* >( 
        looseUnmarshalNestedObject( wireFormat, dataIn ) ) );
    info->setMessageId( dynamic_cast< MessageId* >( 
        looseUnmarshalNestedObject( wireFormat, dataIn ) ) );
    info->setMessageSequenceId( looseUnmarshalLong( wireFormat, dataIn ) );
    info->setSubscritionName( looseUnmarshalString( dataIn ) );
    info->setClientId( looseUnmarshalString( dataIn ) );
    info->setTransactionId( dynamic_cast< TransactionId* >( 
        looseUnmarshalNestedObject( wireFormat, dataIn ) ) );
}

///////////////////////////////////////////////////////////////////////////////
void JournalTopicAckMarshaller::looseMarshal( OpenWireFormat* wireFormat, DataStructure* dataStructure, DataOutputStream* dataOut ) throw( io::IOException ){
    JournalTopicAck* info =
        dynamic_cast<JournalTopicAck*>( dataStructure );
    BaseDataStreamMarshaller::looseMarshal( wireFormat, dataStructure, dataOut );

    looseMarshalNestedObject( wireFormat, info->getDestination(), dataOut );
    looseMarshalNestedObject( wireFormat, info->getMessageId(), dataOut );
    looseMarshalLong( wireFormat, info->getMessageSequenceId(), dataOut );
    looseMarshalString( info->getSubscritionName(), dataOut );
    looseMarshalString( info->getClientId(), dataOut );
    looseMarshalNestedObject( wireFormat, info->getTransactionId(), dataOut );
}

