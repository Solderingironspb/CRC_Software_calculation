/**
 ******************************************************************************
 *  @file CRC_Software_calculation.h
 *  @brief ����������� ������ CRC: CRC8, CRC16, CRC32
 *  @author ������ ����
 *  @date 03.05.2023
 *
 *  YouTube: https://www.youtube.com/channel/UCzZKTNVpcMSALU57G1THoVw
 *  GitHub: https://github.com/Solderingironspb
 *  ������ ��: https://vk.com/solderingiron.stm32
 *  ��� CRC �� ���������: https://ru.wikipedia.org/wiki/%D0%A6%D0%B8%D0%BA%D0%BB%D0%B8%D1%87%D0%B5%D1%81%D0%BA%D0%B8%D0%B9_%D0%B8%D0%B7%D0%B1%D1%8B%D1%82%D0%BE%D1%87%D0%BD%D1%8B%D0%B9_%D0%BA%D0%BE%D0%B4
 *  ���� �� CRC: https://youtu.be/YyQonUJrBn4
 *  ��� �������� ����: http://www.sunshine2k.de/coding/javascript/crc/crc_js.html
 *
 ******************************************************************************
 */

#include <CRC_Software_calculation.h>

/*
 **************************************************************************************************
 *  @breif ������� ������� 8 ��� �� LSB � MSB
 *  ������:           0100 1111 ������ � �������,
 *  �� ������ ������� 1111 0010
 *  @param  *Data - ������, ������� ����� �������������
 *  @retval ���������� ��������������� ������
 **************************************************************************************************
 */

uint8_t Reverse_LSB_MSB_8_Bit(uint8_t *Data) {
	uint8_t Result = 0;
	for (uint16_t i = 0; i < 8; i++) {
		uint8_t X = *Data;
		X = X << (i);
		X = X >> (7);
		if (X) {
			Result |= (1 << i);
		}
	}
	return Result;
}

/*
 **************************************************************************************************
 *  @breif ������� ������� 16 ��� �� LSB � MSB
 *  ������:           0100 1111 0011 0010 ������ � �������,
 *  �� ������ ������� 0100 1100 1111 0010
 *  @param  *Data - ������, ������� ����� �������������
 *  @retval ���������� ��������������� ������
 **************************************************************************************************
 */

uint16_t Reverse_LSB_MSB_16_Bit(uint16_t *Data) {
	uint16_t Result = 0;
	for (uint8_t i = 0; i < 16; i++) {
		uint16_t X = *Data;
		X = X << (i);
		X = X >> (15);
		if (X) {
			Result |= (1 << i);
		}
	}
	return Result;
}

/*
 **************************************************************************************************
 *  @breif ������� ������� 32 ��� �� LSB � MSB
 *  ������:           0100 1111 0011 0010 1100 1100 1101 1110 ������ � �������,
 *  �� ������ ������� 0111 1011 0011 0011 0100 1100 1111 0010
 *  @param  *Data - ������, ������� ����� �������������
 *  @retval ���������� ��������������� ������
 **************************************************************************************************
 */

uint32_t Reverse_LSB_MSB_32_Bit(uint32_t *Data) {
	uint32_t Result = 0;
	for (uint8_t i = 0; i < 32; i++) {
		uint32_t X = *Data;
		X = X << (i);
		X = X >> (31);
		if (X) {
			Result |= (1 << i);
		}
	}
	return Result;
}

