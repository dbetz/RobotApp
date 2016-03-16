#include <string.h>
#include "ELClient.h"
#include "ELClientHTTP.h"

ELClientHTTP::ELClientHTTP(ELClient *e) : m_elc(e)
{
}

int ELClientHTTP::setPath(const char *path)
{
  m_elc->Request(CMD_HTTP_PATH, (uint32_t)&requestCb, 1);
  m_elc->Request(path, strlen(path));
  m_elc->Request();

  ELClientPacket *pkt = m_elc->WaitReturn();
  return pkt ? pkt->value : -1;
}

int ELClientHTTP::sendResponse(int code, const char *body)
{
  m_elc->Request(CMD_HTTP_RESPONSE, code, 1);
  m_elc->Request(body, strlen(body));
  m_elc->Request();

  ELClientPacket *pkt = m_elc->WaitReturn();
  return pkt ? pkt->value : -1;
}
