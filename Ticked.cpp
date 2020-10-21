#include <iostream>
#include <ctime>
#include <iomanip>
#include <string>

 /*ДОМАШНЕЕ ЗАДАНИЕ:
		Задание 3: Система покупки билетов
		+ Свойства билета: (номер, направление, дата, цена)
		+ Реализуйте возможность покупать билеты
		+ По мере покупки билетов они должны пропадать из списка
		+ Реализуйте сортировку билетов по цене
		+ При решении задачи используйте классы и объекты

		В рамках данной задачи считается, что у нас на счету всегда достаточно
		средств для покупки любого билета*/


class Cashbox;


class Ticket
{
public:
	Ticket() = default;

	void creatTicked(const std::string& route = "Travel")
	{
		m_numTicket = ++m_num;
		m_route = route;
		setAutoData();
		setAutoPrice();
		setAutoTime();
	}

	~Ticket()
	{
		--m_num;
	}

	Ticket(const std::string& route) :m_numTicket(++m_num), m_route(route) {
		setAutoData();
		setAutoPrice();
		setAutoTime();
	}

	void setAutoTime()//randomnoe ustanovlenie vremeni
	{
		int i = rand() % 10;

		if (i == 0) { m_hours = "06:00"; }
		else if (i == 1) { m_hours = "08:30"; }
		else if (i == 2) { m_hours = "12:45"; }
		else if (i == 3) { m_hours = "14:15"; }
		else if (i == 4) { m_hours = "16:55"; }
		else if (i == 5) { m_hours = "18:45"; }
		else if (i == 6) { m_hours = "21:05"; }
		else if (i == 7) { m_hours = "22:20"; }
		else if (i == 8) { m_hours = "00:35"; }
		else if (i == 9) { m_hours = "03:00"; }
	}

	void setAutoData()//ustanovki dati s sled mesyaca
	{
		m_month = (rand() % (12 + 1 - 7)) + 7;
		m_year = (rand() % (2021 + 1 - 2020)) + 2020;
		m_day = rand() % 31;
	}

	void setAutoPrice() {//letniy sezon bileti doroje na 1,5(may,iyun,iyul,avgust,sentyabr)
		((m_month < 10 && m_month >4))
			? (m_price = (rand() % (650 + 1 - 500)) + 500)
			: (m_price = (rand() % (400 + 1 - 300)) + 300);
	}
private:

	friend class Cashbox;
	friend std::ostream& operator<<(std::ostream& os, Cashbox n);
	std::string m_route;
	int m_price = 0;
	int m_numTicket = 0;
	static int m_num;
	int m_year = 0;
	int m_month = 0;
	int m_day = 0;
	std::string m_hours;
};

int Ticket::m_num = 0;

class Down {
public:
	bool operator() (const int left, const int right) {
		return left < right;
	}
};

class Up {
public:
	bool operator() (const int left, const int right) {
		return left > right;
	}
};

class Cashbox
{
public:
	Cashbox() = default;

	~Cashbox()
	{
		delete[] m_tickets;
	}
	void addTicket(const std::string& value)
	{
		if (m_index == m_quantityTickked)capacityUp();

		m_tickets[m_index++].creatTicked(value);
	}

	Cashbox(const Cashbox& other)
	{
		m_capacity = other.m_capacity;
		m_index = other.m_index;
		m_quantityTickked = other.m_quantityTickked;
		m_tickets = new Ticket[m_quantityTickked];

		for (int i = 0; i < m_index; i++)
			m_tickets[i] = other.m_tickets[i];

	}

	Cashbox(Cashbox&& other)
	{
		m_capacity = other.m_capacity;
		m_index = other.m_index;
		m_quantityTickked = other.m_quantityTickked;
		m_tickets = other.m_tickets;
		other.m_tickets = nullptr;
	}

	Cashbox& operator=(const Cashbox& other)
	{
		m_capacity = other.m_capacity;
		m_index = other.m_index;
		m_quantityTickked = other.m_quantityTickked;
		m_tickets = new Ticket[m_quantityTickked];

		for (int i = 0; i < m_index; i++)
			m_tickets[i] = other.m_tickets[i];

		return *this;
	}

	Cashbox& operator=(Cashbox&& other)
	{
		m_capacity = other.m_capacity;
		m_index = other.m_index;
		m_quantityTickked = other.m_quantityTickked;
		m_tickets = other.m_tickets;
		other.m_tickets = nullptr;

		return *this;
	}

	void SellTicked(const int number)
	{
		if (m_index)
		{
			Ticket* tmp = new Ticket[m_quantityTickked];
			for (int i = 0, b = 0; i < m_quantityTickked; i++)
			{
				if (m_tickets[i].m_numTicket != number) {
					tmp[b] = m_tickets[i];
					b++;
				}
			}
			delete[] m_tickets;
			m_tickets = tmp;
			m_index--;
		}
	}

