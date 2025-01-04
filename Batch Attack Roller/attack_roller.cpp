#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <time.h>
#include <fstream>

const static int ROWS = 30;
const static int COLUMNS = 7;
const static int SPACE_BETWEEN_COLUMNS = 0;

inline bool is_digits(const std::string& input)
{
	for (size_t i = 0; i < input.size(); ++i)
	{
		switch (input[i])
		{
		case '0': break;
		case '1': break;
		case '2': break;
		case '3': break;
		case '4': break;
		case '5': break;
		case '6': break;
		case '7': break;
		case '8': break;
		case '9': break;
		case '+': break;
		case '-': break;
		default: return false;
		}
	}
	return true;
}

inline void make_lowercase(std::string& str)
{
	for (size_t i = 0; i < str.length(); ++i)
		str[i] = std::tolower(str[i]);
}

inline void no_dmg(std::string& input, int atk)
{
	for (int row = 0; row < ROWS; ++row)
	{
		for (int column = 0; column < COLUMNS; ++column)
		{
			std::cout << "|   ";
			int roll1 = (rand() % 20) + 1 + atk;
			int roll2 = (rand() % 20) + 1 + atk;
			std::string segment;
			std::string roll1_text;
			std::string roll2_text;

			auto roll_to_text = [&](std::string& text, int roll)
				{
					if (roll == atk + 20)
					{
						text = "CRIT";
					}
					else if (roll == 1 + atk)
					{
						text = "nat1";
					}
					else
					{
						text = std::to_string(roll);
					}
				};

			roll_to_text(roll1_text, roll1);
			roll_to_text(roll2_text, roll2);

			if (roll1 == roll2)
			{
				segment = roll1_text;
			}
			else
			{
				segment = roll1_text + " / " + roll2_text;
			}

			int max_segment_width = 4 + 3 + 4;
			while (segment.size() < max_segment_width)
				segment += " ";

			std::cout << segment;
			for (int i = 0; i < SPACE_BETWEEN_COLUMNS; ++i)
				std::cout << " ";
		}
		std::cout << std::endl;
	}
}

struct dice
{
	int dice_count = 0;
	int sides = 0;
	int mod = 0;
	bool subtracted = false;
	std::string dmg_type = "";

	dice(int dice_count, int sides, int mod, bool subtracted, const std::string& dmg_type) : dice_count(dice_count), sides(sides), mod(mod), subtracted(subtracted), dmg_type(dmg_type) {}

	int roll()
	{
		int sum = mod;

		for (int i = 0; i < dice_count; ++i)
		{
			int this_roll = 1 + (rand() % sides);
			sum += this_roll;
		}

		return sum;
	}

	int roll_crit()
	{
		int sum = mod;

		for (int i = 0; i < dice_count; ++i)
		{
			sum += 1 + (rand() % sides);
			sum += 1 + (rand() % sides);
		}

		return sum;
	}

	int roll_dice_only()
	{
		int sum = 0;

		for (int i = 0; i < dice_count; ++i)
		{
			int this_roll = 1 + (rand() % sides);
			sum += this_roll;
		}

		return sum;
	}
};

