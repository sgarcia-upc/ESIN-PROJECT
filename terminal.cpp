#include "terminal.hpp"

/* Constructora. Crea una terminal buida amb n fileres de m places
   cadascuna, i una alçada màxima d'apilament h; a més fixa l'estratègia
   d'inserció i retirada dels contenidors respecte el paràmetre st.
   Genera un error si n=0, m=0, h=0, h > HMAX o
   st no pertany a {FIRST_FIT, LLIURE}. */
terminal::terminal(nat n, nat m, nat h, estrategia st) throw(error){
    if (n == 0) throw error(NumFileresIncorr);
    if (m == 0) throw error(NumPlacesIncorr);
    if (h > HMAX or h == 0) throw error(AlcadaMaxIncorr);
    if (st != FIRST_FIT and st != LLIURE) throw error(EstrategiaIncorr);

    _aespera = NULL;
    _num_fileres = n;
    _num_pisos = h;
    _num_places = m;
    _num_ops = 0;
    _escollida = st;
    _cataleg = new cataleg<our_pair>(130);
    
    terr = new string **[_num_fileres]();
    for (int i = 0; i < _num_fileres; i++)
    {
        terr[i] = new string *[_num_pisos]();
        for (int j = 0; j < _num_pisos; j++)
            terr[i][j] = new string [_num_places]();
    }


}

/* Constructora per còpia, assignació i destructora. */
terminal::terminal(const terminal& b) throw(error){
    _num_fileres = b._num_fileres;
    _num_pisos = b._num_pisos;
    _num_places = b._num_places;
    _num_ops = b._num_ops;
    _escollida = b._escollida;
    _cataleg = b._cataleg; // TODO: Check this
    // TODO: copy list one by one
}

terminal& terminal::operator=(const terminal& b) throw(error){
    _num_fileres = b._num_fileres;
    _num_pisos = b._num_pisos;
    _num_places = b._num_places;
    _num_ops = b._num_ops;
    _escollida = b._escollida;
    // TODO: copy list one by one
    return *this;
}

terminal::~terminal() throw(){

    // Destroy TERR
    for (int i = 0; i < _num_fileres; i++)
    {
        for (int j = 0; j < _num_pisos; j++)
            delete[] terr[i][j];
        delete[] terr[i];
    }
    delete[] terr;
    // TODO: delete list one by one
}

/* Col·loca el contenidor c en l'àrea d'emmagatzematge de la terminal o
   en l'àrea d'espera si no troba lloc en l'àrea d'emmagatzematge usant
   l'estratègia prefixada en el moment de crear la terminal. Si el
   contenidor c es col·loca en l'àrea d'emmagatzematge pot succeir que
   un o més contenidors de l'àrea d'espera puguin ser moguts a l'àrea
   d'emmagatzematge.
   En aquest cas es mouran els contenidors de l'àrea d'espera a l'àrea
   d'emmagatzematge seguint l'ordre que indiqui l'estratègia que s'està
   usant. Finalment, genera un error si ja existís a la terminal un
   contenidor amb una matrícula idèntica que la del contenidor c. */
void terminal::insereix_contenidor(const contenidor &c) throw(error){
    
    if (_cataleg->existeix(c.matricula()))
        throw error(MatriculaDuplicada);

    our_pair op;
    op.c = new contenidor(c);


    // TODO: check if "contenidor" on "area espera" need to be in cataleg
    if (_escollida == FIRST_FIT){
        ubicacio u = insereix_first_fit(c);
        //std::cout << u.filera() << " " << u.pis() << " " << u.placa() << std::endl;
        if (u == ubicacio(-1,0,0)){
            // Area espera
            // Afegir area de espera
            afegir_area_espera(c.matricula());
        } else {
            // S'ha afegit correctament
            // comprovar area de espera
            _num_ops++;
        }

        // Si o si van al cataleg
        op.u = new ubicacio(u);
        _cataleg->assig(c.matricula(), op); // throws ClauStringBuit
    } 

    if (_escollida == LLIURE) {
        op.u = new ubicacio(0,0,0);
    }

}

/* Retira de la terminal el contenidor c la matrícula del qual és igual
   a m. Aquest contenidor pot estar a l'àrea d'emmagatzematge o a l'àrea
   d'espera. Si el contenidor estigués a l'àrea d'emmagatzematge llavors
   s'hauran de moure a l'àrea d'espera tots els contenidors que siguin
   necessaris per netejar la part superior de c, s'hauran de retirar
   possiblement diversos contenidors, començant pel contenidor sense cap
   altre a sobre amb el número de plaça més baix (més a l'esquerra) i així
   successivament (veure exemple amb detall a la subsecció Estratègia
   FIRST_FIT). Un cop s'hagi eliminat el contenidor indicat, s'intenta
   moure contenidors de l'àrea d'espera a l'àrea d'emmagatzematge, seguint
   l'ordre que indiqui l'estratègia que s'està usant. Genera un error si a
   la terminal no hi ha cap contenidor la matrícula del qual sigui igual a m. */
