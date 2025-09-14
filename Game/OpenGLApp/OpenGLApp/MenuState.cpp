//#include <GLFW/glfw3.h>
#include "MenuState.h"
#include "PlayState.h"

#define NEW 0
#define RESUME 1
#define EXIT 2

MenuState::MenuState(StateManager* manager, GLFWwindow* window, irrklang::ISoundEngine* engine)
	: GameState(manager, window, engine), CurrentSelection(NEW), CurrentGame(NULL)
{
    // Initializza FreeType
    if (FT_Init_FreeType(&ft)) {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }

    //pTextNormal = new TextObject(ft, "resources/fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf");
	//pTitle = new TextObject(ft, "resources/fonts/bleeding-cowboys/Bleeding_Cowboys.ttf");

	CurrentGame = PlayState::GetInstance(Manager, Window, Engine);

	pBackground = new TextureObject("resources/textures/background2.png");
	pBackgroundObj = new GameObject(glm::vec2(SCR_WIDTH_F /2, SCR_HEIGHT_F/2), pBackground->getSize()/1.25f, pBackground, 0);

	//float pos_center_y = SCR_HEIGHT_F - 700.0f;
	//float space_y = 140.0f;
	//float offset_x = 840.0f;
	float pos_center_y = SCR_HEIGHT_F * 0.27f;
	float space_y = SCR_HEIGHT_F * 0.14f;
	float offset_x = SCR_WIDTH_F * 0.66f;

	//unselected menu
	pMenu[0] = new TextureObject("resources/textures/new_game.png");
	pMenu[1] = new TextureObject("resources/textures/resume_game.png");
	pMenu[2] = new TextureObject("resources/textures/exit.png");
	pMenuObj[0] = new GameObject(glm::vec2(offset_x + pMenu[0]->getWidth()/2.5f, pos_center_y + space_y), pMenu[0]->getSize()/1.25f, pMenu[0], 0);
	pMenuObj[1] = new GameObject(glm::vec2(offset_x + pMenu[1]->getWidth()/2.5f, pos_center_y), pMenu[1]->getSize()/1.25f, pMenu[1], 0);
	pMenuObj[2] = new GameObject(glm::vec2(offset_x + pMenu[2]->getWidth()/2.5f, pos_center_y - space_y), pMenu[2]->getSize()/1.25f, pMenu[2], 0);

	//selected menu
	pMenuSel[0] = new TextureObject("resources/textures/new_game_yellow.png");
	pMenuSel[1] = new TextureObject("resources/textures/resume_game_yellow.png");
	pMenuSel[2] = new TextureObject("resources/textures/exit_yellow.png");
	pMenuSelObj[0] = new GameObject(glm::vec2(offset_x + pMenuSel[0]->getWidth()/2.5f, pos_center_y + space_y), pMenuSel[0]->getSize()/1.25f, pMenuSel[0], 0);
	pMenuSelObj[1] = new GameObject(glm::vec2(offset_x + pMenuSel[1]->getWidth()/2.5f, pos_center_y), pMenuSel[1]->getSize()/1.25f, pMenuSel[1], 0);
	pMenuSelObj[2] = new GameObject(glm::vec2(offset_x + pMenuSel[2]->getWidth()/2.5f, pos_center_y - space_y), pMenuSel[2]->getSize()/1.25f, pMenuSel[2], 0);

	//resume game not available
	pMenuNoGame = new TextureObject("resources/textures/resume_game_dark.png");;
	pMenuNoGameObj = new GameObject(glm::vec2(offset_x + pMenuNoGame->getWidth() / 2.5f, pos_center_y), pMenuNoGame->getSize() / 1.25f, pMenuNoGame, 0);;

	//menu choice singleplayer-multiplayer
	pMenuMod[0] = new TextureObject("resources/textures/gretel_only.png");
	pMenuMod[1] = new TextureObject("resources/textures/gretel_with_hansel.png");
	pArrow = new TextureObject("resources/textures/arrow.png");
	pMenuModObj[0] = new GameObject(glm::vec2(offset_x + pMenuMod[0]->getWidth() / 2.5f, pos_center_y + space_y*2), pMenuMod[0]->getSize()/ 1.25f, pMenuMod[0], 0);
	pMenuModObj[1] = new GameObject(glm::vec2(offset_x + pMenuMod[1]->getWidth() / 2.5f, pos_center_y + space_y*2), pMenuMod[1]->getSize()/ 1.25f, pMenuMod[1], 0);
	pArrowObj[0] = new GameObject(glm::vec2(1200.0f, pos_center_y + space_y * 2), pArrow->getSize()/8.0f, pArrow, 0);
	pArrowObj[1] = new GameObject(glm::vec2(1200.0f, pos_center_y + space_y * 2), pArrow->getSize()/8.0f, pArrow, 0);
	pArrowObj[1]->FlipX = -1;
}

