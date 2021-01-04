#include "cataleg.hpp"

/* Constructora. Crea un catàleg buit on numelems és el nombre
   aproximat d'elements que com a màxim s'inseriran al catàleg. */
template <typename Valor>
cataleg<Valor>::cataleg(nat numelems) throw(error){
    _M = numelems;
    _excedents = numelems * 0.14;
    
    _taula = new node_hash*[_M+_excedents]; 
}

/* Constructora per còpia, assignació i destructora. */
template <typename Valor>
cataleg<Valor>::cataleg(const cataleg& c) throw(error){
    _M = c._M;
    _excedents = c._excedents;
    _quants = c._quants;

    _taula = c._taula; // TODO: REFACTOR TO DO A COPY 
}

template <typename Valor>
cataleg<Valor>& cataleg<Valor>::operator=(const cataleg& c) throw(error){
    _M = c._M;
    _excedents = c._excedents;
    _quants = c._quants;

    _taula = c._taula; // TODO: REFACTOR TO DO A COPY 
    return *(this);
}

template <typename Valor>
cataleg<Valor>::~cataleg() throw(){

}

/* Mètode modificador. Insereix el parell <clau, valor> indicat.
   En cas que la clau k ja existeixi en el catàleg actualitza el valor
   associat. Genera un error en cas que la clau sigui l'string buit. */
template <typename Valor>
void cataleg<Valor>::assig(const string &k, const Valor &v) throw(error){

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
    return false;
}

/* Retorna el valor associat a la clau k; si no existeix cap parell amb
   clau k llavors genera un error. Exemple:
     cataleg<int> ct;
     ...
     int n = ct["dia"]; */
template <typename Valor>
Valor cataleg<Valor>::operator[](const string &k) const throw(error){
    return 1;
}

/* Retorna el nombre d'elements que s'han inserit en el catàleg
   fins aquest moment. */
template <typename Valor>
nat cataleg<Valor>::quants() const throw(){
    return _quants;
}

template <typename Valor>
int cataleg<Valor>::hash(const string &k) throw() {
    int n = 0;
    for (int i=0; i < k.length(); ++i) {
        n = n + k[i]*i; // a n sumen el codi ascii
    }
    return n % _M;
}
