//#include <GLFW/glfw3.h>
#include "MenuState.h"
#include "PlayState.h"

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
	pSpriteShader = new Shader("shader_sprite.vs", "shader_sprite.fs");

    // Initializza FreeType
    if (FT_Init_FreeType(&ft)) {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }

    //pTextNormal = new TextObject(ft, "resources/fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf");
	//pTitle = new TextObject(ft, "resources/fonts/bleeding-cowboys/Bleeding_Cowboys.ttf");

	int fbWidth, fbHeight;
	glfwGetFramebufferSize(Window, &fbWidth, &fbHeight);
	pBackground = new FlatMesh("resources/textures/background2.png");
	pBackgroundObj = new GameObject(glm::vec2(fbWidth/2, fbHeight/2), pBackground->getSize()/1.25f, pBackground, 0);

	//unselected menu
	pMenu[0] = new FlatMesh("resources/textures/new_game.png");
	pMenu[1] = new FlatMesh("resources/textures/resume_game.png");
	pMenu[2] = new FlatMesh("resources/textures/exit.png");
	pMenuObj[0] = new GameObject(glm::vec2(880.0f + pMenu[0]->getWidth()/2.5f, fbHeight - 440.0f), pMenu[0]->getSize()/1.25f, pMenu[0], 0);
	pMenuObj[1] = new GameObject(glm::vec2(880.0f + pMenu[1]->getWidth()/2.5f, fbHeight - 600.0f), pMenu[1]->getSize()/1.25f, pMenu[1], 0);
	pMenuObj[2] = new GameObject(glm::vec2(880.0f + pMenu[2]->getWidth()/2.5f, fbHeight - 760.0f), pMenu[2]->getSize()/1.25f, pMenu[2], 0);

	//selected menu
	pMenuSel[0] = new FlatMesh("resources/textures/new_game_yellow.png");
	pMenuSel[1] = new FlatMesh("resources/textures/resume_game_yellow.png");
	pMenuSel[2] = new FlatMesh("resources/textures/exit_yellow.png");
	pMenuSelObj[0] = new GameObject(glm::vec2(880.0f + pMenuSel[0]->getWidth()/2.5f, fbHeight - 440.0f), pMenuSel[0]->getSize()/1.25f, pMenuSel[0], 0);
	pMenuSelObj[1] = new GameObject(glm::vec2(880.0f + pMenuSel[1]->getWidth()/2.5f, fbHeight - 600.0f), pMenuSel[1]->getSize()/1.25f, pMenuSel[1], 0);
	pMenuSelObj[2] = new GameObject(glm::vec2(880.0f + pMenuSel[2]->getWidth()/2.5f, fbHeight - 760.0f), pMenuSel[2]->getSize()/1.25f, pMenuSel[2], 0);

	//resume game not available
	pMenuNoGame = new FlatMesh("resources/textures/resume_game_dark.png");;
	pMenuNoGameObj = new GameObject(glm::vec2(880.0f + pMenuNoGame->getWidth()/2.5f, fbHeight - 600.0f), pMenuNoGame->getSize()/1.25f, pMenuNoGame, 0);;

	//pTest = new FlatMesh("resources/textures/awesomeface.png");
	//pTestObj = new GameObject(glm::vec2(0.0f, 0.0f), pTest->getSize(), pTest, 0);

    // setup delle uniform delle shader che non cambieranno nel ciclo di rendering
	float left = -1.0f;   // Puoi modificare questi valori per adattarli alla tua scena
	float right = 1.0f;
	float bottom = -1.0f;
	float top = 1.0f;

	pCamera = new Camera(glm::vec3(0.0f, 0.0f, 0.0f));

	glm::mat4 projectionNDC = glm::ortho(left, right, bottom, top);
	/*glm::mat4 projection = glm::ortho(0.0f, SCR_WIDTH_F, 0.0f, SCR_HEIGHT_F);//left, right, bottom, top*/
	glm::mat4 projectionPixels = glm::ortho(0.0f, static_cast<float>(fbWidth), 0.0f, static_cast<float>(fbHeight));//left, right, bottom, top

	glm::mat4 view = pCamera->GetViewMatrix();

	// setup delle uniform delle shader che non cambieranno nel ciclo di rendering
	// Shader base
	pShader->use();
	pShader->setMat4("projection", projectionNDC);
	pShader->setMat4("view", view);

	pSpriteShader->use();
	pSpriteShader->setMat4("projection", projectionPixels);
	pSpriteShader->setMat4("view", view);

}

