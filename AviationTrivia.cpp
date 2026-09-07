#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <random>
#include <algorithm>
#include "questionHandling.h"
#include <fstream>
#include <cstdlib>
#include <thread>
#include <mutex>
#include <queue>

std::vector<int> questionOrder;
std::vector<std::string> unshuffledQuestions;

std::queue < std::string> commands;
std::mutex commandMutex;

int numberOfRightAnswers = 0;
int numberOfWrongAnswers = 0;
int gamesPlayed = 0;
int rateOfRightAnswers = 0;
bool skipIntro = false;

sf::IntRect getCharacterRect(char c);
void drawText(sf::RenderWindow& window, sf::Texture& fontTexture, const std::string& text, sf::Vector2f position, float scale);
void shuffleOrder();
void loadStats();
void saveStats();
void commandThread();

enum gameState {
	Intro,
	Menu1,
	Game,
	Stats,
	MoreMenu,
	CreditsMenu,
	HowToPlayMenu,
	HowToPlayMenu2,
	HowToPlayMenu3,
	HowToPlayMenu4,
	HowToPlayMenu5,
	HowToPlayMenu6,
	SettingsMenu
};

int main() {

	std::thread commandsThread(commandThread);
	commandsThread.detach();

	loadStats();

	questionHandlingFunction();
	for (int l = 0; l < questions.size(); l++) {
		unshuffledQuestions.push_back(questions[l]);
	}
	shuffleOrder();

	bool problem = false;
	bool answered = false;
	bool scoreUpdated = false;
	int playerChoice = 0;
	int score = 0;
	size_t currentQuestionI = 0;
	int currentQuestion = questionOrder[currentQuestionI];
	int numberOfQuestions = 12;

	sf::RenderWindow window(sf::VideoMode({ 1920, 1080 }), "Aviation trivia V1.0.0", sf::State::Fullscreen);
	window.setFramerateLimit(60);

	std::cout << "Please don't close this window. This is for debuging.\nIf there is a problem it will appear here. Example: Could not load example texture\n\n";
	std::cout << "Problems: \n";

	sf::Texture playButtonTexture;
	if (!playButtonTexture.loadFromFile("playButton.png")) {
		std::cout << "Could not load play button sprite sheet! Make sure you have <<playButton.png>>\n";
		problem = true;
	}
	sf::Texture shrimpTexture;
	if (!shrimpTexture.loadFromFile("shrimp.png")) {
		std::cout << "Couldn't load shrimp texture! Make sure you have <<shrimp.png>>n";
		problem = true;
	}
	sf::Texture vSPTexture;
	if (!vSPTexture.loadFromFile("introtext.png")) {
		std::cout << "Couldn't load intro text texture! Make sure you have <<introtext.png>>\n";
		problem = true;
	}
	sf::Texture moreButtonTexture;
	if (!moreButtonTexture.loadFromFile("moreButton.png")) {
		std::cout << "Couldn't load more button texture! Make sure you have <<moreButton.png>>\n";
		problem = true;
	}
	sf::Texture exitButtonTexture;
	if (!exitButtonTexture.loadFromFile("exitButton.png")) {
		std::cout << "Couldn't load exit button texture! Make sure you have <<exitButton.png>>\n";
		problem = true;
	}
	sf::Texture fontTexture;
	if (!fontTexture.loadFromFile("font.png")) {
		std::cout << "Couldn't load font texture! Make sure you have <<font.png>>\n";
		problem = true;
	}
	sf::Texture selectionBoxTexture;
	if (!selectionBoxTexture.loadFromFile("selectionBox.png")) {
		std::cout << "Couldn't load selection box texture! Make sure you have <<selectionBox.png>>\n";
		problem = true;
	}
	sf::Texture statsButtonTexture;
	if (!statsButtonTexture.loadFromFile("statsButton.png")) {
		std::cout << "Couldn't load stats button texture! Make sure you have <<statsButton.png>>\n";
		problem = true;
	}
	sf::Texture m1BackgroundTexture;
	if (!m1BackgroundTexture.loadFromFile("menu1Background.png")) {
		std::cout << "Couldn't load menu 1 background texture! Make sure you have <<menu1Background.png>>\n";
		problem = true;
	}
	sf::Texture questionBoxTexture;
	if (!questionBoxTexture.loadFromFile("questionBox.png")) {
		std::cout << "Couldn't load question box texture! Make sure you have <<questionBox.png>>\n";
		problem = true;
	}
	sf::Texture gameBackgroundTexture;
	if (!gameBackgroundTexture.loadFromFile("gameBackground.png")) {
		std::cout << "Couldn't load game background texture! Make sure you have <<gameBackground.png>>\n";
		problem = true;
	}
	sf::Texture feedbackButtonTexture;
	if (!feedbackButtonTexture.loadFromFile("feedbackButton.png")) {
		std::cout << "Couldn't load feedback button texture! Make sure you have <<feedbackButtonButton.png>>\n";
		problem = true;
	}
	sf::Texture moreGamesTexture;
	if (!moreGamesTexture.loadFromFile("moreGamesButton.png")) {
		std::cout << "Couldn't load more games texture! Make sure you have <<moreGamesButton.png>>\n";
		problem = true;
	}
	sf::Texture arrowTexture;
	if (!arrowTexture.loadFromFile("arrow.png")) {
		std::cout << "Couldn't load arrow texture! Make sure you have <<arrow.png>>\n";
		problem = true;
	}
	sf::Texture creditsButtonTexture;
	if (!creditsButtonTexture.loadFromFile("creditsButton.png")) {
		std::cout << "Couldn't load credits button texture! Make sure you have <<creditsButton.png>>\n";
		problem = true;
	}
	sf::Texture boxTexture;
	if (!boxTexture.loadFromFile("box.png")) {
		std::cout << "Couldn't load box texture! Make sure you have <<Box.png>>\n";
		problem = true;
	}
	sf::Texture howToPlayTexture;
	if (!howToPlayTexture.loadFromFile("howToPlay.png")) {
		std::cout << "Couldn't load how to play texture! Make sure you have <<howToPlay.png>>\n";
		problem = true;
	}
	sf::Texture howManyQuestionsTexture;
	if (!howManyQuestionsTexture.loadFromFile("howManyQuestions.png")) {
		std::cout << "Couldn't load how many questions texture! Make sure you have <<howManyQuestions.png>>\n";
		problem = true;
	}
	sf::Texture confirmTexture;
	if (!confirmTexture.loadFromFile("confirm.png")) {
		std::cout << "Couldn't load confirm texture! Make sure you have <<confirm.png>>\n";
		problem = true;
	}
	sf::Texture settingsTexture;
	if (!settingsTexture.loadFromFile("settings.png")) {
		std::cout << "Couldn't load settings texture! Make sure you have <<settings.png>>\n";
		problem = true;
	}
	sf::Texture checkBoxTexture;
	if (!checkBoxTexture.loadFromFile("checkBox.png")) {
		std::cout << "Couldn't load check box texture! Make sure you have <<checkBox.png>>\n";
		problem = true;
	}
	sf::Texture resetTexture;
	if (!resetTexture.loadFromFile("reset.png")) {
		std::cout << "Couldn't load reset texture! Make sure you have <<reset.png>>\n";
		problem = true;
	}
	sf::Texture volumeTexture;
	if (!volumeTexture.loadFromFile("volume.png")) {
		std::cout << "Couldn't load volume texture! Make sure you have <<volume.png>>\n";
		problem = true;
	}

	sf::Music introMusic;
	if (!introMusic.openFromFile("Vehicle - OGA - Siberian Intro.wav")) {
		std::cout << "Couldn't open intro music! Make sure you have <<Vehicle - OGA - Siberian Intro.wav>>\n";
		problem = true;
	}
	sf::Music menuMusic;
	if (!menuMusic.openFromFile("chilllofir-loop.ogg")) {
		std::cout << "Couldn't open menu music! Make sure you have <<chilllofir-loop.ogg>>\n";
		problem = true;
	}

	menuMusic.setLooping(true);
	menuMusic.setVolume(50.f);

	if (!problem) {
		std::cout << "No problems!\n";
	}
	std::cout << "\n";
	std::cout << "----------------------------------------------\n\n";
	std::cout << "Enter a command:\n";

	playButtonTexture.setSmooth(false);
	shrimpTexture.setSmooth(false);
	vSPTexture.setSmooth(false);
	moreButtonTexture.setSmooth(false);
	exitButtonTexture.setSmooth(false);
	fontTexture.setSmooth(false);
	selectionBoxTexture.setSmooth(false);
	statsButtonTexture.setSmooth(false);
	m1BackgroundTexture.setSmooth(false);
	questionBoxTexture.setSmooth(false);
	gameBackgroundTexture.setSmooth(false);
	feedbackButtonTexture.setSmooth(false);
	moreGamesTexture.setSmooth(false);
	arrowTexture.setSmooth(false);
	creditsButtonTexture.setSmooth(false);
	boxTexture.setSmooth(false);
	howToPlayTexture.setSmooth(false);
	howManyQuestionsTexture.setSmooth(false);
	confirmTexture.setSmooth(false);
	settingsTexture.setSmooth(false);
	checkBoxTexture.setSmooth(false);
	resetTexture.setSmooth(false);
	volumeTexture.setSmooth(false);

	sf::Sprite playButton(playButtonTexture);
	sf::Sprite introText(vSPTexture);
	sf::Sprite shrimp(shrimpTexture);
	sf::Sprite moreButton(moreButtonTexture);
	sf::Sprite exitButton(exitButtonTexture);
	sf::Sprite selectionBox(selectionBoxTexture);
	sf::Sprite statsButton(statsButtonTexture);
	sf::Sprite m1Background(m1BackgroundTexture);
	sf::Sprite questionBox(questionBoxTexture);
	sf::Sprite gameBackground(gameBackgroundTexture);
	sf::Sprite feedbackButton(feedbackButtonTexture);
	sf::Sprite moreGames(moreGamesTexture);
	sf::Sprite arrow(arrowTexture);
	sf::Sprite creditsButton(creditsButtonTexture);
	sf::Sprite box(boxTexture);
	sf::Sprite howToPlay(howToPlayTexture);
	sf::Sprite howManyQuestions(howManyQuestionsTexture);
	sf::Sprite confirm(confirmTexture);
	sf::Sprite settings(settingsTexture);
	sf::Sprite checkBox(checkBoxTexture);
	sf::Sprite reset(resetTexture);
	sf::Sprite volume(volumeTexture);

	sf::RectangleShape introBackground({ 2000.f, 1500.f });
	introBackground.setOrigin({ 1000.f, 750.f });
	introBackground.setPosition({ 960.f, 540.f });
	introBackground.setFillColor(sf::Color::White);

	playButton.setPosition({ 300.f, 620.f });
	introText.setPosition({ 900.f, 400.f });
	shrimp.setPosition({ 1100.f, 360.f });
	moreButton.setPosition({ 300.f, 820.f });
	exitButton.setPosition({ 1820.f, 100.f });
	selectionBox.setPosition({ 500.f, 480.f });
	statsButton.setPosition({ 400.f, 720.f });
	m1Background.setPosition({ 960.f, 540.f });
	questionBox.setPosition({ 960.f, 415.f });
	gameBackground.setPosition({ 960.f, 540.f });
	feedbackButton.setPosition({ 700.f, 400.f });
	moreGames.setPosition({ 1200.f, 400.f });
	arrow.setPosition({ 100.f, 100.f });
	creditsButton.setPosition({ 700.f, 600.f });
	box.setPosition({ 960.f, 540.f });
	howToPlay.setPosition({ 1200.f, 600.f });
	howManyQuestions.setPosition({ 960.f, 500.f });
	confirm.setPosition({ 960.f, 700.f });
	settings.setPosition({ 700.f, 800.f });
	checkBox.setPosition({ 800.f, 310.f });
	reset.setPosition({ 960.f, 900.f });

	introText.setOrigin({ 40.f, 40.f });
	shrimp.setOrigin({ 32.f, 32.f });
	playButton.setOrigin({ 13.5f, 5.5f });
	moreButton.setOrigin({ 13.5f, 5.5f });
	exitButton.setOrigin({ 6.5f, 6.5f });
	statsButton.setOrigin({ 17.f, 5.5f });
	m1Background.setOrigin({ 240.f, 135.f });
	questionBox.setOrigin({ 240.f, 4.5f });
	gameBackground.setOrigin({ 240.f, 135.f });
	feedbackButton.setOrigin({ 21.f, 5.5f });
	moreGames.setOrigin({ 16.f, 8.5f });
	arrow.setOrigin({ 6.f, 6.f });
	creditsButton.setOrigin({ 18.5f, 5.5f });
	box.setOrigin({ 120.f, 67.5f });
	howToPlay.setOrigin({ 17.5f, 9.f });
	howManyQuestions.setOrigin({ 45.f, 15.f });
	confirm.setOrigin({ 19.f, 5.5f });
	settings.setOrigin({ 22.5f, 5.5f });
	checkBox.setOrigin({4.5, 4.5f});
	reset.setOrigin({ 15.5f, 5.5f });

	playButton.setScale({ 8.f, 8.f });
	introText.setScale({ 8.f, 8.f });
	shrimp.setScale({ 6.f, 6.f });
	moreButton.setScale({ 8.f, 8.f });
	exitButton.setScale({ 8.f, 8.f });
	selectionBox.setScale({ 8.f,8.f });
	statsButton.setScale({ 8.f, 8.f });
	m1Background.setScale({ 4.f, 4.f });
	questionBox.setScale({ 8.f, 8.f });
	gameBackground.setScale({ 4.f, 4.f });
	feedbackButton.setScale({ 8.f, 8.f });
	moreGames.setScale({ 8.f, 8.f });
	arrow.setScale({ 8.f, 8.f });
	creditsButton.setScale({ 8.f, 8.f });
	box.setScale({ 6.f, 8.f });
	howToPlay.setScale({ 8.f, 8.f });
	howManyQuestions.setScale({ 8.f, 8.f });
	confirm.setScale({8.f, 8.f});
	settings.setScale({ 8.f, 8.f });
	checkBox.setScale({ 6.f, 6.f });
	reset.setScale({ 8.f, 8.f });

	sf::Clock introClock;
	float fadeDuration = 3.0f;
	float elapsedTime = 0.0f;

	sf::Clock wait;
	bool started = false;

	sf::Clock waitBeforeQuestion;
	sf::Time waitDuration = sf::seconds(3.f);

	gameState state;

	bool updated = false;
	state = gameState::Intro;

	introMusic.play();

	sf::Sprite selectionBox2 = selectionBox;
	sf::Sprite selectionBox3 = selectionBox;
	sf::Sprite selectionBox4 = selectionBox;

	sf::Sprite statsBox1 = questionBox;
	sf::Sprite statsBox2 = questionBox;
	sf::Sprite statsBox3 = questionBox;
	sf::Sprite statsBox4 = questionBox;

	statsBox1.setPosition({ 960.f, 300.f });
	statsBox2.setPosition({ 960.f, 450.f });
	statsBox3.setPosition({ 960.f, 600.f });
	statsBox4.setPosition({ 960.f, 750.f });

	selectionBox2.setPosition({ 500.f, 580.f });
	selectionBox3.setPosition({ 500.f, 680.f });
	selectionBox4.setPosition({ 500.f, 780.f });

	sf::Clock menuChange;

	sf::Sprite rightArrow = arrow;
	rightArrow.setPosition({ 1850.f ,540 });
	rightArrow.rotate(sf::degrees(180));

	sf::Sprite leftArrow = arrow;
	leftArrow.setPosition({ 70.f ,540 });

	sf::Sprite moreQuestionsArrow = rightArrow;
	sf::Sprite lessQuestionsArrow = leftArrow;

	moreQuestionsArrow.setPosition({ 1060.f, 550.f });
	lessQuestionsArrow.setPosition({ 860.f, 550.f });

	bool playButtonClicked = false;

	bool musicStarted = false;

	while(window.isOpen()){
		{
			std::lock_guard<std::mutex> lock(commandMutex);
		}

		while (!commands.empty()) {
			std::string command = commands.front();
			commands.pop();
			size_t numberOfQ = questions.size();

			if (command == "fileNames") { // fileNames de adaugat .exe
				std::cout << "\n";
				std::cout << "----------------------------------------------\n";
				std::cout << "arrow.png\n";
				std::cout << "AviationTrivia.exe\n";
				std::cout << "box.png\n";
				std::cout << "checkBox.png\n";
				std::cout << "chilllofir-loop.ogg\n";
				std::cout << "confirm.png\n";
				std::cout << "creditsButton.png\n";
				std::cout << "exitButton.png\n";
				std::cout << "feedbackButton.png\n";
				std::cout << "font.png\n";
				std::cout << "gameBackground.png\n";
				std::cout << "howManyQuestions.png\n";
				std::cout << "howToPlay.png\n";
				std::cout << "introText.png\n";
				std::cout << "menu1Background.png\n";
				std::cout << "moreButton.png\n";
				std::cout << "moreGamesButton.png\n";
				std::cout << "playButton.png\n";
				std::cout << "questionBox.png\n";
				std::cout << "questions.txt\n";
				std::cout << "reset.png\n";
				std::cout << "saveFile.txt\n";
				std::cout << "selectionBox.png\n";
				std::cout << "settings.png\n";
				std::cout << "sfml-audio-3.dll\n";
				std::cout << "sfml-graphics-3.dll\n";
				std::cout << "sfml-network-3.dll\n";
				std::cout << "sfml-system-3.dll\n";
				std::cout << "sfml-window-3.dll\n";
				std::cout << "shrimp.png\n";
				std::cout << "statsButton.png\n";
				std::cout << "Vehicle - OGA - Siberian Intro.wav\n";
				std::cout << "\nThese are all the file names for the game.\n";
				std::cout << "If a file is missing, the game may not work properly.\n";
				std::cout << "\n";
				std::cout << "----------------------------------------------\n\n";
			}
			else if (command == "assetDimensions") {
				std::cout << "\n";
				std::cout << "----------------------------------------------\n";
				std::cout << "arrow.png - 24 x 12 (SpSh 2)\n";
				std::cout << "box.png - 240 x 135\n";
				std::cout << "checkBox.png - 36 x 9 (SpSh 4)\n";
				std::cout << "confirm.png - 76 x 11 (SpSh 2)\n";
				std::cout << "creditsButton.png - 74 x 11 (SpSh 2)\n";
				std::cout << "exitButton.png - 30 x 15 (SpSh 2)\n";
				std::cout << "feedbackButton.png - 84 x 11 (SpSh 2)\n";
				std::cout << "font.png - 184 x 5 (SpSh 36)\n";
				std::cout << "gameBackground.png - 480 x 270\n";
				std::cout << "howManyQuestions.png - 90 x 30\n";
				std::cout << "howToPlay.png - 70 x 18 (SpSh 2)\n";
				std::cout << "introText.png - 80 x 80\n";
				std::cout << "menu1Background.png - 480 x 270\n";
				std::cout << "moreButton.png - 55 x 11 (SpSh 2)\n";
				std::cout << "moreGamesButton.png - 64 x 17 (SpSh 2)\n";
				std::cout << "playButton.png - 55 x 11 (SpSh 2)\n";
				std::cout << "questionBox.png - 480 x 9\n";
				std::cout << "reset.png - 62 x 11 (SpSh 2)\n";
				std::cout << "selectionBox.png - 488 x 9 (SpSh 4)\n";
				std::cout << "settings.png - 90 x 11 (SpSh 2)\n";
				std::cout << "shrimp.png - 64 x 64\n";
				std::cout << "statsButton.png - 68 x 11 (SpSh 2)\n";
				std::cout << "These are all the asset dimensions.\n";
				std::cout << " <<SpSh>> means spritesheet and the number next to in is\n";
				std::cout << "the number of elements in the spritesheet\n";
				std::cout << "For example: SpSh 2 means there are 2 elements in the spritesheet\n";
				std::cout << "\n";
				std::cout << "----------------------------------------------\n\n";
			}
			else if (command == "stats") {
				std::cout << "\n";
				std::cout << "----------------------------------------------\n";
				std::cout << "Number of right answers: " << numberOfRightAnswers << "\n";
				std::cout << "Number of wrong answers: " << numberOfWrongAnswers << "\n";
				std::cout << "Number of games played: " << gamesPlayed << "\n";
				std::cout << "Right answer rate: " << rateOfRightAnswers << "%\n";
				std::cout << "This are all the stats as of right now\n";
				std::cout << "\n";
				std::cout << "----------------------------------------------\n\n";
			}
			else if (command == "questions") {
				std::cout << "\n";
				std::cout << "----------------------------------------------\n";
				std::cout<< "Number of questions: " << numberOfQ << "\n";
				std::cout << "Unshuffled questions:\n";
				for (size_t i = 0; i < unshuffledQuestions.size();i++) {
					std::cout << unshuffledQuestions[i] << "\n";
				}
				std::cout << "\n";
				std::cout << "----------------------------------------------\n\n";
			}
			else if (command == "questionOrder") {
				std::cout << "\n";
				std::cout << "----------------------------------------------\n";
				std::cout << "Number of questions: " << numberOfQ << "\n";
				std::cout << "Shuffled questions:\n";
				for (size_t h = 0; h < questions.size(); h++) {
					std::cout << questions[questionOrder[h]] << "\n";
				}
				std::cout << "\n";
				std::cout << "----------------------------------------------\n\n";
			}
			else if (command == "commands") {
				std::cout << "\n";
				std::cout << "----------------------------------------------\n";
				std::cout << "All commands:\n";
				std::cout << "fileNames -- shows all of the file names\n";
				std::cout << "assetDimenstions -- shows all of the assets dimensions\n";
				std::cout << "stats -- shows all current statistics\n";
				std::cout << "questions -- shows all the unshuffled questions\n";
				std::cout << "questionOrder -- shows all the shuffled questions\n";
				std::cout << "commands -- shows all commands and a description\n";
				std::cout << "close -- terminates the program\n";
				std::cout << "\n";
				std::cout << "----------------------------------------------\n\n";
			}
			else if (command == "close") {
				std::cout << "\n";
				std::cout << "----------------------------------------------\n";
				std::cout << "Program terminated\n";
				return 0;
			}
			else {
				std::cout << "\n";
				std::cout << "----------------------------------------------\n";
				std::cout << "Unknown command!\n";
				std::cout << "\n";
				std::cout << "----------------------------------------------\n";
			}
		}

		while (const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				window.close();
			}

			if (const sf::Event::MouseButtonPressed* mouseClicked = event->getIf<sf::Event::MouseButtonPressed>()) {
				if (mouseClicked->button == sf::Mouse::Button::Left) {
					sf::Vector2i mousePos = sf::Mouse::getPosition(window);
					sf::Vector2f mappedPos = window.mapPixelToCoords(mousePos);

					if (state == gameState::Menu1 && !playButtonClicked) {
						if (exitButton.getGlobalBounds().contains(mappedPos)) {
							window.close();	
						}
						if (playButton.getGlobalBounds().contains(mappedPos)) {
							questionHandlingFunction();
							shuffleOrder();
							currentQuestionI = 0;
							currentQuestion = questionOrder[currentQuestionI];
							playButtonClicked = true;
						}
						if (statsButton.getGlobalBounds().contains(mappedPos)) {
							state = gameState::Stats;
						}
						if (moreButton.getGlobalBounds().contains(mappedPos)) {
							state = gameState::MoreMenu;
						}
					}else if (state == gameState::Menu1 && playButtonClicked) {
						if (moreQuestionsArrow.getGlobalBounds().contains(mappedPos)) {
							if (numberOfQuestions < 25) {
								numberOfQuestions++;
							}
						}
						if (lessQuestionsArrow.getGlobalBounds().contains(mappedPos)) {
							if (numberOfQuestions > 1) {
								numberOfQuestions--;
							}
						}
						if (confirm.getGlobalBounds().contains(mappedPos)) {
							state = gameState::Game;
						}
					}
					else if (state == gameState::Game) {
						if (exitButton.getGlobalBounds().contains(mappedPos)) {
							window.close();
						}
						if (arrow.getGlobalBounds().contains(mappedPos)) {
							state = gameState::Menu1;
							answered = false;
							playerChoice = 0;
							scoreUpdated = false;
							currentQuestionI = 0;
							playButtonClicked = false;
						}
						if (!answered) {
							if (selectionBox.getGlobalBounds().contains(mappedPos)) {
								playerChoice = 1;
							}
							else if (selectionBox2.getGlobalBounds().contains(mappedPos)) {
								playerChoice = 2;
							}
							else if (selectionBox3.getGlobalBounds().contains(mappedPos)) {
								playerChoice = 3;
							}
							else if (selectionBox4.getGlobalBounds().contains(mappedPos)) {
								playerChoice = 4;
							}
							if (playerChoice != 0) {
								answered = true;
								waitBeforeQuestion.restart();
								if (playerChoice == correctAnswer[currentQuestion]) {
									score += points[currentQuestion];
								}
							}
						}
					}
					else if (state == gameState::Stats) {
						if (exitButton.getGlobalBounds().contains(mappedPos)) {
							window.close();
						}
						if (arrow.getGlobalBounds().contains(mappedPos)) {
							state = gameState::Menu1;
						}
						if (reset.getGlobalBounds().contains(mappedPos)) {
							numberOfRightAnswers = 0;
							numberOfWrongAnswers = 0;
							gamesPlayed = 0;
							rateOfRightAnswers = 0;
							saveStats();
						}
					}
					else if (state == gameState::MoreMenu) {
						if (feedbackButton.getGlobalBounds().contains(mappedPos)) {
							std::string command = "start https://docs.google.com/forms/d/e/1FAIpQLSd_PYRWfn150R5Bi3gj90nlJ-TgKZuzrlvwGKAd-GypAZUxvg/viewform?usp=dialog";
							std::system(command.c_str());
						}
						if (moreGames.getGlobalBounds().contains(mappedPos)) {
							std::string command = "start https://github.com/stoicaandrei-blip";
							std::system(command.c_str());
						}
						if (exitButton.getGlobalBounds().contains(mappedPos)) {
							window.close();
						}
						if (arrow.getGlobalBounds().contains(mappedPos) && menuChange.getElapsedTime().asSeconds() >= 1.f){
							state = gameState::Menu1;
							menuChange.restart();
						}
						if (creditsButton.getGlobalBounds().contains(mappedPos)) {
							state = gameState::CreditsMenu;
						}
						if (howToPlay.getGlobalBounds().contains(mappedPos)) {
							state = gameState::HowToPlayMenu;
						}
						if (settings.getGlobalBounds().contains(mappedPos)) {
							state = gameState::SettingsMenu;
						}
					}
					else if (state == gameState::CreditsMenu) {
						if (exitButton.getGlobalBounds().contains(mappedPos)) {
							window.close();
						}
						if (arrow.getGlobalBounds().contains(mappedPos)) {
							state = gameState::MoreMenu;
						}
					}
					else if (state == gameState::HowToPlayMenu) {
						if (exitButton.getGlobalBounds().contains(mappedPos)) {
							window.close();
						}
						if (arrow.getGlobalBounds().contains(mappedPos)) {
							state = gameState::MoreMenu;
						}
						if (rightArrow.getGlobalBounds().contains(mappedPos)) {
							state = gameState::HowToPlayMenu2;
						}
					}
					else if (state == gameState::HowToPlayMenu2) {
						if (exitButton.getGlobalBounds().contains(mappedPos)) {
							window.close();
						}
						if (arrow.getGlobalBounds().contains(mappedPos)) {
							state = gameState::MoreMenu;
						}
						if (leftArrow.getGlobalBounds().contains(mappedPos)) {
							state = gameState::HowToPlayMenu;
						}
						if (rightArrow.getGlobalBounds().contains(mappedPos)) {
							state = gameState::HowToPlayMenu3;
						}
					}
					else if (state == gameState::HowToPlayMenu3) {
						if (exitButton.getGlobalBounds().contains(mappedPos)) {
							window.close();
						}
						if (arrow.getGlobalBounds().contains(mappedPos)) {
							state = gameState::MoreMenu;
						}
						if (leftArrow.getGlobalBounds().contains(mappedPos)) {
							state = gameState::HowToPlayMenu2;
						}
						if (rightArrow.getGlobalBounds().contains(mappedPos)) {
							state = gameState::HowToPlayMenu4;
						}
					}
					else if (state == gameState::HowToPlayMenu4) {
						if (exitButton.getGlobalBounds().contains(mappedPos)) {
							window.close();
						}
						if (arrow.getGlobalBounds().contains(mappedPos)) {
							state = gameState::MoreMenu;
						}
						if (leftArrow.getGlobalBounds().contains(mappedPos)) {
							state = gameState::HowToPlayMenu3;
						}
						if (rightArrow.getGlobalBounds().contains(mappedPos)) {
							state = gameState::HowToPlayMenu5;
						}
					}
					else if (state == gameState::HowToPlayMenu5) {
						if (exitButton.getGlobalBounds().contains(mappedPos)) {
							window.close();
						}
						if (arrow.getGlobalBounds().contains(mappedPos)) {
							state = gameState::MoreMenu;
						}
						if (leftArrow.getGlobalBounds().contains(mappedPos)) {
							state = gameState::HowToPlayMenu4;
						}
						if (rightArrow.getGlobalBounds().contains(mappedPos)) {
							state = gameState::HowToPlayMenu6;
						}
					}
					else if (state == gameState::HowToPlayMenu6) {
						if (exitButton.getGlobalBounds().contains(mappedPos)) {
							window.close();
						}
						if (arrow.getGlobalBounds().contains(mappedPos)) {
							state = gameState::MoreMenu;
						}
						if (leftArrow.getGlobalBounds().contains(mappedPos)) {
							state = gameState::HowToPlayMenu5;
						}
					}
					else if (state == gameState::SettingsMenu) {
						if (exitButton.getGlobalBounds().contains(mappedPos)) {
							window.close();
						}
						if (arrow.getGlobalBounds().contains(mappedPos)) {
							state = gameState::MoreMenu;
						}
						if (checkBox.getGlobalBounds().contains(mappedPos) && !skipIntro) {
							skipIntro = true;
							saveStats();
						}
						else if (checkBox.getGlobalBounds().contains(mappedPos) && skipIntro) {
							skipIntro = false;
							saveStats();
						}
					}
				}
			}
		}

		float dt = introClock.restart().asSeconds();

		if (state == gameState::Intro && !skipIntro) {
			if (!started && wait.getElapsedTime().asSeconds() > 3.f) {
				started = true;
			}
			if (started && elapsedTime < fadeDuration) {
				elapsedTime += dt;

				float ratio = elapsedTime / fadeDuration;
				if (ratio > 1.0f) {
					ratio = 1.0f;
				}

				if (started && elapsedTime > 2.f) {
					std::uint8_t beta = static_cast<std::uint8_t>((1.0f - ratio) * 255);
					shrimp.setColor(sf::Color(255, 255, 255, beta));
				}

				std::uint8_t alpha = static_cast<std::uint8_t>((1.0f - ratio) * 255);
				introBackground.setFillColor(sf::Color(alpha, alpha, alpha));
			}

				if (elapsedTime >= fadeDuration) {
					introMusic.stop();
					state = gameState::Menu1;
				}
		}
		else if (state == gameState::Intro && skipIntro) {
			introMusic.stop();
			state = gameState::Menu1;
		}

		if (state != gameState::Intro && !musicStarted) {
			menuMusic.play();
			musicStarted = true;
		}

		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		sf::Vector2f mappedMousePos = window.mapPixelToCoords(mousePos);

		if (state == gameState::Menu1 && !playButtonClicked) {
			if (playButton.getGlobalBounds().contains(mappedMousePos)) {
				playButton.setTextureRect(sf::IntRect({ 27, 0 }, { 27, 11 }));
			}
			else {
				playButton.setTextureRect(sf::IntRect({ 0, 0 }, { 27, 11 }));
			}

			if (moreButton.getGlobalBounds().contains(mappedMousePos)) {
				moreButton.setTextureRect(sf::IntRect({ 27, 0 }, { 27, 11 }));
			}
			else {
				moreButton.setTextureRect(sf::IntRect({ 0, 0 }, { 27, 11 }));
			}
			if (exitButton.getGlobalBounds().contains(mappedMousePos)) {
				exitButton.setTextureRect(sf::IntRect({ 15, 0 }, { 15, 15 }));
			}
			else {
				exitButton.setTextureRect(sf::IntRect({ 0, 0 }, { 15, 15 }));
			}
			if (statsButton.getGlobalBounds().contains(mappedMousePos)) {
				statsButton.setTextureRect(sf::IntRect({ 34, 0 }, { 34, 11 }));
			}
			else {
				statsButton.setTextureRect(sf::IntRect({ 0, 0 }, { 34, 11 }));
			}
		}
		else if (state == gameState::Menu1 && playButtonClicked) {
			if (moreQuestionsArrow.getGlobalBounds().contains(mappedMousePos)) {
				moreQuestionsArrow.setTextureRect(sf::IntRect({ 12, 0 }, { 12, 12 }));
			}
			else {
				moreQuestionsArrow.setTextureRect(sf::IntRect({ 0, 0 }, { 12, 12 }));
			}
			if (lessQuestionsArrow.getGlobalBounds().contains(mappedMousePos)) {
				lessQuestionsArrow.setTextureRect(sf::IntRect({ 12, 0 }, { 12, 12 }));
			}
			else {
				lessQuestionsArrow.setTextureRect(sf::IntRect({ 0, 0 }, { 12, 12 }));
			}
			if (confirm.getGlobalBounds().contains(mappedMousePos)) {
				confirm.setTextureRect(sf::IntRect({ 38, 0 }, { 38, 11 }));
			}
			else {
				confirm.setTextureRect(sf::IntRect({ 0, 0 }, { 38, 11 }));
			}

			playButton.setTextureRect(sf::IntRect({ 0, 0 }, { 27, 11 }));

		}
		else if (state == gameState::Game && !answered) {
			if (exitButton.getGlobalBounds().contains(mappedMousePos)) {
				exitButton.setTextureRect(sf::IntRect({ 15, 0 }, { 15, 15 }));
			}
			else {
				exitButton.setTextureRect(sf::IntRect({ 0, 0 }, { 15, 15 }));
			}
			if (selectionBox.getGlobalBounds().contains(mappedMousePos)) {
				selectionBox.setTextureRect(sf::IntRect({ 122, 0 }, { 122, 9 }));
			}
			else {
				selectionBox.setTextureRect(sf::IntRect({ 0, 0 }, { 122, 9 }));
			}

			if (selectionBox2.getGlobalBounds().contains(mappedMousePos)) {
				selectionBox2.setTextureRect(sf::IntRect({ 122, 0 }, { 122, 9 }));
			}
			else {
				selectionBox2.setTextureRect(sf::IntRect({ 0, 0 }, { 122, 9 }));
			}

			if (selectionBox3.getGlobalBounds().contains(mappedMousePos)) {
				selectionBox3.setTextureRect(sf::IntRect({ 122, 0 }, { 122, 9 }));
			}
			else {
				selectionBox3.setTextureRect(sf::IntRect({ 0, 0 }, { 122, 9 }));
			}

			if (selectionBox4.getGlobalBounds().contains(mappedMousePos)) {
				selectionBox4.setTextureRect(sf::IntRect({ 122, 0 }, { 122, 9 }));
			}
			else {
				selectionBox4.setTextureRect(sf::IntRect({ 0, 0 }, { 122, 9 }));
			}
			if (arrow.getGlobalBounds().contains(mappedMousePos)) {
				arrow.setTextureRect(sf::IntRect({ 12, 0 }, { 12, 12 }));
			}
			else {
				arrow.setTextureRect(sf::IntRect({ 0, 0 }, { 12, 12 }));
			}
		}
		else if (state == gameState::Stats) {
			if (exitButton.getGlobalBounds().contains(mappedMousePos)) {
				exitButton.setTextureRect(sf::IntRect({ 15, 0 }, { 15, 15 }));
			}
			else {
				exitButton.setTextureRect(sf::IntRect({ 0, 0 }, { 15, 15 }));
			}
			if (arrow.getGlobalBounds().contains(mappedMousePos)) {
				arrow.setTextureRect(sf::IntRect({ 12, 0 }, { 12, 12 }));
			}
			else {
				arrow.setTextureRect(sf::IntRect({ 0, 0 }, { 12, 12 }));
			}
			if (reset.getGlobalBounds().contains(mappedMousePos)) {
				reset.setTextureRect(sf::IntRect({ 31, 0 }, { 31, 11 }));
			}
			else {
				reset.setTextureRect(sf::IntRect({ 0, 0 }, { 31, 11 }));
			}
		}
		else if (state == gameState::MoreMenu) {
			if (feedbackButton.getGlobalBounds().contains(mappedMousePos)) {
				feedbackButton.setTextureRect(sf::IntRect({ 42, 0 }, { 42, 11 }));
			}
			else {
				feedbackButton.setTextureRect(sf::IntRect({ 0, 0 }, { 42, 11 }));
			}
			if (moreGames.getGlobalBounds().contains(mappedMousePos)) {
				moreGames.setTextureRect(sf::IntRect({ 32, 0 }, { 32, 17 }));
			}
			else {
				moreGames.setTextureRect(sf::IntRect({ 0, 0 }, { 32, 17 }));
			}
			if (exitButton.getGlobalBounds().contains(mappedMousePos)) {
				exitButton.setTextureRect(sf::IntRect({ 15, 0 }, { 15, 15 }));
			}
			else {
				exitButton.setTextureRect(sf::IntRect({ 0, 0 }, { 15, 15 }));
			}
			if (arrow.getGlobalBounds().contains(mappedMousePos)) {
				arrow.setTextureRect(sf::IntRect({ 12, 0 }, { 12, 12 }));
			}
			else {
				arrow.setTextureRect(sf::IntRect({ 0, 0 }, { 12, 12 }));
			}
			if (creditsButton.getGlobalBounds().contains(mappedMousePos)) {
				creditsButton.setTextureRect(sf::IntRect({ 37, 0 }, { 37, 11 }));
			}
			else {
				creditsButton.setTextureRect(sf::IntRect({ 0, 0 }, { 37, 11 }));
			}
			if (howToPlay.getGlobalBounds().contains(mappedMousePos)) {
				howToPlay.setTextureRect(sf::IntRect({ 35, 0 }, { 35, 18 }));
			}
			else {
				howToPlay.setTextureRect(sf::IntRect({ 0, 0 }, { 35, 18 }));
			}
			if (settings.getGlobalBounds().contains(mappedMousePos)) {
				settings.setTextureRect(sf::IntRect({ 45, 0 }, { 45, 11 }));
			}
			else {
				settings.setTextureRect(sf::IntRect({ 0, 0 }, { 45, 11 }));
			}
		}
		else if (state == gameState::CreditsMenu) {
			if (exitButton.getGlobalBounds().contains(mappedMousePos)) {
				exitButton.setTextureRect(sf::IntRect({ 15, 0 }, { 15, 15 }));
			}
			else {
				exitButton.setTextureRect(sf::IntRect({ 0, 0 }, { 15, 15 }));
			}
			if (arrow.getGlobalBounds().contains(mappedMousePos)) {
				arrow.setTextureRect(sf::IntRect({ 12, 0 }, { 12, 12 }));
			}
			else {
				arrow.setTextureRect(sf::IntRect({ 0, 0 }, { 12, 12 }));
			}
		}
		else if (state == gameState::HowToPlayMenu) {
			if (exitButton.getGlobalBounds().contains(mappedMousePos)) {
				exitButton.setTextureRect(sf::IntRect({ 15, 0 }, { 15, 15 }));
			}
			else {
				exitButton.setTextureRect(sf::IntRect({ 0, 0 }, { 15, 15 }));
			}
			if (arrow.getGlobalBounds().contains(mappedMousePos)) {
				arrow.setTextureRect(sf::IntRect({ 12, 0 }, { 12, 12 }));
			}
			else {
				arrow.setTextureRect(sf::IntRect({ 0, 0 }, { 12, 12 }));
			}
			if (rightArrow.getGlobalBounds().contains(mappedMousePos)) {
				rightArrow.setTextureRect(sf::IntRect({ 12, 0 }, { 12, 12 }));
			}
			else {
				rightArrow.setTextureRect(sf::IntRect({ 0, 0 }, { 12, 12 }));
			}
		}
		else if (state == gameState::HowToPlayMenu2) {
			if (exitButton.getGlobalBounds().contains(mappedMousePos)) {
				exitButton.setTextureRect(sf::IntRect({ 15, 0 }, { 15, 15 }));
			}
			else {
				exitButton.setTextureRect(sf::IntRect({ 0, 0 }, { 15, 15 }));
			}
			if (arrow.getGlobalBounds().contains(mappedMousePos)) {
				arrow.setTextureRect(sf::IntRect({ 12, 0 }, { 12, 12 }));
			}
			else {
				arrow.setTextureRect(sf::IntRect({ 0, 0 }, { 12, 12 }));
			}
			if (leftArrow.getGlobalBounds().contains(mappedMousePos)) {
				leftArrow.setTextureRect(sf::IntRect({ 12, 0 }, { 12, 12 }));
			}
			else {
				leftArrow.setTextureRect(sf::IntRect({ 0, 0 }, { 12, 12 }));
			}
			if (rightArrow.getGlobalBounds().contains(mappedMousePos)) {
				rightArrow.setTextureRect(sf::IntRect({ 12, 0 }, { 12, 12 }));
			}
			else {
				rightArrow.setTextureRect(sf::IntRect({ 0, 0 }, { 12, 12 }));
			}
		}
		else if (state == gameState::HowToPlayMenu3) {
			if (exitButton.getGlobalBounds().contains(mappedMousePos)) {
				exitButton.setTextureRect(sf::IntRect({ 15, 0 }, { 15, 15 }));
			}
			else {
				exitButton.setTextureRect(sf::IntRect({ 0, 0 }, { 15, 15 }));
			}
			if (arrow.getGlobalBounds().contains(mappedMousePos)) {
				arrow.setTextureRect(sf::IntRect({ 12, 0 }, { 12, 12 }));
			}
			else {
				arrow.setTextureRect(sf::IntRect({ 0, 0 }, { 12, 12 }));
			}
			if (leftArrow.getGlobalBounds().contains(mappedMousePos)) {
				leftArrow.setTextureRect(sf::IntRect({ 12, 0 }, { 12, 12 }));
			}
			else {
				leftArrow.setTextureRect(sf::IntRect({ 0, 0 }, { 12, 12 }));
			}
			if (rightArrow.getGlobalBounds().contains(mappedMousePos)) {
				rightArrow.setTextureRect(sf::IntRect({ 12, 0 }, { 12, 12 }));
			}
			else {
				rightArrow.setTextureRect(sf::IntRect({ 0, 0 }, { 12, 12 }));
			}
		}
		else if (state == gameState::HowToPlayMenu4) {
			if (exitButton.getGlobalBounds().contains(mappedMousePos)) {
				exitButton.setTextureRect(sf::IntRect({ 15, 0 }, { 15, 15 }));
			}
			else {
				exitButton.setTextureRect(sf::IntRect({ 0, 0 }, { 15, 15 }));
			}
			if (arrow.getGlobalBounds().contains(mappedMousePos)) {
				arrow.setTextureRect(sf::IntRect({ 12, 0 }, { 12, 12 }));
			}
			else {
				arrow.setTextureRect(sf::IntRect({ 0, 0 }, { 12, 12 }));
			}
			if (leftArrow.getGlobalBounds().contains(mappedMousePos)) {
				leftArrow.setTextureRect(sf::IntRect({ 12, 0 }, { 12, 12 }));
			}
			else {
				leftArrow.setTextureRect(sf::IntRect({ 0, 0 }, { 12, 12 }));
			}
			if (rightArrow.getGlobalBounds().contains(mappedMousePos)) {
				rightArrow.setTextureRect(sf::IntRect({ 12, 0 }, { 12, 12 }));
			}
			else {
				rightArrow.setTextureRect(sf::IntRect({ 0, 0 }, { 12, 12 }));
			}
		}
		else if (state == gameState::HowToPlayMenu5) {
			if (exitButton.getGlobalBounds().contains(mappedMousePos)) {
				exitButton.setTextureRect(sf::IntRect({ 15, 0 }, { 15, 15 }));
			}
			else {
				exitButton.setTextureRect(sf::IntRect({ 0, 0 }, { 15, 15 }));
			}
			if (arrow.getGlobalBounds().contains(mappedMousePos)) {
				arrow.setTextureRect(sf::IntRect({ 12, 0 }, { 12, 12 }));
			}
			else {
				arrow.setTextureRect(sf::IntRect({ 0, 0 }, { 12, 12 }));
			}
			if (leftArrow.getGlobalBounds().contains(mappedMousePos)) {
				leftArrow.setTextureRect(sf::IntRect({ 12, 0 }, { 12, 12 }));
			}
			else {
				leftArrow.setTextureRect(sf::IntRect({ 0, 0 }, { 12, 12 }));
			}
			if (rightArrow.getGlobalBounds().contains(mappedMousePos)) {
				rightArrow.setTextureRect(sf::IntRect({ 12, 0 }, { 12, 12 }));
			}
			else {
				rightArrow.setTextureRect(sf::IntRect({ 0, 0 }, { 12, 12 }));
			}
		}
		else if (state == gameState::HowToPlayMenu6) {
			if (exitButton.getGlobalBounds().contains(mappedMousePos)) {
				exitButton.setTextureRect(sf::IntRect({ 15, 0 }, { 15, 15 }));
			}
			else {
				exitButton.setTextureRect(sf::IntRect({ 0, 0 }, { 15, 15 }));
			}
			if (arrow.getGlobalBounds().contains(mappedMousePos)) {
				arrow.setTextureRect(sf::IntRect({ 12, 0 }, { 12, 12 }));
			}
			else {
				arrow.setTextureRect(sf::IntRect({ 0, 0 }, { 12, 12 }));
			}
			if (leftArrow.getGlobalBounds().contains(mappedMousePos)) {
				leftArrow.setTextureRect(sf::IntRect({ 12, 0 }, { 12, 12 }));
			}
			else {
				leftArrow.setTextureRect(sf::IntRect({ 0, 0 }, { 12, 12 }));
			}
		}
		else if (state == gameState::SettingsMenu) {
			if (exitButton.getGlobalBounds().contains(mappedMousePos)) {
				exitButton.setTextureRect(sf::IntRect({ 15, 0 }, { 15, 15 }));
			}
			else {
				exitButton.setTextureRect(sf::IntRect({ 0, 0 }, { 15, 15 }));
			}
			if (arrow.getGlobalBounds().contains(mappedMousePos)) {
				arrow.setTextureRect(sf::IntRect({ 12, 0 }, { 12, 12 }));
			}
			else {
				arrow.setTextureRect(sf::IntRect({ 0, 0 }, { 12, 12 }));
			}
			if (!skipIntro) {
				if (checkBox.getGlobalBounds().contains(mappedMousePos)) {
					checkBox.setTextureRect(sf::IntRect({ 9, 0 }, { 9, 9 }));
				}
				else {
					checkBox.setTextureRect(sf::IntRect({ 0, 0 }, { 9, 9 }));
				}
			}
			else if (skipIntro) {
				if (checkBox.getGlobalBounds().contains(mappedMousePos)) {
					checkBox.setTextureRect(sf::IntRect({ 27, 0 }, { 9, 9 }));
				}
				else {
					checkBox.setTextureRect(sf::IntRect({ 18, 0 }, { 9, 9 }));
				}
			}
		}

		if (state == gameState::Game && answered) {
			if (waitBeforeQuestion.getElapsedTime() >= waitDuration) {
				answered = false;
				playerChoice = 0;
				rateOfRightAnswers = (numberOfRightAnswers * 100) / (numberOfRightAnswers + numberOfWrongAnswers);
				currentQuestionI++;
				updated = false;
				saveStats();

				if (currentQuestionI >= numberOfQuestions) {
					gamesPlayed++;
					saveStats();
					state = gameState::Menu1;
					playButtonClicked = false;
				}
				else {
					currentQuestion = questionOrder[currentQuestionI];
				}	
			}
		}

		window.clear();
		if (state == gameState::Intro) {
			window.draw(introBackground);
			window.draw(shrimp);
			window.draw(introText);
		}
		else if (state == gameState::Menu1) {
			window.draw(m1Background);
			window.draw(playButton);
			window.draw(moreButton);
			window.draw(exitButton);
			window.draw(statsButton);
			if (playButtonClicked) {
				window.draw(howManyQuestions);
				window.draw(moreQuestionsArrow);
				window.draw(lessQuestionsArrow);
				window.draw(confirm);
				drawText(window, fontTexture, std::to_string(numberOfQuestions), { 930.f, 540.f }, 6.f);
			}
		}
		else if (state == gameState::Game && currentQuestion < questions.size()) {

				window.draw(gameBackground);
				window.draw(exitButton);
				window.draw(questionBox);
				window.draw(arrow);
				
				if (answered) {
					if (playerChoice != correctAnswer[currentQuestion]) {
						if (playerChoice == 1) {
							selectionBox.setTextureRect(sf::IntRect({ 366, 0 }, { 122, 9 }));
						}
						else if (playerChoice == 2) {
							selectionBox2.setTextureRect(sf::IntRect({ 366, 0 }, { 122, 9 }));
						}
						else if (playerChoice == 3) {
							selectionBox3.setTextureRect(sf::IntRect({ 366, 0 }, { 122, 9 }));
						}
						else if (playerChoice == 4) {
							selectionBox4.setTextureRect(sf::IntRect({ 366, 0 }, { 122, 9 }));
						}
					}
					if (correctAnswer[currentQuestion] == 1) {
						selectionBox.setTextureRect(sf::IntRect({ 244, 0 }, { 122, 9 }));
					}
					else if (correctAnswer[currentQuestion] == 2) {
						selectionBox2.setTextureRect(sf::IntRect({ 244, 0 }, { 122, 9 }));
					}
					else if (correctAnswer[currentQuestion] == 3) {
						selectionBox3.setTextureRect(sf::IntRect({ 244, 0 }, { 122, 9 }));
					}
					else if (correctAnswer[currentQuestion] == 4) {
						selectionBox4.setTextureRect(sf::IntRect({ 244, 0 }, { 122, 9 }));
					}
					if (playerChoice == correctAnswer[currentQuestion] && !updated) {
						numberOfRightAnswers++;
						updated = true;
						saveStats();
					}
					else if (playerChoice != correctAnswer[currentQuestion] && !updated) {
						numberOfWrongAnswers++;
						updated = true;
						saveStats();
					}
				}

				window.draw(selectionBox);
				window.draw(selectionBox2);
				window.draw(selectionBox3);
				window.draw(selectionBox4);
				drawText(window, fontTexture, questions[currentQuestion], { 50.f, 405.f }, 4);
				drawText(window, fontTexture, answer1[currentQuestion], { 530.f, 505.f }, 4);
				drawText(window, fontTexture, answer2[currentQuestion], { 530.f, 605.f }, 4);
				drawText(window, fontTexture, answer3[currentQuestion], { 530.f, 705.f }, 4);
				drawText(window, fontTexture, answer4[currentQuestion], { 530.f, 805.f }, 4);
				drawText(window, fontTexture, "SCORE ", { 100.f, 1000.f }, 6);
				drawText(window, fontTexture, std::to_string(score), { 300.f, 1000.f }, 6);

		}
		else if (state == gameState::Stats) {
			window.draw(gameBackground);
			window.draw(arrow);
			window.draw(exitButton);
			window.draw(statsBox1);
			window.draw(statsBox2);
			window.draw(statsBox3);
			window.draw(statsBox4);
			window.draw(reset);
			drawText(window, fontTexture, "NUMBER OF RIGHT ANSWERS " + std::to_string(numberOfRightAnswers), {500.f, 285.f}, 6);
			drawText(window, fontTexture, "NUMBER OF WRONG ANSWERS " + std::to_string(numberOfWrongAnswers), {500.f, 435.f}, 6);
			drawText(window, fontTexture, "GAMES PLAYED " + std::to_string(gamesPlayed), {500.f, 585.f}, 6);
			drawText(window, fontTexture, "RIGHT ANSWER RATE " + std::to_string(rateOfRightAnswers), {500.f, 735.f}, 6);
		}
		else if (state == gameState::MoreMenu) {
			window.draw(gameBackground);
			window.draw(arrow);
			window.draw(feedbackButton);
			window.draw(moreGames);
			window.draw(exitButton);
			window.draw(creditsButton);
			window.draw(howToPlay);
			window.draw(settings);
		}
		else if (state == gameState::CreditsMenu) {
			window.draw(gameBackground);
			window.draw(box);
			window.draw(exitButton);
			window.draw(arrow);
			drawText(window, fontTexture, "CREDITS", { 800.f, 100.f }, 8);
			drawText(window, fontTexture, "ALL ART IS MADE BY ME", { 280.f, 400.f }, 5);
			drawText(window, fontTexture, "ALL MUSIC IS FROM OPENGAMEART", { 280.f, 450.f }, 5);
			drawText(window, fontTexture, "INTRO MUSIC IS SIBERIAN INTRO BY VEHICLE", { 280.f, 500.f }, 5);
			drawText(window, fontTexture, "GAME MUSIC IS CHILL LOFI INSPIRED LOOP EDIT BY QUBODUP", { 280.f, 550.f }, 5);
		}
		else if (state == gameState::HowToPlayMenu) {
			window.draw(gameBackground);
			window.draw(box);
			window.draw(exitButton);
			window.draw(arrow);
			window.draw(rightArrow);
			drawText(window, fontTexture, "HOW TO PLAY", { 700.f, 100.f }, 8);
			drawText(window, fontTexture, "A GUIDE ABOUT MOST THINGS IN THIS GAME", { 350.f, 150.f }, 6);
			drawText(window, fontTexture, "SECTION 1 BUTTONS", { 300.f, 200.f }, 5);
			drawText(window, fontTexture, "A MENU IS ANY DIFFERENT COMBINATION OF BUTTONS AND IMAGES", { 300.f, 230.f }, 4);
			drawText(window, fontTexture, "IN THE MAIN MENU THERE ARE 4 BUTTONS", { 300.f, 260.f }, 4);
			drawText(window, fontTexture, "THE EXIT BUTTON IS IN EVERY MENU EXCEPT THE INTRO MENU AND CLOSES", { 300.f, 290.f }, 4);
			drawText(window, fontTexture, "THE GAME", { 300.f, 320.f }, 4);
			drawText(window, fontTexture, "THE PLAY BUTTON DIRRECTS YOU TO THE QUIZ PART OF THE GAME", { 300.f, 350.f }, 4);
			drawText(window, fontTexture, "THE STATS BUTTON LEADS YOU TO THE STATISTICS MENU", { 300.f, 380.f }, 4);
			drawText(window, fontTexture, "THE MORE BUTTON DIRECTS YOU TO THE MORE MENU", { 300.f, 410.f }, 4);
			drawText(window, fontTexture, "IN THE QUIZ PART THERE IS AN EXIT BUTTON THAT CLOSES THE GAME", { 300.f, 440.f }, 4);
			drawText(window, fontTexture, "THE ARROW RETURNS YOU TO THE MAIN MENU", { 300.f, 470.f }, 4);
			drawText(window, fontTexture, "YOU CAN CLICK ON A BOX TO SELECT THAT ANSWER TO A QUESTION", { 300.f, 500.f }, 4);
			drawText(window, fontTexture, "IN THE STATS MENU YOU HAVE A BUTTON TO RESET ALL STATS", { 300.f, 530.f }, 4);
			drawText(window, fontTexture, "IF YOU PRESS IT THE GAME WILL ASK YOU TO CONFIRM THE RESET", { 300.f, 560.f }, 4);
			drawText(window, fontTexture, "IN THE MORE MENU YOU WILL FIND SOME MORE BUTTONS", { 300.f, 590.f }, 4);
			drawText(window, fontTexture, "THE FEEDBACK BUTTON REDIRECTS YOU TO THE FEEDBACK SITE", { 300.f, 620.f }, 4);
			drawText(window, fontTexture, "THE MORE GAMES BUTTON REDIRECTS YOU TO GITHUB", { 300.f, 650.f }, 4);
			drawText(window, fontTexture, "THE CREDITS BUTTON REDIRECTS YOU TO THE CREDITS MENU WHERE", { 300.f, 680.f }, 4);
			drawText(window, fontTexture, "YOU CAN FIND OUT WHO CONTRIBUTED TO THE GAME", { 300.f, 710.f }, 4);
			drawText(window, fontTexture, "THE HOW TO PLAY BUTTON REDIRECTS YOU TO THIS MENU", { 300.f, 740.f }, 4);
			drawText(window, fontTexture, "THE SETTINGS BUTTON REDIRECTS YOU TO THE SETTINGS MENU", { 300.f, 770.f }, 4);
			drawText(window, fontTexture, "THE ACHIEVMENTS BUTTON REDIRECTS YOU TO THE ACHIEVMENTS MENU", { 300.f, 800.f }, 4);
			drawText(window, fontTexture, "IN THE SETTING MENU THERE ARE CHECK BOXES FOR SKIPPING THE INTRO", { 300.f, 830.f }, 4);
			drawText(window, fontTexture, "AND RECIEVING NOTIFICTIONS FOR ACHIEVMENTS", { 300.f, 860.f }, 4);
			drawText(window, fontTexture, "THERE ARE 2 BARS FOR SELECTING SFX AND MUSIC VOLUME", { 300.f, 890.f }, 4);
			drawText(window, fontTexture, "THERE IS ABUTTON FOR RESETING ALL SETTING THAT NEEDS CONFIRMATION", { 300.f, 920.f }, 4);
			drawText(window, fontTexture, "IN THE ACHIEVMENT MENU THERE ARE THE ACHIEVMENTS AND YOU CAN", { 300.f, 950.f }, 4);
			drawText(window, fontTexture, "CLICK ON THEM TO FIND OUT MORE ABOUT THEM", { 300.f, 980.f }, 4);
			drawText(window, fontTexture, "OCCASIONALLY THERE WILL BE ARROWS TO MOVE BETWEEN LARGER MENUS", { 300.f, 1010.f }, 4);
			drawText(window, fontTexture, "LIKE THIS ONE", { 300.f, 1040.f }, 4);
		}
		else if (state == gameState::HowToPlayMenu2) {
			window.draw(gameBackground);
			window.draw(exitButton);
			window.draw(arrow);
			window.draw(box);
			window.draw(leftArrow);
			window.draw(rightArrow);
			drawText(window, fontTexture, "HOW TO PLAY", { 700.f, 100.f }, 8);
			drawText(window, fontTexture, "A GUIDE ABOUT MOST THINGS IN THIS GAME", { 350.f, 150.f }, 6);
			drawText(window, fontTexture, "SECTION 2 PLAYING", { 300.f, 200.f }, 5);
			drawText(window, fontTexture, "WHEN PRESSING THE PLAY BUTTON YOU ENTER THE PLAY MENU", { 300.f, 230.f }, 4);
			drawText(window, fontTexture, "THERE YOU WILL FIND A QUESTION", { 300.f, 260.f }, 4);
			drawText(window, fontTexture, "UNDER THAT YOU WILL SEE 4 DIFFERENT OPTIONS", { 300.f, 290.f }, 4);
			drawText(window, fontTexture, "HOVER THE MOUSE OVER THEM AND PRESS WHICH ONE YOU THINK IS RIGHT", { 300.f, 320.f }, 4);
			drawText(window, fontTexture, "IF YOU CHOSE THE RIGHT ANSWER THE BOX WILL BECOME GREEN AND YOU", { 300.f, 350.f }, 4);
			drawText(window, fontTexture, "WILL RECEIVE THE ASSOCIATED NUMBER OF POINTS", { 300.f, 380.f }, 4);
			drawText(window, fontTexture, "IF YOU CHOSE A WRONG ANSWER THE BOX WILL TURN RED AND THE RIGHT", { 300.f, 410.f }, 4);
			drawText(window, fontTexture, "ONE WILL TURN GREEN AND YOU WILL RECEIVE NO POINTS", { 300.f, 440.f }, 4);
			drawText(window, fontTexture, "AFTER EACH QUESTION THERE WILL BE A 3 SECOND PAUSE", { 300.f, 470.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 500.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 530.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 560.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 590.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 620.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 650.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 680.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 710.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 740.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 770.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 800.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 830.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 860.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 890.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 920.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 950.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 980.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 1010.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 1040.f }, 4);
		}
		else if (state == gameState::HowToPlayMenu3) {
			window.draw(gameBackground);
			window.draw(exitButton);
			window.draw(arrow);
			window.draw(box);
			window.draw(leftArrow);
			window.draw(rightArrow);
			drawText(window, fontTexture, "HOW TO PLAY", { 700.f, 100.f }, 8);
			drawText(window, fontTexture, "A GUIDE ABOUT MOST THINGS IN THIS GAME", { 350.f, 150.f }, 6);
			drawText(window, fontTexture, "SECTION 3 CUSTOMIZING", { 300.f, 200.f }, 5);
			drawText(window, fontTexture, "HOW TO CUSTOMIZE ASSETS?", { 300.f, 230.f }, 4);
			drawText(window, fontTexture, "SIMPLY DELETE THE ASSET YOU WANT TO UPDATE", { 300.f, 260.f }, 4);
			drawText(window, fontTexture, "THEN PLACE THE NEW ONE IN THE SAME FOLDER AS THE EXECUATBLE FILE", { 300.f, 290.f }, 4);
			drawText(window, fontTexture, "AND JUST RENAME THE NEW ONE TO THE EXACT NAME OF THE PREVIOUS ONE", { 300.f, 320.f }, 4);
			drawText(window, fontTexture, "TO FIND THE EXACT NAMES AND DIMENSIONS THE ASSETS SHOULD BE PLEASE", { 300.f, 350.f }, 4);
			drawText(window, fontTexture, "CONSULT THE COMMANDS SECTION OF THIS GUIDE", { 300.f, 380.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 410.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 440.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 470.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 500.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 530.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 560.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 590.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 620.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 650.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 680.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 710.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 740.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 770.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 800.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 830.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 860.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 890.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 920.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 950.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 980.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 1010.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 1040.f }, 4);
			}
		else if (state == gameState::HowToPlayMenu4) {
			window.draw(gameBackground);
			window.draw(exitButton);
			window.draw(arrow);
			window.draw(box);
			window.draw(leftArrow);
			window.draw(rightArrow);
			drawText(window, fontTexture, "HOW TO PLAY", { 700.f, 100.f }, 8);
			drawText(window, fontTexture, "A GUIDE ABOUT MOST THINGS IN THIS GAME", { 350.f, 150.f }, 6);
			drawText(window, fontTexture, "SECTION 4 SETTINGS", { 300.f, 200.f }, 5);
			drawText(window, fontTexture, "IN THE SETTINGS MENU YOU WILL FIND", { 300.f, 230.f }, 4);
			drawText(window, fontTexture, "A CHECK BOX TO SKIP THE INTRO EACH TIME YOU ENTER THE GAME", { 300.f, 260.f }, 4);
			drawText(window, fontTexture, "A BAR CONSISTING OF 3 BOXES FOR MUSIC VOLUME", { 300.f, 290.f }, 4);
			drawText(window, fontTexture, "PRESS THE BOX CORRESPONDING TO 0 AND THE MUSIC WILL BE MUTTED", { 300.f, 320.f }, 4);
			drawText(window, fontTexture, "THE BOX WITH 1 MEANS NORMAL VOLUME AND THE BOX WITH 2 IS DOUBLE", { 300.f, 350.f }, 4);
			drawText(window, fontTexture, "VOLUME THE SAME APPLIES TO THE SFX VOLUME BOXES", { 300.f, 380.f }, 4);
			drawText(window, fontTexture, "THERE IS ALSO A CHECK BOX FOR RECEIVING A NOTIFICATION IN GAME ", { 300.f, 410.f }, 4);
			drawText(window, fontTexture, "WHEN YOU GET AN ACHIEVMENT", { 300.f, 440.f }, 4);
			drawText(window, fontTexture, "AT THE BOTTOM YOU WILL FIND A BUTTON THAT RESETS ALL SETTING TO", { 300.f, 470.f }, 4);
			drawText(window, fontTexture, "DEFAULT AND IT REQUIRES CONFIRMATION", { 300.f, 500.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 530.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 560.f }, 4);
			drawText(window, fontTexture, "NOTE THAT NOT ALL SETTINGS ARE ADDED", { 300.f, 590.f }, 6);
			drawText(window, fontTexture, "THAT WILL BE IN ANEWER VERSION", { 300.f, 650.f }, 6);
			drawText(window, fontTexture, "", { 300.f, 650.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 680.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 710.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 740.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 770.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 800.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 830.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 860.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 890.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 920.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 950.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 980.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 1010.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 1040.f }, 4);
		}
		else if (state == gameState::HowToPlayMenu5) {
			window.draw(gameBackground);
			window.draw(exitButton);
			window.draw(arrow);
			window.draw(box);
			window.draw(leftArrow);
			window.draw(rightArrow);
			drawText(window, fontTexture, "HOW TO PLAY", { 700.f, 100.f }, 8);
			drawText(window, fontTexture, "A GUIDE ABOUT MOST THINGS IN THIS GAME", { 350.f, 150.f }, 6);
			drawText(window, fontTexture, "SECTION 5 ACHIEVMENTS", { 300.f, 200.f }, 5);
			drawText(window, fontTexture, "THERE ARE 16 ACHIEVMENTS IN THE GAME", { 300.f, 230.f }, 4);
			drawText(window, fontTexture, "THERE ARE 4 ACHIEVMENTS FOR HAVING A DAY STREAK IN THE GAME", { 300.f, 260.f }, 4);
			drawText(window, fontTexture, "THERE ARE 4 ACHIEVMENTS FOR ANSWERING QUESTIONS", { 300.f, 290.f }, 4);
			drawText(window, fontTexture, "THERE ARE 4 ACHIEVMENTS FOR ANSWERING CORRECTLY", { 300.f, 320.f }, 4);
			drawText(window, fontTexture, "AND THERE ARE 4 ACHIEVMENTS FOR ANSWERING WRONG", { 300.f, 350.f }, 4);
			drawText(window, fontTexture, "IF YOU GO TO THE ACHIEVMENTS MENU YOU CAN SEE WHAT YOU HAVE", { 300.f, 380.f }, 4);
			drawText(window, fontTexture, "CLICKING ON AN ACHIEVMENT WILL LET YOU SEE SOME INFO ABOUT IT", { 300.f, 410.f }, 4);
			drawText(window, fontTexture, "NOTE THAT ADVERTISEMENTS ARE NOT YET ADDED", { 300.f, 540.f }, 6);
			drawText(window, fontTexture, "THEY WILL BE IN A NEWER VERSION", { 300.f, 590.f }, 6);
			drawText(window, fontTexture, "", { 300.f, 500.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 530.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 560.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 590.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 620.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 650.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 680.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 710.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 740.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 770.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 800.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 830.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 860.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 890.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 920.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 950.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 980.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 1010.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 1040.f }, 4);
		}
		else if (state == gameState::HowToPlayMenu6) {
			window.draw(gameBackground);
			window.draw(exitButton);
			window.draw(arrow);
			window.draw(box);
			window.draw(leftArrow);
			drawText(window, fontTexture, "HOW TO PLAY", { 700.f, 100.f }, 8);
			drawText(window, fontTexture, "A GUIDE ABOUT MOST THINGS IN THIS GAME", { 350.f, 150.f }, 6);
			drawText(window, fontTexture, "SECTION 6 TERMINAL COMMANDS", { 300.f, 200.f }, 5);
			drawText(window, fontTexture, "WHEN OPENING THE GAME THERE WILL BE 2 WINDOWS OPENING", { 300.f, 230.f }, 4);
			drawText(window, fontTexture, "THE GAME ONE YOU ARE IN RIGHT NOW AND A TERMINAL", { 300.f, 260.f }, 4);
			drawText(window, fontTexture, "IN THE TERMINAL YOU WILL BE ABLE TO SEE IF THERE ARE ANY PROBLEMS", { 300.f, 290.f }, 4);
			drawText(window, fontTexture, "WITH LOADING THE GAME FILES", { 300.f, 320.f }, 4);
			drawText(window, fontTexture, "HERE ARE SOME COMMANDS THAT ARE WRITED LIKE THIS", { 300.f, 350.f }, 4);
			drawText(window, fontTexture, "THE FIRST WORD IS WITH SMALL LETTERS AND THE REST START WITH CAPITAL", { 300.f, 380.f }, 4);
			drawText(window, fontTexture, "TYPE FILENAMES TO FIND OUT ALL THE FILE NAMES", { 300.f, 410.f }, 4);
			drawText(window, fontTexture, "TYPE ASSETDIMENSION TO FIND OUT ALL ASSETS DIMENSIONS", { 300.f, 440.f }, 4);
			drawText(window, fontTexture, "TYPE QUIT TO TERMINATE THE PROGRAM", { 300.f, 470.f }, 4);
			drawText(window, fontTexture, "TYPE COMMANDS TO SHOW ALL AVAILABLE COMMANDS", { 300.f, 500.f }, 4);
			drawText(window, fontTexture, "TYPE STATS TO SHOW CURRENT STATS", { 300.f, 530.f }, 4);
			drawText(window, fontTexture, "TYPE QUESTION TO VIEW ALL QUESTIONS", { 300.f, 560.f }, 4);
			drawText(window, fontTexture, "TYPE QUESTIONORDER FOR QUESTION ORDER", { 300.f, 590.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 620.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 650.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 680.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 710.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 740.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 770.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 800.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 830.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 860.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 890.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 920.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 950.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 980.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 1010.f }, 4);
			drawText(window, fontTexture, "", { 300.f, 1040.f }, 4);
		}
		else if (state == gameState::SettingsMenu) {
			window.draw(gameBackground);
			window.draw(arrow);
			window.draw(exitButton);
			window.draw(box);
			drawText(window, fontTexture, "SETTINGS", { 750.f, 100.f }, 8);
			drawText(window, fontTexture, "SKIP INTRO ", { 400.f, 300.f }, 6);
			window.draw(checkBox);
		}
		window.display();

	}

	return 0;
}

