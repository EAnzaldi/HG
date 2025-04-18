#include "MenuState.h"
#include "PlayState.h"
#include "StateManager.h"

#define NEW 0
#define RESUME 1
#define EXIT 2

MenuState::MenuState(StateManager* manager, GLFWwindow* window, irrklang::ISoundEngine* engine)
	: GameState(manager, window, engine), CurrentSelection(NEW), CurrentGame(NULL)
{
    // build and compile our shader zprogram
    // -------------------------------------
    pShader = new Shader("shader.vs", "shader.fs");
    pTextShader = new Shader("shader_text.vs", "shader_text.fs");

    // Initializza FreeType
    if (FT_Init_FreeType(&ft)) {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }

    pTextNormal = new TextObject(ft, "resources/fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf");
	pTitle = new TextObject(ft, "resources/fonts/bleeding-cowboys/Bleeding_Cowboys.ttf");

    // setup delle uniform delle shader che non cambieranno nel ciclo di rendering
    pShader->use();


    // musica di sottofondo
    engine->play2D("resources/sounds/ost.wav", true);
}

MenuState::~MenuState() {

}

void MenuState::ProcessInput()
{

	if (glfwGetKey(Window, GLFW_KEY_DOWN) == GLFW_PRESS)
		SelectionDown();
	else if (glfwGetKey(Window, GLFW_KEY_UP) == GLFW_PRESS)
		SelectionUp();
	else if (glfwGetKey(Window, GLFW_KEY_ENTER) == GLFW_PRESS)
		SelectionChosen();
}


void MenuState::Render()
{
	//Aggiunte le seguenti due righe per gestire correttamente la trasparenza
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(192.0f / 255.0f, 121.0f / 255.0f, 0.5f / 255.0f, 205.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::vec3 TitleColor = { 0.0, 0.0, 0.0 };

	pTitle->Render(*pTextShader, "Hansel + Gretel", SCR_WIDTH / 8, 1000.0f, 3.0f, TitleColor);


	glm::vec3 NonSelectedColor = { 255.0, 255.0, 255.0 };
	glm::vec3 SelectedColor = { 0.0, 255.0, 0.0 };

	std::vector <glm::vec3> colors = { NonSelectedColor , NonSelectedColor , NonSelectedColor } ;

	colors[CurrentSelection] = SelectedColor;

	pTextNormal->Render(*pTextShader, "New Game", SCR_WIDTH/2, SCR_HEIGHT/2 + 200.0f, 1.2f, colors[0]);
	pTextNormal->Render(*pTextShader, "Resume Game", SCR_WIDTH / 2, SCR_HEIGHT / 2, 1.2f, colors[1]);
	pTextNormal->Render(*pTextShader, "Exit", SCR_WIDTH/2, SCR_HEIGHT/2 - 200.0f, 1.2f, colors[2]);

}

void MenuState::EnterState()
{
	// Checks whether there is a current game active
	CurrentSelection = 0;
}

void MenuState::SelectionUp()
{
	if(CurrentSelection > NEW)
		CurrentSelection--;
}

void MenuState::SelectionDown()
{
	if (CurrentSelection < EXIT)
		CurrentSelection++;
}

void MenuState::SelectionChosen()
{
	switch (CurrentSelection)
	{
	case NEW:
		if (!CurrentGame) {
			CurrentGame = new PlayState(Manager, Window, Engine);
		}	
		CurrentGame->Reset();
		ChangeState(CurrentGame);
		break;

	case RESUME:
		if (CurrentGame && !CurrentGame->IsGameOver())
			ChangeState(CurrentGame);
		break;

	case EXIT:
		glfwSetWindowShouldClose(Window, true);//quit
		break;
	}
}
