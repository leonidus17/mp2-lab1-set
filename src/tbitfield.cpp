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
    if (len < 1)
        throw len;
    if (len < 32)
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
    if (n < 0 || n >= BitLen)
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
    if (n < 0 || n >= BitLen)
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
    if (n < 0 || n >= BitLen)
    {
        throw n;
    }
    else
    {
        return (pMem[GetMemIndex(n)] & GetMemMask(n)) !=0;
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
    int resLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    TBitField res(resLen);
    for (int i = 0; i < res.MemLen;++i)
    {
        TELEM a = (i < MemLen) ? pMem[i] : TELEM(0);
        TELEM b = (i < bf.MemLen) ? bf.pMem[i] : TELEM(0);
        res.pMem[i] = a | b;
    }
    int r = res.BitLen & 31;
    if (r != 0)
        res.pMem[res.MemLen - 1] &= (TELEM(1) << r) - 1;
    return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int resLen = (BitLen < bf.BitLen) ? BitLen : bf.BitLen;
    TBitField res(resLen);
    for (int i = 0; i < res.MemLen;++i)
    {
        TELEM a = (i < MemLen) ? pMem[i] : TELEM(0);
        TELEM b = (i < bf.MemLen) ? bf.pMem[i] : TELEM(0);
        res.pMem[i] = a & b;
    }
    int r = res.BitLen & 31;
    if (r != 0)
        res.pMem[res.MemLen - 1] &= (TELEM(1) << r) - 1;
    return res;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField res(BitLen);
    for (int i = 0; i < MemLen;++i)
        res.pMem[i] = ~pMem[i];
    int r = res.BitLen & 31;
    if (r != 0)
        res.pMem[res.MemLen - 1] &= (TELEM(1) << r) - 1;
    return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    int len = bf.GetLength();
    char c;
    for (int i = len - 1; i >= 0; --i)
    {
        if (!(istr >> c))
            break;
        if (c == '1')
            bf.SetBit(i);
        else
            bf.ClrBit(i);
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = bf.GetLength() - 1; i >= 0;--i)
        ostr << bf.GetBit(i);
    return ostr;
}
