#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

enum FoodItems {Sandwich, Poha, Vada, Burger};
enum BeverageItems {Tea, Coffee, Water};

class Store
{
public:
	Store(vector<pair<int,int>>&,vector<pair<int,int>>&);
	void purchaseFood(FoodItems, int);
	void purchaseBeverage(BeverageItems, int);
	void setBeverageRates(const unordered_map<int, int> &beverageRates);
	void setFoodRates(const unordered_map<int, int> &foodRates);
	const unordered_map<int, int>& getFoodUnitsSold() const;
	const string& getId() const;

private:
	unordered_map<int, int> foodSupply;
	unordered_map<int, int> beverageSupply;
	unordered_map<int, int> foodUnitsSold;
	unordered_map<int, int> beverageUnitsSold;
	unordered_map<int, int> foodRates;
	unordered_map<int, int> beverageRates;
	string id;
	string getUniqueId();
};

class City
{
public:
	City(vector<pair<int,int>>&,vector<pair<int,int>>&);
	void addStore(Store &);
	void purchaseFood(string, FoodItems, int);
	void purchaseBeverage(string, BeverageItems, int);
	const string& getId() const;
	const vector<Store>& getStores() const;

private:
	string id;
	string getUniqueId();
	unordered_map<int, int> foodPrices;
	unordered_map<int, int> beveragePrices;
	vector<Store> stores;
};

class State
{
public:
	State();
	void addCity(City&);
	const vector<City>& getCities() const;
	const string& getId() const;
	void purchaseFood(string,string, FoodItems, int);
	void purchaseBeverage(string,string, BeverageItems, int);
private:
	string id;
	string getUniqueId();
	vector<City> cities;
};

class System
{
public:
	System() { }
	void addState(State&);
	const vector<State>& getStates() const;
	void purchaseFood(string,string,string, FoodItems, int);
	void purchaseBeverage(string,string,string, BeverageItems, int);

private:
	vector<State> states;
};

Store::Store(vector<pair<int,int>>& foods, vector<pair<int,int>>& beverages)
{
	this->id = getUniqueId();
	for(auto &food: foods)
	{
		foodSupply[food.first] = food.second;
	}
	for(auto &beverage: beverages)
	{
		beverageSupply[beverage.first] = beverage.second;
	}
}

string Store::getUniqueId()
{
	static int storeId = 1;
	return "store" + to_string(storeId++);
}

void Store::purchaseFood(FoodItems foodItem, int qty)
{
	if(foodSupply[foodItem] < qty)
	{
		cout << "Not enough stocks\n";
		return;
	}

    cout << "Purchasing ... " << endl;
    cout << "[Before Purchase] foodUnitsSold[foodItem]: " << foodUnitsSold[foodItem] << endl;

	foodSupply[foodItem] -= qty;
	foodUnitsSold[foodItem] += qty;

    cout << "[After Purchase] foodUnitsSold[foodItem]: " << foodUnitsSold[foodItem] << endl;
}
void Store::purchaseBeverage(BeverageItems beverageItem, int qty)
{
	if(beverageSupply[beverageItem] < qty)
	{
		cout << "Not enough stocks\n";
		return;
	}

	beverageSupply[beverageItem] -= qty;
	beverageUnitsSold[beverageItem] += qty;
}

void Store::setBeverageRates(const unordered_map<int, int> &beverageRates) {
	this->beverageRates = beverageRates;
}

void Store::setFoodRates(const unordered_map<int, int> &foodRates) {
	this->foodRates = foodRates;
}

const unordered_map<int, int>& Store::getFoodUnitsSold() const {
	return foodUnitsSold;
}

const string& Store::getId() const {
	return id;
}
City::City(vector<pair<int,int>>& foodPrices,vector<pair<int,int>>& beveragePrices)
{
	this->id = getUniqueId();
	for(auto &food: foodPrices)
	{
		this->foodPrices[food.first] = food.second;
	}
	for(auto &beverage: beveragePrices)
	{
		this->beveragePrices[beverage.first] = beverage.second;
	}
}