	int getIndex()const
	{
		return m_index;
	}

	template<typename T>
	void operator()(T sort) {
		for (int i = 0; i < m_index; i++) {
			for (int j = i + 1; j < m_index; j++) {
				if (sort(m_tickets[i].m_price, m_tickets[j].m_price)) {
					std::swap(m_tickets[i], m_tickets[j]);
				}
			}
		}
	}

private:
	void capacityUp()
	{
		m_quantityTickked += m_capacity;

		Ticket* tmp = new Ticket[m_quantityTickked];
		for (int i = 0; i < m_index; i++)
		{
			tmp[i] = m_tickets[i];
		}

		delete[] m_tickets;
		m_tickets = tmp;
	}

	friend std::ostream& operator<<(std::ostream& os, Cashbox n);
	int m_quantityTickked = 20;
	int m_capacity = 5;
	int m_index = 0;
	Ticket* m_tickets = new Ticket[m_quantityTickked];

};

std::ostream& operator<<(std::ostream& os, Cashbox n)
{
	if (n.m_index)
	{
		for (int i = 0; i < n.m_index; i++)
		{
			os << "  _______________________________________\n |                                  # "
				<< std::right << std::setw(2) << std::setfill('0') << n.m_tickets[i].m_numTicket << " |\n"
				<< " |  Go to   : " << std::right << std::setw(15) << std::setfill(' ') << n.m_tickets[i].m_route << "            |\n"
				<< " |  Price   : " << std::right << std::setw(13) << std::setfill(' ') << n.m_tickets[i].m_price << " $            |\n"
				<< " |       " << "                                |\n" << " |  Data    :      " << n.m_tickets[i].m_year << "-"
				<< std::right << std::setw(2) << std::setfill('0') << n.m_tickets[i].m_month << "-"
				<< std::right << std::setw(2) << std::setfill('0') << n.m_tickets[i].m_day << "            |\n"
				<< " |  Time    : " << std::right << std::setw(15) << std::setfill(' ') << n.m_tickets[i].m_hours
				<< "            |\n |_______________________________________|";
			std::cout << "\n\n==============================================================\n\n";
		}
	}
	else
	{
		std::cout << "All tickets sold\n";
	}
	return os;

}

enum { EXIT, BUY, PRINT, SORT_COST, ONE = 1, TWO };

int main()
{
	Cashbox cashbox;
	cashbox.addTicket("Baku");
	cashbox.addTicket("Istanbul");
	cashbox.addTicket("Moscow");
	cashbox.addTicket("Istanbul");
	cashbox.addTicket("Paris");
	cashbox.addTicket("London");
	cashbox.addTicket("Kiev");
	cashbox.addTicket("New-York");
	cashbox.addTicket("Munich");
	cashbox.addTicket("Washington");
	cashbox.addTicket("Istanbul");
	cashbox.addTicket("Kiev");
	cashbox.addTicket("Moscow");
	cashbox.addTicket("Istanbul");
	cashbox.addTicket("Paris");
	cashbox.addTicket("London");
	cashbox.addTicket("New-York");
	cashbox.addTicket("Munich");
	Up up;
	Down down;
	int act;

	while (true)
	{
		std::cout << R"(
		
0 - Exit
1 - Buy ticket
2 - Print all Ticket
3 - Sort by price

)";
		std::cin >> act;
		system("cls");
		if (act == EXIT) {
			std::cout << "You left the Cashbox\n";
			break;
		}
		else if ((act == BUY)) {
			if (cashbox.getIndex())
			{
				std::cout << cashbox;
				std::cout << "What flight are you planning to buy ? \n";
				std::cin >> act;
				cashbox.SellTicked(act);
			}
			else
			{
				std::cout << "There are no tickets at the box office\n";
			}
		}
		else if (act == PRINT) {
			std::cout << cashbox;
		}
		else if (act == SORT_COST)
		{
			if (cashbox.getIndex() > ONE)
			{
				do
				{
					std::cout << "1-Sorting  (1->9)\n2-Sorting  (9->1)\n";
					std::cin >> act;
					if (act == ONE)
					{
						cashbox(up);
						break;
					}
					else if (act == TWO)
					{
						cashbox(down);
						break;
					}
					else
					{
						std::cout << "Wrong choice , try again \n";
					}

				} while (true);
			}
			else
			{
				std::cout << "No tickets to sort\n";
			}
		}
		else {
			std::cout << "Wrong choice , try again \n";
		}
	}

}

