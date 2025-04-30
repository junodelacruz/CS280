#include <iostream>
#include <cmath>
#include "val.h"

using namespace std;

// numeric overloaded add this to op
Value Value::operator+(const Value &op) const
{
    if (IsInt() && op.IsInt())
    {
        return Value(GetInt() + op.GetInt());
    }
    else if (IsReal() && op.IsReal())
    {
        return Value(IsReal() + op.IsReal());
    }
    else
    {
        cout << "Run-Time Error-Illegal Mixed Type Operands" << endl;
        return Value();
    }
}

// numeric overloaded subtract op from this
Value Value::operator-(const Value &op) const
{
    if (IsInt() && op.IsInt())
    {
        return Value(GetInt() - op.GetInt());
    }
    else if (IsReal() && op.IsReal())
    {
        return Value(IsReal() - op.IsReal());
    }
    else
    {
        cout << "Run-Time Error-Illegal Mixed Type Operands" << endl;
        return Value();
    }
}

// numeric overloaded multiply this by op
Value Value::operator*(const Value &op) const
{
    if (IsInt() && op.IsInt())
    {
        return Value(GetInt() * op.GetInt());
    }
    else if (IsReal() && op.IsReal())
    {
        return Value(IsReal() * op.IsReal());
    }
    else
    {
        cout << "Run-Time Error-Illegal Mixed Type Operands" << endl;
        return Value();
    }
}

// numeric overloaded divide this by oper
Value Value::operator/(const Value &op) const
{
    if (IsInt() && op.IsInt())
    {
        return Value(GetInt() / op.GetInt());
    }
    else if (IsReal() && op.IsReal())
    {
        return Value(IsReal() / op.IsReal());
    }
    else
    {
        cout << "Run-Time Error-Illegal Mixed Type Operands" << endl;
        return Value();
    }
}

// numeric MOD: overloaded Remainder this by op
Value Value::operator%(const Value &op) const
{
    if (IsInt() && op.IsInt())
    {
        return Value(GetInt() % op.GetInt());
    }
    else
    {
        cout << "Run-Time Error-Illegal Mixed Type Operands" << endl;
        return Value();
    }
}

//(Relational = (Equal): overloaded equality operator of this with op
Value Value::operator==(const Value &op) const
{
    switch (GetType())
    {
    case VINT:
        return Value(GetInt() == op.GetInt());
    case VREAL:
        return Value(GetReal() == op.GetReal());
    case VSTRING:
        return Value(GetString() == op.GetString());
    case VCHAR:
        return Value(GetChar() == op.GetChar());
    case VBOOL:
        return Value(GetBool() == op.GetBool());
    default:
        cout << "Run-Time Error-Illegal Mixed Type Operands" << endl;
        return Value();
    }
}

// numeric MOD: overloaded Remainder this by op
Value Value::operator!=(const Value &op) const
{
    switch (GetType())
    {
    case VINT:
        return Value(GetInt() != op.GetInt());
    case VREAL:
        return Value(GetReal() != op.GetReal());
    case VSTRING:
        return Value(GetString() != op.GetString());
    case VCHAR:
        return Value(GetChar() != op.GetChar());
    case VBOOL:
        return Value(GetBool() != op.GetBool());
    default:
        cout << "Run-Time Error-Illegal Mixed Type Operands" << endl;
        return Value();
    }
}

// overloaded greater than operator of this with op
Value Value::operator>(const Value &op) const
{
    switch (GetType())
    {
    case VINT:
        return Value(GetInt() > op.GetInt());
    case VREAL:
        return Value(GetReal() > op.GetReal());
    case VSTRING:
        return Value(GetString() > op.GetString());
    case VCHAR:
        return Value(GetChar() > op.GetChar());
    case VBOOL:
        return Value(GetBool() > op.GetBool());
    default:
        cout << "Run-Time Error-Illegal Mixed Type Operands" << endl;
        return Value();
    }
}

