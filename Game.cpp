//#include "Game.h"
//
//
////Private functions
//void Game::initVariables()
//{
//	this->window = nullptr;
//
//	//Game logic
//	this->points = 0;
//	this->enemySpawnTimerMax = 1000.f;
//	this->enemySpawnTimer = this->enemySpawnTimerMax;
//	this->maxEnemies = 5;
//}
//
//void Game::initWindow()
//{
//	this->videoMode.height = 600;
//	this->videoMode.width = 800;
//
//	this->window = new sf::RenderWindow(this->videoMode, "Game 1", sf::Style::Titlebar | sf::Style::Close);
//
//	this->window->setFramerateLimit(60);
//}
//
//void Game::initEnemies()
//{
//	this->enemy.setPosition(10.f, 10.f);
//	this->enemy.setSize(sf::Vector2f(100.f, 100.f));
//	this->enemy.setScale(sf::Vector2f(0.5f, 0.5f));
//	this->enemy.setFillColor(sf::Color::Cyan);
//	this->enemy.setOutlineColor(sf::Color::Green);
//	this->enemy.setOutlineThickness(1.f);
//}
//
//
////Constructors / Distructors
//Game::Game()
//{
//	this->initVariables();
//	this->initWindow();
//	this->initEnemies();
//}
//
//Game::~Game()
//{
//	delete this->window;
//}
//
//
////Accessors
//const bool Game::running() const
//{
//	return this->window->isOpen();
//}
//
//
////Functions
//void Game::spawnEnemy()
//{
//	/*
//		@return void
//
//		Spawns enemies and sets their colors and positions.
//		-Sets a random position.
//		-Sets a random color.
//		-Adds enemy to the vector.
//	*/
//
//	this->enemy.setPosition(
//		static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)),
//		static_cast<float>(rand() % static_cast<int>(this->window->getSize().y - this->enemy.getSize().y))
//	);
//
//	this->enemy.setFillColor(sf::Color::Green);
//
//	//Spawn the enemy
//	this->enemies.push_back(this->enemy);
//}
//
//void Game::pollEvents()
//{
//	while (this->window->pollEvent(this->ev))
//	{	
//		switch (this->ev.type)
//		{
//		case sf::Event::Closed:
//			this->window->close();
//			break;
//		case sf::Event::KeyPressed:
//			if (this->ev.key.code == sf::Keyboard::Escape)
//				this->window->close();
//			break;
//		}
//	}
//}
//
//void Game::updateMousePositions()
//{
//	/*
//	@ return void
//
//	Updates the mouse positions:
//		* Mouse position relative to window (Vector2i)
//	*/
//
//	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
//}
//
//void Game::updateEnemies()
//{
//	/*
//		@return void
//
//		Updates the enemy spawn timer and spawns enemies
//		when the total amount of enemies is smaller than the maximum.
//		Moves the enemies downwards.
//		Removes the enemies at the edge of the screen.
//	*/
//
//
//	//Updating the timer for enemy spawning
//	if (this->enemies.size() < this->maxEnemies)
//	{
//		if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
//		{
//			//Spawn the enemy and reset the timer
//			this->spawnEnemy();
//			this->enemySpawnTimer = 0.f;
//		}
//		else
//			this->enemySpawnTimer += 1.f;
//	}
//}
//
//void Game::update()
//{
//	this->pollEvents();
//
//	this->updateMousePositions();
//
//	this->updateEnemies();
//}
//
//void Game::renderEnemies()
//{
//
//}
//
//
//void Game::render()
//{
//	this->window->clear();
//
//	//Draw game
//	this->renderEnemies();
//
//	this->window->display();
//}