sf::IntRect getCharacterRect(char c) {
	int index = -1;

	if (c >= 'A' && c <= 'Z') {
		index = c - 'A';
	}
	else if (c >= '0' && c <= '9') {
		index = 26 + (c - '0');
	}
	else if (c == '?') {
		index = 36;
	}

	if (index == -1) {
		return sf::IntRect({ 0,0 }, { 0,0 });
	}

	return sf::IntRect({ index * 5, 0 }, { 5,5 });
}

void drawText(sf::RenderWindow& window, sf::Texture& fontTexture, const std::string& text, sf::Vector2f position, float scale) {
	float characterWidth = 5.f * scale;

	for (int i = 0; i < text.size(); i++) {
		char c = text[i];

		if (c == ' ') {
			position.x += characterWidth;
			continue;
		}

		sf::IntRect rect = getCharacterRect(c);

		if (rect.size.x == 0) {
			continue;
		}

		sf::Sprite letter(fontTexture);

		letter.setTextureRect(rect);
		letter.setScale({ scale, scale });
		letter.setPosition(position);

		window.draw(letter);

		position.x += characterWidth + 1;

	}
}

void shuffleOrder() {
	questionOrder.clear();
	for (size_t j = 0; j < questions.size(); j++) {
		questionOrder.push_back(j);
	}

	std::shuffle(questionOrder.begin(), questionOrder.end(), gen);
}

void saveStats() {
	std::ofstream file("saveFile.txt");

	if (!file.is_open()) {
		std::cout << "Couldn't open saveFile.txt!\n";
		return;
	}

	file << numberOfRightAnswers << "\n";
	file << numberOfWrongAnswers << "\n";
	file << gamesPlayed << "\n";
	file << rateOfRightAnswers << "\n";
	file << skipIntro << "\n";
}

void loadStats() {
	std::ifstream file("saveFile.txt");

	if (!file.is_open()) {
		std::cout << "Couldn't open saveFile.txt!\n";
		return;
	}

	file >> numberOfRightAnswers;
	file >> numberOfWrongAnswers;
	file >> gamesPlayed;
	file >> rateOfRightAnswers;
	file >> skipIntro;

	file.close();
}

void commandThread() {
	std::string command;

	while (std::cin >> command) {
		std::lock_guard<std::mutex> lock(commandMutex);
		commands.push(command);
	}
}