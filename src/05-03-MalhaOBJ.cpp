#include "bibutil.h"
#include <stdio.h>
#include <stdlib.h>
#include <GL/freeglut.h>

//#define DEBUG 1
// Variáveis para controles de navegação
GLfloat angle, fAspect;
GLfloat rotX, rotY, rotX_ini, rotY_ini;
GLfloat obsX, obsY, obsZ, obsX_ini, obsY_ini, obsZ_ini;
int x_ini,y_ini,bot;

// Apontador para objeto
OBJ *pedra;
OBJ *objeto2;

// Função responsável pela especificação dos parâmetros de iluminação
void DefineIluminacao (void)
{
	GLfloat luzAmbiente[4]={0.2,0.2,0.2,1.0};
	GLfloat luzDifusa[4]={1.0,1.0,1.0,1.0};	   	// "cor"
	GLfloat luzEspecular[4]={1.0, 1.0, 1.0, 1.0};	// "brilho"
	GLfloat posicaoLuz[4]={0.0, 10.0, 100.0, 1.0};

	// Capacidade de brilho do material
	GLfloat especularidade[4]={1.0,1.0,1.0,1.0};
	GLint especMaterial = 60;


	// Define a refletância do material
	glMaterialfv(GL_FRONT,GL_SPECULAR, especularidade);
	// Define a concentração do brilho
	glMateriali(GL_FRONT,GL_SHININESS,especMaterial);

	// Ativa o uso da luz ambiente
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

	// Define os parâmetros da luz de número 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa );
	glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular );
	glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz );
}

void DesenhaChao(void){
//Lado multicolorido - FRENTE
    glBegin(GL_POLYGON);
    glVertex3f( -400, -200, -400);       // P1
    glVertex3f( -400,  200, -400);       // P2
    glVertex3f(  400,  200, -400);       // P3
    glVertex3f(  400, -200, -400);
    glEnd();

  // Lado branco - TRASEIRA
    glBegin(GL_POLYGON);
    glVertex3f(  400, -200, 400 );
    glVertex3f(  400,  200, 400 );
    glVertex3f( -400,  200, 400 );
    glVertex3f( -400, -200, 400 );
    glEnd();

  // Lado roxo - DIREITA
    glBegin(GL_POLYGON);
    glVertex3f( 400, -200, -400 );
    glVertex3f( 400,  200, -400 );
    glVertex3f( 400,  200,  400 );
    glVertex3f( 400, -200,  400 );
    glEnd();

  // Lado verde - ESQUERDA
    glBegin(GL_POLYGON);
    glVertex3f( -400, -200, 400 );
    glVertex3f( -400,  200, 400 );
    glVertex3f( -400,  200, -400);
    glVertex3f( -400, -200, -400 );
    glEnd();

  // Lado vermelho - BASE
    glBegin(GL_POLYGON);
    glVertex3f(  400, -200, -400 );
    glVertex3f(  400, -200,  400 );
    glVertex3f( -400, -200, 400 );
    glVertex3f( -400, -200, -400 );
    glEnd();

      // Lado azul - TOPO
    glColor3f(0.0f, 0.5f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex3f(  400,  200,  400 );
    glVertex3f(  400,  200, -400 );
    glVertex3f( -400,  200, -400 );
    glVertex3f( -400,  200,  400 );
    glEnd();
}

// Função callback de redesenho da janela de visualização
void Desenha(void)
{
	// Limpa a janela de visualização com a cor
	// de fundo definida previamente
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	// Altera a cor do desenho para rosa
	glColor3f(0.0f, 0.5f, 0.0f);

	glPushMatrix();
    glRotatef(rotX,1,0,0);
	glRotatef(rotY,0,1,0);

    //desenha chão
    glTranslatef(0.0, -210.0, 0.0);
    glColor3f(0.5,0.25,0.20);
    DesenhaChao();
    glTranslatef(0.0, 210.0, 0.0);

	// Desenha o objeto 3D lido do arquivo com a cor corrente
    glColor3f(1.0f, 0.0f, 0.0f);

	glTranslatef(-10.0, 0.0, 0.0);
    glScalef(5.0,5.0,5.0);
	DesenhaObjeto(pedra);
	glTranslatef(-10.0, 0.0, 0.0);
	DesenhaObjeto(pedra);
	glTranslatef(20.0, 0.0, 0.0);
	glScalef(0.09,0.09,0.09);
	DesenhaObjeto(objeto2);


    glPopMatrix();

	// Executa os comandos OpenGL
	glutSwapBuffers();
}

// Função usada para especificar a posição do observador virtual
void PosicionaObservador(void){
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();//carrega a matriz de identidade
    gluLookAt(obsX,obsY,obsZ,//posição da câmera
              0.0,0.0,0.0,//para onde a câmera aponta
              0.0,1.0,0.0);//vetor view-up//
}

// Função usada para especificar o volume de visualização
void EspecificaParametrosVisualizacao(void)
{

	// Especifica sistema de coordenadas de projeção
	glMatrixMode(GL_PROJECTION);
	// Inicializa sistema de coordenadas de projeção
	glLoadIdentity();

	// Especifica a projeção perspectiva(angulo,aspecto,dnear,dfar)
	gluPerspective(angle,fAspect,0.01,1200);

	// Chama as funções que especificam os parâmetros da câmera e os parâmetros de iluminação
	PosicionaObservador();
	DefineIluminacao();

}

// Função callback chamada quando o tamanho da janela é alterado
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
	// Para previnir uma divisão por zero
	if ( h == 0 ) h = 1;

	// Especifica as dimensões da viewport
	glViewport(0, 0, w, h);

	// Calcula a correção de aspecto
	fAspect = (GLfloat)w/(GLfloat)h;

	EspecificaParametrosVisualizacao();
}