MenuState::~MenuState() {
	for (int i = 0; i < 3; i++) {
		delete pMenu[i];
		delete pMenuObj[i];
		delete pMenuSel[i];
		delete pMenuSelObj[i];
	}

	for (int i = 0; i < 2; i++) {
		delete pMenuMod[i];
		delete pMenuModObj[i];
		delete pArrowObj[i];
	}

	delete pArrow;
	delete pMenuNoGame;
	delete pMenuNoGameObj;

	FT_Done_FreeType(ft);
}

MenuState* MenuState::GetInstance(StateManager* manager, GLFWwindow* window, irrklang::ISoundEngine* engine)
{
	static MenuState Instance(manager, window, engine);
	return &Instance;
}

//DEBOUNCE: avoids that pressing key in other frames (or states) is considered in this frame (or state)
bool canPressUp = true;
bool canPressDown = true;
bool canPressRight = true;
bool canPressLeft = true;
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

	if (glfwGetKey(Window, GLFW_KEY_RIGHT) == GLFW_PRESS && canPressRight) {
		if (PlayState::MultiplayerUnlocked) {
			CurrentGame->SwitchMode();
		}	
		canPressRight = false;
	}
	else if (glfwGetKey(Window, GLFW_KEY_RIGHT) == GLFW_RELEASE)
		canPressRight = true;

	if (glfwGetKey(Window, GLFW_KEY_LEFT) == GLFW_PRESS && canPressLeft) {
		if (PlayState::MultiplayerUnlocked) {
			CurrentGame->SwitchMode();
		}
		canPressLeft = false;
	}
	else if (glfwGetKey(Window, GLFW_KEY_LEFT) == GLFW_RELEASE)
		canPressLeft = true;

	if (glfwGetKey(Window, GLFW_KEY_ENTER) == GLFW_PRESS && canPressEnter) {
		SelectionChosen();
		canPressEnter = false;
	}
	else if (glfwGetKey(Window, GLFW_KEY_ENTER) == GLFW_RELEASE)
		canPressEnter = true;

}
void MenuState::MouseMoving(double xpos, double ypos)
{
	Mouse->Move(xpos, ypos);

	//Controllare se il cursore sia su una delle box
	for (int i = 0; i < 3; i++) {
		if (i == 1 && CurrentGame->GetStatus() == GameStatus::None)//no game -> skip resume
			i++;
		Hitbox bounds = pMenuObj[i]->GetHitbox();
		bool isColliding = (xpos <= bounds.Max.x && xpos >= bounds.Min.x && ypos <= bounds.Max.y && ypos >= bounds.Min.y);
		//Se è su una box selezionarla
		if (isColliding) {
			if(i!=CurrentSelection)
				Engine->play2D("resources/sounds/click.wav");
			CurrentSelection = i;
			//printf("x:%lf, y:%lf, select:%d\n", xpos, ypos, CurrentSelection);
			break;
		}
	}
}

