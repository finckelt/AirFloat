//
//  SocketEndPoint.cpp
//  AirFloat
//
//  Created by Kristian Trenskow on 3/8/12.
//  Copyright (c) 2012 The Famous Software Company. All rights reserved.
//

#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include "sockaddr.h"

struct sockaddr* sockaddr_create(const char* host, uint16_t port, sockaddr_type version, uint32_t scope_id) {
    
    if (!version) version = sockaddr_type_inet_4;
    
    if (version == sockaddr_type_inet_4) {
        
        struct sockaddr_in* ret = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_storage));
        bzero(ret, sizeof(struct sockaddr_storage));
        ret->sin_len = sizeof(struct sockaddr_in);
        ret->sin_family = AF_INET;
        ret->sin_addr.s_addr = INADDR_ANY;
        ret->sin_port = htons(port);
        
        if (host != NULL)
            inet_pton(AF_INET, host, &ret->sin_addr);
        
        return (struct sockaddr*)ret;
        
    } else if (version == sockaddr_type_inet_6) {
        
        struct sockaddr_in6* ret = (struct sockaddr_in6*)malloc(sizeof(struct sockaddr_in6));
        bzero(ret, sizeof(struct sockaddr_in6));
        ret->sin6_len = sizeof(struct sockaddr_in6);
        ret->sin6_family = AF_INET6;
        ret->sin6_addr = in6addr_any;
        ret->sin6_port = htons(port);
        ret->sin6_scope_id = scope_id;
        
        if (host != NULL)
            inet_pton(AF_INET6, host, &ret->sin6_addr);
        
        return (struct sockaddr*)ret;
        
    }
    
    return NULL;
    
}

void sockaddr_destroy(struct sockaddr* addr) {
    
    free(addr);
    
}

struct sockaddr* sockaddr_copy(struct sockaddr* addr) {
    
    struct sockaddr* ret = (struct sockaddr*)malloc(addr->sa_len);
    
    memcpy(ret, addr, addr->sa_len);
    
    return ret;
    
}

bool sockaddr_equals(struct sockaddr* addr1, struct sockaddr* addr2) {
    
    return (addr1->sa_len == addr2->sa_len && memcmp(addr1, addr2, addr1->sa_len) == 0);
    
}

bool sockaddr_equals_host(struct sockaddr* addr1, struct sockaddr* addr2) {
    
    if (addr1->sa_family == addr2->sa_family) {
        if (addr1->sa_family == AF_INET)
            return (memcmp(&((struct sockaddr_in*)addr1)->sin_addr, &((struct sockaddr_in*)addr2)->sin_addr, sizeof(struct in_addr)) == 0);
        else if (addr1->sa_family == AF_INET6)
            return (memcmp(&((struct sockaddr_in6*)addr1)->sin6_addr, &((struct sockaddr_in6*)addr2)->sin6_addr, sizeof(struct in6_addr)) == 0);
    }
    
    return 0;
    
}

const char* sockaddr_get_host(struct sockaddr* addr) {
    
    static char ret[1024];
    
    ret[0] = '\0';
    
    if (addr->sa_family == AF_INET)
        inet_ntop(AF_INET, &((struct sockaddr_in*)addr)->sin_addr, ret, 1024);
    else if (addr->sa_family == AF_INET6)
        inet_ntop(AF_INET6, &((struct sockaddr_in6*)addr)->sin6_addr, ret, 1024);
    else
        return NULL;
    
    return ret;
    
}

uint16_t sockaddr_get_port(struct sockaddr* addr) {
    
    if (addr->sa_family == AF_INET)
        return ntohs(((struct sockaddr_in*)addr)->sin_port);
    else if (addr->sa_family == AF_INET6)
        return ntohs(((struct sockaddr_in6*)addr)->sin6_port);
    
    return 0;
    
}

void sockaddr_set_port(struct sockaddr* addr, uint16_t new_port) {
    
    if (addr->sa_family == AF_INET)
        ((struct sockaddr_in*)addr)->sin_port = htons(new_port);
    else if (addr->sa_family == AF_INET6)
        ((struct sockaddr_in6*)addr)->sin6_port = htons(new_port);
    
}

bool sockaddr_is_ipv6(struct sockaddr* addr) {
    
    return (addr->sa_family == AF_INET6);
    
}
