#ifndef COMMON_ENCRYPTHELPER_H
#define COMMON_ENCRYPTHELPER_H

#include <QString>

#include "Encrypt/AesCipher.h"
#include "Encrypt/Base64.h"
#include "Encrypt/md5.h"
#include "traderbusiness_global.h"

/*
���ܽ���Key���ɹ���
��һ��������һ���ⲿkey(�磺�û���),Ȼ���ⲿkey��������ַ���������md5ֵ��
�ڶ������Զ���һ��������md5ֵ
��������������md5ֵ�ڻ�ϣ�Ȼ��ȡ���32λ

���ܽ�����ʹ��AES�㷨
*/

namespace Common
{
	class TRADERBUSINESS_EXPORT EncryptHelper
	{
	public:
		static QString encrypt(QString externalKey, QString encryptStr)
		{
			QString md5key = EncryptHelper::getMd5key(externalKey);
			char pOutBuffer[100];
			char pOutBufferBase64[100];
			AesCipher crypto;
			crypto.makeRoundKey(md5key.toStdString().c_str());
			crypto.encrypt(encryptStr.toStdString().c_str(), pOutBuffer, 16, AesCipher::ECB);
			Base64::binToBase64((unsigned char*)pOutBuffer, pOutBufferBase64, 16);
			return pOutBufferBase64;
		}

		static QString decrypt(QString externalKey, QString decryptStr)
		{
			QString md5key = EncryptHelper::getMd5key(externalKey);
			char pOutBuffer[100];
			char pOutBufferBase64[100];
			AesCipher crypto;
			crypto.makeRoundKey(md5key.toStdString().c_str());
			Base64::base64ToBin(decryptStr.toStdString().c_str(), (unsigned char*)pOutBuffer, Base64::calculateBase64Len(16));
			crypto.decrypt(pOutBuffer, pOutBufferBase64, 16, AesCipher::ECB);
			return pOutBufferBase64;
		}

		static QString getMd5key(QString externalKey)
		{
			MD5 md5;
			//�ⲿkey��������ַ���������md5ֵ
			QString csConfusedCharacter = EncryptHelper::getConfusedCharacter(externalKey);           
			md5.update(csConfusedCharacter.toStdString()); 
			string sMD5_Character = md5.toString();
			md5.reset();
			
			//�����ε�md5ֵ
			QString csConfuseSalt = "vFs6G9h#";		
			md5.update(csConfuseSalt.toStdString()); 

			//�������md5ֵ
			return EncryptHelper::getConfuseSalt(sMD5_Character.c_str(), md5.toString().c_str());
		}

		static QString getConfusedCharacter(QString externalKey)
		{
			QString confusedCharacter = "k9B#Sj1h9+/R";
			QString confusedStringResult;
			for(int i=0;i<externalKey.length()*2;i++)
			{
				if ((i % 2)==0)
				{
					confusedStringResult.append(externalKey[i/2]);
					if (i > confusedCharacter.length() - 1)
					{
						confusedStringResult.append(confusedCharacter[confusedCharacter.length() - 1]);
					}
					else 
					{
						confusedStringResult.append(confusedCharacter[i]);
					}
				}
			}
			return confusedStringResult;
		}

		static QString getConfuseSalt(QString character, QString salt)
		{
			QString confusedStringResult;
			for(int i=0;i<character.length();i++)
			{
				confusedStringResult.append(character[i]);
				confusedStringResult.append(salt[i]);
			}
			return confusedStringResult.toUpper().left(32);
		}

		

		
	};
}
#endif