inline bool dmg(std::string& input, int atk, bool prompt_ac, int& ac)
{
	std::vector<dice> dmg_dice;
	std::string temp;
	//std::cout << "Preprocessing: " << input << std::endl;
	temp.reserve(input.size());
	size_t start = 0;
	for (start = 0; start < input.size(); ++start)
	{
		char c = input[start];
		if (
			c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9'
			)
			break;
	}
	for (size_t i = start; i < input.size(); ++i)
	{
		if (input[i] != ' ')
			temp += input[i];
	}
	input = temp;
	//std::cout << "Processed: " << input << std::endl;
	int dice_count = 0;
	int sides = 0;
	int mod = 0;
	std::string dmg_type = "";
	bool subtracted = false;
	bool prevnum = false;
	auto push_dice = [&]() -> void
		{
			//std::cout << "Called push_dice, " << dice_count << "d" << sides << " + " << mod << std::endl;
			if (!(dice_count == 0 && sides == 0 && mod == 0))
			{
				//std::cout << "PUSHING DIE: " << dice_count << "d" << sides << " + " << mod << std::endl;
				dmg_dice.emplace_back(dice_count, sides, mod, subtracted, dmg_type);
				dice_count = 0;
				sides = 0;
				mod = 0;
				subtracted = false;
				dmg_type = "";
			}
		};

	int digits = 0;
	auto push_digit = [&](int digit) -> void
		{
			digits *= 10;
			digits += digit;
			prevnum = true;
			//std::cout << "NEW DIGIT: " << digit << std::endl;
		};
	for (size_t i = 0; i < input.size(); ++i)
	{
		char c = input[i];
		//std::cout << c << std::endl;
		switch (c)
		{
			case '+': {
				if (sides != 0 && dice_count != 0 && digits != 0)
				{
					mod = digits;
					digits = 0;
					push_dice();
				}
				else if (sides != 0 && dice_count != 0 && digits == 0)
				{

				}
				if (dice_count != 0)
				{
					sides = digits;
					digits = 0;
				}
				prevnum = false;
				break;
			}

			case '-': {
				if (dice_count != 0)
				{
					sides = digits;
					digits = 0;
				}
				subtracted = true;
				prevnum = false;
				break;
			}

			case 'd': {
				if (prevnum)
				{
					push_dice();
					dice_count = digits;
					digits = 0;
				}
				else
				{
					dmg_type += c;
				}
				prevnum = false;
				break;
			}
			case '0': {
				push_digit(0);
				break;
			}
			case '1': {
				push_digit(1);
				break;
			}
			case '2': {
				push_digit(2);
				break;
			}
			case '3': {
				push_digit(3);
				break;
			}
			case '4': {
				push_digit(4);
				break;
			}
			case '5': {
				push_digit(5);
				break;
			}
			case '6': {
				push_digit(6);
				break;
			}
			case '7': {
				push_digit(7);
				break;
			}
			case '8': {
				push_digit(8);
				break;
			}
			case '9': {
				push_digit(9);
				break;
			}

			default: {
				dmg_type += c;
				prevnum = false;
				break;
			}
		}
	}

	if (digits != 0)
	{
		if (sides == 0)
		{
			sides = digits;
		}
		else
		{
			mod = digits;
			if (subtracted)
			{
				mod = -mod;
			}
		}
	}

	push_dice();

	if (dmg_dice.size() == 0)
		return false;

	if (prompt_ac)
	{
		std::cout << "Enter Target AC? (Y/N):" << std::endl;
		std::string ac_input = "";
		bool first_inp = true;
		while (ac_input != "y" && ac_input != "Y" && ac_input != "n" && ac_input != "N")
		{
			if (!first_inp)
				std::cout << "Y or N only" << std::endl;
			std::getline(std::cin, ac_input);
			first_inp = false;
		}

		ac_input[0] = std::tolower(ac_input[0]);
		ac = -1;

		if (ac_input == "y")
		{
			while (ac == -1)
			{
				std::cout << "Enter AC: " << std::endl;
				std::getline(std::cin, ac_input);
				if (is_digits(ac_input))
				{
					ac = std::stoi(ac_input);
					if (ac < 0)
						ac = -1;
				}
			}
		}
	}

	std::ofstream file;
	file.open("rolls.txt");
	if (!file.is_open())
	{
		std::cout << "Error: Could not open 'rolls.txt'" << std::endl;
		return false;
	}

	auto print = [&](const std::string& line)
		{
			std::cout << line;
			file << line;
		};
	
	auto print_full_crit_dmg = [&]() -> void
		{
			for (size_t i = 0; i < dmg_dice.size(); ++i)
			{
				dice& die = dmg_dice[i];
				print(std::to_string(die.roll_crit()));
				if(dmg_dice[i].dmg_type.size()!=0)
					print(" " + dmg_dice[i].dmg_type);
				if (i != dmg_dice.size() - 1)
					print(" + ");
			}
		};

	auto print_normal_dmg = [&]() -> void
		{
			for (size_t i = 0; i < dmg_dice.size(); ++i)
			{
				dice& die = dmg_dice[i];
				int roll = die.roll();
				print(std::to_string(roll));

				if (dmg_dice[i].dmg_type.size() != 0)
					print(" " + dmg_dice[i].dmg_type);
				if (i != dmg_dice.size() - 1)
					print(" + ");
			}
		};

	for (int attack_count = 0; attack_count < 40; ++attack_count)
	{
		int atk_roll1 = 1 + (rand() % 20) + atk;
		int atk_roll2 = 1 + (rand() % 20) + atk;
		
		bool crit1 = atk_roll1 == 20 + atk;
		bool crit2 = atk_roll2 == 20 + atk;

		bool atk1_nat1 = atk_roll1 == 1 + atk;
		bool atk2_nat1 = atk_roll2 == 1 + atk;

		auto get_string = [&](int atk_roll) -> std::string
			{
				if (atk_roll == 20 + atk)
					return "CRIT";
				else if (atk_roll == 1 + atk)
					return "nat1";
				else
					return std::to_string(atk_roll);
			};
		if (attack_count != 0)
			print("\n");
		if (atk_roll1 == atk_roll2)
		{
			if (crit1)
			{
				if(ac==-1)
					print("CRIT -> ");
				print_full_crit_dmg();
				if (ac != -1)
					print("*");
				//print("\n");
			}
			else
			{
				if (ac == -1)
				{
					if (atk1_nat1)
					{
						print("nat1");
					}
					else
					{
						print(std::to_string(atk_roll1));
						print(" -> ");
						print_normal_dmg();
					}
				}
				else
				{
					if (atk_roll1 >= ac && !atk1_nat1)
					{
						print_normal_dmg();
					}
					else
					{
						print("-");
					}
				}	
			}
		}
		else
		{
			if (!crit1 && !crit2)
			{
				if (ac == -1)
				{
					print(std::to_string(atk_roll1));
					print(" / ");
					print(std::to_string(atk_roll2));
					print(" -> ");
					print_normal_dmg();
				}
				else
				{
					bool hit1 = !((atk_roll1 < ac) || atk1_nat1);
					bool hit2 = !((atk_roll2 < ac) || atk2_nat1);
					if (!hit1 && !hit2)
					{
						print("-");
					}
					else
					{
						
						if (hit1 && !hit2)
						{
							print_normal_dmg();
							print(" / -");
						}
						else if (!hit1 && hit2)
						{
							print("- / ");
							print_normal_dmg();
						}
						else if (hit1 && hit2)
						{
							print_normal_dmg();
						}
					}
					
				}
				//print("\n");
			}
			else if (crit1 && !crit2)
			{

				if (ac == -1)
				{
					print("CRIT / ");
					if (atk2_nat1)
						print("nat1");
					else
						print(std::to_string(atk_roll2));

					print(" -> ");

					if (atk2_nat1)
					{
						print_full_crit_dmg();
						//print("\n");
					}
					else
					{
						print("[");
						print_full_crit_dmg();
						print("] / [");
						print_normal_dmg();
						print("]");
					}
				}
				else
				{
					print_full_crit_dmg();
					print("* / ");
					if (atk_roll2 >= ac && !atk2_nat1)
					{
						print_normal_dmg();
					}
					else
					{
						print("-");
					}
				}
			}
			else if (!crit1 && crit2)
			{
				if (ac == -1)
				{
					if (atk1_nat1)
						print("nat1");
					else
						print(std::to_string(atk_roll1));

					print(" / CRIT -> ");

					if (atk1_nat1)
					{
						print_full_crit_dmg();
						//print("\n");
					}
					else
					{
						print("[");
						print_normal_dmg();
						print("] / [");
						print_full_crit_dmg();
						print("]");
					}
				}
				else
				{
					if (atk_roll1 >= ac && !atk1_nat1)
					{
						print_normal_dmg();
						print(" / ");
					}
					else
					{
						print("- / ");
					}
					print_full_crit_dmg();
					print("*");
				}
			}
		}
	}
	file.close();
	std::cout << std::endl;
	return true;
}

