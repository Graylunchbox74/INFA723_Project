#ifndef UTILS
#define UTILS
#include "../Infint/InfInt.h"
InfInt factorial(InfInt n);
InfInt power_mod(InfInt base, InfInt power, InfInt mod);
InfInt euclidean_gcd(InfInt first_value, InfInt second_value);
InfInt extended_euclidean_inverse(InfInt value, InfInt mod);
InfInt random_number(InfInt min, InfInt max);

InfInt factorial(InfInt n)
{
	InfInt fac = 1;
	for(InfInt i = n; i > 1; i--)
	{
		fac *= i;
	}
	return fac;
}

InfInt random_number(InfInt MIN_RANDOM_NUMBER = 0, InfInt MAX_RANDOM_NUMBER = "99999999999999999999")
{
	std::string random = "";
	unsigned int dig = rand();
	for (unsigned int i = 0; i < MIN_RANDOM_NUMBER.numberOfDigits() + (dig % (MAX_RANDOM_NUMBER - MIN_RANDOM_NUMBER).numberOfDigits()); i++)
	{
		random.push_back((rand() % 10) + '0');
	}
	InfInt num(random);
	num %= (MAX_RANDOM_NUMBER - MIN_RANDOM_NUMBER);
	num += MIN_RANDOM_NUMBER;
	//num += MIN_RANDOM_NUMBER;
	return num;
}

InfInt power_mod(InfInt base, InfInt power, InfInt mod)
{
	InfInt a = base;
	InfInt b = 1;
	while (power > 0)
	{
		if (power % 2 == 1)
		{
			b = (b * a) % mod;
		}
		a = (a * a) % mod;
		power = power / 2;
	}
	return b;
}

InfInt euclidean_gcd(InfInt first_value, InfInt second_value)
{
	InfInt divisor, dividend, remainder, quotient;
	if (first_value > second_value)
	{
		divisor = first_value;
		dividend = second_value;
	}
	else if (second_value > first_value)
	{
		divisor = second_value;
		dividend = first_value;
	}
	else
	{
		return first_value;
	}
	quotient = divisor / dividend;
	remainder = divisor % dividend;
	InfInt previous_remainder = remainder;
	while (remainder != 0)
	{
		previous_remainder = remainder;
		divisor = dividend;
		dividend = remainder;
		quotient = divisor / dividend;
		remainder = divisor % dividend;
	}
	return previous_remainder;
}

InfInt extended_euclidean_inverse(InfInt value, InfInt mod)
	{
	    InfInt u = 1;
	    InfInt g = value;
	    InfInt x = 0;
	    InfInt y = mod;
		while (y != 0)
		{
	        InfInt q = g / y;
	        InfInt t = g % y;
	        InfInt s = u - q * x;
	        u = x;
	        g = y;
	        x = s;
	        y = t;
	    }
	    if (u > 0)
	     {
	     	return (InfInt)u;
	     }
	    else
	    {
	        while (u < 0)
	        {
	            u += mod/g;
	        }
	    	return (InfInt)u;
	    }
	}

#endif
