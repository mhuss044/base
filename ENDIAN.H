// John Adler
// 2007-12-06
// endian.h
// Converts little and big endian formats

#ifndef ENDIAN_CONV_H_
#define ENDIAN_CONV_H_

#define LITTLE 0
#define BIG 1

class CEndian
{
	public:
		
		CEndian();
		~CEndian();
		
		unsigned int convertLittleToBig(unsigned int num);
		unsigned int convertBigToLittle(unsigned int num);
		void uintToByteArray(unsigned int num, int bytes, unsigned char *arr, int type);
		unsigned int byteArrayToUInt(unsigned char *arr, unsigned int bytes, int type);
		
		int convertLittleToBig(int num);
		int convertBigToLittle(int num);
		void intToByteArray(int num, int bytes, unsigned char *arr, int type);
		int byteArrayToInt(unsigned char *arr, unsigned int bytes, int type);
		
		short convertLittleToBig(short num);
		short convertBigToLittle(short num);
		void shortToByteArray(short num, int bytes, unsigned char *arr, int type);
		short byteArrayToShort(unsigned char *arr, unsigned int bytes, int type);
		
		unsigned short convertLittleToBig(unsigned short num);
		unsigned short convertBigToLittle(unsigned short num);
		void ushortToByteArray(unsigned short num, int bytes, unsigned char *arr, int type);
		unsigned short byteArrayToUShort(unsigned char *arr, unsigned int bytes, int type);
		
		double convertLittleToBig(double num);
		double convertBigToLittle(double num);
		void doubleToByteArray(double num, int bytes, unsigned char *arr, int type);
		double byteArrayToDouble(unsigned char *arr, unsigned int bytes, int type);
		
		
		
} endianConverter;

// Constructor and Destructor
CEndian::CEndian(void)
{
	
}

CEndian::~CEndian(void)
{
	
}

// Unsigned int conversion
unsigned int CEndian::convertLittleToBig(unsigned int num)
{
	unsigned char arr[sizeof(num)];
	uintToByteArray(num, sizeof(num), arr, BIG);
	return byteArrayToUInt(arr, sizeof(num), LITTLE);
}
unsigned int CEndian::convertBigToLittle(unsigned int num)
{
	unsigned char arr[sizeof(num)];
	uintToByteArray(num, sizeof(num), arr, LITTLE);
	return byteArrayToUInt(arr, sizeof(num), BIG);
}
void CEndian::uintToByteArray(unsigned int num, int bytes, unsigned char *arr, int type)
{  
	int i;
	unsigned char ch;
	for (i = 0; i < bytes; i++)
	{
		ch = (num >> ((i & 7) << 3)) & 0xFF;
		if (type == LITTLE)
			arr[i] = ch;
		else if (type == BIG)
			arr[bytes - i - 1] = ch;
	}  
}  
unsigned int CEndian::byteArrayToUInt(unsigned char *arr, unsigned int bytes, int type)
{
	unsigned int num = 0;
	unsigned int tmp;
	
	size_t i;
	for (i = 0; i < bytes; i++)
	{
		tmp = 0;
		if (type == LITTLE)
			tmp = arr[i];
		else if (type == BIG)
			tmp = arr[bytes - i - 1];
		
		num |= (tmp << ((i & 7) << 3));
	}
	return num;
}

// Int conversion
int CEndian::convertLittleToBig(int num)
{
	unsigned char arr[sizeof(num)];
	intToByteArray(num, sizeof(num), arr, BIG);
	return byteArrayToInt(arr, sizeof(num), LITTLE);
}
int CEndian::convertBigToLittle(int num)
{
	unsigned char arr[sizeof(num)];
	intToByteArray(num, sizeof(num), arr, LITTLE);
	return byteArrayToInt(arr, sizeof(num), BIG);
}
void CEndian::intToByteArray(int num, int bytes, unsigned char *arr, int type)
{  
	int i;
	unsigned char ch;
	for (i = 0; i < bytes; i++)
	{
		ch = (num >> ((i & 7) << 3)) & 0xFF;
		if (type == LITTLE)
			arr[i] = ch;
		else if (type == BIG)
			arr[bytes - i - 1] = ch;
	}  
}  
int CEndian::byteArrayToInt(unsigned char *arr, unsigned int bytes, int type)
{
	int num = 0;
	int tmp;
	
	size_t i;
	for (i = 0; i < bytes; i++)
	{
		tmp = 0;
		if (type == LITTLE)
			tmp = arr[i];
		else if (type == BIG)
			tmp = arr[bytes - i - 1];
		
		num |= (tmp << ((i & 7) << 3));
	}
	return num;
}


