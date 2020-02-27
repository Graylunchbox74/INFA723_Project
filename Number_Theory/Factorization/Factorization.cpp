#include <vector>
#include <iostream>
#include "../Primality_Test/Primality_Test.hpp"
#include "../Utils/Utils.hpp"
#include "../../../db/db_utils.hpp"
#include <chrono>

namespace Factorization{
	std::vector<std::vector<InfInt> > Factor(InfInt n);

	InfInt Pollard(InfInt n);



	std::vector<std::vector<InfInt> > Factor(InfInt n){

		std::vector<std::vector<InfInt> > factors;
		std::vector<InfInt> standard;
		standard.push_back(2);
		standard.push_back(1);
		//check db if this number is prime or if it has already been factored
		
		
		while(n % 2 == 0)
		{
			if(factors.size() == 0)
			{
				standard[0] = 2;
				factors.push_back(std::vector<InfInt>(standard));
			}
			else
			{
				factors[0][1] += 1;
			}
			n/=2;
		}
		if(Primality::Is_Prime(n)){
			standard[0] = n;
			factors.push_back(standard);
			return factors;
		}

		InfInt current_factor;
		bool is_found = false;

		while(!Primality::Is_Prime(n))
		{
			current_factor = Pollard(n);
			is_found = false;
			for(int i = 0; i < factors.size(); i++)
			{
				if(factors[i][0] == current_factor)
				{
					factors[i][1] += 1;
					is_found = true;
					break;
				}
			}
			if(!is_found)
			{
				standard[0] = current_factor;
				factors.push_back(standard);
			}
			n/=current_factor;
		}
		is_found = false;
		for(int i = 0; i < factors.size(); i++)
		{
			if(factors[i][0] == n)
			{
				factors[i][1] += 1;
				is_found = true;
				break;
			}
		}
		if(!is_found)
		{
			standard[0] = n;
			factors.push_back(standard);
		}

		return factors;
	}

	InfInt Pollard(InfInt n)
	{
		InfInt a = 2;
		InfInt d;
		for(InfInt i = 1; i < n; i++)
		{
			a = power_mod(a,i,n); //equivalent of factorial since i is incrementing
			d = euclidean_gcd(a-1,n);
			if(d > 1 && d < n)
			{
				return d;
			}
		}
		return 1;
	}
};

int main()
{
	//int prime_test = 8675309;
	//optimized	
	auto start = std::chrono::high_resolution_clock::now();
	for(InfInt i = 20; i < 1000000; i++){Primality::Is_Prime(i);}
	//sievin
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop-start);
	std::cout << duration.count() << std::endl;
	start = std::chrono::high_resolution_clock::now();
	for(InfInt i = 20; i < 1000000; i++){for(InfInt x = 2; x < i.intSqrt(); x++){if(i%x==0)break;}}
	stop = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop-start);
	std::cout << duration.count() << std::endl;

//	InfInt num = "98787678957648";
//	std::vector<std::vector<InfInt> > a = Factorization::Factor(num);
//	std::cout << "Factors of: " << num << " digits: " << num.numberOfDigits() << std::endl;
//	for(int i = 0; i < a.size(); i++){
//	 	std::cout << a[i][0] <<" : " << a[i][1] << std::endl;
//	 }
}
