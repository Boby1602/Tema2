#include <string>
#include<iostream>
#include<ios>
#include<limits>
#include <vector>
#include <map>
#include <memory>
#include <iostream>

enum class Tip_Device {
    Telefon,
    Tableta
};

class IOStream {
protected:
    virtual void Print(std::ostream &) const = 0;

    friend std::ostream &operator<<(std::ostream &, const IOStream &);

    friend std::ostream &operator<<(std::ostream &, const IOStream *);

    virtual ~IOStream();
};

std::ostream &operator<<(std::ostream &os, const IOStream &ob) {
    ob.Print(os);
    return os;
}

std::ostream &operator<<(std::ostream &os, const IOStream *ob) {
    ob->Print(os);
    return os;
}

IOStream::~IOStream() = default;

class Device : public IOStream {
protected:
    std::string Marca;
    std::string Model;
    int An_fabricatie;
public:
    Device(std::string marca, std::string model, int an);

    Device(const Device &rhs);

    Device &operator=(const Device &rhs);

    const std::string &GetMarca() const;

    void SetMarca(std::string marca);

    void SetModel(std::string model);

    int GetAn_fabricatie() const;

    void SetAn_fabricatie(int an);

    virtual Tip_Device GetTip_Device() const = 0;

    virtual ~Device();
};

Device::Device(std::string marca, std::string model, int an) : Marca(std::move(marca)), Model(std::move(model)),
                                                                   An_fabricatie(an) {
}

Device::Device(const Device &rhs) : Device(rhs.Marca, rhs.Model, rhs.An_fabricatie) {
}

Device &Device::operator=(const Device &rhs) {
    if (this != &rhs) {
        Marca = rhs.Marca;
        Model = rhs.Model;
        An_fabricatie = rhs.An_fabricatie;
    }
    return *this;
}

const std::string &Device::GetMarca() const {
    return Marca;
}

void Device::SetMarca(std::string marca) {
    Device::Marca = std::move(marca);
}

void Device::SetModel(std::string model) {
    Device::Model = std::move(model);
}

int Device::GetAn_fabricatie() const {
    return An_fabricatie;
}

void Device::SetAn_fabricatie(int an) {
    Device::An_fabricatie = an;
}

Device::~Device() = default;

class Telefon : public Device {
    std::string SIM;
public:
    Telefon(std::string marca, std::string model, int an, std::string sim_Telefon);

    Telefon(const Telefon &rhs);

    Telefon &operator=(Telefon rhs);

    const std::string &GetSIM() const;

    void SetSIM(std::string sim_Telefon);

    void Print(std::ostream &) const override;

    Tip_Device GetTip_Device() const override { return Tip_Device::Telefon; }
};

Telefon::Telefon(std::string marca, std::string model, int an, std::string sim_Telefon)
        : Device(std::move(marca), std::move(model), an), SIM(std::move(sim_Telefon)) {
}

Telefon::Telefon(const Telefon &rhs) : Device(rhs.Marca, rhs.Model, rhs.An_fabricatie),
                                       SIM(rhs.SIM) {
}

Telefon &Telefon::operator=(Telefon rhs) {
    Device::operator=(rhs);
    std::swap(SIM, rhs.SIM);

    return *this;
}

const std::string &Telefon::GetSIM() const {
    return SIM;
}

void Telefon::SetSIM(std::string sim_Telefon) {
    Telefon::SIM = std::move(sim_Telefon);
}

void Telefon::Print(std::ostream &os) const {
    os << "Telefon(" << Marca << " " << Model << ", " << An_fabricatie << ", " << SIM << ")" << "\n";
}



class Tableta : public Device {
    std::string Culoare;
public:
    Tableta(std::string marca, std::string model, int an, std::string culoare);

    const std::string &GetCuloare() const;

    void SetCuloare(std::string culoare);

    void Print(std::ostream &os) const override;

    Tip_Device GetTip_Device() const override { return Tip_Device::Tableta; }
};

Tableta::Tableta(std::string marca, std::string model, int an, std::string culoare)
        : Device(std::move(marca), std::move(model), an), Culoare(std::move(culoare)) {
}

const std::string &Tableta::GetCuloare() const {
    return Culoare;
}

void Tableta::SetCuloare(std::string culoare) {
    Tableta::Culoare = std::move(Culoare);
}

void Tableta::Print(std::ostream &os) const {
    os << "Tableta(" << Marca << " " << Model << ", " << An_fabricatie << ", " << Culoare << ")" << "\n";
}

class Nu_exista : public std::exception, public IOStream {
public:
    Nu_exista() = default;

    const char *what() const noexcept override;

    void Print(std::ostream &) const;
};

const char *Nu_exista::what() const noexcept {
    return "\nAcest device nu exista.";
}

void Nu_exista::Print(std::ostream &os) const {
    os << this->what();
}

class MagElectronice {
    static std::vector<std::shared_ptr<Device>> Deviceuri;
public:
    MagElectronice() = delete;

    static void Adauga_device(std::shared_ptr<Device> device);

    static std::vector<std::shared_ptr<Device>> &GetDeviceuri();

