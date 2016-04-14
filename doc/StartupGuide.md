Start-up Guide
============

[TOC]

# Introduction {#Introduction}

This page is a startup guide for any C++ developer willing to make its
application available on the web. A step by step tutorial illustrates the
definition and implementation of application specific features that will easily
be accessed through a REST interface.

# Tutorial {#Tutorial}

## HeartBeat event {#HeartBeatEvent}

The heart-beat event is sent on a regular bases to inform the RESTBridge that
the rendering resource is running and ready to process requests. When the
RESTBridge receives the first heart-beat event from the rendering resource, the
vocabulary available in the rendering resource is requested and made available
to the user-end web application. The RESTBridge will keep on returning 501 HTTP
errors (Resource not available) to the user-end web application until the
rendering resource vocabulary is registered.

~~~
    #include <zeroeq/zeroeq.h>
    #include <zeroeq/hbp/hbp.h>
    #define DEFAULT_HEARTBEAT_TIME 1000.0f
    lunchbox::Clock heartbeatClock;
    ...
    // In this example, the rendering resource is constantly rendering,
    // and the following code is executed for every frame.
    if( heartbeatClock.getTimef() >= DEFAULT_HEARTBEAT_TIME )
    {
        heartbeatClock.reset();
        publisher.publish( zeroeq::Event( zeroeq::vocabulary::EVENT_HEARTBEAT ) );
    }
~~~

## Vocabulary event {#VocabularyEvent}

The vocabulary event is used to register the commands that are supported by the
rendering resource. The rendering resource can publish this event at any time to
inform the RESTBridge of any changes in its functionalities. There are two
types of events:
 - Publisher: An event that is emitted by the rendering resource.
 - Subscriber: An event that is received by the rendering resource.

As an example, a ChangeBackgroundColor event would be a subscriber, and an
ImageJPEG event a publisher.
If an event can be both a subscriber and a publisher, then it should be
registered twice.

A vocabulary event contains the entire list of supported events and completely
replaces any existing registration. In other words, any event that is not listed
in the vocabulary is removed from the RESTBridge internal maps, even it was
previously registered by another vocabulary event.
The following code snippet illustrates the use of the vocabulary event to
register ImageJPEG and Camera events.

~~~
    #include <zeroeq/zeroeq.h>
    #include <zeroeq/hbp/hbp.h>
    // Declare vocabulary
    zeroeq::EventDescriptors vocabulary;
    // Add ImageJPEG event to vocabulary
    vocabulary.push_back( zeroeq::EventDescriptor( zeroeq::hbp::IMAGEJPEG,
                                                zeroeq::hbp::EVENT_IMAGEJPEG,
                                                zeroeq::hbp::SCHEMA_IMAGEJPEG,
                                                zeroeq::PUBLISHER ) );
    // Add CAMERA event to vocabulary
    vocabulary.push_back( zeroeq::EventDescriptor( zeroeq::hbp::CAMERA,
                            zeroeq::hbp::EVENT_CAMERA,
                            zeroeq::hbp::SCHEMA_CAMERA,
                            zeroeq::SUBSCRIBER ) );
    // Publish vocabulary to RESTBridge
    const zeroeq::Event& vocEvent =
        zeroeq::vocabulary::serializeVocabulary( vocabulary );
    publisher.publish( vocEvent );
~~~

## Camera event {#CameraEvent}

The camera event is defined by a 4x4 matrix, with values defined in microns. The
FlatBuffers definition of the event is the following:

~~~
    namespace zeroeq.hbp;
    table Camera
    {
        matrix:[float];
    }
    root_type Camera;
~~~

## ImageJPEG event {#ImageJpegEvent}

The ImageJPEG event is defined by an array of bytes containing a 32bit BGRA
image. The FlatBuffers definition of the event is the following:

~~~
    namespace zeroeq.hbp;
    table ImageJPEG
    {
        data:[ubyte];
    }
    root_type ImageJPEG;
~~~

Here is a simple example of how to generate and publish the event in the
rendering resource application code:

~~~
    #include <zeroeq/zeroeq.h>
    #include <zeroeq/hbp/hbp.h>
    const uint8_t* dataPtr = ...; // Buffer containing the JPEG image bytes
    const uint64_t dataSize = ...; // Buffer size
    const zeroeq::hbp::data::ImageJPEG image( dataSize, dataPtr );
    const zeroeq::Event& image_event = zeroeq::hbp::serializeImageJPEG( image );
    publisher.publish( image_event );
~~~

## ZeroEQ Handlers {#ZeroEQHandlers}

For every ZeroEQ event supported by the C++ application, a handler must be
defined and registered. A Handler is a class method that implements the code
related to the ZeroEQ event. The following example illustrates the registering
of the MyApplication::onCamera method that takes care of the ZeroEQ Camera
event.

~~~
    subscriber_ = new zeroeq::Subscriber(servus::URI( zeroeqSchema_ + "cmd://" ));
    subscriber_->registerHandler( zeroeq::hbp::EVENT_CAMERA,
    boost::bind( &MyApplication::onCamera, this , _1 ));
~~~

## RESTBridge Server {#RESTBridgeServer}

The RestBridge class implements the http server and starts a dedicated thread on
the host and port specified in the class constructor. HTTP requests addressed to
the HTTP server is transformed into a ZeroEQ event, according to the registered
vocabulary. Only GET requests expect a ZeroEQ event in return. If the HTTP
request contains a unknown command (not registered in the vocabulary), a
BAD_REQUEST (HTTP 400) error is returned.

~~~
   restbridge::RestBridge mainServer("localhost", 3000);
   mainServer.run("restSchema");
~~~
