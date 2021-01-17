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
    _cataleg = new cataleg<contenidor_ubicacio>(3);
    
    _terminal = new string **[_num_fileres]();
    for (nat i = 0; i < _num_fileres; i++)
    {
        _terminal[i] = new string *[_num_pisos]();
        for (nat j = 0; j < _num_pisos; j++)
            _terminal[i][j] = new string [_num_places]();
    }


}

/* Constructora per còpia, assignació i destructora. */
terminal::terminal(const terminal& b) throw(error){
    _num_fileres = b._num_fileres;
    _num_pisos = b._num_pisos;
    _num_places = b._num_places;
    _num_ops = b._num_ops;
    _escollida = b._escollida;
    _cataleg = new cataleg<contenidor_ubicacio>(*b._cataleg);
}

terminal& terminal::operator=(const terminal& b) throw(error){
    _num_fileres = b._num_fileres;
    _num_pisos = b._num_pisos;
    _num_places = b._num_places;
    _num_ops = b._num_ops;
    _escollida = b._escollida;
    _cataleg = new cataleg<contenidor_ubicacio>(*b._cataleg);
    return *this;
}

/* 
    Destructora del terminal
    
    S'encarregara de destruir el _terminal (totes les seves posicions),
    Esborrara l'area d'espera i els seus nodes i es cridara la destructora de
    cataleg
*/
terminal::~terminal() throw(){

    // Destroy _terminal
    for (nat i = 0; i < _num_fileres; i++)
    {
        for (nat j = 0; j < _num_pisos; j++)
            delete[] _terminal[i][j];
        delete[] _terminal[i];
    }
    delete[] _terminal;
    
    node_list *n = _aespera;
    while (n != NULL){
        node_list *to_delete = n;
        n = n->next;
        delete to_delete;
    }

    delete _cataleg;
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
    if (_cataleg->existeix(c.matricula())){
        throw error(MatriculaDuplicada);
    }

    contenidor_ubicacio op;
    op._contenidor = new contenidor(c);


    if (_escollida == FIRST_FIT){
        ubicacio u = insereix_first_fit(c);

        // Afegim al cataleg
        op._ubicacio = new ubicacio(u);
        _cataleg->assig(c.matricula(), op); // throws ClauStringBuit

        if (u == ubicacio(-1,0,0)){
            // Afegir area de espera
            afegir_area_espera(c.matricula());
        } else {
            // S'ha afegit correctament
            // Reinsertar area_espera
            _num_ops++;
            reinsertar_area_espera();
        }
    } 

    if (_escollida == LLIURE) {
        op._ubicacio = new ubicacio(0,0,0);
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
    
    if (_escollida == FIRST_FIT){
        ubicacio u = on(m);
        if (u == ubicacio(-1,-1,-1)){
            throw error(MatriculaInexistent); 
        } else if (u == ubicacio(-1, 0, 0)){
            // Esta al area de espera 
            retirar_area_espera(m);
            _cataleg->elimina(m);
        } else {
            _num_ops++;
            retirar_first_fit(u, false); 
            // Inserir area espera
            reinsertar_area_espera();
        }
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
        contenidor_ubicacio p = _cataleg->operator[](m);
        return *p._ubicacio;

    } catch (error(ClauInexistent)) {
        return ubicacio(-1, -1, -1);
    }

}

/* Retorna la longitud del contenidor la matrícula del qual és igual
   a m. Genera un error si no existeix un contenidor a la terminal
   la matrícula del qual sigui igual a m. */
nat terminal::longitud(const string &m) const throw(error){
    try {
        contenidor_ubicacio p = _cataleg->operator[](m);
        return p._contenidor->longitud();
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

    m = _terminal[u.filera()][u.pis()][u.placa()];
}  

/* Retorna el nombre de places de la terminal que en aquest instant
   només hi cabrien un contenidor de 10 peus, però no un de més llarg.
   Per exemple, la filera de la figura 1 de l'enunciat contribuirà amb
   7 unitats a la fragmentació total (corresponen a les ubicacions
   <f, 0, 1>, <f, 1, 2>, <f, 2, 1>, <f, 7, 1>, <f, 8, 0>, <f, 9, 1> i
   <f, 10, 0>). */
nat terminal::fragmentacio() const throw(){
    
    nat contador = 0; 
    

    for (nat fila = 0; fila < _num_fileres; fila++ ) 
    {
        for ( nat placa = 0 ; placa < _num_places; placa++ )
        {
            for ( nat piso = 0; piso < _num_pisos; piso++ )
            {
                if ( _num_pisos == 1 and _num_fileres == 1 and _num_places == 1 and _terminal[fila][piso][placa].length() == 0 )
                {
                    contador = 1;
                    return contador;
                    break;
                }
                
                if ( _terminal[fila][piso][placa].length() == 0 )
                {
                    if (placa == 0){
                        if (piso > 0 and _terminal[fila][piso][placa+1].length() == 0 and _terminal[fila][piso-1][placa+1].length() == 0 and _terminal[fila][piso-1][placa].length() != 0){ //tengo que mirar también si tengo algo debajo, porque si contamos las vacías sin nada debajo nos salen 35
                            contador++;
                        } else if ( (piso == 0 and _terminal[fila][piso][placa+1].length() != 0) or ( piso > 0 and _terminal[fila][piso-1][placa].length() != 0) and _terminal[fila][piso][placa+1].length() != 0) contador++;

                    } else if (placa == _num_places-1){
                        if (piso > 0 and _terminal[fila][piso][placa-1].length() == 0 and _terminal[fila][piso-1][placa-1].length() == 0 and _terminal[fila][piso-1][placa].length() != 0){
                            contador++;
                        } else if ( (piso == 0 and _terminal[fila][piso][placa-1].length() != 0) or ( piso > 0 and _terminal[fila][piso-1][placa].length() != 0 and _terminal[fila][piso][placa-1].length() != 0)) contador++;

                    } else {
                        if ( piso == 0 and _terminal[fila][piso][placa-1].length() != 0 and _terminal[fila][piso][placa+1].length() != 0){
                            contador++;
                        }else if ( piso > 0 ){
                            
                            if (_terminal[fila][piso-1][placa].length() != 0){
                                //si tinc algo a sota i compleixo la resta de condicions comptaré, si no tinc res puc ser una posició "flotant"

                                if ( _terminal[fila][piso][placa-1].length() != 0 and _terminal[fila][piso-1][placa+1].length() == 0){
                                    contador++;
                                } else if ( _terminal[fila][piso][placa+1].length() != 0 and _terminal[fila][piso-1][placa-1].length() == 0){
                                    contador++;
                                } else if ( _terminal[fila][piso][placa-1].length() != 0 and _terminal[fila][piso][placa+1].length() == 0 and _terminal[fila][piso-1][placa+1].length() == 0){
                                    contador++;
                                } else if ( _terminal[fila][piso][placa+1].length() != 0 and _terminal[fila][piso][placa-1].length() == 0 and _terminal[fila][piso-1][placa-1].length() == 0){
                                    contador++;
                                } else if ( _terminal[fila][piso][placa-1].length() != 0 and _terminal[fila][piso][placa+1].length() != 0 ){
                                    contador++;
                                } else if ( _terminal[fila][piso-1][placa-1].length() == 0 and _terminal[fila][piso-1][placa+1].length() == 0 ){
                                    contador++;
                                }
                            }
                        }
                    } 
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
   de l'àrea d'espera de la terminal, en ordre alfabètic creixent. 

    Pre: l es una llista buida
    Post: l te totes les matricules de l'area d'espera en ordre alfabetic
    O(n) on n son nodes en l'area de espera
*/
void terminal::area_espera(list<string> &l) const throw(){

    node_list *n = _aespera;
    while (n != NULL){
        list<string>::iterator it = l.begin();

        if (l.begin() == l.end()){
            l.push_back(n->matricula);
        } else if (n->matricula < *l.begin()){
            l.insert(l.begin(), n->matricula);
        } else {
            bool added = false;
            while (it != l.end()){
                //std::cout << n->matricula << " < " << *it << std::endl;
                if (n->matricula < *it){
                    l.insert(it, n->matricula);
                    added = true;
                    break;
                }
                ++it;
            }

            if (not added) l.push_back(n->matricula);
        }
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


/**
    El metode d'insercio first intentara sempre colocar els contenidors als primers pisos i a les primeres places.
    
    Pre: c es un contenidor valid
    Post: El terminal sera escrit per la matricula del contenidor c en
            cas de que hi hagui un lloc lliure al terminal de ser aixi retornara la ubicacio nova.

            En el cas de no tenir espai es retornara la ubicacio de l'area de espera (-1, 0, 0)
    
    Cost: O(n^3)
**/
ubicacio terminal::insereix_first_fit (const contenidor &c){
    nat filera, pis, placa;
    for (filera = 0; filera < _num_fileres; filera++){
        for (placa = 0; placa < _num_places; placa++){
            for (pis = 0; pis < _num_pisos; pis++){
                // estamos en alguna parte deberemos comprobar si hay espacio suficiente
                if (_num_places - placa >= c.longitud()/10){

                    //si estamos en el suelo palante
                    //si estamos en algun piso comprobar si abajo hay algo
                    //si estamos en algun piso comprobar si hay algo
                    if (_terminal[filera][pis][placa].length() == 0){ // Estamos en un sitio donde almenos hay 1 espacio
                        //std::cout << "pis: " << pis << "  placa: " << placa << std::endl;
                        if (c.longitud() == 10){ // Si nuestro contenedor es de 1 espacio lo almacenamos
                            _terminal[filera][pis][placa] = c.matricula();
                            return ubicacio(filera,placa,pis);
                        }
                    
                        if (c.longitud() == 20){ // Si nuestro contenedor es de 2 espacios deberemos comprobar si nuestra casilla adyacente es valida
                            if (_terminal[filera][pis][placa+1].length() == 0){ // Hay espacio hacia la izquierda
                                if (pis == 0 or _terminal[filera][pis-1][placa].length() != 0 and _terminal[filera][pis-1][placa+1].length() != 0){ // Comprobamos que abajo hay suelo o un contenedor
                                    _terminal[filera][pis][placa] = c.matricula();
                                    _terminal[filera][pis][placa+1] = c.matricula();
                                    return ubicacio(filera,placa,pis);
                                }
                            }
                        }

                        if (c.longitud() == 30){
                            if (_terminal[filera][pis][placa+1].length() == 0 and _terminal[filera][pis][placa+2].length() == 0){ 
                                // Hay 2 espacios hacia la izquierda
                                if (pis == 0 or (_terminal[filera][pis-1][placa].length() != 0 and _terminal[filera][pis-1][placa+1].length() != 0 and _terminal[filera][pis-1][placa+2].length() != 0)){ 
                                    // Si en esos espacios hay suelo
                                    _terminal[filera][pis][placa] = c.matricula();
                                    _terminal[filera][pis][placa+1] = c.matricula();
                                    _terminal[filera][pis][placa+2] = c.matricula();
                                    return ubicacio(filera,placa,pis);
                                }
                            }
                        }
            
                    }
                }
            }
        } 
    }

    return ubicacio(-1, 0, 0);
}


/**
    El metode d'insercio lliure intentara sempre colocar els contenidors a els pisos mes baixos.
    
    Hem escollit aquest metode per la seva sencilleça.

    Les alternatives que varem pensar eren:
            * colocar per primer fileres - mateixa complexitat que fer-ho per pisos
            * afegir-los per tamany - pero fent servir aquest tipus es deperdiciaba molta terminal.

    Pre: c es un contenidor valid
    Post: El terminal sera escrit per la matricula del contenidor c en
            cas de que hi hagui un lloc lliure al terminal de ser aixi retornara la ubicacio nova.

            En el cas de no tenir espai es retornara la ubicacio de l'area de espera (-1, 0, 0)
    
    Cost: O(n^3)
**/
ubicacio terminal::insereix_lliure (const contenidor &c){
    nat filera, pis, placa;
    for (filera = 0; filera < _num_fileres; filera++){
        for (pis = 0; pis < _num_pisos; pis++){
            for (placa = 0; placa < _num_places; placa++){
                // estamos en alguna parte deberemos comprobar si hay espacio suficiente
                if (_num_places - placa >= c.longitud()/10){

                    //si estamos en el suelo palante
                    //si estamos en algun piso comprobar si abajo hay algo
                    //si estamos en algun piso comprobar si hay algo
                    if (_terminal[filera][pis][placa].length() == 0){ // Estamos en un sitio donde almenos hay 1 espacio
                        //std::cout << "pis: " << pis << "  placa: " << placa << std::endl;
                        if (c.longitud() == 10){ // Si nuestro contenedor es de 1 espacio lo almacenamos
                            _terminal[filera][pis][placa] = c.matricula();
                            return ubicacio(filera,placa,pis);
                        }
                    
                        if (c.longitud() == 20){ // Si nuestro contenedor es de 2 espacios deberemos comprobar si nuestra casilla adyacente es valida
                            if (_terminal[filera][pis][placa+1].length() == 0){ // Hay espacio hacia la izquierda
                                if (pis == 0 or _terminal[filera][pis-1][placa].length() != 0 and _terminal[filera][pis-1][placa+1].length() != 0){ // Comprobamos que abajo hay suelo o un contenedor
                                    _terminal[filera][pis][placa] = c.matricula();
                                    _terminal[filera][pis][placa+1] = c.matricula();
                                    return ubicacio(filera,placa,pis);
                                }
                            }
                        }

                        if (c.longitud() == 30){
                            if (_terminal[filera][pis][placa+1].length() == 0 and _terminal[filera][pis][placa+2].length() == 0){ 
                                // Hay 2 espacios hacia la izquierda
                                if (pis == 0 or (_terminal[filera][pis-1][placa].length() != 0 and _terminal[filera][pis-1][placa+1].length() != 0 and _terminal[filera][pis-1][placa+2].length() != 0)){ 
                                    // Si en esos espacios hay suelo
                                    _terminal[filera][pis][placa] = c.matricula();
                                    _terminal[filera][pis][placa+1] = c.matricula();
                                    _terminal[filera][pis][placa+2] = c.matricula();
                                    return ubicacio(filera,placa,pis);
                                }
                            }
                        }
            
                    }
                }
            }
        } 
    }

    return ubicacio(-1, 0, 0);
}

/*
    Pre: m=Matricula
    Post: L'area de espera no contindra Matricula

    Cost: O(n) on n es el nombre de contenidors a l'area de espera. 
*/
void terminal::retirar_area_espera(const string &m){
    node_list *ant = NULL;
    node_list *n = _aespera;

    bool deleted = false;
    while (n != NULL and deleted == false){
        if (ant == NULL and n ==_aespera and n->matricula == m){
            _aespera = n->next;
            delete n;
            deleted = true;  
        } 
        else if (ant != NULL and n->matricula == m){
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

/**
    Pre: m=M
    Post: m es al principi de l'area d'espera
    Cost: O(1)
**/
void terminal::afegir_area_espera(const string &m){
    node_list *nn = new node_list;
    nn->matricula = m;
    nn->next = NULL;

    // Al principio
    if (_aespera == NULL){
        _aespera = nn;
    } else {
        nn->next = _aespera;
        _aespera = nn;
    }
}

/**
    Pre: ubi es una ubicacio valida
    Post: elimina el contenidor que estigui a ubi, pasa a l'area d'espera tots 
            els contenidor que siguin a sobre i intentara reinsertar els contenidors
            de l'area d'espera a la terminal.
**/
void terminal::retirar_first_fit(ubicacio &ubi, bool area_espera){
    string matricula;
    contenidor_ocupa(ubi, matricula);

    contenidor_ubicacio p = _cataleg->operator[](matricula);
    contenidor c = *p._contenidor;
    ubicacio u = *p._ubicacio;

    nat contador = 0;
    for (nat pis = u.pis(); pis < _num_pisos; pis++){

        for (nat placa=u.placa(); placa < u.placa()+c.longitud()/10; placa++){
            // Som amunt de tot o no tenim res adalt
            if (pis == _num_pisos-1 or _terminal[u.filera()][pis+1][placa].length() == 0){
                contador++;
                if (contador == c.longitud()/10){
                    if (c.longitud() >= 10){
                        _terminal[u.filera()][pis][placa] = "";
                    } 
                    if (c.longitud() >= 20){
                        _terminal[u.filera()][pis][placa-1] = "";
                    }
                    if (c.longitud() >= 30){
                        _terminal[u.filera()][pis][placa-2] = "";
                    }
                
                    contenidor_ubicacio p = _cataleg->operator[](matricula);
                    if (area_espera){
                        p._ubicacio = new ubicacio(-1,0,0);
                        _num_ops++;
                        afegir_area_espera(matricula);
                    } else {
                        _cataleg->elimina(matricula);
                    }
                    break;
                }
            } else {
                ubicacio au = ubicacio(u.filera(), placa, pis+1);
                retirar_first_fit(au, true);
                placa--;
            }
        }
    }
    

}

/*
    Recorrerà l’àrea d’espera, començant pel que es va inserir en últim lloc en l’àrea d’espera, 
    en busca d’un contenidor que pugui ser mogut a l’àrea d’emmagatzematge. Si la cerca té èxit 
    es moura el contenidor en qüestió des de l’àrea d’espera a la d’emmagatzematge. Aquest procés
    es repeteix fins que l’àrea d’espera s’ha examinat completament sense que hi hagi cap
    contenidor susceptible de ser mogut a l’àrea d’emmagatzematge. 

    Pre: buit
    Post: S'elimina de l'area de espera tots els contenidors que hagin sigut susceptibles de 
            ser moguts a l'area d'emmagatzematge

*/
void terminal::reinsertar_area_espera() 
{
    node_list *n = _aespera;
    while (n != NULL){
        contenidor_ubicacio p = _cataleg->operator[](n->matricula);
        contenidor pc = *p._contenidor;
        ubicacio pu(-1,0,0);
        if (_escollida == FIRST_FIT)
            pu = insereix_first_fit(pc);
        else 
            pu = insereix_lliure(pc);
         
        if (pu != ubicacio(-1,0,0)){ // Area espera -> area magatzem
            _num_ops++;
            p._ubicacio = new ubicacio(pu); 

            _cataleg->assig(n->matricula, p);

            retirar_area_espera(n->matricula);
            // Com hem eliminat un contenidor de l'area d'espera tornem a començar per si un cas.
            n = _aespera;
        } else  {
            n = n->next;
        }
    }
}

