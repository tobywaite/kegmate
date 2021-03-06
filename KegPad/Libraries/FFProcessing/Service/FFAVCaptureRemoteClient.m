//
//  FFAVCaptureRemoteClient.m
//  KegPad
//
//  Created by Gabriel Handford on 8/8/11.
//  Copyright 2011. All rights reserved.
//

#import "FFAVCaptureRemoteClient.h"
#import "FFUtils.h"


@interface FFAVCaptureRemoteClient ()
@property (retain, nonatomic) KBKegTimeHost *host;
- (void)_closeConnection;
@end


@implementation FFAVCaptureRemoteClient

@synthesize host=_host;

- (id)initWithHost:(KBKegTimeHost *)host {
  if ((self = [super init])) {
    _host = [host retain];
  }
  return self;
}

- (void)dealloc {
  [self _closeConnection];
  [_host release];
  [super dealloc];
}

- (void)_closeConnection {
  _connection.delegate = nil;
  [_connection close];
  [_connection release];
  _connection = nil;
}

- (BOOL)connect {
  [self close];  
  if (_host) {
    _connection = [[FFConnection alloc] init];
    _connection.delegate = self;
    return [_connection openWithName:_host.name ipAddress:_host.ipAddress port:(SInt32)_host.portValue];
  }
  return NO;
}

- (void)connectionDidClose:(FFConnection *)connection {
  [[self gh_proxyAfterDelay:4.0] reconnect];
}

- (void)connection:(FFConnection *)connection didReadBytes:(uint8_t *)bytes length:(NSUInteger)length {
  [self readBytes:bytes length:length];
}

@end