void MenuState::MouseClick(double xpos, double ypos, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		//Controllare che il cursore sia sulla box

		//Mouse->Move(xpos, ypos);
		printf("%lf %lf", xpos, ypos);

		Hitbox bounds = pMenuObj[CurrentSelection]->GetHitbox();
		bool isColliding = (xpos <= bounds.Max.x && xpos >= bounds.Min.x && ypos <= bounds.Max.y && ypos >= bounds.Min.y);
		//Se è sulla box scegliere la selezione
		if (isColliding) {
			SelectionChosen();
			return;
		}
		//Controllare che il cursore sia sulla selezione modalità
		if (PlayState::MultiplayerUnlocked == false)
			return;
		bounds = pMenuModObj[CurrentGame->IsMultiplayer()]->GetHitbox();
		bool isCollidingText = (xpos <= bounds.Max.x && xpos >= bounds.Min.x && ypos <= bounds.Max.y && ypos >= bounds.Min.y);
		bounds = pArrowObj[CurrentGame->IsMultiplayer()]->GetHitbox();
		bool isCollidingArrow = (xpos <= bounds.Max.x && xpos >= bounds.Min.x && ypos <= bounds.Max.y && ypos >= bounds.Min.y);
		if (isCollidingText || isCollidingArrow) {
			Engine->play2D("resources/sounds/click.wav");
			CurrentGame->SwitchMode();
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

	pBackgroundObj->Render(*pShader);

	glDisable(GL_DEPTH_TEST);//evita di considerare la profondità delle sprite

	if (PlayState::MultiplayerUnlocked) {
		pMenuModObj[CurrentGame->IsMultiplayer()]->Render(*pShader);
		pArrowObj[CurrentGame->IsMultiplayer()]->Render(*pShader);
	}

	for (int i = 0; i < 3; i++)
		if (i == 1 && CurrentGame->GetStatus() == GameStatus::None)
			pMenuNoGameObj->Render(*pShader);
		else if(CurrentSelection!=i)
			pMenuObj[i]->Render(*pShader);
		else
			pMenuSelObj[i]->Render(*pShader);

	//pTestObj->RenderFlat(*pSpriteShader);

	//Mouse->Move();
	//Mouse->Render(*pShader);

	glEnable(GL_DEPTH_TEST);

	/*if (CurrentGame->GetStatus() != GameStatus::Paused)
		colors[1] = NonAvailableColor;

	glm::vec3 NonSelectedColor = { 255.0, 255.0, 255.0 };
	glm::vec3 SelectedColor = { 0.0, 255.0, 0.0 };
	glm::vec3 NonAvailableColor = { 0.0, 0.0, 0.0 };

	std::vector <glm::vec3> colors = { NonSelectedColor , NonSelectedColor , NonSelectedColor } ;

	colors[CurrentSelection] = SelectedColor;

	if (CurrentGame->GetStatus() != GameStatus::Paused)
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

	// Shader base
	ShaderManager::SetProjection(*pShader, Window, ProjectionType::Pixels);

	// musica di sottofondo
	ost = Engine->play2D("resources/sounds/ost.wav", true, false, true);
}
void MenuState::LeaveState() {
	ost->stop();
}
void MenuState::SelectionUp()
{
	if(CurrentSelection > NEW)
		CurrentSelection--;
	if (CurrentSelection == RESUME && CurrentGame->GetStatus() == GameStatus::None)//no game -> skip resume
			CurrentSelection--;
	Engine->play2D("resources/sounds/click.wav");
}

void MenuState::SelectionDown()
{
	if (CurrentSelection < EXIT)
		CurrentSelection++;
	if (CurrentSelection == RESUME && CurrentGame->GetStatus() == GameStatus::None)//no game -> skip resume
		CurrentSelection++;
	Engine->play2D("resources/sounds/click.wav");
}

void MenuState::SelectionChosen()
{
	Engine->play2D("resources/sounds/click.wav");
	switch (CurrentSelection)
	{
	case NEW:
		CurrentGame->ResetGame();
		ChangeState(CurrentGame);
		break;

	case RESUME:
		if (CurrentGame->GetStatus() != GameStatus::None) {
			if (CurrentGame->GetStatus() != GameStatus::Paused && CurrentGame->GetStatus() != GameStatus::End)
				CurrentGame->ResetLevel();//Inizia nuovo livello
			ChangeState(CurrentGame);
		}
		break;

	case EXIT:
		Engine->drop();
		glfwSetWindowShouldClose(Window, true);//quit
		break;
	}
}
//enum class GameStatus { None, Playing, Paused, GameOver, Victory };