inline void roll_attacks()
{
	std::cout << "Attack Bonus: ";
	int atk = 0;
	std::string input = "k";
	while (!is_digits(input))
	{
		std::getline(std::cin, input);
	}
	atk = std::stoi(input);
	bool initial_prompt = true;
	std::cout << "Enter Damage? Y/N:\n";
	while (input != "y" && input != "n")
	{
		if (!initial_prompt)
		{
			std::cout << "Invalid input. Try again." << std::endl << "Enter Damage? Y/N:" << std::endl;
		}
		std::getline(std::cin, input);
		if (input.length() == 1)
		{
			input[0] = std::tolower(input[0]);
		}
		initial_prompt = false;
	}

	if (input == "n" || input == "N")
	{
		no_dmg(input, atk);
	}
	else
	{
		int ac = -1;
		bool valid_input = false;
		std::cout << "Enter damage formula:" << std::endl;
		while (!valid_input)
		{
			std::getline(std::cin, input);
			//std::cout << "Got raw input '" << input << "'" << std::endl;
			valid_input = dmg(input, atk, true, ac);
			if (valid_input)
			{
				std::string formula = input;
				std::cout << "Go again?\n";
				while (input != "y" && input != "Y" && input != "n" && input != "N")
					std::getline(std::cin, input);
				while (input == "y" || input == "Y")
				{
					dmg(formula, atk, false, ac);
					std::cout << "Go again?\n";
					input = "";
					while(input != "y" && input != "Y" && input != "n" && input != "N")
						std::getline(std::cin, input);
				}
				return;
			}
		}
	}
	
	input = "a";
	while (input != "exit" && input != "quit" && input != "close")
	{
		std::getline(std::cin, input);
		make_lowercase(input);
	}
}

int main(int argc, char** args)
{
	srand(time(NULL));

	roll_attacks();
}