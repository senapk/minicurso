#include <list>
#include <BuggFX.h>
#include <string>

//#define PRONTO
#ifdef PRONTO

class Cell : Imagem {
    public:
        int dim;
        Cell(string path, int dim) : Imagem(path, dim, dim){
            this->dim = dim;
        }

        void draw( int x, int y){
            Imagem::draw(x * dim, y * dim);
        }
};

class DrawCell{
public:
    Desenhavel *desenho;
    int dim;
    DrawCell(Desenhavel *d, int dim){
        desenho = d;
        this->dim = dim;
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
        DrawCell cell;
        int x, y;
        int vida, forca, vidaMax;
        string nome;
        int dim;

        Heroi(int x, int y, int dim, Desenhavel *d, string nome):
              cell(d, dim){
                this->x = x;
                this->y = y;
                this->nome = nome;
                this->dim = dim;
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
            e.setColor(Color::White);
            string status = nome + Util::str("F %d PV %d", forca, vida);
            e.setColor(Color::White);
            e.setSize(7);
            e.write(x * dim -20, y * dim - 20, status);
        }

        //cmd[0] esq, cmd[1] dir, cmd[2] cima, cmd[3] baixo
        void input(char c, string cmd, Heroi &heroi, Game game){
            int xbak = x;
            int ybak = y;


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
                    (y * dim >= game.altura) || vida < 0){
                x = xbak;
                y = ybak;
            }
        }

};



int main(){
    Game game(1000, 600);
    Janela janela(game.largura, game.altura,"Digite ou morra!");
    janela.setBackgroundColor(Color::Black);

    const int TAM = 100;

    Imagem imPeixe("../rpg/imagens/output.png", TAM, TAM);
    Heroi peixe(4, 4, TAM, &imPeixe, "Mal");
    peixe.setup(40, 8);

    Animacao imMel(30);
    imMel.addMany("../rpg/imagens/mel-", ".png", 12);
    imMel.setSize(TAM, TAM);
    Heroi mel(2, 2, TAM, &imMel, "Mel");
    mel.setup(45, 10);


    Timer healTimer;
    Escritor e;
    char c;
    while(janela.isOpen()) {
        c = janela.getKeyChar();


        mel.input(c, "adws", peixe, game);
        peixe.input(c, "jlik", mel, game);
        janela.clear();

        mel.show(e);
        peixe.show(e);

        if(healTimer.hasPassed(400)){
            healTimer.update();
            mel.heal();
            peixe.heal();
        }

        janela.update();

    }
    return 0;
}
#endif
