#ifndef UTILITIES_UNITS_HPP
#define UTILITIES_UNITS_HPP

#include <type_traits>


namespace unit
{
    namespace implementation_detail
    {
        template<typename T>
        constexpr T trivial_pow(T x, int p)
        {
            return p == 0 ? 1 : (p > 0 ? x * trivial_pow(x, p - 1) : 1 / trivial_pow(x, -p));
        }
    }

    namespace ratio
    {
        struct one { constexpr static const double value = 1.0; };

        template<int N>
        struct pow_10 { constexpr static const double value = implementation_detail::trivial_pow(10.0, N); };

        template<typename... Ratios>
        struct multiply { constexpr static double value = (1.0 * ... * Ratios::value); };

        template<typename R1, typename R2>
        struct divide { constexpr static double value = R1::value/R2::value; };

        template<typename Ratio, unsigned int N>
        struct pow { constexpr static double value = implementation_detail::trivial_pow(Ratio::value, N); };

        // Alias
        using yocto = pow_10<-24>;
        using zepto = pow_10<-21>;
        using atto = pow_10<-18>;
        using femto = pow_10<-15>;
        using pico = pow_10<-12>;
        using nano = pow_10<-9>;
        using micro = pow_10<-6>;
        using milli = pow_10<-3>;
        using centi = pow_10<-2>;
        using deci = pow_10<-1>;
        using deca = pow_10<1>;
        using hecto = pow_10<2>;
        using kilo = pow_10<3>;
        using mega = pow_10<6>;
        using giga = pow_10<9>;
        using tera = pow_10<12>;
        using peta = pow_10<15>;
        using exa = pow_10<18>;
        using zetta = pow_10<21>;
        using yotta = pow_10<24>;
    }

    // Définition de la structure dimension qui contient les information sur la dimension d'une grandeur
    // à l'ide de 7 entiers correspondants aux exposants des unités de base du SI
    template<int M_, int T_, int L_, int Theta_, int I_, int N_, int J_>
    struct dimension {
        dimension() = delete;

        enum { M = M_ };
        enum { T = T_ };
        enum { L = L_ };
        enum { Theta = Theta_ };
        enum { I = I_ };
        enum { N = N_ };
        enum { J = J_ };

        using ratio = ratio::one;
    };

    // Récupérer la dimension à partir des composantes d'une dimension emmélée
    template<typename Dim>
    using base_dim = dimension<Dim::M,
                               Dim::T,
                               Dim::L,
                               Dim::Theta,
                               Dim::I,
                               Dim::N,
                               Dim::J>;

    // Créer une dimension à partir de 2 dimensions en additionnant les exposants,
    // utile pour la multiplication de grandeurs
    template<typename Dim1, typename Dim2>
    using add_dim = dimension<Dim1::M + Dim2::M,
                              Dim1::T + Dim2::T,
                              Dim1::L + Dim2::L,
                              Dim1::Theta + Dim2::Theta,
                              Dim1::I + Dim2::I,
                              Dim1::N + Dim2::N,
                              Dim1::J + Dim2::J>;

    // Créer une dimension à partir de 2 dimensions en soustrayant les exposants,
    // utile pour la division de grandeurs
    template<typename Dim1, typename Dim2>
    using substract_dim = dimension<Dim1::M - Dim2::M,
                                    Dim1::T - Dim2::T,
                                    Dim1::L - Dim2::L,
                                    Dim1::Theta - Dim2::Theta,
                                    Dim1::I - Dim2::I,
                                    Dim1::N - Dim2::N,
                                    Dim1::J - Dim2::J>;

    // Fonction pour vérifier que 2 dimensions sont équivalentes
    template<typename Dim1, typename Dim2>
    using equiv_dim = std::bool_constant<Dim1::M == Dim2::M
                                      && Dim1::T == Dim2::T
                                      && Dim1::L == Dim2::L
                                      && Dim1::Theta == Dim2::Theta
                                      && Dim1::I == Dim2::I
                                      && Dim1::N == Dim2::N
                                      && Dim1::J == Dim2::J>;

    namespace dim
    {
        // Définition d'alias pour les dimensions de base
        using scalar                    = dimension<0, 0, 0, 0, 0, 0, 0>;
        using mass                      = dimension<1, 0, 0, 0, 0, 0, 0>;
        using time                      = dimension<0, 1, 0, 0, 0, 0, 0>;
        using length                    = dimension<0, 0, 1, 0, 0, 0, 0>;
        using thermodynamic_temperature = dimension<0, 0, 0, 1, 0, 0, 0>;
        using electric_current          = dimension<0, 0, 0, 0, 1, 0, 0>;
        using amount_of_substance       = dimension<0, 0, 0, 0, 0, 1, 0>;
        using luminous_intensity        = dimension<0, 0, 0, 0, 0, 0, 1>;
    }

