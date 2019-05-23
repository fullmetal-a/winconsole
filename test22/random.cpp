#include "random.h"

RandomEngine::RandomEngine()
{
	CryptAcquireContextW(&m_hProvider, 0, 0, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_SILENT);
}

RandomEngine::~RandomEngine()
{
	CryptReleaseContext(m_hProvider, 0);
}