//
// Created by thomas on 10/10/17.
//

#include "dct.hpp"
#include <cmath>
#include "misc.hpp"

namespace ut
{
    double C(int x);

    array2<double> DCT(const array2_view<std::uint8_t>& pixel)
    {
        constexpr double PI = acos(-1);

        if(pixel.dim<0>() != pixel.dim<1>())
            throw std::exception();

        array2<double> DCT(pixel.dim());

        const unsigned int N = pixel.dim<0>();

        for(unsigned int i = 0; i < N; i++)
        {
            for(unsigned int j = 0; j < N; j++)
            {
                DCT(i, j) = 2.0/N * C(i) * C(j) * sum<double>(0, N-1, [&pixel, i, j, N](int x)->double { return
                                                                      sum<double>(0, N-1, [&pixel, i, j, x, N](int y)->double { return
                                                                                          pixel(x, y) * cos((2*x + 1) * i * PI /(2 * N)) * cos((2*y + 1) * j * PI /(2 * N)); }); });
            }
        }

        return DCT;
    }


    array2<std::uint8_t> DCT_inv(const array2_view<double>& DCT)
    {
        constexpr double PI = acos(-1);

        if(DCT.dim<0>() != DCT.dim<1>())
            throw std::exception();

        array2<std::uint8_t> pixel(DCT.dim());

        const unsigned int N = DCT.dim<0>();

        for(unsigned int x = 0; x < N; x++)
        {
            for(unsigned int y = 0; y < N; y++)
            {
                pixel(x, y) = clamp(2.0/N * sum<double>(0, N-1, [&DCT, x, y, N](int i)->double { return
                                                                sum<double>(0, N-1, [&DCT, x, y, i, N](int j)->double { return
                                                                                    C(i)*C(j) * DCT(i, j) * cos((2*x + 1) * i * PI /(2 * N)) * cos((2*y + 1) * j * PI /(2 * N)); }); }),
                                    0.0, 255.0);
            }
        }

        return pixel;
    }

    double C(int x)
    {
        return (x == 0) ? 1/sqrt(2) : 1;
    }

    array2<double> compute_DCT(const array2_view<std::uint8_t>& image, unsigned int N)
    {
        auto dim_image = image.dim();
        auto current_dim = dim_image;
        std::array<size_t, 2> new_dim = {static_cast<size_t>(ceil(static_cast<double>(dim_image[0])/N)*N),
                                         static_cast<size_t>(ceil(static_cast<double>(dim_image[1])/N)*N)};

        // On travaille sur une image redimensionnée pour que chaque dimension soit un multiple de N (le plus petit multiple de N >= dimension de base)
        array2<std::uint8_t> temp_image(new_dim);
        temp_image({0, dim_image[0]-1}, {0, dim_image[1]-1}) = image;

        // On remplit ces potentiels espaces vides
        while(new_dim[0] > current_dim[0])
        {
            // Pour chaque colonne
            for(size_t i = 0; i < current_dim[1]; ++i)
            {
                // On copie depuis le dernier élément non vide de la colonne à l'envers
                // Le nombre d'éléments à copier est le plus petit entre le nombre d'éléments vide à remplir ou le nombre d'éléments non vide
                // La destination de la copie est le premier élément vide de la colonne à l'endroit
                // On a ainsi une copie en "miroir"
                std::copy_n(temp_image({0, new_dim[0]-1}, {i, i}).riter(current_dim[0]-1, 0),
                            std::min(current_dim[0], new_dim[0]-current_dim[0]),
                            temp_image({0, new_dim[0]-1}, {i, i}).iter(current_dim[0], 0));
            }
            current_dim[0] = std::min(2*current_dim[0], new_dim[0]);
        }
        while(new_dim[1] > current_dim[1])
        {
            for(size_t i = 0; i < current_dim[0]; ++i)
            {
                // Idem en travaillant sur les lignes au lieu des colonnes
                std::copy_n(temp_image.riter(i, current_dim[1]-1),
                            std::min(current_dim[1], new_dim[1]-current_dim[1]),
                            temp_image.iter(i, current_dim[1]));
                // (Ici pas besoin d'utiliser de vues donc le code est plus digeste)
            }
            current_dim[1] = std::min(2*current_dim[1], new_dim[1]);
        }

        array2<double> DCT_image(temp_image.dim());

        // Pour chaque carré de N*N
        for(size_t i = 0; i < new_dim[0]/N; ++i)
            for(size_t j = 0; j < new_dim[1]/N; ++j)
                DCT_image({i*N, i*N+N-1}, {j*N, j*N+N-1}) = DCT(temp_image({i*N, i*N+N-1}, {j*N, j*N+N-1}));

        return DCT_image;
    }


    array2<std::uint8_t> compute_DCT_inv(const array2_view<double>& DCT_image, unsigned int N)
    {
        array2<std::uint8_t> image(DCT_image.dim());

        for(size_t i = 0; i < image.dim(0)/N; ++i)
            for(size_t j = 0; j < image.dim(1)/N; ++j)
                image({i*N, i*N+N-1}, {j*N, j*N+N-1}) = DCT_inv(DCT_image({i*N, i*N+N-1}, {j*N, j*N+N-1}));

        return image;
    }
};