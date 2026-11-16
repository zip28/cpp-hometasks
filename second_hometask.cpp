#include <iostream>
#include <iomanip>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include <cctype>
#include <ostream>

class FleetError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class InspectionRejected : public FleetError {
public:
    using FleetError::FleetError;
};

class ValidationError : public FleetError {
public:
    using FleetError::FleetError;
};

class Thingy {
    std::string name_;
    std::string number_;
public:
    Thingy(std::string name, std::string number)
        : name_(std::move(name)), number_(std::move(number)) {}

    virtual std::string Name()   const { return name_; }
    virtual std::string Number() const { return number_; }
    
    // Делаем базис абстрактным
    virtual std::string Describe() const = 0; 
    
    virtual ~Thingy() = default;
};

std::ostream& operator<<(std::ostream& os, const Thingy& item) {
    os << item.Number() << ' ' << item.Name();
    return os;
}

bool operator<(const Thingy& lhs, const Thingy& rhs) {
    return lhs.Number() < rhs.Number();
}

class Transport : virtual public Thingy {
    int score_;
public:
    Transport(int score) : score_(score) {}
    virtual int Score() const { return score_; }
    ~Transport() override = default;
};

class Electro_thingy : virtual public Thingy {
protected:
    double energy_consumption_;
public:
    Electro_thingy(double energy) : energy_consumption_(energy) {}
    
    virtual double Consumption() const = 0; 
    
    ~Electro_thingy() override = default;
};

//all constructors (except for this one) are made with deepseek cuz they are similar
//same for destructors

class Bike : public Transport {
public:
    Bike(int score, std::string name, std::string num)
        : Thingy(std::move(name), std::move(num)),
          Transport(score) {}
    std::string Describe() const override { return Number() + " " + Name(); }
    ~Bike() override = default;
};

class Scooter : public Electro_thingy, public Transport {
public:
    Scooter(double e, int score, std::string n, std::string num)
        : Thingy(std::move(n), std::move(num)),
          Electro_thingy(e),
          Transport(score) {}
    double Consumption() const override { return energy_consumption_; }
    std::string Describe() const override { return Number() + " " + Name(); }
    ~Scooter() override = default;
};

class Ebike : public Electro_thingy, public Transport {
public:
    Ebike(double e, int score, std::string n, std::string num)
        : Thingy(std::move(n), std::move(num)),
          Electro_thingy(e),
          Transport(score) {}
    double Consumption() const override { return energy_consumption_; }
    std::string Describe() const override { return Number() + " " + Name(); }
    ~Ebike() override = default;
};

class Helmet : public Thingy {
public:
    Helmet(std::string n, std::string num)
        : Thingy(std::move(n), std::move(num)) {}
    std::string Describe() const override { return Number() + " " + Name(); }
    ~Helmet() override = default;
};

class Dock : public Electro_thingy {
public:
    Dock(double e, std::string n, std::string num)
        : Thingy(std::move(n), std::move(num)),
          Electro_thingy(e) {}
    double Consumption() const override { return energy_consumption_; }
    std::string Describe() const override { return Number() + " " + Name(); }
    ~Dock() override = default;
};

template <typename T>
bool Checker_function(const T& to_check) {
    std::string name = to_check.Name();
    // Приводим к нижнему регистру для поиска "broken" без учета регистра
    std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c){ return std::tolower(c); });
    return name.find("broken") == std::string::npos;
}

template <typename T>
class Tech_support {
    bool (*checker_)(const T&);
public:
    explicit Tech_support(bool (*checker)(const T&)) : checker_(checker) {}
    bool Check(const T& to_check) const { return checker_(to_check); }
};

class Park {
    std::unique_ptr<Tech_support<Thingy>>                 sup_;
    std::map<std::string, std::shared_ptr<Thingy>>        things_;
    std::map<std::string, std::weak_ptr<Electro_thingy>>  electro_things_;
    std::map<std::string, std::weak_ptr<Transport>>       transports_;
    std::weak_ptr<Thingy> focus_;

