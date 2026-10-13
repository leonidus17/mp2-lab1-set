// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
    if (BitLen < 1)
        throw len;
    if (BitLen < 32)
    {
        BitLen = len;
        MemLen = 1;
        pMem = new TELEM[1];
    }
    else
    {
        BitLen = len;
        MemLen = (len - 1) >> 5;
        pMem = new TELEM[MemLen];
    }
    for (int i = 0; i < MemLen; ++i)
    {
        pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    if (pMem != nullptr)
    {
        for (int i = 0; i < MemLen; ++i)
        {
            pMem[i] = bf.pMem[i];
        }
    }
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n >> 5;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return 1<<(n&31);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (-1 <= n || n >= BitLen)
    {
        throw n;
    }
    else
    {
        pMem[GetMemIndex(n)] |= GetMemMask(n);
    }
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (-1 <= n || n >= BitLen)
    {
        throw n;
    }
    else
    {
        pMem[GetMemIndex(n)] &= ~GetMemMask(n);
    }
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (-1 <= n || n >= BitLen)
    {
        throw n;
    }
    else
    {
        return pMem[GetMemIndex(n) & GetMemMask(n)];
    }
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (*this != bf)
    {
        BitLen = bf.BitLen;
        if (MemLen != bf.MemLen)
        {
            MemLen = bf.MemLen;
            if (pMem != nullptr)
                delete[]pMem;
            pMem = new TELEM[MemLen];
        }
        if (pMem != nullptr)
        {
            for (int i = 0; i < MemLen; ++i)
            {
                pMem[i] = bf.pMem[i];
            }
        }
    }
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    int res = 1;
    if (BitLen != bf.BitLen)
        res = 0;
    else
    {
        for (int i = 0; i < MemLen; ++i)
        {
            unsigned int memMask = 0;
            if (i < MemLen - 1)
            {
                memMask = 0xFFFFFFFF;
            }
            else
            {
                memMask = (1 << (BitLen % 32)) - 1;
            }
            if ((memMask & pMem[i]) != (memMask & bf.pMem[i]))
            {
                res = 0;
                break;
            }
        }
    }
    return res;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    return FAKE_BITFIELD;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    return FAKE_BITFIELD;
}

TBitField TBitField::operator~(void) // отрицание
{
    
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    return ostr;
}
