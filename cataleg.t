#include "cataleg.hpp"

/* Constructora. Crea un catàleg buit on numelems és el nombre
   aproximat d'elements que com a màxim s'inseriran al catàleg. */
template <typename Valor>
cataleg<Valor>::cataleg(nat numelems) throw(error){

    if (numelems == 0) numelems = 1;

    _M = next_prime(numelems);

    _ex = 0;

    float ex = _M * 0.14;
    if (ex == (int) ex) _excedents = (int)ex;
    else _excedents = (int) (ex+1);

    _taula = new node_hash[_M+_excedents];
    //std::cout << "PRIMER: " << _M << std::endl;
    //std::cout << _M+_excedents << std::endl;

    for (int i=0; i < _M+_excedents; i++){
        node_hash *n = &_taula[i];
        n->_ss = FREE;
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

    _taula = new node_hash[_M+_excedents];

    for (int i=0; i < _M+_excedents; i++){
        node_hash *nou = &_taula[i];
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
    _taula = new node_hash[_M+_excedents];

    for (int i=0; i < _M+_excedents; i++){
        node_hash *nou = &_taula[i];
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

    //for (int i=0; i<_M+_excedents; i++){
    //    delete &_taula[i];
    //}
    delete [] _taula;
}

/* Mètode modificador. Insereix el parell <clau, valor> indicat.
   En cas que la clau k ja existeixi en el catàleg actualitza el valor
   associat. Genera un error en cas que la clau sigui l'string buit. */
template <typename Valor>
void cataleg<Valor>::assig(const string &k, const Valor &v) throw(error){
    if ( k.length() == 0 ) throw error(ClauStringBuit);

    float load_factor = _quants / _M;
    if (load_factor >= 0.75)
       rehash();

    int key = hash(k);
    //std::cout << "CATALEG::ASSIG " << k << " - " << hash(k) << " " << std::endl;

    node_hash *n = &_taula[key];
    node_hash *if_nothing = NULL;

    switch (n->_ss)  {
        case FREE:
            n->_ss = BUSY;
            n->_k = k;
            n->_v = v;
            _quants++;

        case DELETED:
            if_nothing = n;
        case BUSY:
            if (n->_k == k){
                // Si es la mateixa matricula, actualizem el valor
                n->_v = v;
            } else {
                _ex++;
                // Si no es la mateixa matricula, l'afegim
                // Trobar el ultim node on el hash es igual
                bool added = false;
                while (n->next != -1 and not added){ // DELETED ??
                    n = &_taula[n->next];
                    if (n->_ss == BUSY and n->_k == k){
                        n->_v = v;
                        added = true;
                    }
                    if (if_nothing == NULL and n->_ss == DELETED){
                        if_nothing = n;
                    }
                }

                if (not added){
                    if (if_nothing != NULL){
                        if_nothing->_k = k;
                        if_nothing->_v = v;
                        if_nothing->_ss = BUSY;
                        _quants++;
                    } else { 
                        // Busquem una posicio lliure a zona d'exedents
                        for (nat i = _M; i < _M+_excedents; i++){
                            node_hash *nou = &_taula[i];
                            if (nou->_ss == FREE){
                                nou->_ss = BUSY;
                                nou->_k = k;
                                nou->_v = v;
                                n->next = i;
                                _quants++;
                                break;
                            }
                        }
                    }
                }
            }
            break;

            default:
                break;
    }
}

/* Elimina del catàleg el parell que té com a clau k.
   En cas que la clau k no existeixi en el catàleg genera un error. */
template <typename Valor>
void cataleg<Valor>::elimina(const string &k) throw(error){

    //std::cout << "CATALEG::ELIMINA " << k << " hash:" << hash(k) << std::endl;
    int key = hash(k);

    bool found = false;
    node_hash *node = &_taula[key];

    //std::cout << "Clau: " << k << " ?? " << node->_k << " estado: " << node->_ss << std::endl;
    if (node->_ss != FREE){

        if (node->_k == k) {
            //std::cout << "<--- deleted" << std::endl;
            found = true; // if the first case is our k
            node->_ss = DELETED;
        } else {
            // if there other node_hash
            while (node->next != -1 and found == false){
                node = &_taula[node->next];
                //std::cout << "Clau: " << k << " ?? " << node->_k << " estado: " << node->_ss << std::endl;
                if (node->_ss == BUSY)
                    if (node->_k == k){
                        //std::cout << "<--- deleted" << std::endl;
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

    if (node->_ss != FREE){

        if (node->_k == k and node->_ss == BUSY) {
            found = true; // if the first case is out k
        } else {
            // if there other node_hash
            while (node->next != -1 and found == false){
                node=&_taula[node->next];

                if (node->_k == k and node->_ss == BUSY) {
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

    if (node->_ss != FREE){ // debe entrar si es busy o deleted

        if (node->_k == k and node->_ss == BUSY) { 
            return node->_v;
        } 

        if (node->_k != k){
            while (node->next != -1){
                node = &_taula[node->next];
                if (node->_k == k and node->_ss == BUSY) { 
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
    //std::cout << _M << std::endl;
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

template <typename Valor>
float cataleg<Valor>::sqroot(const int m) const {
    float i=0;
    float x1,x2;
    while( (i*i) <= m )
            i+=0.1;
    x1=i;
    for(int j=0;j<10;j++)
    {
        x2=m;
        x2/=x1;
        x2+=x1;
        x2/=2;
        x1=x2;
    }

    return x2;
}

/*
    Pre: num es un numero enter positiu
    Post: restorna num si es primer o el seguent primer trobat de num. Sigui per a num=8 retornara=11
*/
template <typename Valor>
int cataleg<Valor>::next_prime(int num) const {

    if (num == 1) return 2; // auto cast num 1

    while (is_prime(num) != true){
        if (num % 2 == 0)
            num++;
        else
            num=num+2;
    }
    return num;
}

/*
    Pre: num es un numero enter positiu
    Post: retorna true si num es un numero primer, fals en cas contrari
*/
template <typename Valor>
bool cataleg<Valor>::is_prime(const int num) const {

    if (num < 2)  return false;
    if (num == 2) return true;

    float sqrt = sqroot(num);
    if (sqrt != (int) sqrt) sqrt = (int) (sqrt+1); // Si hi ha decimals cap amunt

    for (int i=2; i <= sqrt; i++){
        if (num % i == 0){
            return false;
        }
    }
    return true;
}

template <typename Valor>
void cataleg<Valor>::rehash() {

    int m = _M;
    int e = _excedents;

    _M = next_prime(_M*2); // taula minimament el doble de gran
    float ex = m * 0.14;     // Excedents

    if (ex == (int) ex) _excedents = (int)ex;
    else _excedents = (int) (ex+1);

    node_hash *antic = _taula;
    _taula = new node_hash[_M+_excedents];

    for (int i=0; i < _M+_excedents; i++){
        node_hash *n = &_taula[i];
        n->_ss = FREE;
        n->next = -1;
    }
    _quants = 0;

    for (int i=0; i< m+e; i++){
        if (antic[i]._ss == BUSY){
            assig(antic[i]._k, antic[i]._v);
        }
    }


    delete [] antic;
}

