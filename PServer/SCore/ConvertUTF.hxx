#pragma once

using UTF32 = unsigned long;	/* at least 32 bits */
using UTF16 = unsigned short;	/* at least 16 bits */
using UTF8 = unsigned char;		/* typically 8 bits */
using Boolean = unsigned char;	/* 0 or 1 */

constexpr UTF32 UNI_REPLACEMENT_CHAR = 0x0000FFFD;
constexpr UTF32 UNI_MAX_BMP = 0x0000FFFF;
constexpr UTF32 UNI_MAX_UTF16 = 0x0010FFFF;
constexpr UTF32 UNI_MAX_UTF32 = 0x7FFFFFFF;
constexpr UTF32 UNI_MAX_LEGAL_UTF32 = 0x0010FFFF;

enum class ConversionResult
{
    conversionOK = 0,       /* conversion successful */
    sourceExhausted,        /* partial character in source, but hit end */
    targetExhausted,        /* insuff. room in target for conversion */
    sourceIllegal           /* source sequence is illegal/malformed */
};

enum class ConversionFlags
{
    strictConversion = 0,
    lenientConversion
};

/* This is for C++ and does no harm in C */

ConversionResult ConvertUTF8toUTF16(const UTF8** sourceStart, const UTF8* sourceEnd, UTF16** targetStart, UTF16* targetEnd, ConversionFlags flags);

ConversionResult ConvertUTF16toUTF8(const UTF16** sourceStart, const UTF16* sourceEnd, UTF8** targetStart, UTF8* targetEnd, ConversionFlags flags);

ConversionResult ConvertUTF8toUTF32(const UTF8** sourceStart, const UTF8* sourceEnd, UTF32** targetStart, UTF32* targetEnd, ConversionFlags flags);

ConversionResult ConvertUTF32toUTF8(const UTF32** sourceStart, const UTF32* sourceEnd, UTF8** targetStart, UTF8* targetEnd, ConversionFlags flags);

ConversionResult ConvertUTF16toUTF32(const UTF16** sourceStart, const UTF16* sourceEnd, UTF32** targetStart, UTF32* targetEnd, ConversionFlags flags);

ConversionResult ConvertUTF32toUTF16(const UTF32** sourceStart, const UTF32* sourceEnd, UTF16** targetStart, UTF16* targetEnd, ConversionFlags flags);

Boolean isLegalUTF8Sequence(const UTF8* source, const UTF8* sourceEnd);

int GetUTF8TrailingBytes(const UTF8 utf8Character);