// overloaded less than operator of this with op
Value Value::operator<(const Value &op) const
{
    switch (GetType())
    {
    case VINT:
        return Value(GetInt() < op.GetInt());
    case VREAL:
        return Value(GetReal() < op.GetReal());
    case VSTRING:
        return Value(GetString() < op.GetString());
    case VCHAR:
        return Value(GetChar() < op.GetChar());
    case VBOOL:
        return Value(GetBool() < op.GetBool());
    default:
        cout << "Run-Time Error-Illegal Mixed Type Operands" << endl;
        return Value();
    }
}

// Relational <= (less than or equal): overloaded less than or equal operator of this with op
Value Value::operator<=(const Value &op) const
{
    switch (GetType())
    {
    case VINT:
        return Value(GetInt() == op.GetInt() || GetInt() < op.GetInt());
    case VREAL:
        return Value(GetReal() == op.GetReal() || GetInt() < op.GetInt());
    case VSTRING:
        return Value(GetString() == op.GetString() || GetInt() < op.GetInt());
    case VCHAR:
        return Value(GetChar() == op.GetChar() || GetInt() < op.GetInt());
    case VBOOL:
        return Value(GetBool() == op.GetBool() || GetInt() < op.GetInt());
    default:
        cout << "Run-Time Error-Illegal Mixed Type Operands" << endl;
        return Value();
    }
}

// Relational >= (Greater than or equal): overloaded Greater than or equal operator of this with op
Value Value::operator>=(const Value &op) const
{
    switch (GetType())
    {
    case VINT:
        return Value(GetInt() == op.GetInt() || GetInt() > op.GetInt());
    case VREAL:
        return Value(GetReal() == op.GetReal() || GetInt() > op.GetInt());
    case VSTRING:
        return Value(GetString() == op.GetString() || GetInt() > op.GetInt());
    case VCHAR:
        return Value(GetChar() == op.GetChar() || GetInt() > op.GetInt());
    case VBOOL:
        return Value(GetBool() == op.GetBool() || GetInt() > op.GetInt());
    default:
        cout << "Run-Time Error-Illegal Mixed Type Operands" << endl;
        return Value();
    }
}

// Logical and: overloaded logical Anding operator of this with op
Value Value::operator&&(const Value &op) const
{
    if (IsBool() && op.IsBool())
    {
        return Value(GetBool() && op.GetBool());
    }
    else
    {
        cout << "Run-Time Error-Illegal Mixed Type Operands" << endl;
        return Value();
    }
}

// Logical or: overloaded logical Oring operator of this with op
Value Value::operator||(const Value &op) const
{
    if (IsBool() && op.IsBool())
    {
        return Value(GetBool() || op.GetBool());
    }
    else
    {
        cout << "Run-Time Error-Illegal Mixed Type Operands" << endl;
        return Value();
    }
}

// Logical not: overloaded logical Complement operator of this object
Value Value::operator!(void) const
{
    if (IsBool())
    {
        return Value(!GetBool());
    }
    else
    {
        cout << "Run-Time Error-Illegal Mixed Type Operands" << endl;
        return Value();
    }
}

// Concatenation &: Concatenate this with op
Value Value::Concat(const Value &op) const
{
    switch (GetType())
    {
    case VSTRING:
        if (op.IsString())
        {
            return Value(GetString() + op.GetString());
        }
        else if (op.IsChar())
        {
            return Value(GetString() + op.GetChar());
        }
    case VCHAR:
        if (op.IsString())
        {
            return Value(GetChar() + op.GetString());
        }
        else if (op.IsChar())
        {
            return Value(GetChar() + op.GetChar());
        }
    default:
        cout << "Run-Time Error-Illegal Mixed Type Operands" << endl;
        return Value();
    }
}

// Exponentiation **: raise this to the power of op
Value Value::Exp(const Value &op) const
{
    if (IsReal() && op.IsReal())
    {
        if (op.GetReal() > 0)
        {
            return Value(pow(GetReal(), op.GetReal()));
        }
        else if (op.GetReal() == 0)
        {
            return Value(1);
        }
        else if (op.GetReal() < 0)
        {
            return Value(1 / (pow(GetReal(), abs(op.GetReal()))));
        }
    }
    else
    {
        cout << "Run-Time Error-Illegal Mixed Type Operands" << endl;
        return Value();
    }
    return Value();
}