/*
 **************************************************************************************************
 *  @breif ������� ������� ����������� ����� CRC8 � ���������� �����������
 *  @attention ������� �������� � �������, ������� ������������ � 8-������ �������
 *  @param  *Data - ������, �� ������� ����� ��������� ����������� �����.
 *  @param  Buffer_lenght - ������ ������ � 8 �����. �.�. 1 - ��� 1 ����.
 *  @param  InitVal - ��������� �������� CRC8. �������� 0xFF
 *  @param  Polynome - �������. �������� 0x9B
 *  @param  RefIn - ����, ����������� �� ������ � ����������� ����������,
 *          ��� ����������� ������� ������ ������ ��������������� ������� �������� � ������.
 *          ���������� ��� ��������: False � ������� �� �������� ��������� ���� (MSB-first)
 *          ��� True � � �������� (LSB-first)
 *  @param  RefOut - ����, ������������, ������������� �� ������� ����� �������� ��� ����� �� ������� XOR
 *  @param  XorOut - �����, � ������� ������������ �� ������ 2 ���������� ���������
 *  @retval ���������� ������������ ����������� ����� CRC8
 **************************************************************************************************
 */

uint8_t CRC8_Bytes_Calculate_software(uint8_t *Data, uint32_t Buffer_lenght, uint8_t InitVal, uint8_t Polynome, bool RefIn, bool RefOut, uint8_t XorOut) {
	uint8_t CRC8 = InitVal;
	while (Buffer_lenght--) {
		uint8_t Value = *Data++;

		if (RefIn) {
			uint8_t Reverse_byte = 0x00;
			for (uint8_t i = 0; i < 1; i++) {
				Reverse_byte = Reverse_byte | (uint16_t) Reverse_LSB_MSB_8_Bit(((uint8_t*) &Value + i));
			}
			Value = Reverse_byte;
		}

		uint8_t Processed_Data = *((uint8_t*) &Value);
		//printf("%X\r\n", Processed_Data);
		CRC8 ^= Processed_Data;
		for (uint8_t i = 0; i < 8; i++) {
			if (CRC8 & 0x80) {
				CRC8 = (CRC8 << 1u) ^ Polynome;
			} else {
				CRC8 = (CRC8 << 1u);
			}
		}

	}
	if (RefOut) {
		CRC8 = Reverse_LSB_MSB_8_Bit((uint8_t*) &CRC8);
	}
	CRC8 = CRC8 ^ XorOut;
	return CRC8;
}

/*
 **************************************************************************************************
 *  @breif ������� ������� ����������� ����� CRC16 � ���������� �����������.
 *  @attention ������� �������� � �������, ������� ������������ � 8-������ �������
 *  @param  *Data - ������, �� ������� ����� ��������� ����������� �����
 *  @param  Buffer_lenght - ������ ������ � ������. �.�. 1 - ��� 1 ����.
 *  @param  InitVal - ��������� �������� CRC16. �������� 0xFFFF
 *  @param  Polynome - �������. �������� 0x8005
 *  @param  RefIn - ����, ����������� �� ������ � ����������� ����������,
 *          ��� ����������� ������� ������ ������ ��������������� ������� �������� � ������.
 *          ���������� ��� ��������: False � ������� �� �������� ��������� ���� (MSB-first)
 *          ��� True � � �������� (LSB-first)
 *  @param  RefOut - ����, ������������, ������������� �� ������� ����� �������� ��� ����� �� ������� XOR
 *  @param  XorOut - �����, � ������� ������������ �� ������ 2 ���������� ���������
 *  @retval ���������� ������������ ����������� ����� CRC16
 **************************************************************************************************
 */

uint16_t CRC16_Bytes_Calculate_software(uint8_t *Data, uint32_t Buffer_lenght, uint16_t InitVal, uint16_t Polynome, bool RefIn, bool RefOut, uint16_t XorOut) {
	uint16_t CRC16 = InitVal;
	while (Buffer_lenght--) {
		uint8_t Value = *Data++;
		if (RefIn) {
			uint8_t Reverse_byte = 0x00;
			Reverse_byte = Reverse_LSB_MSB_8_Bit((uint8_t*) &Value);
			Value = Reverse_byte;
		}
		uint16_t Processed_Data = *((uint8_t*) &Value) << 8U;
		CRC16 ^= Processed_Data;
		for (uint8_t i = 0; i < 8; i++) {
			if (CRC16 & 0x8000) {
				CRC16 = (CRC16 << 1u) ^ Polynome;
			} else {
				CRC16 = (CRC16 << 1u);
			}
		}
	}
	if (RefOut) {
		CRC16 = Reverse_LSB_MSB_16_Bit((uint16_t*) &CRC16);
	}
	CRC16 = CRC16 ^ XorOut;
	return CRC16;
}

