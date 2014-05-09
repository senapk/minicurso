#include <list>
#include <BuggFX.h>
#include <string>

class Cell {
public:
    int dim;
    Desenhavel * desenho;

    Cell(Desenhavel *d, int dim){
        desenho = d;
        this->dim = dim;
        d->setSize(dim, dim);
    }

    void draw( int x, int y){
        desenho->draw(x * dim, y * dim);
    }
};

struct Game{
    int largura, altura;

    Game(int _largura, int _altura){
        largura = _largura;
        altura = _altura;
    }
};

class Heroi{
public:
    Cell cell;
    int x, y;
    int dim;
    int vida, forca, vidaMax;
    string nome;

    Heroi(int x, int y, Desenhavel *d, int dim, string nome):
        cell(d, dim){
        this->x = x;
        this->y = y;
        this->dim = dim;
        this->nome = nome;
    }

    void heal(){
        if(vida < vidaMax)
            vida ++;
    }

    void setup(int vidaMax, int forca){
        this->vidaMax = vidaMax;
        this->vida = vidaMax;
        this->forca = forca;
    }


    void show(Escritor &e){

        cell.draw(x, y);
        e.setColor(Color::Red);
        string status = nome + Util::str("F %d PV %d", forca, vida);
        e.setColor(Color::White);
        e.setSize(7);
        e.write(x * dim -20, y * dim - 20, status);
    }

    //cmd[0] esq, cmd[1] dir, cmd[2] cima, cmd[3] baixo
    void input(char c, string cmd, Heroi &heroi, Game game){
        int xbak = x;
        int ybak = y;


        if(vida > 0){
            if(c == cmd[0])
                x--;
            if(c == cmd[1])
                x++;
            if(c == cmd[3])
                y++;
            if(c == cmd[2])
                y--;

            if(x == heroi.x && y == heroi.y){
                x = xbak;
                y = ybak;
                if(heroi.vida > (-1 * heroi.vidaMax))
                    heroi.vida -= forca;
            }
            if( (x < 0) || (y < 0) ||
                    (x * dim >= game.largura) ||
                    (y * dim >= game.altura)){
                x = xbak;
                y = ybak;
            }
        }
    }

};



int main(){
    const int TAM = 100;
    Game game(1000, 600);
    Janela janela(game.largura, game.altura,"Digite ou morra!");
    janela.setBackgroundColor(Color::Black);

    Imagem imPeixe("../rpg/imagens/output.png");
    Heroi peixe(4, 4, &imPeixe , TAM, "Mal");
    peixe.setup(40, 8);

    Animacao animeMel(100);
    animeMel.addMany("../rpg/imagens/mel-", ".png", 13);

    Heroi mel(2, 2, &animeMel, TAM, "Mel");
    mel.setup(45, 10);

    Escritor e;
    char c;
    Timer healTimer;
    while(janela.isOpen()) {
        c = janela.getKeyChar();


        mel.input(c, "adws", peixe, game);
        peixe.input(c, "jlik", mel, game);

        if(healTimer.hasPassed(300)){
            healTimer.update();
            mel.heal();
            peixe.heal();
        }

        janela.clear();

        mel.show(e);
        peixe.show(e);



        janela.update();

    }
    return 0;
}