    template <typename Vehicle, typename... Args>
    std::shared_ptr<Vehicle> MakeChecked(Args&&... args) {
        auto up = std::make_unique<Vehicle>(std::forward<Args>(args)...);
        if (!sup_->Check(*up))
            throw InspectionRejected("inspection rejected");
        return std::shared_ptr<Vehicle>(std::move(up));
    }

public:
    Park() : sup_(std::make_unique<Tech_support<Thingy>>(&Checker_function<Thingy>)) {}

    //all the adds except for the first one are vibecoded
    //make a MakeChecked function instead of copying the Tech_support.Check... string to all the Adds
    //is the deepseek's idea

    void AddScooter(std::string id, std::string name, double energy, int score) {
        if (score < 1 || score > 10)
            throw ValidationError("score must be between 1 and 10");
        if (energy < 0)
            throw ValidationError("energy consumption must be positive");

        auto sp = MakeChecked<Scooter>(energy, score, std::move(name), id);
        electro_things_.emplace(id, sp);
        transports_.emplace(id, sp);
        things_.emplace(std::move(id), std::move(sp));
    }

    void AddBike(std::string id, std::string name, int score) {
        if (score < 1 || score > 10)
            throw ValidationError("score must be between 1 and 10");

        auto sp = MakeChecked<Bike>(score, std::move(name), id);
        transports_.emplace(id, sp);
        things_.emplace(std::move(id), std::move(sp));
    }

    void AddEbike(std::string id, std::string name, double energy, int score) {
        if (score < 1 || score > 10)
            throw ValidationError("score must be between 1 and 10");
        if (energy < 0)
            throw ValidationError("energy consumption must be positive");

        auto sp = MakeChecked<Ebike>(energy, score, std::move(name), id);
        electro_things_.emplace(id, sp);
        transports_.emplace(id, sp);
        things_.emplace(std::move(id), std::move(sp));
    }

    void AddHelmet(std::string id, std::string name) {
        auto sp = MakeChecked<Helmet>(std::move(name), id);
        things_.emplace(std::move(id), std::move(sp));
    }

    void AddDock(std::string id, std::string name, double energy) {
        if (energy < 0)
            throw ValidationError("energy consumption must be positive");

        auto sp = MakeChecked<Dock>(energy, std::move(name), id);
        electro_things_.emplace(id, sp);
        things_.emplace(std::move(id), std::move(sp));
    }

    void PrintEnergy() const {
        double ans = 0;
        for (const auto& [id, w] : electro_things_)
            if (auto e = w.lock())
                ans += e->Consumption();
        std::cout << std::fixed << std::setprecision(1) << ans << '\n';
    }

    void PrintNovice() const {
        for (const auto& [id, w] : transports_)
            if (auto t = w.lock())
                if (t->Score() >= 6)
                    std::cout << t->Name() << ' ' << t->Number() << '\n';
    }

    void PrintList() const {
        
	//the map is already sorted, but not to make the < useless, let's keep it
	std::vector<std::shared_ptr<Thingy>> sorted;
        for (const auto& [id, ptr] : things_)
            sorted.push_back(ptr);
        
        std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) {
            return *a < *b; 
        });
        
        for (const auto& ptr : sorted)
            std::cout << *ptr << '\n';
    }

    void Focus(const std::string& id) {
        auto it = things_.find(id);
        focus_ = (it != things_.end()) ? std::weak_ptr<Thingy>(it->second)
                                       : std::weak_ptr<Thingy>();
    }

    void PrintFocus() const {
        if (auto sp = focus_.lock())
            std::cout << sp->Number() << ' ' << sp->Name() << '\n';
        else
            std::cout << "removed\n";
    }

    void Remove(const std::string& id) {
        things_.erase(id);
        electro_things_.erase(id);
        transports_.erase(id);
    }

    void TransferTo(const std::string& id, Park& dest) {
        if (&dest == this) return;

        auto it = things_.find(id);
        if (it == things_.end())
            throw ValidationError("no such id: " + id);
        if (dest.things_.count(id))
            throw ValidationError("destination already has id: " + id);

        auto sp = it->second;

        if (auto f = focus_.lock(); f == sp)
            focus_.reset();
        things_.erase(it);
        electro_things_.erase(id);
        transports_.erase(id);

        if (auto e = std::dynamic_pointer_cast<Electro_thingy>(sp))
            dest.electro_things_.emplace(id, e);
        if (auto t = std::dynamic_pointer_cast<Transport>(sp))
            dest.transports_.emplace(id, t);
        dest.things_.emplace(id, std::move(sp));
    }
};

