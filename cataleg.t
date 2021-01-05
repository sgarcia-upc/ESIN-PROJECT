#include "cataleg.hpp"

/* Constructora. Crea un catàleg buit on numelems és el nombre
   aproximat d'elements que com a màxim s'inseriran al catàleg. */
template <typename Valor>
cataleg<Valor>::cataleg(nat numelems) throw(error){
    _M = numelems;
    _excedents = numelems * 0.14;
    
    // TODO: comprovar si _M + _excedents es lo que queremos
    _taula = new node_hash[_M+_excedents]; 

    for (int i=0; i < _M+_excedents; i++){
        node_hash *n = new node_hash;
        n->_ss = FREE;
        n->next = NULL;
    }
    _quants = 0;
}

/* Constructora per còpia, assignació i destructora. */
template <typename Valor>
cataleg<Valor>::cataleg(const cataleg& c) throw(error){
    _M = c._M;
    _excedents = c._excedents;
    _quants = c._quants;

    //_taula = c._taula; // TODO: REFACTOR TO DO A COPY 
}

template <typename Valor>
cataleg<Valor>& cataleg<Valor>::operator=(const cataleg& c) throw(error){
    _M = c._M;
    _excedents = c._excedents;
    _quants = c._quants;

    //_taula = c._taula; // TODO: REFACTOR TO DO A COPY 
    return *(this);
}

template <typename Valor>
cataleg<Valor>::~cataleg() throw(){

    delete [] _taula;
}

/* Mètode modificador. Insereix el parell <clau, valor> indicat.
   En cas que la clau k ja existeixi en el catàleg actualitza el valor
   associat. Genera un error en cas que la clau sigui l'string buit. */
template <typename Valor>
void cataleg<Valor>::assig(const string &k, const Valor &v) throw(error){
    if ( k.length() == 0 ) throw error(ClauStringBuit);

    int key = hash(k);

    node_hash *n = &_taula[key];

    switch (n->_ss)  {
        case FREE:
        case DELETED:
            n->_ss = BUSY; 
            n->_k = k; 
            n->_v = v; 
            _quants++;
            break;
    
        case BUSY:
            if (n->_k == k){
                // Si es la mateixa matricula, actualizem el valor
                n->_v = v; 
            } else {
                // Si no es la mateixa matricula, l'afegim
                node_hash *node = n;

                //Trobar el ultim node on el hash es igual
                while (node->next != NULL){
                    node = node->next; 
                }

                // Busquem una posicio lliure a zona d'exedents
                for (nat i = _M; i < _excedents; i++){
                    n = &_taula[i];
                    if (n->_ss != BUSY){
                        n->_ss = BUSY; 
                        n->_k = k; 
                        n->_v = v; 
                        node->next = n;
                        _quants++;
                        break;
                    }
                }
                //TODO: QUE LA PASTA SI NO TENEMOS ESPACIO: REDIMENSIONAAAAAR.....
            }
            break;
    }
}

/* Elimina del catàleg el parell que té com a clau k.
   En cas que la clau k no existeixi en el catàleg genera un error. */
template <typename Valor>
void cataleg<Valor>::elimina(const string &k) throw(error){
    
}

/* Retorna true si i només si la clau k existeix dins del catàleg; false
   en cas contrari. */
template <typename Valor>
bool cataleg<Valor>::existeix(const string &k) const throw(){
    
    for (int i = 0; i < _M+_excedents; i++){
        node_hash *node = &_taula[i];
        if (node->_ss == BUSY and node->_k == k)
            return true;
    }
    //int key = hash(k);
    
    bool found = false;
    //node_hash *node = &_taula[key];
    //
    //if (node->_ss == BUSY){
 
    //    if (node->_k == k) {
    //        found = true; // If the first case is out k
    //    } else {
    //        // If there other node_hash 
    //        while (node->next != NULL and found == false){
    //            node=node->next;
    //            if (node->_k == k){
    //                found = true;
    //            }
    //        }
    //    }
    //}

    return found;
}

/* Retorna el valor associat a la clau k; si no existeix cap parell amb
   clau k llavors genera un error. Exemple:
     cataleg<int> ct;
     ...
     int n = ct["dia"]; */
template <typename Valor>
Valor cataleg<Valor>::operator[](const string &k) const throw(error){
    for (int i = 0; i < _M+_excedents; i++){
        node_hash *node = &_taula[i];
        if (node->_ss == BUSY and node->_k == k)
            return node->_v;
    }

    throw error(ClauInexistent);
}

/* Retorna el nombre d'elements que s'han inserit en el catàleg
   fins aquest moment. */
template <typename Valor>
nat cataleg<Valor>::quants() const throw(){
    return _quants;
}

template <typename Valor>
int cataleg<Valor>::hash(const string &k) const throw() {
    int n = 0;
    for (int i=0; i < k.length(); ++i) {
        n = n + k[i]*i; // a n sumen el codi ascii
    }
    return n % _M;
}