    // Définition d'une structure unit qui contient les informations concernant une unité
    // Ça comporte un ratio par rapport à une référence et la référence qui peut être soit une autre unité soit une dimension
    // Par exemple l'unité mètre peut être définit comme une unité ayant un ratio de 1/1 par rapport à la dimension
    // 'longueur'. Puis l'unité millimètre peut être définit comme une unité ayant un ratio de 1/1000 par rapport à l'unité mètre
    template<typename Ratio, typename Dim>
    struct unit
    {
        unit() = delete;
        using ratio = ratio::multiply<Ratio, typename Dim::ratio>;
        using dimension = Dim;
        enum { M = Dim::M };
        enum { T = Dim::T };
        enum { L = Dim::L };
        enum { Theta = Dim::Theta };
        enum { I = Dim::I };
        enum { N = Dim::N };
        enum { J = Dim::J };
    };

    // L'unité inverse
    template<typename Unit>
    using per = unit<ratio::divide<ratio::one, typename Unit::ratio>, substract_dim<dim::scalar, typename Unit::dimension>>;
/*
    // Composition d'unité (multiplication)
    template<typename... Units>
    using compose = unit<
                            ratio::multiply<(... , Units::ratio)>,
                            dimension<(... + Units::M),
                                      (... + Units::T),
                                      (... + Units::L),
                                      (... + Units::Theta),
                                      (... + Units::I),
                                      (... + Units::N),
                                      (... + Units::J)>
                        >;*/

    namespace implementation_detail
    {
        template<typename Unit1, typename Unit2>
        using compose_base = unit<ratio::multiply<typename Unit1::ratio, typename Unit2::ratio>,
            add_dim<typename Unit1::dimension, typename Unit2::dimension>>;

        template<typename U1, typename U2, typename... Us>
        struct compose_impl
        {
            using type = typename compose_impl<compose_base<U1, U2>, Us...>::type;
        };
        template<typename U1, typename U2>
        struct compose_impl<U1, U2>
        {
            using type = compose_base<U1, U2>;
        };
    }

    template<typename... Us>
    using compose = typename implementation_detail::compose_impl<Us...>::type;



    // Puissance d'unité
    template<typename Unit, unsigned int N>
    using pow = unit<
                        ratio::pow<typename Unit::ratio, N>,
                        dimension<Unit::M * N,
                                  Unit::T * N,
                                  Unit::L * N,
                                  Unit::Theta * N,
                                  Unit::I * N,
                                  Unit::N * N,
                                  Unit::J * N>
                    >;

    // Alias for pow 2
    template<typename Unit>
    using square = pow<Unit, 2>;

    // Alias for pow 3
    template<typename Unit>
    using cube = pow<Unit, 3>;

    // Définition de la structure quantity qui contient une valeur et des information de dimension
    template<typename Dim, typename T = double>
    struct quantity
    {
        T value;

        template<typename Unit>
        constexpr T as() const { static_assert(equiv_dim<Dim, typename Unit::dimension>::value, "Incompatible dimension");
                                 return value / Unit::ratio::value; }

        constexpr quantity() : value{} {}
        explicit constexpr quantity(T v) : value{v} {}

        constexpr bool operator==(quantity q) const { return value == q.value; }
        constexpr bool operator!=(quantity q) const { return value != q.value; }
        constexpr bool operator<(quantity q) const { return value < q.value; }
        constexpr bool operator<=(quantity q) const { return value <= q.value; }
        constexpr bool operator>(quantity q) const { return value > q.value; }
        constexpr bool operator>=(quantity q) const { return value >= q.value; }
    };

    template<typename T>
    struct quantity<dim::scalar, T>
    {
        T value;

        template<typename Unit>
        constexpr T as() const { static_assert(equiv_dim<dim::scalar, typename Unit::dimension>::value, "Incompatible dimension");
            return value / Unit::ratio::value; }

        constexpr quantity() : value{} {}
        constexpr quantity(T v) : value{v} {}
        constexpr quantity& operator=(T v) { value = v; return *this; }
        constexpr operator T() { return value; }

        constexpr bool operator==(quantity q) const { return value == q.value; }
        constexpr bool operator!=(quantity q) const { return value != q.value; }
        constexpr bool operator<(quantity q) const { return value < q.value; }
        constexpr bool operator<=(quantity q) const { return value <= q.value; }
        constexpr bool operator>(quantity q) const { return value > q.value; }
        constexpr bool operator>=(quantity q) const { return value >= q.value; }
    };

