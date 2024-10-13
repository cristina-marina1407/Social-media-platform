**Echipa tema 3 SD**

cpostelnicu1407
bianca.farcasanu

## Descriere:

	Aceasta tema are ca scop implementarea prin trei task uri a unei platforme
ce se aseamana cu retelele de socializare. Astfel, se vor crea o retea de
prieteni, postari si prin combinarea celor doua un feed de social media.
In realizarea acestor task uri am folosit, in primul rand, functii care
lucreaza cu liste, coada si grafuri.

	Functiile pentru liste se afla in fisierul doubly_linked_list.c si
realizeaza urmatoarele operatii:

- functia `dll_create` are rolul de a crea o lista dublu inlantuita
- functia `dll_get_nth_node` returneaza nodul de pe pozitia n
- functia `dll_add_nth_node` adauga un nod in lista pe pozitia n, iar daca n
este mai mare decat list->size adauga elementul pe ultima pozitie
- functia `dll_remove_nth_node` sterge un element de pe pozitia n din lista si
returneaza nodul pe care l a sters
- functia `dll_free` elibereaza memoria pentru o lista dublu inlantuita.
- functia `check_if_node_exist` verifica daca exista un nod, returnand -1 daca
nu exista si indexul unde se afla daca exista

	Functiile pentru coada se afla in fisierul queue.c si realizeaza
urmatoarele operatii:

- functia `q_create` are rolul de a crea o coada
- functia `q_is_empty` verifica daca o coada are sau nu elemente
- functia `q_front` returneaza primul element din coada fara a il modifica
- functia `q_dequeue` elimina primul element din coada
- functia `q_enqueue` adauga un element in coada
- functia `q_clear` elibereaza memoria pentru toate elementele din coada
- functia `q_free` elibereaza memoria pentru toate elementele din coada si
pentru coada

	Functiile pentru graf se afla in fisierul graph.c si realizeaza urmatoarele
operatii:

- functia `lg_create` initializeaza graful cu numarul de noduri primit ca
parametru si aloca memorie pentru lista de adiacenta a grafului
- functia `lg_add_edge` adauga o muchie intre nodurile primite ca parametri
- functia `lg_has_edge` returneaza 1 daca exista muchie intre doua noduri si 0
in caz contrar
- functia `lg_remove_edge` elimina muchia dintre nodurile primite ca parametri
- functia `lg_free` elibereaza memoria folosita de lista de adiacenta a
grafului

		TASK 1 (Retea de prietenie)

	Functiile implementate pentru acest task sunt urmatoarele:

*add_friends*
Functia incepe prin obtinerea user_id urilor utilizatorilor folosind functia
get_user_id, dupa care adauga muchie in graf intre nodurile ce contin aceste id
uri. Muchiile sunt adaugate atat de la id1 la id2, cat si de la id2 la id1.

*remove_friends*
Functie incepe prin obtinerea user_id urilor utilizatorilor folosind functia
get_user_id, dupa care elimina muchiile din graf dintre nodurile ca contin
aceste id uri. Se sterge atat muchia de la id1 la id2, cat si de la id2 la id1.

*get_min_path*
Functia are rolul de a calcula distanta minima dintre doua noduri. La inceput,
se creează o coadă pentru a gestiona nodurile care urmează a fi explorate si se
alocă memorie pentru un vector de noduri care va păstra informații despre
fiecare nod .Se incepe prin marcarea nodului src ca vizitat, dupa care acesta
este adaugat in coada pentru a începe parcurgerea. Se foloseste BFS pentru
parcurgerea grafului. Atata timp cat coada nu este goala, sunt verificati
vecinii nodului curent, fiind marcati ca vizitati si se creste distanta. Odată
ce BFS este complet, vectorul de noduri conține distanța minimă de la sursă la
fiecare nod. Pentru a determina drumul de la src la dest, se reconstruiește
drumul invers folosind părinții nodurilor.

*add_in_order*
Functia este folosita pentru a adauga noduri intr o lista in ordinea
crescatoare a id urilor.

*suggestions*
Functia are rolul de a gasi toti prietenii prietenilor care nu sunt deja
prieteni cu un utilizator, acestia devenind sugestii pentru el. La inceput,
cream o lista de sugestii in care vom adauga ulterior utilizatori. Se parcurge
lista de prieteni a utilizatorului, dupa care se parcurg pe rand listele
prieteniilor lor. Daca gasim un prieten pe care utilizatorul nu il are, acesta
este adaugat in lista suggestion_list in ordinea id urilor. La final, se
printeaza numele utilizatorilor din lista si se elibereaza memoria alocata
pentru aceasta.

