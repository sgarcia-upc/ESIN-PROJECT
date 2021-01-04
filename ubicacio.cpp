#include "ubicacio.hpp"
#include <iostream>


/* Constructora. Crea la ubicació <i, j, k>. Genera un error si
<i, j, k> no pertany a {<u, v, w> | u >= 0 ^ v >= 0 ^ w >= 0}
 o a {<-1, 0, 0>,  <-1,-1,-1>}. */
ubicacio::ubicacio(int i, int j, int k) throw(error)
{
    // <i, j, k>
    if ( i>= 0 and j >= 0 and k >=0 or i==-1 and j==0 and k==0 or i==-1 and j==-1 and k==-1 ){
        row = i;
        place = j;
        floor = k;
    } else {
        throw error(UbicacioIncorrecta);
    }
}

/* Constructora per còpia, assignació i destructora. */
ubicacio::ubicacio(const ubicacio& u) throw(error)
{
    row = u.row;
    place = u.place;
    floor = u.floor;
}

ubicacio& ubicacio::operator=(const ubicacio& u) throw(error)
{
    row = u.row;
    place = u.place;
    floor = u.floor;
    
    return (*this);
}

ubicacio::~ubicacio() throw()
{
    //res
}

/* Consultors. Retornen respectivament el primer, segon i tercer
 component de la ubicació. */
int ubicacio::filera() const throw()
{
    return row;
}

int ubicacio::placa() const throw()
{
    return place;
}

int ubicacio:: pis() const throw()
{
    return floor;
}

/* Operadors de comparació.
 L'operador d'igualtat retorna cert si i només si les dues ubicacions
 tenen la mateixa filera, plaça i pis.
 La resta d'operadors es defineixen consistentment respecte <. */
bool ubicacio::operator==(const ubicacio &u) const throw()
{
    return (row==u.row and place == u.place and floor == u.floor);
}

bool ubicacio::operator!=(const ubicacio &u) const throw()
{
    return not (*this == u);
}
/*
L'operador menor retorna cert si
i només si la filera del paràmetre implícit és més petit que la
d'u, o si les dues fileres són iguals i la plaça del paràmetre
implícit és més petita que la d'u, o si les fileres i les places
coincideixen i el pis del paràmetre implícit és més petit que el d'u.
*/
bool ubicacio::operator<(const ubicacio &u) const throw()
{
    if ( row < u.row or (row == u.row and place < u.place ) or (row == u.row and place == u.place and floor < u.floor ) )
    {
        return true;
    }
    return false;
}

bool ubicacio::operator<=(const ubicacio &u) const throw()
{
    return not (*this > u);
}

bool ubicacio::operator>(const ubicacio &u) const throw()
{
    if ( u.row < row or (u.row == row and u.place < place ) or ( u.row == row and u.place == place and u.floor < floor ) )
    {
        return true;
    }
    return false;
}

bool ubicacio::operator>=(const ubicacio &u) const throw()
{
    return not (*this < u );
}
