#include "contenidor.hpp"

/* Constructora. Crea un contenidor amb matrícula m i _longitud l.
 Es produeix un error si m no és una seqüència de un o més caràcters,
 formada exclusivament per lletres majúscules i dígits,
 o si l no pertany a {10, 20, 30} */
contenidor::contenidor (const string &m, nat l) throw(error)
{
    _matricula = m;
    _longitud = l;
}

/* Constructora per còpia, assignació i destructora. */
contenidor::contenidor (const contenidor &u) throw(error)
{
    _matricula = u._matricula;
    _longitud = u._longitud;
}

contenidor& contenidor::operator=(const contenidor &u) throw(error)
{
    _matricula = u._matricula;
    _longitud = u._longitud;
    return (*this);
}

contenidor::~contenidor() throw()
{
    //res by the moment
}

/* Consultors. Retornen respectivament la _longitud i la matrícula del
 contenidor. */
nat contenidor::longitud() const throw()
{
    return _longitud;
}

string contenidor::matricula() const throw()
{
    return _matricula;
}

/* Operadors de comparació.*/
 
/*
L'operador d'igualtat retorna cert si i
només si els dos contenidors contenen la mateixa matrícula i _longitud.
*/
bool contenidor::operator==(const contenidor &c) const throw()
{
    return ( _longitud == c._longitud and _matricula == c._matricula );
}

bool contenidor::operator!=(const contenidor &c) const throw()
{
    return not (*this == c );
}

/*
 L'operador menor retorna cert si i només si la matrícula del
 paràmetre implícit es més petit en ordre alfabètic que la c o si les
 dues matrícules són iguals i la _longitud del paràmetre implícit és més
 petita que la de c.
 
 La resta d'operadors es defineixen consistentment
 respecte a <.
*/
bool contenidor::operator<(const contenidor &c) const throw()
{
    return ( _matricula < c._matricula or ( _matricula == c._matricula and _longitud < c._longitud ) );
}

bool contenidor::operator<=(const contenidor &c) const throw()
{
    //és el contrari del >
    return not (*this > c );
}

bool contenidor::operator>(const contenidor &c) const throw()
{
    return ( _matricula > c._matricula or ( _matricula == c._matricula and _longitud > c._longitud ) );
}

bool contenidor::operator>=(const contenidor &c) const throw()
{
    //és el contrari del <
    return not (*this < c );
}

