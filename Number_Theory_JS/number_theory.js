
const Utils = {
	factorial(n) {
		let fac = 1n;
		for (let i = BigInt(n); i > 1n; i--) {
			fac *= i;
		}
		return fac;
	},

	number_of_digits(n) {
		let digits = 0n;
		while (n > 0n) {
			digits++;
			n /= 10n;
		}
		return digits;
	},

	random_number(min, max) {
		min = min ?? 2n;
		max = max ?? 99999999999999999999n;

		let random = "";
		const dig = BigInt(Math.floor(Math.random() * 10000000000000));
		const min_dig = this.number_of_digits(min);
		const max_dig = this.number_of_digits(max);
		for (let i = 0n; i < min_dig + (dig % (max_dig - min_dig)); i++) {
			random += Math.floor(Math.random() * 10).toString();
		}
		let num = BigInt(random);
		num %= (max - min);
		num += min;
		return num;
	},

	power_mod(base, power, mod) {
		let a = base;
		let b = 1n;
		while (power > 0n) {
			if (power % 2n == 1n) {
				b = (b * a) % mod;
			}
			a = (a * a) % mod;
			power = power / 2n;
		}
		return b;
	},

	euclidean_gcd(first, second) {
		let divisor, dividend, remainder;
		if (first > second) {
			divisor = first;
			dividend = second;
		}
		else if (second > first) {
			divisor = second;
			dividend = first;
		}
		else {
			return first;
		}

		remainder = divisor % dividend;
		let previous_remainder = remainder;
		while (remainder != 0) {
			previous_remainder = remainder;
			divisor = dividend;
			dividend = remainder;
			remainder = divisor % dividend;
		}
		return previous_remainder;
	},

	extended_euclidean_inverse(value, mod) {
		let u = 1n;
		let g = value;
		let x = 0n;
		let y = mod;
		while (y != 0n) {
			let q = g / y;
			let t = g % y;
			let s = u - q * x;
			u = x;
			g = y;
			x = s;
			y = t;
		}
		if (u > 0n) {
			return u;
		} else {
			while (u < 0n) {
				u += mod / g;
			}
			return u;
		}
	}
}

const Factorization = {
	Factor(n) {
		let factors = [];
		let standard = [];
		standard.push(2n);
		standard.push(1n);
		//check db if this number is prime or if it has already been factored

		while (n % 2n == 0n) {
			if(factors.length == 0n) {
				standard[0] = 2n;
				factors.push(standard.slice());
			} else {
				factors[0][1] += 1n;
			}
			n /= 2n;
		}

		if(Primality.Is_Prime(n)) {
			standard[0] = n;
			factors.push(standard.slice());
			return factors;
		}

		let current_factor;
		let is_found = false;

		while (!Primality.Is_Prime(n)) {
			current_factor = this.Pollard(n);
			is_found = false;
			for (let i = 0; i < factors.length; i++) {
				if(factors[i][0] == current_factor) {
					factors[i][1] += 1n;
					is_found = true;
					break;
				}
			}
			if (!is_found) {
				standard[0] = current_factor;
				factors.push(standard.slice());
			}
			n /= current_factor;
		}

		is_found = false;

		for (let i = 0; i < factors.length; i++) {
			if(factors[i][0] == n) {
				factors[i][1] += 1n;
				is_found = true;
				break;
			}
		}

		if (!is_found)
		{
			standard[0] = n;
			factors.push(standard.slice());
		}

		return factors;
	},

	Pollard(n) {
		let a = 2n;
		let d;

		for (let i = 1n; i < n; i++) {
			a = Utils.power_mod(a, i, n); //equivalent of factorial since i is incrementing
			d = Utils.euclidean_gcd(a - 1n, n);
			if (d > 1n && d < n) {
				return d;
			}
		}
		return 1n;
	}
};

const Primality = {
	Generate_Random_Prime(min, max) {
		let tmp = Utils.random_number(min, max);
		while (this.Is_Prime(tmp) == false) {
			tmp = Utils.random_number(min, max);
		}
		return tmp;
	},

	Is_Prime(n) {
		if (n % 2n == 0n) return false;

		if (n < 100n) {
			for(i = 3n ; i < n; i += 2n) {
				if(n % i == 0n) {
					return false;
				}
			}
			return true;
		}

		return this.Fermat_Primality_Test(n) && this.Miller_Rabin_Primality_Test(n);
	},

	Fermat_Primality_Test(test_value) {
		return !(Utils.power_mod(2n, test_value - 1n, test_value) != 1n)
	},

	miller_rabin_witness(test_value, witness) {
		let q = test_value - 1n
		let k = 0n
		while (q % 2n == 0n)
		{
			k++;
			q = q / 2n;
		}

		witness = Utils.power_mod(witness, q, test_value);
		if (witness == 1n)
			return false; //not a witness

		for (let i = 0n; i < k; i++)
		{
			if (witness % test_value == test_value - 1n)
				return false; //not a witness

			witness = Utils.power_mod(witness, 2n, test_value);
		}
		return true; //is a witness and n is composite
	},

	Miller_Rabin_Primality_Test(test_value) {
		let tested_values = [];
		//this will be 99.999999% accurate
		for (let i = 0n; i < 14n; i++)
		{
			if(BigInt(tested_values.length) >= test_value - 3n)
				break;

			let tmp = Utils.random_number(2n, test_value - 1n);
			while (tested_values.includes(tmp)) {
				tmp = Utils.random_number(2n, test_value - 1n);
			}

			tested_values.push(tmp);
			if (this.miller_rabin_witness(test_value, tested_values[tested_values.length - 1])) {
				return false; //number is certainly composite
			}
		}
		return true; //number is probably primes
	},
};

const RSA_Encryption = {
	Generate_Private_Key(min, max) {
		let key = { Public_Key: {} };
		key.factor1 = Primality.Generate_Random_Prime(min, max);
		key.factor2 = Primality.Generate_Random_Prime(min, max);
		key.Public_Key.N = key.factor1 * key.factor2;

		const totient_n = (key.factor1 - 1n) * (key.factor2 - 1n);
		key.Public_Key.e = Utils.random_number(min, max) % totient_n;

		while (Utils.euclidean_gcd(key.Public_Key.e, totient_n) != 1n) {
			key.Public_Key.e = Utils.random_number(min, max) % totient_n;
		}
		key.d = Utils.extended_euclidean_inverse(key.Public_Key.e, totient_n);
		return key;
	},

	Encrypt_Number(value, key) {
		return Utils.power_mod(value, key.Public_Key.e, key.Public_Key.N);
	},

	Decrypt_Number(msg, key) {
		return Utils.power_mod(msg, key.d, key.Public_Key.N);
	}
};
