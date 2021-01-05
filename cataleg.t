#include "cataleg.hpp"

/* Constructora. Crea un catàleg buit on numelems és el nombre
   aproximat d'elements que com a màxim s'inseriran al catàleg. */
template <typename Valor>
cataleg<Valor>::cataleg(nat numelems) throw(error){
    
    if (numelems < 13 ) 
        _M = 13;
    else
        _M = numelems;

    _excedents = _M * 0.14;
    
    // TODO: comprovar si _M + _excedents es lo que queremos
    _taula = new node_hash[_M+_excedents]; 

    for (int i=0; i < _M+_excedents; i++){
        node_hash *n = new node_hash;
        n->_ss = FREE;
        n->_k = "-5";
        n->_v = -5;
        n->next = -1;
    }
    _quants = 0;
}

/* Constructora per còpia, assignació i destructora. */
template <typename Valor>
cataleg<Valor>::cataleg(const cataleg& c) throw(error){
    _M = c._M;
    _excedents = c._excedents;
    _quants = c._quants;

    for (int i=0; i < _M+_excedents; i++){
        node_hash *nou = new node_hash;
        node_hash *antic = &c._taula[i];
        nou->_ss = antic->_ss;
        nou->_k = antic->_k;
        nou->_v = antic->_v;
        nou->next = antic->next;
    }
    
}

template <typename Valor>
cataleg<Valor>& cataleg<Valor>::operator=(const cataleg& c) throw(error){
    _M = c._M;
    _excedents = c._excedents;
    _quants = c._quants;

    for (int i=0; i < _M+_excedents; i++){
        node_hash *nou = new node_hash;
        node_hash *antic = &c._taula[i];
        nou->_ss = antic->_ss;
        nou->_k = antic->_k;
        nou->_v = antic->_v;
        nou->next = antic->next;
    }
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

    std::cout << key << std::endl;
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
                //Trobar el ultim node on el hash es igual
                while (n->next != -1){
                    n = &_taula[n->next]; 
                }

                // Busquem una posicio lliure a zona d'exedents
                for (nat i = _M; i < _excedents; i++){

                    node_hash *nou = &_taula[i];
                    if (nou->_ss != BUSY){
                        nou->_ss = BUSY; 
                        nou->_k = k; 
                        nou->_v = v; 
                        n->next = i;
                        _quants++;
                        break;
                    }
                }
                //TODO: QUE LA PASTA SI NO TENEMOS ESPACIO: REDIMENSIONAAAAAR.....
            }
            break;

            default:
                std::cout << "NE MAL NE MAL" << std::endl;
                break;
    }
}

/* Elimina del catàleg el parell que té com a clau k.
   En cas que la clau k no existeixi en el catàleg genera un error. */
template <typename Valor>
void cataleg<Valor>::elimina(const string &k) throw(error){
    
    int key = hash(k);
    
    bool found = false;
    node_hash *node = &_taula[key];
    
    if (node->_ss == BUSY){
 
        if (node->_k == k) {
            found = true; // if the first case is out k
            node->_ss = DELETED;
        } else {
            // if there other node_hash 
            while (node->next != -1 and found == false){
                node = &_taula[node->next];
                if (node->_ss == BUSY)
                    if (node->_k == k){
                        node->_ss = DELETED;
                        found = true;
                    }
            }
        }
    }

    if (not found) throw error(ClauInexistent);
    else _quants--;
}

/* Retorna true si i només si la clau k existeix dins del catàleg; false
   en cas contrari. */
template <typename Valor>
bool cataleg<Valor>::existeix(const string &k) const throw(){
    
    int key = hash(k);
    
    bool found = false;
    node_hash *node = &_taula[key];
    
    if (node->_ss == BUSY){
 
        if (node->_k == k) {
            found = true; // if the first case is out k
        } else {
            // if there other node_hash 
            while (node->next != -1 and found == false){
                node=&_taula[node->next];
                
                if (node->_ss == BUSY)
                    if (node->_k == k){
                        found = true;
                    }
            }
        }
    }

    return found;
}

/* Retorna el valor associat a la clau k; si no existeix cap parell amb
   clau k llavors genera un error. Exemple:
     cataleg<int> ct;
     ...
     int n = ct["dia"]; */
template <typename Valor>
Valor cataleg<Valor>::operator[](const string &k) const throw(error){
    
    int key = hash(k);
    
    node_hash *node = &_taula[key];
    
    if (node->_ss == BUSY){
 
        if (node->_k == k) {
            return node->_v;
        } else {
            // if there other node_hash 
            while (node->next != -1){
                node = &_taula[node->next];
                if (node->_ss == BUSY)
                    if (node->_k == k){
                        return node->_v;
                    }
            }
        }
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
    if (_M > 0){
        int n = 0;
        for (int i=0; i < k.length(); ++i) {
            n = n + k[i]*i; // a n sumen el codi ascii
        }
        return n % _M;
    } else {
        return 0;
    }
}