// Short conversion
short CEndian::convertLittleToBig(short num)
{
	unsigned char arr[sizeof(num)];
	shortToByteArray(num, sizeof(num), arr, BIG);
	return byteArrayToShort(arr, sizeof(num), LITTLE);
}
short CEndian::convertBigToLittle(short num)
{
	unsigned char arr[sizeof(num)];
	shortToByteArray(num, sizeof(num), arr, LITTLE);
	return byteArrayToShort(arr, sizeof(num), BIG);
}
void CEndian::shortToByteArray(short num, int bytes, unsigned char *arr, int type)
{  
	int i;
	unsigned char ch;
	for (i = 0; i < bytes; i++)
	{
		ch = (num >> ((i & 7) << 3)) & 0xFF;
		if (type == LITTLE)
			arr[i] = ch;
		else if (type == BIG)
			arr[bytes - i - 1] = ch;
	}  
}  
short CEndian::byteArrayToShort(unsigned char *arr, unsigned int bytes, int type)
{
	short num = 0;
	short tmp;
	
	size_t i;
	for (i = 0; i < bytes; i++)
	{
		tmp = 0;
		if (type == LITTLE)
			tmp = arr[i];
		else if (type == BIG)
			tmp = arr[bytes - i - 1];
		
		num |= (tmp << ((i & 7) << 3));
	}
	return num;
}


// Unsigned Short conversion
unsigned short CEndian::convertLittleToBig(unsigned short num)
{
	unsigned char arr[sizeof(num)];
	ushortToByteArray(num, sizeof(num), arr, BIG);
	return byteArrayToUShort(arr, sizeof(num), LITTLE);
}
unsigned short CEndian::convertBigToLittle(unsigned short num)
{
	unsigned char arr[sizeof(num)];
	ushortToByteArray(num, sizeof(num), arr, LITTLE);
	return byteArrayToUShort(arr, sizeof(num), BIG);
}
void CEndian::ushortToByteArray(unsigned short num, int bytes, unsigned char *arr, int type)
{  
	int i;
	unsigned char ch;
	for (i = 0; i < bytes; i++)
	{
		ch = (num >> ((i & 7) << 3)) & 0xFF;
		if (type == LITTLE)
			arr[i] = ch;
		else if (type == BIG)
			arr[bytes - i - 1] = ch;
	}  
}  
unsigned short CEndian::byteArrayToUShort(unsigned char *arr, unsigned int bytes, int type)
{
	unsigned short num = 0;
	unsigned short tmp;
	
	size_t i;
	for (i = 0; i < bytes; i++)
	{
		tmp = 0;
		if (type == LITTLE)
			tmp = arr[i];
		else if (type == BIG)
			tmp = arr[bytes - i - 1];
		
		num |= (tmp << ((i & 7) << 3));
	}
	return num;
}

// Double conversion
double CEndian::convertLittleToBig(double num)
{
	unsigned char arr[sizeof(num)];
	doubleToByteArray(num, sizeof(num), arr, BIG);
	return byteArrayToDouble(arr, sizeof(num), LITTLE);
}
double CEndian::convertBigToLittle(double num)
{
	unsigned char arr[sizeof(num)];
	doubleToByteArray(num, sizeof(num), arr, LITTLE);
	return byteArrayToDouble(arr, sizeof(num), BIG);
}
void CEndian::doubleToByteArray(double num, int bytes, unsigned char *arr, int type)
{
	long longnum = (long)num;
	int i;
	unsigned char ch;
	for (i = 0; i < bytes; i++)
	{
		ch = (longnum >> ((i & 7) << 3)) & 0xFF;
		if (type == LITTLE)
			arr[i] = ch;
		else if (type == BIG)
			arr[bytes - i - 1] = ch;
	}  
}  
double CEndian::byteArrayToDouble(unsigned char *arr, unsigned int bytes, int type)
{
	long num = 0;
	int tmp;
	
	size_t i;
	for (i = 0; i < bytes; i++)
	{
		tmp = 0;
		if (type == LITTLE)
			tmp = arr[i];
		else if (type == BIG)
			tmp = arr[bytes - i - 1];
		
		num |= (tmp << ((i & 7) << 3));
	}
	return (double)num;
}


#endif	/*ENDIAN_CONV_H_*/

