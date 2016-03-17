#include "Person.h"
#include "Planets.h"
#include <cassert>
#include <iostream>

int main(int argc, const char* argv[]) {
    {
        auto& andrzej1 = earth().registerCitizen("Andrzej");
        auto& andrzej2 = earth().registerCitizen("Andrzej");
        auto& mietek = earth().registerCitizen("Mietek");

        assert(earth().findCitizen(andrzej1.id()).id() == andrzej1.id());
        assert(earth().findCitizen(andrzej2.id()).id() == andrzej2.id());
        assert(earth().findCitizen(mietek.id()).id() == mietek.id());

        assert(earth().findCitizen(andrzej1.id()).name() == andrzej1.name());
        assert(earth().findCitizen(andrzej2.id()).name() == andrzej2.name());
        assert(earth().findCitizen(mietek.id()).name() == mietek.name());

        assert(andrzej1.name() == "Andrzej");
        assert(andrzej2.name() == "Andrzej");
        assert(mietek.name() == "Mietek");

        try {
            earth().findCitizen("Jan");
            assert(0);
        } catch (...) {
        }
    }

    {
        auto& andrzej1 = qonos().registerCitizen("Andrzej");
        auto& andrzej2 = qonos().registerCitizen("Andrzej");
        auto& mietek = qonos().registerCitizen("Mietek");

        assert(qonos().findCitizen(andrzej1.id()).id() == andrzej1.id());
        assert(qonos().findCitizen(andrzej2.id()).id() == andrzej2.id());
        assert(qonos().findCitizen(mietek.id()).id() == mietek.id());

        assert(qonos().findCitizen(andrzej1.id()).name() == andrzej1.name());
        assert(qonos().findCitizen(andrzej2.id()).name() == andrzej2.name());
        assert(qonos().findCitizen(mietek.id()).name() == mietek.name());

        assert(andrzej1.name() == "Andrzej");
        assert(andrzej2.name() == "Andrzej");
        assert(mietek.name() == "Mietek");

        try {
            qonos().findCitizen("Jan");
            assert(0);
        } catch (...) {
        }
    }

    {
        auto& andrzej1 = bynaus().registerCitizen("Andrzej");
        auto& andrzej2 = bynaus().registerCitizen("Andrzej");
        auto& mietek = bynaus().registerCitizen("Mietek");
        auto& double_andrzej = bynaus().registerCitizen(andrzej1, andrzej2);

        assert(bynaus().findCitizen(andrzej1.id()).id() == andrzej1.id());
        assert(bynaus().findCitizen(andrzej2.id()).id() == andrzej2.id());
        assert(bynaus().findCitizen(mietek.id()).id() == mietek.id());
        assert(bynaus().findCitizen(double_andrzej.id()).id() == double_andrzej.id());
        assert(andrzej1.id() != double_andrzej.id());
        assert(andrzej1.name() + '&' + andrzej2.name() == double_andrzej.name());

        assert(bynaus().findCitizen(andrzej1.id()).name() == andrzej1.name());
        assert(bynaus().findCitizen(andrzej2.id()).name() == andrzej2.name());
        assert(bynaus().findCitizen(mietek.id()).name() == mietek.name());

        assert(andrzej1.name() == "Andrzej");
        assert(andrzej2.name() == "Andrzej");
        assert(mietek.name() == "Mietek");

        try {
            qonos().findCitizen("Jan");
            assert(0);
        } catch (...) {
        }
    }

    std::cerr << "Finish OK" << std::endl;
    return 0;
}
