#include "../Utils/Utils.hpp"

namespace Primality{
	bool Is_Prime(InfInt n);
	bool Fermat_Primality_Test(InfInt n);
	bool Miller_Rabin_Primality_Test(InfInt n);
	static bool miller_rabin_witness(InfInt test_value, InfInt witness);


	InfInt Generate_Random_Prime(InfInt min, InfInt max)
	{
		InfInt tmp = random_number(min, max);
		while(Is_Prime(tmp) == false){
			tmp = random_number(min,max);
		}
		return tmp;
	}

	bool Is_Prime(InfInt n)
	{
		if(n%2 == 0)
			return false;

		if(n < 100)
		{
			for(InfInt i = 3; i < n; i+=2)
			{
				if(n%i == 0)
				{
					return false;
			
				}
			}
			return true;
		}

		return Fermat_Primality_Test(n) && Miller_Rabin_Primality_Test(n);
	}


	bool Fermat_Primality_Test(InfInt test_value)
	{
		if (power_mod(2, test_value - 1, test_value) != 1)
			return false;
		return true;
	}

	static bool miller_rabin_witness(InfInt test_value, InfInt witness)
	{
		InfInt q = test_value - 1;
		InfInt k = 0;
		while (q % 2 == 0)
		{
			k++;
			q = q / 2;
		}
		witness = power_mod(witness, q, test_value);
		if (witness == 1)
		{
			return false; //not a witness
		}
		for(InfInt i = 0; i < k; i++)
		{
			if (witness % test_value == test_value - 1)
				return false; //not a witness

			witness = power_mod(witness, 2, test_value);
		}
		return true; //is a witness and n is composite
	}

	bool Miller_Rabin_Primality_Test(InfInt test_value)
	{
		std::vector<InfInt> tested_values;
		//this will be 99.999999% accurate
		for (InfInt i = 0; i < 14; i++)
		{
			if((InfInt)tested_values.size() >= test_value - 3)
				break;

			InfInt tmp = random_number(2,test_value-1);
			while (std::find(tested_values.begin(), tested_values.end(), tmp) != tested_values.end())
			{
				tmp = random_number(2,test_value-1);
			}
			tested_values.push_back(tmp);
			if (miller_rabin_witness(test_value, tested_values[tested_values.size() - 1]) == true)
			{
				return false; //number is certainly composite
			}
		}
		return true; //number is probably primes
	}
};
