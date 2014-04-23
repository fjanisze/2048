#include <stdexcept>

#ifndef SIMPLE_MATRIX_HPP
#define SIMPLE_MATRIX_HPP

namespace simple_matrix
{

    enum rotation_angle{
        rotate_0 = 0,
        rotate_90 = 90,
        rotate_180 = 180,
        rotate_270 = 270
    };

    template<typename T>
    class simple_square_matrix
    {
        int size;
        T** matrix;
    public:
        simple_square_matrix(long edge_size);
        ~simple_square_matrix();
        T& get(int x,int y);
        void transponse_matrix();
        void reverse_row();
        void reverse_col();
        int rotate(rotation_angle angle = rotation_angle::rotate_90);
        int get_size();
    };

    template<typename T>
    simple_square_matrix<T>::simple_square_matrix(long edge_size)
    {
        if(edge_size<=0) throw std::out_of_range("edge_size is less than 0");
        matrix = new T*[edge_size];
        for(int i=0;i<edge_size;i++)
        {
            matrix[i]=new T[edge_size];
        }
        size=edge_size;
    }

    template<typename T>
    simple_square_matrix<T>::~simple_square_matrix()
    {
        for(int i=0;i<size;i++)
        {
            delete[] matrix[i];
        }
        delete[] matrix;
    }

    template<typename T>
    T& simple_square_matrix<T>::get(int x, int y)
    {
        if((x<0)||(x>=size)||(y<0)||(y>=size)) throw std::out_of_range("invalid index in the matrix used for the set operation");
        return matrix[x][y];
    }

    template<typename T>
    void simple_square_matrix<T>::transponse_matrix()
    {
        for(int y=0;y<size;y++)
        {
            for(int x=y+1;x<size;x++)
            {
                std::swap(matrix[y][x],matrix[x][y]);
            }
        }
    }

    template<typename T>
    void simple_square_matrix<T>::reverse_row()
    {
        for(int y=0;y<size;y++)
        {
            for(int x=0;x<size/2;x++)
            {
                std::swap(matrix[x][y],matrix[size-x-1][y]);
            }
        }
    }

    template<typename T>
    void simple_square_matrix<T>::reverse_col()
    {
        for(int x=0;x<size;x++)
        {
            for(int y=0;y<size/2;y++)
            {
                std::swap(matrix[x][y],matrix[x][size-y-1]);
            }
        }
    }


    template<typename T>
    int simple_square_matrix<T>::rotate(rotation_angle angle)
    {
        switch(angle)
        {
        case rotation_angle::rotate_90:
            transponse_matrix();
            reverse_row();
            break;
        case rotation_angle::rotate_180:
            reverse_row();
            reverse_col();
            break;
        case rotation_angle::rotate_270:
            transponse_matrix();
            reverse_col();
            break;
        default:
            //Ignored
            ;
        };
        return 0;
    }

    template<typename T>
    int simple_square_matrix<T>::get_size()
    {
        return size;
    }
}

#endif