//Making this class was deepseek's idea, but I made it myself
class ParkManager {
    unsigned int active = 0;
    std::vector<std::unique_ptr<Park>> Parks;
public:
    ParkManager() {
        Parks.push_back(std::make_unique<Park>());
    }

    unsigned int AddPark() {
        Parks.push_back(std::make_unique<Park>());
        return Parks.size() - 1;
    }

    void SwitchPark(unsigned int num) {
        if (num >= Parks.size()) throw ValidationError("no such id");
        active = num;
    }

    void PrintCurrentIdx() const {
        std::cout << active << '\n';
    }

    Park& Current() { return *Parks[active]; }
    
    Park& GetPark(unsigned int idx) {
        if (idx >= Parks.size()) throw ValidationError("no such park");
        return *Parks[idx];
    }
    
    unsigned int Size() const { return Parks.size(); }
};

int main() {
    ParkManager mgr;
    std::string cmd;

    //the input is vibecoded

    while (std::cin >> cmd) {
        try {
            if (cmd == "add") {
                std::string type, id, name;
                std::cin >> type >> id >> name;
                Park& p = mgr.Current();
                if (type == "scooter") {
                    double energy; int score;
                    std::cin >> energy >> score;
                    p.AddScooter(id, name, energy, score);
                } else if (type == "bike") {
                    int score;
                    std::cin >> score;
                    p.AddBike(id, name, score);
                } else if (type == "ebike") {
                    double energy; int score;
                    std::cin >> energy >> score;
                    p.AddEbike(id, name, energy, score);
                } else if (type == "helmet") {
                    p.AddHelmet(id, name);
                } else if (type == "dock") {
                    double energy;
                    std::cin >> energy;
                    p.AddDock(id, name, energy);
                } else {
                    throw ValidationError("unknown type: " + type);
                }
            }
            else if (cmd == "energy")     mgr.Current().PrintEnergy();
            else if (cmd == "novice")     mgr.Current().PrintNovice();
            else if (cmd == "list")       mgr.Current().PrintList();
            else if (cmd == "show_focus") mgr.Current().PrintFocus();
            else if (cmd == "focus") {
                std::string id; std::cin >> id;
                mgr.Current().Focus(id);
            }
            else if (cmd == "remove") {
                std::string id; std::cin >> id;
                mgr.Current().Remove(id);
            }
            else if (cmd == "new_park") {
                std::cout << mgr.AddPark() << '\n';
            }
            else if (cmd == "switch") {
                unsigned int idx; std::cin >> idx;
                mgr.SwitchPark(idx);
            }
            else if (cmd == "current") {
                mgr.PrintCurrentIdx();
            }
            else if (cmd == "transfer") {
                std::string id; unsigned int dest;
                std::cin >> id >> dest;
                mgr.Current().TransferTo(id, mgr.GetPark(dest));
            }
            else if (cmd == "quit") break;
            else throw ValidationError("unknown command: " + cmd);
        }
        catch (const std::exception& e) {
            std::cerr << "error: " << e.what() << '\n';
        }
    }
    return 0;
}