void terminal::retira_contenidor(const string &m) throw(error){
    
    if (not _cataleg->existeix(m))
        throw error(MatriculaInexistent);

    //_cataleg->elimina(m); // throws ClauStringBuit

    if (_escollida == FIRST_FIT){
        ubicacio u = on(m);
        if (u == ubicacio(-1,-1,-1)){
            error(MatriculaInexistent); 
        } else if (u == ubicacio(-1, 0, 0)){
            // Esta al area de espera 
            retirar_area_espera(m);
            _cataleg->elimina(m);
        } else {
            retirar_first_fit(u); 
            _cataleg->elimina(m);
        }
        // obtenir ubicacio 
        // obtenir tamany

        // Si hi ha algo encima recall con el piso incrementado
        // BASE: si no tengo nada salgo
    } 

    if (_escollida == LLIURE) {
        
    }
}

/* Retorna la ubicació <i, j, k> del contenidor la matrícula del qual és
   igual a m si el contenidor està a l'àrea d'emmagatzematge de la terminal,
   la ubicació <-1, 0, 0> si el contenidor està a l'àrea d'espera, i la
   ubicació <-1, -1, -1> si no existeix cap contenidor que tingui una
   matrícula igual a m.
   Cal recordar que si un contenidor té més de 10 peus, la seva ubicació
   correspon a la plaça que tingui el número de plaça més petit. */
ubicacio terminal::on(const string &m) const throw(){

    try {
        our_pair p = _cataleg->operator[](m);
        return *p.u;

    } catch (error(ClauInexistent)) {
        return ubicacio(-1, -1, -1);
    }

}

/* Retorna la longitud del contenidor la matrícula del qual és igual
   a m. Genera un error si no existeix un contenidor a la terminal
   la matrícula del qual sigui igual a m. */
nat terminal::longitud(const string &m) const throw(error){
    //TODO: Area Espera
    try {
        our_pair p = _cataleg->operator[](m);
        return p.c->longitud();
    } catch (error(ClauInexistent)) {
        throw(error(MatriculaInexistent));
    }
}

/* Retorna la matrícula del contenidor que ocupa la ubicació u = <i, j, k>
   o la cadena buida si la ubicació està buida. Genera un error si
   i < 0, i >= n, j < 0, j >= m, k < 0 o k >= h, o sigui si <i, j, k> no
   identifica una ubicació vàlida de l'àrea d'emmagatzematge. Cal observar
   que si m, obtinguda amb t.contenidor_ocupa(u, m), és una matrícula (no
   la cadena buida) pot succeir que u != t.on(m), ja que un contenidor pot 
   ocupar diverses places i la seva ubicació es correspon amb la de la
   plaça ocupada amb número de plaça més baix. */
void terminal::contenidor_ocupa(const ubicacio &u, string &m) const throw(error){
    
    if (u.filera() < 0 or u.filera() >= _num_fileres) throw error(UbicacioNoMagatzem);
    if (u.pis() < 0 or u.pis() >= _num_pisos) throw error(UbicacioNoMagatzem);
    if (u.placa() < 0 or u.placa() >= _num_places) throw error(UbicacioNoMagatzem);

    m = terr[u.filera()][u.pis()][u.placa()];
}  

/* Retorna el nombre de places de la terminal que en aquest instant
   només hi cabrien un contenidor de 10 peus, però no un de més llarg.
   Per exemple, la filera de la figura 1 de l'enunciat contribuirà amb
   7 unitats a la fragmentació total (corresponen a les ubicacions
   <f, 0, 1>, <f, 1, 2>, <f, 2, 1>, <f, 7, 1>, <f, 8, 0>, <f, 9, 1> i
   <f, 10, 0>). */
nat terminal::fragmentacio() const throw(){
    
    nat contador = 0; 

    for (int fila = 0; fila < _num_fileres; fila++ ) 
    {
        for ( int placa = 0 ; placa < _num_places; placa++ )
        {
            for ( int piso = 0; piso < _num_pisos; piso++ )
            {
                if ( terr[fila][piso][placa].length() == 0 )
                {
                    contador++;
                    break;
                }
            }
        }
    }
    
    return contador;
}

/* Retorna el número d'operacions de grua realitzades des del moment
   de creació de la terminal.
   Es requereix d'una operació de grua per moure un contenidor
   des de l'àrea d'espera a l'àrea d'emmagatzematge o viceversa.
   També es requereix d'una operació de grua per inserir o
   retirar directament un contenidor de l'àrea d'emmagatzematge.
   En canvi no requereix cap operació de grua inserir o
   retirar directament un contenidor de l'àrea d'espera. */
nat terminal::ops_grua() const throw(){
    return _num_ops;
}