MenuState::~MenuState() {

}

MenuState* MenuState::GetInstance(StateManager* manager, GLFWwindow* window, irrklang::ISoundEngine* engine)
{
	static MenuState Instance(manager, window, engine);
	return &Instance;
}
//DEBOUNCE: avoids that pressing key in other frames (or states) is considered in this frame (or state)
bool canPressUp = true;
bool canPressDown = true;
bool canPressEnter = true;

void MenuState::ProcessInput()
{

	if (glfwGetKey(Window, GLFW_KEY_DOWN) == GLFW_PRESS && canPressDown) {
		SelectionDown();
		canPressDown = false;
	}
	else if (glfwGetKey(Window, GLFW_KEY_DOWN) == GLFW_RELEASE)
		canPressDown = true;

	if (glfwGetKey(Window, GLFW_KEY_UP) == GLFW_PRESS && canPressUp) {
		SelectionUp();
		canPressUp = false;
	}
	else if (glfwGetKey(Window, GLFW_KEY_UP) == GLFW_RELEASE)
		canPressUp = true;

	if (glfwGetKey(Window, GLFW_KEY_ENTER) == GLFW_PRESS && canPressEnter) {
		SelectionChosen();
		canPressEnter = false;
	}
	else if (glfwGetKey(Window, GLFW_KEY_ENTER) == GLFW_RELEASE)
		canPressEnter = true;

}
void MenuState::MouseMoving(double xpos, double ypos)
{
	int fbWidth, fbHeight;
	glfwGetFramebufferSize(Window, &fbWidth, &fbHeight);
	ypos = fbHeight - ypos; //OpenGL ha coordinata y Invertita rispetto a GLFW

	//Controllare se il cursore sia su una delle box
	for (int i = 0; i < 3; i++) {
		Hitbox bounds = pMenuObj[i]->GetHitbox();
		bool isColliding = (xpos <= bounds.Max.x && xpos >= bounds.Min.x && ypos <= bounds.Max.y && ypos >= bounds.Min.y);
		//Se è su una box selezionarla
		if (isColliding) {
			CurrentSelection = i;
			//printf("x:%lf, y:%lf, select:%d\n", xpos, ypos, CurrentSelection);
			break;
		}
	}
}

void MenuState::MouseClick(int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		//Controllare che il cursore sia sulla box
		double xpos, ypos;
		int fbWidth, fbHeight;
		glfwGetCursorPos(Window, &xpos, &ypos);
		glfwGetFramebufferSize(Window, &fbWidth, &fbHeight);
		ypos = fbHeight - ypos;
		Hitbox bounds = pMenuObj[CurrentSelection]->GetHitbox();
		bool isColliding = (xpos <= bounds.Max.x && xpos >= bounds.Min.x && ypos <= bounds.Max.y && ypos >= bounds.Min.y);
		//Se è sulla box scegliere la selezione
		if (isColliding) {
			SelectionChosen();
		}
	}
}

