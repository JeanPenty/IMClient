#pragma once
class CBase64
{
public:
	CBase64(void);
	~CBase64(void);

	static CBase64* GetInstance();

public:
	std::string base64_encode(unsigned char const*, unsigned int len);
	std::string base64_decode(std::string const& s);
};

#define Base64	CBase64::GetInstance()