    static std::shared_ptr<Device> Cauta_device(const std::string& marca);

    static std::vector<std::shared_ptr<Tableta>> Cauta_toate_tabletele();

    static std::vector<std::shared_ptr<Telefon>> Cauta_toate_telefoanele();
};

std::vector<std::shared_ptr<Device>> MagElectronice::Deviceuri;

void MagElectronice::Adauga_device(std::shared_ptr<Device> device) {
    Deviceuri.push_back(device);
}

std::vector<std::shared_ptr<Device>> &MagElectronice::GetDeviceuri() {
    return MagElectronice::Deviceuri;
}

std::shared_ptr<Device> MagElectronice::Cauta_device(const std::string &marca) {
    int x=0;
    for (auto &device: MagElectronice::Deviceuri)
        if (device->GetMarca() == marca){
            x++;
            if(x!=1)
                std::cout<<* device;
        }
    for (auto &device: MagElectronice::Deviceuri) {
        if (device->GetMarca() == marca)
            return device;
    }
    throw Nu_exista();
}

std::vector<std::shared_ptr<Tableta>> MagElectronice::Cauta_toate_tabletele() {
    std::vector<std::shared_ptr<Tableta>> tablete;

    for (auto &device: MagElectronice::Deviceuri) {
        if (typeid(*device) == typeid(Tableta)) {
            auto tableta = std::dynamic_pointer_cast<Tableta>(device);
            tablete.push_back(tableta);
        }
    }

    return tablete;
}

std::vector<std::shared_ptr<Telefon>> MagElectronice::Cauta_toate_telefoanele() {
    std::vector<std::shared_ptr<Telefon>> telefoane;

    for (auto &device: MagElectronice::Deviceuri) {
        if (typeid(*device) == typeid(Telefon)) {
            auto telefon = std::dynamic_pointer_cast<Telefon>(device);
            telefoane.push_back(telefon);
        }
    }

    return telefoane;
}


int main() {



    int alegere,n,an,alegere2;
    std::string marca_device,model_device,culoare,SIM;
    do {
        std::cout<< "---------------------- \n Alegeti actiunea:\n0 = Iesire \n1 = Cauta device dupa marca \n2 = Afiseaza toate device urile\n3 = Afiseaza toate tabletele\n4 = Afiseaza toate telefoanele\n5 = Adaugare n device uri \n ---------------------- \n";
        std::cin >> alegere;
        switch (alegere) {
            case 0:
                std::cout << "Final";
                break;
            case 1:
                std::cout << "Citeste marca deviceului pe care doresti sa il cauti:\n";
                std::cin >> marca_device;
                try {
                    std::cout << *MagElectronice::Cauta_device(marca_device);
                } catch (const Nu_exista &err) {
                    std::cout << err;
                }
                break;
            case 2:
                std::cout << "Afisarea deviceurilor:\n";
                for (auto &device: MagElectronice::Cauta_toate_tabletele()) {
                    std::cout << *device << '\n';

                }
                for(auto &device: MagElectronice::Cauta_toate_telefoanele()){
                    std::cout<< *device << '\n';
                }
                break;
            case 3:
                std::cout << "Afisarea tabletelor:\n";
                for (auto &device: MagElectronice::Cauta_toate_tabletele()) {
                    std::cout << *device << '\n';
                }
                break;
            case 4:
                std::cout << "Afisarea telefoanelor:\n";
                for(auto &device: MagElectronice::Cauta_toate_telefoanele()){
                    std::cout<< *device << '\n';
                }
                break;
            case 5:
                std::cout<<"Cate deviceuri doriti sa cititi?\n";
                std::cin>>n;
                for(int i=0;i<n;i++)
                {
                    std::cout<<"Ce doriti sa fie?Tableta sau telefon? Scrieti 1 pentru telefon si 2 pentru tableta.\n";
                    std::cin>>alegere2;
                    switch(alegere2)
                    {
                        case 1: {
                            std::cout << "Scrieti marca telefonului\n";
                            std::cin >> marca_device;
                            std::cout << "Scrieti modelul telefonului\n";
                            std::cin >> model_device;
                            std::cout << "Scrieti anul de fabricatie\n";
                            std::cin >> an;
                            std::cout << "Scrieti reteaua SIM\n";
                            std::cin >> SIM;
                            auto adauga_telefon = std::make_shared<Telefon>(marca_device, model_device, an, SIM);
                            MagElectronice::Adauga_device(adauga_telefon);
                            break;
                        }
                        case 2: {
                            std::cout << "Scrieti marca tabletei\n";
                            std::cin >> marca_device;
                            std::cout << "Scrieti modelul tabletei\n";
                            std::cin >> model_device;
                            std::cout << "Scrieti anul de fabricatie\n";
                            std::cin >> an;
                            std::cout << "Scrieti culoarea tabletei\n";
                            std::cin >> SIM;
                            auto adauga_tableta = std::make_shared<Tableta>(marca_device, model_device, an, SIM);
                            MagElectronice::Adauga_device(adauga_tableta);
                            break;
                        }
                    }

                }

        }
    } while (alegere != 0);
    return 0;
}