void MenuState::Render()
{
	//Aggiunte le seguenti due righe per gestire correttamente la trasparenza
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(192.0f / 255.0f, 121.0f / 255.0f, 0.5f / 255.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glm::vec3 TitleColor = { 0.0, 0.0, 0.0 };

	//pTitle->Render(*pTextShader, "Hansel + Gretel", SCR_WIDTH / 8, 1000.0f, 3.0f, TitleColor);
	//pTitleObj->Render(*pShader);

	pBackgroundObj->Render(*pSpriteShader);

	glDisable(GL_DEPTH_TEST);//evita di considerare la profondità delle sprite

	for (int i = 0; i < 3; i++)
		if (i == 1 && Status != GameStatus::Paused)
			pMenuNoGameObj->Render(*pSpriteShader);
		else if(CurrentSelection!=i)
			pMenuObj[i]->Render(*pSpriteShader);
		else
			pMenuSelObj[i]->Render(*pSpriteShader);

	//pTestObj->RenderFlat(*pSpriteShader);

	glEnable(GL_DEPTH_TEST);


	/*if (Status != GameStatus::Paused)
		colors[1] = NonAvailableColor;

	glm::vec3 NonSelectedColor = { 255.0, 255.0, 255.0 };
	glm::vec3 SelectedColor = { 0.0, 255.0, 0.0 };
	glm::vec3 NonAvailableColor = { 0.0, 0.0, 0.0 };

	std::vector <glm::vec3> colors = { NonSelectedColor , NonSelectedColor , NonSelectedColor } ;

	colors[CurrentSelection] = SelectedColor;

	if (Status != GameStatus::Paused)
		colors[1] = NonAvailableColor;

	switch (CurrentSelection) {
		case NEW:
			pTextNormal->Render(*pTextShader, "New Game <", SCR_WIDTH / 2, SCR_HEIGHT / 2 + 200.0f, 1.2f, colors[0]);
			pTextNormal->Render(*pTextShader, "Resume Game", SCR_WIDTH / 2, SCR_HEIGHT / 2, 1.2f, colors[1]);
			pTextNormal->Render(*pTextShader, "Exit", SCR_WIDTH / 2, SCR_HEIGHT / 2 - 200.0f, 1.2f, colors[2]);
			break;
		case RESUME:
			pTextNormal->Render(*pTextShader, "New Game", SCR_WIDTH / 2, SCR_HEIGHT / 2 + 200.0f, 1.2f, colors[0]);
			pTextNormal->Render(*pTextShader, "Resume Game <", SCR_WIDTH / 2, SCR_HEIGHT / 2, 1.2f, colors[1]);
			pTextNormal->Render(*pTextShader, "Exit", SCR_WIDTH / 2, SCR_HEIGHT / 2 - 200.0f, 1.2f, colors[2]);
			break;
		case EXIT:
			pTextNormal->Render(*pTextShader, "New Game", SCR_WIDTH / 2, SCR_HEIGHT / 2 + 200.0f, 1.2f, colors[0]);
			pTextNormal->Render(*pTextShader, "Resume Game", SCR_WIDTH / 2, SCR_HEIGHT / 2, 1.2f, colors[1]);
			pTextNormal->Render(*pTextShader, "Exit <", SCR_WIDTH / 2, SCR_HEIGHT / 2 - 200.0f, 1.2f, colors[2]);
			break;
	}
	*/

}

void MenuState::EnterState()
{
	// Checks whether there is a current game active
	CurrentSelection = NEW;
	canPressEnter = false;

	// musica di sottofondo
	Engine->play2D("resources/sounds/ost.wav", true);
}

void MenuState::SelectionUp()
{
	
	if(CurrentSelection > NEW)
		CurrentSelection--;
	if (CurrentSelection == RESUME && Status != GameStatus::Paused)//no game -> skip resume
			CurrentSelection--;
}

void MenuState::SelectionDown()
{
	if (CurrentSelection < EXIT)
		CurrentSelection++;
	if (CurrentSelection == RESUME && Status != GameStatus::Paused)//no game -> skip resume
		CurrentSelection++;
}

void MenuState::SelectionChosen()
{
	switch (CurrentSelection)
	{
	case NEW:
		if (Status == GameStatus::None) {
			CurrentGame = PlayState::GetInstance(Manager, Window, Engine);
		}	
		ChangeState(CurrentGame);
		CurrentGame->Reset();
		break;

	case RESUME:
		if (Status == GameStatus::Paused)
			ChangeState(CurrentGame);
		break;

	case EXIT:
		Engine->drop();
		glfwSetWindowShouldClose(Window, true);//quit
		break;
	}
}
