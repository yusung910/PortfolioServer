#include "stdafx.hxx"
#include "RSAUtil.h"

#include <openssl/rsa.h>
#include <openssl/ossl_typ.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/pkcs12.h>
#include <openssl/err.h>

#include "StringUtil.h"
#include "Base64Util.h"

#pragma comment(lib, "crypt32.lib")

#pragma comment(lib, "libssl.lib")
#pragma comment(lib, "libcrypto.lib")

bool RSAUtil::VerifyJWT(const std::string& _base64_N, const std::string& _base64_E, const std::string& _base64_Payload, const std::string& _base64_Signature)
{
    bool lRet = false;

    if (true == _base64_N.empty()
        || true == _base64_E.empty()
        || true == _base64_Payload.empty()
        || true == _base64_Signature.empty())
        return lRet;

    BIGNUM* lN = BN_new();
    BIGNUM* lE = BN_new();

    char* lTmpN = new char[_base64_N.size()] {0, };
    char* lTmpE = new char[_base64_N.size()] {0, };

    int lNSize = static_cast<int>(Base64Util::Decode(_base64_N, lTmpN, _base64_N.size()));
    int lESize = static_cast<int>(Base64Util::Decode(_base64_E, lTmpE, _base64_E.size()));

    BN_bin2bn((const unsigned char*)lTmpN, lNSize, lN);
    BN_bin2bn((const unsigned char*)lTmpE, lESize, lE);

    RSA* lRSA = RSA_new();
    RSA_set0_key(lRSA, lN, lE, nullptr);

    EVP_PKEY* lPublicKey = EVP_PKEY_new();
    EVP_PKEY_set1_RSA(lPublicKey, lRSA);
    EVP_MD_CTX* lCTX = EVP_MD_CTX_create();


    int lSignatureSize = 0;
    char lSignBuffer[512] = { 0, };
    lSignatureSize = static_cast<int>(Base64Util::Decode(_base64_Signature, lSignBuffer, 512));

    if (1 == EVP_DigestVerifyInit(lCTX, nullptr, EVP_sha256(), nullptr, lPublicKey))
    {
        if (1 == EVP_DigestVerifyUpdate(lCTX, _base64_Payload.c_str(), _base64_Payload.length()))
        {
            if (1 == EVP_DigestVerifyFinal(lCTX, (unsigned char*)lSignBuffer, lSignatureSize))
                lRet = true;
        }
    }

    EVP_MD_CTX_free(lCTX);
    lCTX = nullptr;

    EVP_PKEY_free(lPublicKey);
    lPublicKey = nullptr;

    RSA_free(lRSA);
    lRSA = nullptr;

    return lRet;
}