    // Fonction qui construit une quantité à partir d'une unité et d'une valeur
    template<typename Unit, typename T>
    constexpr auto as(T v)
    {
        quantity<base_dim<typename Unit::dimension>, T> x;
        x.value = v * Unit::ratio::value;
        return x;
    }

    // Fonction qui renvoit la valeur d'une quantité exprimé dans une certaine unité
    template<typename Unit, typename Dim, typename T>
    constexpr T as(quantity<Dim, T> q)
    {
        static_assert(equiv_dim<Dim, typename Unit::dimension>::value, "Incompatible dimension");
        return q.value / Unit::ratio::value;
    }

    // Aditionner deux quantités de même unité
    template<typename Dim, typename T>
    auto operator+(quantity<Dim, T> a, quantity<Dim, T> b)
    {
        a.value += b.value;
        return a;
    };

    // Soustraire deux quantités de même unités
    template<typename Dim, typename T>
    auto operator-(quantity<Dim, T> a, quantity<Dim, T> b)
    {
        a.value -= b.value;
        return a;
    };

    // Multiplier deux quantités en changeant l'unité
    template<typename Dim1, typename Dim2, typename T>
    auto operator*(quantity<Dim1, T> a, quantity<Dim2, T> b)
    {
        quantity<base_dim<add_dim<Dim1, Dim2>>, T> x;
        x.value = a.value * b.value;
        return x;
    };

    // Diviser deux quantités en changeant l'unité
    template<typename Dim1, typename Dim2, typename T>
    auto operator/(quantity<Dim1, T> a, quantity<Dim2, T> b)
    {
        quantity<base_dim<substract_dim<Dim1, Dim2>>, T> x;
        x.value = a.value / b.value;
        return x;
    };


    // Définitions d'alias pour les unités
    using radian   = unit<ratio::one, dim::scalar>;
    using gram     = unit<ratio::pow_10<3>, dim::mass>; // Kilogram is the SI unit so gram is 1/1000 of kilogram
    using second   = unit<ratio::one, dim::time>;
    using metre    = unit<ratio::one, dim::length>;
    using kelvin   = unit<ratio::one, dim::thermodynamic_temperature>;
    using ampere   = unit<ratio::one, dim::electric_current>;
    using mole     = unit<ratio::one, dim::amount_of_substance>;
    using candela  = unit<ratio::one, dim::luminous_intensity>;


    #define ALIAS_10_POW_UNIT(unit_base_name, prefix) \
        using prefix ## unit_base_name = unit<ratio::prefix, unit_base_name>;

    #define ALIAS_ALL_10_POW_UNIT(unit_base_name) \
        ALIAS_10_POW_UNIT(unit_base_name, yocto) \
        ALIAS_10_POW_UNIT(unit_base_name, zepto) \
        ALIAS_10_POW_UNIT(unit_base_name, atto) \
        ALIAS_10_POW_UNIT(unit_base_name, femto) \
        ALIAS_10_POW_UNIT(unit_base_name, pico) \
        ALIAS_10_POW_UNIT(unit_base_name, nano) \
        ALIAS_10_POW_UNIT(unit_base_name, micro) \
        ALIAS_10_POW_UNIT(unit_base_name, milli) \
        ALIAS_10_POW_UNIT(unit_base_name, centi) \
        ALIAS_10_POW_UNIT(unit_base_name, deci) \
        ALIAS_10_POW_UNIT(unit_base_name, deca) \
        ALIAS_10_POW_UNIT(unit_base_name, hecto) \
        ALIAS_10_POW_UNIT(unit_base_name, kilo) \
        ALIAS_10_POW_UNIT(unit_base_name, mega) \
        ALIAS_10_POW_UNIT(unit_base_name, giga) \
        ALIAS_10_POW_UNIT(unit_base_name, tera) \
        ALIAS_10_POW_UNIT(unit_base_name, peta) \
        ALIAS_10_POW_UNIT(unit_base_name, exa) \
        ALIAS_10_POW_UNIT(unit_base_name, zetta) \
        ALIAS_10_POW_UNIT(unit_base_name, yotta)

    ALIAS_ALL_10_POW_UNIT(gram);
    ALIAS_ALL_10_POW_UNIT(second);
    ALIAS_ALL_10_POW_UNIT(metre);
    ALIAS_ALL_10_POW_UNIT(kelvin);
    ALIAS_ALL_10_POW_UNIT(ampere);
    ALIAS_ALL_10_POW_UNIT(mole);
    ALIAS_ALL_10_POW_UNIT(candela);


