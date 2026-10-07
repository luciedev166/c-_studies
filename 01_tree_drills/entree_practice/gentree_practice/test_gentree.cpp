#include "gentree_skeleton.hpp"
#include <cassert>
int main() {
    MyGenTree t;
    node *r = t.addRoot(1); 
    bool threw = false; try { t.addRoot(2); } catch (logic_error &e) { threw = (string(e.what()) == "Already has root"); }
    assert(threw);
    node *a = t.addChild(r, 2), *b = t.addChild(r, 3), *c = t.addChild(r, 4);
    node *d = t.addChild(a, 5), *e = t.addChild(a, 6), *f = t.addChild(d, 7);
    assert(r->height == 3 && a->height == 2 && d->height == 1 && f->height == 0 && b->height == 0);
    assert(r->depth == 0 && a->depth == 1 && d->depth == 2 && f->depth == 3);
    assert(r->num_child == 3 && r->children[2] == c);
    t.remove(a);                                   // has children: prints, no change
    assert(r->num_child == 3);
    t.remove(f);
    assert(d->height == 0 && a->height == 1 && r->height == 2);
    t.remove(d); t.remove(e);
    assert(a->num_child == 0 && a->height == 0 && r->height == 1);
    t.remove(b);                                   // middle child: shifts
    assert(r->num_child == 2 && r->children[0] == a && r->children[1] == c && r->children[2] == nullptr);
    t.remove(a); t.remove(c);
    assert(r->num_child == 0 && r->height == 0);
    t.remove(r);
    node *r2 = t.addRoot(9);                       // root was removed, so this must work again
    assert(r2->depth == 0);
    cout << "ALL OK" << endl;
}