/*
 **************************************************************************************************
 *  @breif ������� ������� ����������� ����� CRC32 � ���������� �����������.
 *  @attention ������� �������� � �������, ������� ������������ � 8-������ �������
 *  @param  *Data - ������, �� ������� ����� ��������� ����������� �����
 *  @param  Buffer_lenght - ������ ������ � ������. �.�. 1 - ��� 1 ����.
 *  @param  InitVal - ��������� �������� CRC32. �������� 0xFFFFFFFF
 *  @param  Polynome - �������. �������� 0x04C11DB7
 *  @param  RefIn - ����, ����������� �� ������ � ����������� ����������,
 *          ��� ����������� ������� ������ ������ ��������������� ������� �������� � ������.
 *          ���������� ��� ��������: False � ������� �� �������� ��������� ���� (MSB-first)
 *          ��� True � � �������� (LSB-first)
 *  @param  RefOut - ����, ������������, ������������� �� ������� ����� �������� ��� ����� �� ������� XOR
 *  @param  XorOut - �����, � ������� ������������ �� ������ 2 ���������� ���������
 *  @retval ���������� ������������ ����������� ����� CRC32
 **************************************************************************************************
 */

uint32_t CRC32_Bytes_Calculate_software(uint8_t *Data, uint32_t Buffer_lenght, uint32_t InitVal, uint32_t Polynome, bool RefIn, bool RefOut, uint32_t XorOut) {
	uint32_t CRC32 = InitVal;
	while (Buffer_lenght--) {
		uint8_t Value = *Data++;
		if (RefIn) {
			uint8_t Reverse_byte = 0x00;
			Reverse_byte = Reverse_LSB_MSB_8_Bit((uint8_t*) &Value);
			Value = Reverse_byte;
		}
		uint32_t Processed_Data = *((uint8_t*) &Value) << 24U;
		CRC32 ^= Processed_Data;
		for (uint8_t i = 0; i < 8; i++) {
			if (CRC32 & 0x80000000) {
				CRC32 = (CRC32 << 1u) ^ Polynome;
			} else {
				CRC32 = (CRC32 << 1u);
			}
		}
	}
	if (RefOut) {
		CRC32 = Reverse_LSB_MSB_32_Bit((uint32_t*) &CRC32);
	}
	CRC32 = CRC32 ^ XorOut;
	return CRC32;
}

/*
 **************************************************************************************************
 *  @breif ������� ������� ����������� ����� CRC16 � ���������� �����������
 *  @attention ������� �������� � �������, ������� ������������ � 16-������ �������
 *  @param  *Data - ������, �� ������� ����� ��������� ����������� �����.
 *  @param  Buffer_lenght - ������ ������ � 16 �����. �.�. 1 - ��� 2 �����. 2 - ��� 4 ����� � �.�.
 *  @param  InitVal - ��������� �������� CRC16. �������� 0xFFFF
 *  @param  Polynome - �������. �������� 0x8005
 *  @param  RefIn - ����, ����������� �� ������ � ����������� ����������,
 *          ��� ����������� ������� ������ ������ ��������������� ������� �������� � ������.
 *          ���������� ��� ��������: False � ������� �� �������� ��������� ���� (MSB-first)
 *          ��� True � � �������� (LSB-first)
 *  @param  RefOut - ����, ������������, ������������� �� ������� ����� �������� ��� ����� �� ������� XOR
 *  @param  XorOut - �����, � ������� ������������ �� ������ 2 ���������� ���������
 *  @retval ���������� ������������ ����������� ����� CRC16
 **************************************************************************************************
 */