    // Units composed by the previous ones
    namespace implementation_detail
    {
        struct rad_to_deg_ratio { constexpr static double const value = 3.1415926535898 / 180; };
    }
    using degre     = unit<implementation_detail::rad_to_deg_ratio, radian>;
    using farad     = compose<square<ampere>, pow<second, 4>, per<kilogram>, per<square<metre>>>;
    ALIAS_ALL_10_POW_UNIT(farad);
    using katal     = compose<mole, per<second>>;
    ALIAS_ALL_10_POW_UNIT(katal);
    using sievert   = compose<square<metre>, per<square<second>>>;
    ALIAS_ALL_10_POW_UNIT(sievert);
    using coulomb   = compose<ampere, second>;
    ALIAS_ALL_10_POW_UNIT(coulomb);
    using siemens   = compose<square<ampere>, cube<second>, per<kilogram>, per<square<metre>>>;
    ALIAS_ALL_10_POW_UNIT(siemens);
    using volt      = compose<kilogram, square<metre>, per<ampere>, per<cube<second>>>;
    ALIAS_ALL_10_POW_UNIT(volt);
    using ohm       = compose<kilogram, square<metre>, per<square<ampere>>, per<cube<second>>>;
    ALIAS_ALL_10_POW_UNIT(ohm);
    using joule     = compose<kilogram, square<metre>, per<square<metre>>>;
    ALIAS_ALL_10_POW_UNIT(joule);
    using newton    = compose<kilogram, metre, per<square<second>>>;
    ALIAS_ALL_10_POW_UNIT(newton);
    using hertz     = per<second>;
    ALIAS_ALL_10_POW_UNIT(hertz);
    using lux       = compose<candela, per<square<metre>>>;
    ALIAS_ALL_10_POW_UNIT(lux);
    using henry     = compose<kilogram, square<metre>, per<square<ampere>>, per<square<second>>>;
    ALIAS_ALL_10_POW_UNIT(henry);
    using lumen     = candela;
    ALIAS_ALL_10_POW_UNIT(lumen);
    using weber     = compose<kilogram, square<metre>, per<ampere>, per<square<second>>>;
    ALIAS_ALL_10_POW_UNIT(weber);
    using tesla     = compose<kilogram, per<ampere>, per<square<second>>>;
    ALIAS_ALL_10_POW_UNIT(tesla);
    using watt      = compose<kilogram, per<square<metre>>, per<cube<second>>>;
    ALIAS_ALL_10_POW_UNIT(watt);
    using pascal    = compose<kilogram, per<metre>, per<square<second>>>;
    ALIAS_ALL_10_POW_UNIT(pascal);
    using becquerel = per<second>;
    ALIAS_ALL_10_POW_UNIT(becquerel);
    using gray      = compose<square<metre>, per<square<second>>>;
    ALIAS_ALL_10_POW_UNIT(gray);

    // mass units
    namespace implementation_detail
    {
        struct avoirdupois_ounce_ratio { constexpr static const double value = 28.349523125; };
        struct troy_ounce_ratio { constexpr static const double value = 31.1034768; };
        struct maria_theresa_ounce_ratio { constexpr static const double value = 28.0668; };
        struct spanish_ounce_ratio { constexpr static const double value = 28.75; };
        struct french_ounce_ratio { constexpr static const double value = 30.59; };
        struct portuguese_ounce_ratio { constexpr static const double value = 28.69; };
        struct roman_ounce_ratio { constexpr static const double value = 27.4; };
        struct chinese_metric_ounce_ratio { constexpr static const double value = 50; };
        struct english_tower_ounce_ratio { constexpr static const double value = 29.16; };

    }

    using avoirdupois_ounce     = unit<implementation_detail::avoirdupois_ounce_ratio, gram>;
    using troy_ounce            = unit<implementation_detail::troy_ounce_ratio, gram>;
    using apothecaries_ounce    = troy_ounce;
    using maria_theresa_ounce   = unit<implementation_detail::maria_theresa_ounce_ratio, gram>;
    using spanish_ounce         = unit<implementation_detail::spanish_ounce_ratio, gram>;
    using french_ounce          = unit<implementation_detail::french_ounce_ratio, gram>;
    using portuguese_ounce      = unit<implementation_detail::portuguese_ounce_ratio, gram>;
    using roman_ounce           = unit<implementation_detail::roman_ounce_ratio, gram>;
    using dutch_metric_ounce    = unit<ratio::pow_10<2>, gram>;
    using chinese_metric_ounce  = unit<implementation_detail::chinese_metric_ounce_ratio, gram>;
    using english_tower_ounce   = unit<implementation_detail::english_tower_ounce_ratio, gram>;

    using ton = megagram;




}

#endif //UTILITIES_UNITS_HPP