/* Retorna la llista de les matrícules de tots els contenidors
   de l'àrea d'espera de la terminal, en ordre alfabètic creixent. */
void terminal::area_espera(list<string> &l) const throw(){

    node_list *n = _aespera;
    while (n != NULL){
        l.push_back(n->value);
        n = n->next;
    }
}

/* Retorna el número de fileres de la terminal. */
nat terminal::num_fileres() const throw(){
    return _num_fileres;
}

/* Retorna el número de places per filera de la terminal. */
nat terminal::num_places() const throw(){
    return _num_places;
}

/* Retorna l'alçada màxima d'apilament de la terminal. */
nat terminal::num_pisos() const throw(){
    return _num_pisos;
}

/* Retorna l'estratègia d'inserció i retirada de contenidors de
   la terminal. */
terminal::estrategia terminal::quina_estrategia() const throw(){
    return _escollida;
} 


// insereix_first_fit ens diu on ha de anar el contenidor
// retorna 
// <-2, -2, -2> <- S'afegeix a area d'espera
// altre: s'afegeix al terminal
ubicacio terminal::insereix_first_fit (const contenidor &c){
    int filera, pis, placa;
    for (filera = 0; filera < _num_fileres; filera++){
        for (pis = 0; pis < _num_pisos; pis++){
            for (placa = 0; placa < _num_places; placa++){
                // estamos en alguna parte deberemos comprobar si hay espacio suficiente
                if (_num_places - placa >= c.longitud()/10){
                    if (terr[filera][pis][placa].length() == 0){

                        if (c.longitud() == 10){
                            terr[filera][pis][placa] = c.matricula();
                            return ubicacio(filera,placa,pis);
                        }
                    
                        if (c.longitud() == 20){
                            if (terr[filera][pis][placa+1].length() == 0)
                                terr[filera][pis][placa] = c.matricula();
                                terr[filera][pis][placa+1] = c.matricula();
                                return ubicacio(filera,placa,pis);
                        }

                        if (c.longitud() == 30){
                            if (terr[filera][pis][placa+1].length() == 0){
                                if (terr[filera][pis][placa+2].length() == 0)
                                terr[filera][pis][placa] = c.matricula();
                                terr[filera][pis][placa+1] = c.matricula();
                                terr[filera][pis][placa+2] = c.matricula();
                                    return ubicacio(filera,placa,pis);
                            }
                        }
            
                    }
                }
            }
        } 
    }

    return ubicacio(-1, 0, 0);
}

void terminal::retirar_area_espera(const string &m){
    node_list *ant = NULL;
    node_list *n = _aespera;

    bool deleted = false;
    while (n != NULL and deleted == false){
        if (ant == NULL and n ==_aespera and n->value == m){
            _aespera = n->next;
            delete n;
            deleted = true;  
        } 
        else if (ant != NULL and n->value == m){
            ant->next = n->next;
            delete n;
            deleted = true;  
        }
        else if (ant != NULL and n->next == NULL){
            ant->next = NULL;
            delete n;
            deleted = true;
        }


        if (not deleted){
            ant = n;
            n = n->next;   
        }

    }
}

void terminal::afegir_area_espera(const string &m){
    node_list *ant = NULL;
    node_list *n = _aespera;

    node_list *nn = new node_list;
    nn->value = m;
    nn->next = NULL;


    // Al principio
    if (_aespera == NULL){
        _aespera = nn;
    } else {

        bool added = false;
        while (n != NULL and added == false){

            if (ant == NULL and n->value > m){
                nn->next = n;
                _aespera = nn;
                added = true;
            } else if (n->value > m){
                nn->next = ant->next;
                ant->next = nn;
                added = true;
            } else if (n->next == NULL){
                n->next = nn;
                added = true;
            }

            if (not added){
                ant=n;
                n = n->next;
            }
        }
    }
}

void terminal::retirar_first_fit(ubicacio &u){
    string matricula;
    contenidor_ocupa(u, matricula);
    
    our_pair p = _cataleg->operator[](matricula);
    contenidor c = *p.c;

    int contador = 0;
    for (int pis = u.pis()+1; pis < _num_pisos; pis++){
        for (int placa=u.placa(); placa < c.longitud()/10; placa++){
            if (terr[u.filera()][pis][placa].length() == 0){
                contador++;
                if (contador == c.longitud()/10){
                    if (c.longitud()/10 >= 1){
                        terr[u.filera()][pis][placa] = "";
                    } 
                    if (c.longitud()/10 >= 2){
                        terr[u.filera()][pis][placa-1] = "";
                    }
                    if (c.longitud()/10 >= 3){
                        terr[u.filera()][pis][placa-2] = "";
                    }
                    break;
                }
            } else {
                ubicacio au = ubicacio(u.filera(), placa, pis);
                retirar_first_fit(au);
            }
        }
    }
    

}