uint16_t CRC16_Calculate_software(uint16_t *Data, uint32_t Buffer_lenght, uint16_t InitVal, uint16_t Polynome, bool RefIn, bool RefOut, uint16_t XorOut) {
	uint16_t CRC16 = InitVal;
	while (Buffer_lenght--) {
		uint16_t Value = *Data++;

		if (RefIn) {
			uint16_t Reverse_byte = 0x0000;
			for (uint8_t i = 0; i < 2; i++) {
				Reverse_byte = Reverse_byte | (uint16_t) Reverse_LSB_MSB_8_Bit(((uint8_t*) &Value + i)) << (i * 8);
			}
			Value = Reverse_byte;
		}
		for (uint8_t i = 0; i < 2; i++) {
			uint16_t Processed_Data = *((uint8_t*) &Value + (1 - i)) << 8U;
			//printf("%X\r\n", Processed_Data);
			CRC16 ^= Processed_Data;
			for (uint8_t i = 0; i < 8; i++) {
				if (CRC16 & 0x8000) {
					CRC16 = (CRC16 << 1u) ^ Polynome;
				} else {
					CRC16 = (CRC16 << 1u);
				}
			}
		}
	}
	if (RefOut) {
		CRC16 = Reverse_LSB_MSB_16_Bit((uint16_t*) &CRC16);
	}
	CRC16 = CRC16 ^ XorOut;
	return CRC16;
}



/*
 **************************************************************************************************
 *  @breif ������� ������� ����������� ����� CRC32 � ���������� �����������
 *  @attention ������� �������� � �������, ������� ������������ � 32-������ �������
 *  @param  *Data - ������, �� ������� ����� ��������� ����������� �����.
 *  @param  Buffer_lenght - ������ ������ � 32 �����. �.�. 1 - ��� 4 �����. 2 - ��� 8 ���� � �.�.
 *  @param  InitVal - ��������� �������� CRC32. �������� 0xFFFFFFFF
 *  @param  Polynome - �������. �������� 0x04C11DB7
 *  @param  RefIn - ����, ����������� �� ������ � ����������� ����������,
 *          ��� ����������� ������� ������ ������ ��������������� ������� �������� � ������.
 *          ���������� ��� ��������: False � ������� �� �������� ��������� ���� (MSB-first)
 *          ��� True � � �������� (LSB-first)
 *  @param  RefOut - ����, ������������, ������������� �� ������� ����� �������� ��� ����� �� ������� XOR
 *  @param  XorOut - �����, � ������� ������������ �� ������ 2 ���������� ���������
 *  @retval ���������� ������������ ����������� ����� CRC32
 **************************************************************************************************
 */

uint32_t CRC32_Calculate_software(uint32_t *Data, uint32_t Buffer_lenght, uint32_t InitVal, uint32_t Polynome, bool RefIn, bool RefOut, uint32_t XorOut) {
	uint32_t CRC32 = InitVal;
	while (Buffer_lenght--) {
		uint32_t Value = *Data++;
		if (RefIn) {
			uint32_t Reverse_byte = 0x00000000;
			for (uint8_t i = 0; i < 4; i++) {
				Reverse_byte = Reverse_byte | (uint32_t) Reverse_LSB_MSB_8_Bit(((uint8_t*) &Value + i)) << (i * 8);
			}
			Value = Reverse_byte;
		}
		for (uint8_t i = 0; i < 4; i++) {
			uint32_t Processed_Data = *((uint8_t*) &Value + (3 - i)) << 24U;
			CRC32 ^= Processed_Data;
			for (uint8_t i = 0; i < 8; i++) {
				if (CRC32 & 0x80000000) {
					CRC32 = (CRC32 << 1u) ^ Polynome;
				} else {
					CRC32 = (CRC32 << 1u);
				}
			}
		}
	}
	if (RefOut) {
		CRC32 = Reverse_LSB_MSB_32_Bit((uint32_t*) &CRC32);
	}
	CRC32 = CRC32 ^ XorOut;
	return CRC32;
}


