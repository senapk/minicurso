#include <list>
#include <BuggFX.h>

class Cell : public Imagem{
    public:
    int dim;
    Cell(int dim):Imagem(){
        this->dim = dim;
    }

    void load(string path){
        Imagem::load(path, dim, dim);
    }

    void draw(float x, float y){
        Imagem::draw(x * dim, y * dim);
    }
};

using namespace std;
int main(){
    Cell peixe(50);
    peixe.load("../rpg/output.png");


    Janela j(800, 600, "oi");
    int cont = 0;
    while(j.isOpen()){
        j.clear();
        peixe.draw(cont++,1);
        j.update();
    }

    return 0;
}