string City::getUniqueId()
{
	static int cityId = 1;
	return "city" + to_string(cityId++);
}

void City::addStore(Store& store)
{
	store.setFoodRates(foodPrices);
	store.setBeverageRates(beveragePrices);
	stores.push_back(store);
}

void City::purchaseFood(string storeId, FoodItems foodItem, int qty)
{
	for(Store& store: stores)
	{
		if(store.getId() == storeId)
		{
			store.purchaseFood(foodItem, qty);
			break;
		}
	}
}
void City::purchaseBeverage(string storeId, BeverageItems beverageItem, int qty)
{
	for(Store& store: stores)
	{
		if(store.getId() == storeId)
		{
			store.purchaseBeverage(beverageItem, qty);
			break;
		}
	}
}

const string& City::getId() const {
	return id;
}

const vector<Store>& City::getStores() const {
	return stores;
}

State::State()
{
	this->id = getUniqueId();
}

string State::getUniqueId()
{
	static int stateId = 1;
	return "state" + to_string(stateId++);
}

void State::addCity(City& city)
{
	cities.push_back(city);
}

void State::purchaseFood(string cityId, string storeId, FoodItems foodItem, int qty)
{
	for(City& city: cities)
	{
		if(city.getId() == cityId)
		{
			city.purchaseFood(storeId, foodItem, qty);
			break;
		}
	}
}
void State::purchaseBeverage(string cityId, string storeId, BeverageItems beverageItem, int qty)
{
	for(City& city: cities)
	{
		if(city.getId() == cityId)
		{
			city.purchaseBeverage(storeId, beverageItem, qty);
			break;
		}
	}
}

const vector<City>& State::getCities() const {
	return cities;
}

const string& State::getId() const {
	return id;
}

void System::addState(State& state)
{
	states.push_back(state);
}

void System::purchaseFood(string stateId, string cityId, string storeId, FoodItems foodItem, int qty)
{
	for(State& state: states)
	{
		if(state.getId() == stateId)
		{
			state.purchaseFood(cityId, storeId, foodItem, qty);
			break;
		}
	}
}
void System::purchaseBeverage(string stateId, string cityId, string storeId, BeverageItems beverageItem, int qty)
{
	for(State& state: states)
	{
		if(state.getId() == stateId)
		{
			state.purchaseBeverage(cityId, storeId, beverageItem, qty);
			break;
		}
	}
}

const vector<State>& System::getStates() const {
	return states;
}

int main() {
    vector<pair<int, int>> foodSupply;
    vector<pair<int, int>> beverageSupply;

    for (int i = 0; i < FoodItems::Burger; ++i)
    {
        foodSupply.push_back({ i, i + 1 });
        beverageSupply.push_back({ i, i + 1 });
    }

    foodSupply.push_back({ 3, 4 });

    Store store(foodSupply, beverageSupply);
    City city(foodSupply, beverageSupply);
    city.addStore(store);

    State state;
    state.addCity(city);

    System system;
    system.addState(state);

    string state1 = "state1";
    string city1 = "city1";
    string store1 = "store1";

    system.purchaseFood(state1, city1, store1, FoodItems::Burger, 2);

    for (State state : system.getStates())
    {
        if (state.getId() == state1)
        {
            for (City city : state.getCities())
            {
                if (city.getId() == city1)
                {
                    for (Store store : city.getStores())
                    {
                        if (store.getId() == store1)
                        {
                            for (auto p : store.getFoodUnitsSold())
                            {
                                cout << p.first << " " << p.second << endl;
                            }
                            break;
                        }
                    }
                    break;
                }
            }
            break;
        }
    }

	system.purchaseFood(state1, city1, store1, FoodItems::Burger, 3);
	return 0;
}
