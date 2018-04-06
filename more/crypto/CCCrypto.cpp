
#include "cocos2d.h"
#include "CCCrypto.h"

extern "C" {
#include "base64/libbase64.h"
#include "md5/md5.h"
}
#include "xxtea/xxtea.h"


#include "CCLuaEngine.h"

extern "C" {
#include "lua.h"
}
#include "tolua_fix.h"
#include <string.h>

USING_NS_CC;


int Crypto::getAES256KeyLength(void)
{
    return 64;
}

unsigned char* Crypto::encryptXXTEA(unsigned char* plaintext,
                                      int plaintextLength,
                                      unsigned char* key,
                                      int keyLength,
                                      int* resultLength)
{
    xxtea_long len;
    unsigned char* result = xxtea_encrypt(plaintext, (xxtea_long)plaintextLength, key, (xxtea_long)keyLength, &len);
    *resultLength = (int)len;
    return result;
}

unsigned char* Crypto::decryptXXTEA(unsigned char* ciphertext,
                                      int ciphertextLength,
                                      unsigned char* key,
                                      int keyLength,
                                      int* resultLength)
{
    xxtea_long len;
    unsigned char* result = xxtea_decrypt(ciphertext, (xxtea_long)ciphertextLength, key, (xxtea_long)keyLength, &len);
    *resultLength = (int)len;
    return result;
}

int Crypto::encodeBase64Len(const char* input, int inputLength)
{
    return Base64encode_len(inputLength);
}

int Crypto::encodeBase64(const char* input,
                           int inputLength,
                           char* output,
                           int outputBufferLength)
{
    CCAssert(Base64encode_len(inputLength) <= outputBufferLength, "Crypto::encodeBase64() - outputBufferLength too small");
    return Base64encode(output, input, inputLength);
}

int Crypto::decodeBase64Len(const char* input)
{
    return Base64decode_len(input);
}

int Crypto::decodeBase64(const char* input,
                           char* output,
                           int outputBufferLength)
{
    CCAssert(Base64decode_len(input) <= outputBufferLength, "Crypto::decodeBase64() - outputBufferLength too small");
    return Base64decode(output, input);
}

void Crypto::MD5(void* input, int inputLength, unsigned char* output)
{
    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, input, inputLength);
    MD5_Final(output, &ctx);
}

void Crypto::MD5File(const char* path, unsigned char* output)
{
    FILE *file = fopen(path, "rb");
    if (file == NULL)
        return;
    
    MD5_CTX ctx;
    MD5_Init(&ctx);
    
    int i;
    const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    while ((i = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        MD5_Update(&ctx, buffer, (unsigned) i);
    }
    
    fclose(file);
    MD5_Final(output, &ctx);
}

const std::string Crypto::MD5String(void* input, int inputLength)
{
    unsigned char buffer[MD5_BUFFER_LENGTH];
    MD5(static_cast<void*>(input), inputLength, buffer);

    //LuaStack* stack = LuaEngine::getInstance()->getLuaStack();
    //stack->clean();
    
    char* hex = bin2hex(buffer, MD5_BUFFER_LENGTH);
    std::string ret(hex);
    delete[] hex;
    return ret;
}



int Crypto::cryptAES256Lua(bool isDecrypt,
                                             const char* input,
                                             int inputLength,
                                             const char* key,
                                             int keyLength)
{
    return 1;
}

int Crypto::encryptXXTEALua(const char* plaintext,
                                     int plaintextLength,
                                     const char* key,
                                     int keyLength)
{
    LuaStack* stack = LuaEngine::getInstance()->getLuaStack();
    stack->clean();

    int resultLength;
    unsigned char* result = encryptXXTEA((unsigned char*)plaintext, plaintextLength, (unsigned char*)key, keyLength, &resultLength);
    
    if (resultLength <= 0)
    {
        lua_pushnil(stack->getLuaState());
    }
    else
    {
        lua_pushlstring(stack->getLuaState(), (const char*)result, resultLength);
        free(result);
    }
    return 1;
}

int Crypto::decryptXXTEALua(const char* plaintext,
                                     int plaintextLength,
                                     const char* key,
                                     int keyLength)
{
    LuaStack* stack = LuaEngine::getInstance()->getLuaStack();
    stack->clean();
    
    int resultLength;
    unsigned char* result = decryptXXTEA((unsigned char*)plaintext, plaintextLength, (unsigned char*)key, keyLength, &resultLength);
    
    if (resultLength <= 0)
    {
        lua_pushnil(stack->getLuaState());
    }
    else
    {
        lua_pushlstring(stack->getLuaState(), (const char*)result, resultLength);
        free(result);
    }
    return 1;
}

int Crypto::encodingBase64Lua(bool isDecoding,
                                       const char* input,
                                       int inputLength)
{
    LuaStack* stack = LuaEngine::getInstance()->getLuaStack();
    stack->clean();

    int bufferSize = isDecoding ? Base64decode_len(input) : Base64encode_len(inputLength);
    char *buffer = bufferSize ? (char*)malloc(bufferSize) : NULL;
    int size = 0;

    if (buffer)
    {
        size = isDecoding ? Base64decode(buffer, input) : (Base64encode(buffer, input, inputLength) - 1);
    }
    if (size)
    {
        stack->pushString(buffer, size);
    }
    else
    {
        stack->pushNil();
    }
    if (buffer)
    {
        free(buffer);
    }
    return 1;
}

int Crypto::MD5Lua(const char* inputStr, bool isRawOutput)
{
    unsigned char buffer[MD5_BUFFER_LENGTH];
    char* input = (char*)inputStr;
    MD5(static_cast<void*>(input), (int)strlen(input), buffer);
    
    LuaStack* stack = LuaEngine::getInstance()->getLuaStack();
    stack->clean();
    
    if (isRawOutput)
    {
        stack->pushString((char*)buffer, MD5_BUFFER_LENGTH);
    }
    else
    {
        char* hex = bin2hex(buffer, MD5_BUFFER_LENGTH);
        stack->pushString(hex);
        delete[] hex;
    }
    
    return 1;
}

int Crypto::MD5FileLua(const char* path)
{
    unsigned char buffer[MD5_BUFFER_LENGTH];
    MD5File(path, buffer);
    
    LuaStack* stack = LuaEngine::getInstance()->getLuaStack();
    stack->clean();
    
    char* hex = bin2hex(buffer, MD5_BUFFER_LENGTH);
    stack->pushString(hex);
    delete[] hex;
    
    return 1;
}

char* Crypto::bin2hex(unsigned char* bin, int binLength)
{
    static const char* hextable = "0123456789abcdef";
    
    int hexLength = binLength * 2 + 1;
    char* hex = new char[hexLength];
    memset(hex, 0, sizeof(char) * hexLength);
    
    int ci = 0;
    for (int i = 0; i < 16; ++i)
    {
        unsigned char c = bin[i];
        hex[ci++] = hextable[(c >> 4) & 0x0f];
        hex[ci++] = hextable[c & 0x0f];
    }
    
    return hex;
}