// Função callback chamada para gerenciar eventos de teclas normais (ESC)
void Teclas (unsigned char tecla, int x, int y)
{
	if(tecla==27) // ESC ?
	{
		// Libera memória e finaliza programa
		LiberaObjeto(pedra);
		LiberaObjeto(objeto2);
		exit(0);
	}
	if(tecla=='m')
	{
		if(glIsEnabled(GL_LIGHTING))
			glDisable(GL_LIGHTING);
		else
            glEnable(GL_LIGHTING);
	}
	glutPostRedisplay();
}

// Função callback para tratar eventos de teclas especiais
void TeclasEspeciais (int tecla, int x, int y)
{
	switch (tecla)
	{
		case GLUT_KEY_HOME:	if(angle>=10)  angle -=5;
							break;
		case GLUT_KEY_END:	if(angle<=150) angle +=5;
							break;
	}
	EspecificaParametrosVisualizacao();
	glutPostRedisplay();
}

// Função callback para eventos de botões do mouse
void GerenciaMouse(int button, int state, int x, int y)
{
	if(state==GLUT_DOWN)
	{
		// Salva os parâmetros atuais
		x_ini = x;
		y_ini = y;
		obsX_ini = obsX;
		obsY_ini = obsY;
		obsZ_ini = obsZ;
		rotX_ini = rotX;
		rotY_ini = rotY;
		bot = button;
	}
	else bot = -1;
}

// Função callback para eventos de movimento do mouse
#define SENS_ROT	5.0
#define SENS_OBS	15.0
#define SENS_TRANSL	20.0
void GerenciaMovim(int x, int y)
{
	// Botão esquerdo
	if(bot==GLUT_LEFT_BUTTON)
	{
		// Calcula diferenças
		int deltax = x_ini - x;
		int deltay = y_ini - y;
		// E modifica ângulos
		rotY = rotY_ini - deltax/SENS_ROT;
		rotX = rotX_ini - deltay/SENS_ROT;
	}
	// Botão direito (zoom-in e zoom-out)
	else if(bot==GLUT_RIGHT_BUTTON){
		// Calcula diferença
		int deltaz = y_ini - y;
		// E modifica distância do observador
		obsZ = obsZ_ini + deltaz/SENS_OBS;
	}
	// Botão do meio
	else if(bot==GLUT_MIDDLE_BUTTON)
	{
		// Calcula diferenças
		int deltax = x_ini - x;
		int deltay = y_ini - y;
		// E modifica posições
		obsX = obsX_ini + deltax/SENS_TRANSL;
		obsY = obsY_ini - deltay/SENS_TRANSL;
	}
	PosicionaObservador();
	glutPostRedisplay();
}

// Função responsável por inicializar parâmetros e variáveis
void Inicializa (void)
{
	char nomeArquivo[30];

	// Define a cor de fundo da janela de visualização como branca
	glClearColor(0, 1.0f, 1.0f, 1.0f);

	// Habilita a definição da cor do material a partir da cor corrente
	glEnable(GL_COLOR_MATERIAL);
	//Habilita o uso de iluminação
	glEnable(GL_LIGHTING);
	// Habilita a luz de número 0
	glEnable(GL_LIGHT0);
	// Habilita o depth-buffering
	glEnable(GL_DEPTH_TEST);

	// Habilita o modelo de tonalização de Gouraud
	glShadeModel(GL_SMOOTH);

	// Inicializa a variável que especifica o ângulo da projeção
	// perspectiva
	angle=55;

	// Inicializa as variáveis usadas para alterar a posição do
	// observador virtual
	obsX = 0;
	obsY = 0;
	obsZ = 150;

	// Lê o nome do arquivo e chama a rotina de leitura
	//printf("Digite o nome do arquivo que contem o modelo 3D: ");
	//gets(nomeArquivo);

	// Carrega o objeto 3D
	objeto2 = CarregaObjeto("teddy.obj",true);
	pedra = CarregaObjeto("monkey.obj",true);

    printf("Objeto carregado!");

	// E calcula o vetor normal em cada face
	if(pedra->normais)
	{
		// Se já existirem normais no arquivo, apaga elas
		free(pedra->normais);
		pedra->normais_por_vertice = false;
	}
	CalculaNormaisPorFace(pedra);
	if(objeto2->normais)
	{
		// Se já existirem normais no arquivo, apaga elas
		free(objeto2->normais);
		objeto2->normais_por_vertice = false;
	}
	CalculaNormaisPorFace(objeto2);
}

// Programa Principal
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
	// Define do modo de operação da GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	// Especifica a posição inicial da janela GLUT
	glutInitWindowPosition(5,5);

	// Especifica o tamanho inicial em pixels da janela GLUT
	glutInitWindowSize(450,450);

	// Cria a janela passando como argumento o título da mesma
	glutCreateWindow("Desenho de um objeto 3D com cálculo do vetor normal");

	// Registra a função callback de redesenho da janela de visualização
	glutDisplayFunc(Desenha);

	// Registra a função callback de redimensionamento da janela de visualização
	glutReshapeFunc(AlteraTamanhoJanela);

	// Registra a função callback para tratamento das teclas normais
	glutKeyboardFunc (Teclas);

	// Registra a função callback para tratamento das teclas especiais
	glutSpecialFunc (TeclasEspeciais);

	// Registra a função callback para eventos de botões do mouse
	glutMouseFunc(GerenciaMouse);

	// Registra a função callback para eventos de movimento do mouse
	glutMotionFunc(GerenciaMovim);

	// Chama a função responsável por fazer as inicializações
	Inicializa();

	// Inicia o processamento e aguarda interações do usuário
	glutMainLoop();

	return 0;
}