*common_friends*
Functia are rolul de a gasi prietenii comuni a doi utilizatori. La inceput,
cream o lista de prieteni comuni in care vom adauga ulterior utilizatori. Se
parcurg listele de prieteni ale celor doi utilizatori, iar daca se gaseste un
prieten comun, acesta este adaugat in lista common_friends in ordinea id urilor.
La final, se printeaza numele utilizatorilor din lista si se elibereaza memoria
alocata pentru aceasta.

*nr_friends*
Functia are rolul de a gasi numarul de prieteni al unui utilizator, returnand
dimensiunea listei de prieteni a acestuia.

*most_popular*
Functia are rolul de a gasi cel mai popular prieten al unui utilizator. Mai
intai, se pastreaza numarul de prieteni ai utilizatorului, deoarece in cazul de
egalitate dintre el si un prieten, se va afisa utilizatorul dat ca fiind cel
mai popular. Initializam maximul cu numarul de prieteni ai utilizatorului. Se
parcurge lista de prieteni a utilizatorului si se determina maximul dintre
numarul de prieteni ai prietenilor lui. De asemenea, se pastreaza id ul
maximului pentru afisare.

		TASK 2 (Postari si repostari)

	Structurile folosite in acest task sunt:

- structura `data_in_node` care contine datele care se gasesc intr un nod.
Campurile acestei structuri sunt id ul postarii, titlul acesteia, id ul
utilizatorului si lista de like uri.

- structura `t_node_t` ce reprezinta un nod din arbore. Campurile acestei
structuri sunt vectorul de copii al fiecarui nod, numarul de copii ai acestuia
si data.

-structura `tree_t` ce reprezinta arborele. Campurile acestei structuri sunt
radacina arborelui, numarul de repostari si data_size.

-structura `tree_array_t` constituie vectorul de postari, ce este un vector de
arbori. Campurile acestei structuri sunt vectorul de arbori, posts, dimensiunea
acestuia, id ul la crescator la care au ajuns postarile si memoria alocata
vectorului de postari.

	In rezolvarea acestui task, pe langa functiile ce rezolva cerintele
precizate in enunt, am folosit urmatoarele functii auxiliare:

- functia `t_node_create` are rolul de a crea un nod din arbore
- functia `tree_create` are rolul de a crea un arbore
- functia tree_array_create are rolul de a crea vectorul de arbori, vectorul de
postari
- functiile `__tree_array_free` si `tree_array_free` elibereaza memoria recursiv

- functia `get_index_post_id` returneaza indexul unei postari din vectorul de
postari

- functia `get_index_repost_id` returneaza indexul unei repostari

- functia `search_children` are rolul de a cauta un anumit nod in arbore in
functie de repost_id ul dat. Daca repost_id ul nu este gasit, functia parcurge
recursiv copiii nodului radacina pana cand se returneaza nodul corespunzator.

	Functiile implementate pentru acest task sunt urmatoarele:

*create_post*
Functia incepe prin verificarea memoriei disponibile, realocand vectorul de
postari daca dupa adaugarea unei postari dimenisunea vectorului este mai mare
decat memoria alocata. Apoi, se creeaza un arbore, campurile acestuia fiind
adaugate informatiile despre postare. De asemenea, se incrementeaza id ul
curent.

*create_repost*
Functia are rolul de a crea o repostare la o postare sau o repostare facuta
anterior. Astfel, exista doua cazuri cand se reposteaza o postare
(repost_id ul este 0) si cand se reposteaza o repostare. In primul caz, se
adauga copii la nodul cu post_id ul dat. Se realoca vectorul de copii al
acestuia daca dupa adaugarea unei repostari dimensiunea vectorului este mai
mare decat memoria alocata. Se creeaza nodul care trebuie adaugat, se creste
numarul de repostari si se incrementeaza id ul curent. In cel de al doilea caz
trebuie sa se adauge nodul printre copiii unei repostari, aceasta fiind gasita
cu functia serch_children. Dupa ce repostarea este gasita, se adauga un nod
in vectorul de copii al acesteia repetandu se procesul de mai sus.

*print_post*
Functia este folosita in get_reposts pentru a printa recursiv toată ierarhia de
reposturi pentru o postare/repostare.

*get_reposts*
Functia este folosita pentru a obtine ierarhia de reposturi pentru o
postare/repostare. In cazul in care trebuie printate repostarile pentru o
repostare, mai intai trebuie gasita aceasta repostare printre copiii unei
postari cu functia search_children.

*like_function*
Functia are rolul de a adauga un like la o postare sau o repostare. In ambele
cazuri se verifica in lista de like uri daca exista un nod cu id ul
utilizatorului care doreste sa dea like, iar daca acesta exista este sters.
In caz contrar, acesta este adaugat in lista.

