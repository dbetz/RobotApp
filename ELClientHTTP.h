#ifndef __ELCLIENTHTTP_H__
#define __ELCLIENTHTTP_H__

#include "FP.h"
#include "ELClient.h"

class ELClientHTTP {
public:
    ELClientHTTP(ELClient *e);
    int setPath(const char *url);
    int sendResponse(int code, const char *body);
    
    FP<void, void*> requestCb;
private:
    ELClient *m_elc;
};

#endif