*get_likes*
Functia este folosita pentru a returna numarul de like uri de la o poatare sau
o repostare. In ambele cazuri, se returneaza dimensiunea listei de like uri
din nodul corespunzator.

*is_ancestor*
Functia verifica daca un nod este stramos pentru un alt nod in arbore.
Mai intai se verifica daca cele doua noduri sunt egale, functia returnand 1 in
acest caz. Daca nu sunt egale functia parcurge recursiv toti copiii nodului
root pana cand gaseste un nod pentru care nodul dat este decendent.

*find_ancestor*
Functia are rolul de a gasi primul stramos comun dintre doua noduri.
Se verifică dacă strămoșul comun este unul dintre nodurile r1 sau r2. Dacă
este, atunci returnează acel nod. In caz contrar, functia verifica daca a
ajuns la unul dintre aceste noduri. Daca aceste cazuri nu sunt indeplinite,
functia parcurge recursiv toti copiii nodului root pana cand gaseste stramosul
comun.

*first_common_repost*
Functia gaseste prima repostare comuna a doua repostari, folosind functiile
pentru verificarea si gasirea stramosilor explicate mai sus.

*get_max_likes_repost*
Functia returneaza repostarea cu numarul maxim de la like uri pentru o posatre,
parcurgand recursiv arborele unei postari. 

*ratio*
Functia are rolul de a determina daca o repostare a reusit sa adune mai multe
like uri decat o postare, folosind functia get_max_likes_repost.

*find_parent_of_repost*
Functia gaseste parintele unei repostari, pentru a putea sterge acea repostare
din copiii parintelui in functia delete. Functia parcurge recursiv toti copiii
nodului din care pornim ('root' care se actualizeaza la fiecare cautare).

*delete*
Functia are rolul de a sterge o postare sau o repostare.
In cazul in care se sterge o postare se elibereaza memoria pentru tot arborele
de repostari si se muta elementele din vectorul posts cu o pozitie la stanga.
In cazul in care se sterge o repostare, mai intai, se gaseste aceasta printre
copiii postarii si apoi parintele ei, folosind functiile corespunzatoare. Se
sterge nodul din vectorul de copii al parintelui si se muta elementele cu o
pozitie la stanga.

		TASK 3 (Social Media)

	Functiile implementate pentru acest task sunt urmatoarele:

*friendship_verification*
Functia are rolul de a verifica daca doi utilizatori sunt prieteni, determinand
daca id ul prietenului se afla in lista de prieteni a utilizatorului

*feed*
Functia afiseaza cele mai recente feed_size postari ale unui utilizator si ale
prietenilor sai. Se afiseaza descrescator titlurile postarilor care apartin
prietenilor utilizatorului.

*search_repost*
Functia are rolul de a cauta dupa numele utilizatorului o repostare intr un
arbore. Funcția parcurge recursiv toți copiii nodului root și verifică dacă
repostarea se află printre ei. Dacă găsește repostarea, o returnează. De
asemenea, este utilizata ca functie auxiliara pentru functia view_profile,
afisand deopotriva repost-urile facute de persoana ceruta.

*view_profile*
Functia afiseaza toate postarile si repostarile unui utilizator. Mai intai,
cauta dupa user_id in vectorul de postari pentru a printa postarile
utilizatorului. Apoi, cauta repostarile acestuia in copiii tuturor postarilor,
folosind functia search_repost.

*friends_repost*
Functia afișează toti prietenii unui utilizator care au dat repost la o postare
data. Se parcurg prietenii utilizatorului, se verifica daca id ul
celor (child_id) care au repostat postarea data este egal cu cel al fiecarui
prieten din lista si se afiseaza numele acestora.

*all_are_friends*
Functia verifica daca toti utilizatorii dintr o lista sunt prieteni fiecare cu
fiecare, folosind functia friendship_verification. Returneaza 1 daca sunt, 0
in cazul in care cel putin unul dintre prieteni nu este prieten cu cineva din
lista.

*print_clique*
Functia printeaza elementele unei liste care reprezinta o clica.

*common_group*
Functia are rolul de a afisa cel mai mare grup care il contine pe un
utilizator. La inceput, se creeaza lista in care se va stoca grupul.
Apoi, se parcurge lista de prieteni a utilizatorului pentru a parcurge, apoi,
lista de prieteni a fiecaruia dintre ei. Se verifica daca toti cei din lista de
prieteni a unui prieten al utilizatorului dat sunt prieteni intre ei cu functia
all_are_friends si daca dimensiunea acestei liste este mai mare decat cea a
grupului stocat anterior. Daca se intampla acest lucru, se elibereaza clica
precedenta si se adauga noua clica in ordinea id urilor, incepand cu adaugarea
prietenului in a carui lista de prieteni ne aflam.
